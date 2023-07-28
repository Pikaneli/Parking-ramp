#include <at89c51rc2.h>
#include <intrins.h>
/*
P0_0 - automobil na ulazu
P0_1 - automobil na izlazu

P2_0 - rampa se spusta
P2_1 - rampa se podize

P2_2 - rampa spustena
P2_3 - rampa podignuta
*/

//Komande za displej
#define LCD_EN P3_2
#define LCD_RS P3_3
#define LCD_DB7 P1_0
#define LCD_DB6 P1_1
#define LCD_DB5 P1_2
#define LCD_DB4 P1_3
#define LCD_BL P1_4

char *greska="Error\r\n";
char *ok="Okej\r\n";
char *pokZaSlanje;
char buffer[10];
char buffer_it=0;
int mesto=5;
unsigned char ulaz=0;
unsigned char izlaz=0;
char *pomocna;

char* num2str(int broj)
{   
    char str[9];
	unsigned int i=0;
	unsigned int k=0;
	unsigned int ostatak=0;
	char pom[9];
	char* pok_pom=pom;
    char* pok=str;	
	do{
		ostatak=broj%10;
		broj=(broj/10);
		*pok_pom=ostatak+48;
		pok_pom++;
		i++;
	}while(broj!=0);
		*pok_pom='\0';

	while(i>0)
	{
	pok_pom--;
	str[k]=*pok_pom;
	++k;
	--i;
	}
	str[k]='\0';
	return pok;				
}

void initP1P3(void) {			

	P1=0xE0;
	P3=0xF9;
}

void wait1sec()
{
	unsigned char i;

	TMOD = (TMOD&0xF0)|0x01; 	
	
	for(i=0;i<200;i++) {

		TH0=0xEE;		   		
		TL0=0x00;				
		TR0=1;				  	
		while(!TF0) {}		  
			TF0=0;				 
	}
}

void wait50us()
{
	TH0=0xFF;
	TL0=0xD2;

	TR0=1;

	while(!TF0)
	{

	}
	TF0=0;
}

void wait2ms()
{
 	TH0=0xF8;
	TL0=0xCD;

	TR0=1;

	while(!TF0)
	{

	}
	TF0=0;
}

void wait4ms()
{
 	wait2ms();
	wait2ms();	
}

void wait5sec()
{
 	wait1sec();
	wait1sec();
	wait1sec();
	wait1sec();
	wait1sec();
}

bit getbit(unsigned char n,unsigned char k)
{
 	unsigned char mask=1<<k;
	unsigned char masked_n=n&mask;
	bit thebit=masked_n>>k;
	return thebit;
}

void writeChar(unsigned char karakter)
{
 	LCD_EN=1;
	LCD_RS=1;
	LCD_DB7=getbit(karakter,7);
	LCD_DB6=getbit(karakter,6);
	LCD_DB5=getbit(karakter,5);
	LCD_DB4=getbit(karakter,4);
	LCD_EN=0;
	_nop_();
	LCD_EN=1;
	LCD_DB7=getbit(karakter,3);
	LCD_DB6=getbit(karakter,2);
	LCD_DB5=getbit(karakter,1);
	LCD_DB4=getbit(karakter,0);
	LCD_EN=0;
	wait50us();
}

void writeLine(char *str)
{
	char i=0;
	while(*(str+i)!='\0')
	{
		writeChar(*(str+i));
	 	i++;
	}
}

void clearDisplay() {  				
    LCD_RS=0;		  
	LCD_EN=1;
	LCD_DB7=0;
	LCD_DB6=0;
	LCD_DB5=0;
	LCD_DB4=0;
	LCD_EN=0;
  	
	_nop_();

	LCD_RS=0;		  
	LCD_EN=1;
	LCD_DB7=0;
	LCD_DB6=0;
	LCD_DB5=0;
	LCD_DB4=1;
	LCD_EN=0;
	wait2ms();
	wait2ms();

}

void newLine() {		 				
	LCD_RS=0;
	LCD_EN=1;	
	LCD_DB7=1;
	LCD_DB6=1;
	LCD_DB5=0;
	LCD_DB4=0;
	LCD_EN=0;
	_nop_();
	LCD_EN=1;
	LCD_DB7=0;
	LCD_DB6=0;
	LCD_DB5=0;
	LCD_DB4=0;
	LCD_EN=0;
	wait2ms();
	wait2ms();
}

void inicijalizacija()
{
	initP1P3();

	wait1sec();

 	LCD_EN=1;
	LCD_RS=0;

	LCD_DB7=0;
	LCD_DB6=0;
	LCD_DB4=0;
	LCD_DB5=1;
	LCD_EN=0;

	wait50us();

	LCD_EN=1;
	LCD_RS=0;
	LCD_DB7=0;
	LCD_DB6=0;
	LCD_DB4=0;
	LCD_DB5=1;
	LCD_EN=0;
	_nop_();
	LCD_EN=1;
	LCD_RS=0;
	LCD_DB7=1;
	LCD_DB6=0;
	LCD_DB4=0;
	LCD_DB5=0;
	LCD_EN=0;

	wait50us();

	LCD_EN=1;
	LCD_RS=0;
	LCD_DB7=0;
	LCD_DB6=0;
	LCD_DB4=0;
	LCD_DB5=0;
	LCD_EN=0;
	_nop_();
	LCD_EN=1;
	LCD_RS=0;
	LCD_DB7=1;
	LCD_DB6=1;
	LCD_DB4=1;
	LCD_DB5=1;
	LCD_EN=0;

	wait50us();

	LCD_EN=1;
	LCD_RS=0;
	LCD_DB7=0;
	LCD_DB6=0;
	LCD_DB4=0;
	LCD_DB5=0;
	LCD_EN=0;
	_nop_();
	LCD_EN=1;
	LCD_RS=0;
	LCD_DB7=0;
	LCD_DB6=0;
	LCD_DB4=1;
	LCD_DB5=0;
	LCD_EN=0;

	wait4ms();

	LCD_EN=1;
	LCD_RS=0;
	LCD_DB7=0;
	LCD_DB6=0;
	LCD_DB4=0;
	LCD_DB5=0;
	LCD_EN=0;
	_nop_();
	LCD_EN=1;
	LCD_RS=0;
	LCD_DB7=0;
	LCD_DB6=1;
	LCD_DB4=0;
	LCD_DB5=1;
	LCD_EN=0;

	wait50us();
	wait2ms();

	LCD_BL=1;
}

void parsirajPoruku()
{
 	if(buffer[0]=='(' && buffer[1]=='p' && buffer[2]==':' && buffer[3]>='0' && buffer[3]<='1' && buffer[4]>='0' && buffer[4]<='9' && buffer[5]==')')
	{
		if(buffer[3]=='0')
		{
		 	mesto=(int)(buffer[4])-48;
			//pokZaSlanje=num2str(mesto);
		}
		else if(buffer[4]>='6')
		{
		 	pokZaSlanje=greska;		
		}
		else
		{
		 	mesto=10 + ((int)(buffer[4])-48);			
			//pokZaSlanje=num2str(mesto);
		}

		switch(mesto)
		{
		 	case 0:
				pokZaSlanje="0\r\n";
				break;
			case 1:
				pokZaSlanje="1\r\n";
				break;
			case 2:
				pokZaSlanje="2\r\n";
				break;
			case 3:
				pokZaSlanje="3\r\n";
				break;
			case 4:
				pokZaSlanje="4\r\n";
				break;
			case 5:
				pokZaSlanje="5\r\n";
				break;
			case 6:
				pokZaSlanje="6\r\n";
				break;
			case 7:
				pokZaSlanje="7\r\n";
				break;
			case 8:
				pokZaSlanje="8\r\n";
				break;
			case 9:
				pokZaSlanje="9\r\n";
				break;
			case 10:
				pokZaSlanje="10\r\n";
				break;
			case 11:
				pokZaSlanje="11\r\n";
				break;
			case 12:
				pokZaSlanje="12\r\n";
				break;
			case 13:
				pokZaSlanje="13\r\n";
				break;
			case 14:
				pokZaSlanje="14\r\n";
				break;
			case 15:
				pokZaSlanje="15\r\n";
				break;
			default:
				pokZaSlanje=greska;
		}

		clearDisplay();
		writeLine("Mesta:\0");
		newLine();
		writeChar(buffer[3]);
		writeChar(buffer[4]);
	}

	else
	{
		pokZaSlanje=greska;
	}

	SBUF=*pokZaSlanje;
	buffer[0]='\0';
	buffer_it=0;
}

void serijski_prekid(void) interrupt 4
{
	if(TI)
	{
	 	TI=0;
		pokZaSlanje++;
		if(*pokZaSlanje!='\0')
		{
		SBUF=*pokZaSlanje;
		}
	}
	if(RI)
	{
		char prijem;
	 	RI=0;
		prijem=SBUF;

		if(prijem=='(')
		{
		 	buffer_it=0;
		}

		buffer[buffer_it]=prijem;
		buffer_it=(buffer_it+1)%10;

		if(prijem==')')
		{
		 	parsirajPoruku();
		}
	}
}

void ispis()
{
	pomocna=num2str(mesto);
	clearDisplay();
	writeLine("Mesta:\0");
	newLine();
	writeLine(pomocna);
}

void provera()
{
 		//Taster za ulazak
		if(P0_0==0)
		{
		 	ulaz=1;
		}
		else
		{
		 	ulaz=0;
		}

		//Taster za izlazak
		if(P0_1==0)
		{
		 	izlaz=1;
		}
		else
		{
		 	izlaz=0;
		}
}

void provera_mesta()
{
 	if(mesto<=0)
			{
				mesto=0;
				izlaz=0;
				P2=0x04;
			}

			if(mesto>=15)
			{
				mesto=15;
				ulaz=0;
				P2=0x04;	
			}
}

void main(void)
{
	//Inicijalizacija
	inicijalizacija();
	SCON=0x50;
	PCON=0x80;
	BDRCON=0x1C;
	BRL=253; 
	EA=1;
	ES=1;
	//na pocetku spustena
	P2=0x04;
	P0=0xFF;
		
	while(1)
	{
			provera();
			provera_mesta();

		//Izlazak iz parkinga
		if(izlaz==1 && mesto>0)
		{
			//podizanje
		 	P2_0=0;
			P2_1=1;
			P2_2=0;
			P2_3=0;
			wait1sec();
			wait1sec();
			//stiglo do vrha
			P2_0=0;
			P2_1=0;
			P2_2=0;
			P2_3=1;	

			wait5sec();
			provera();

			mesto=mesto-1;
			provera_mesta();
			ispis();
			

			//dokle god ima kola na izlazu ili ulazu drzati rampu podignutu da kola prolaze
			while(izlaz==1 || ulaz==1)
			{	
				//ostaje podignuto
			 	P2_0=0;
				P2_1=0;
				P2_2=0;
				P2_3=1;

				provera_mesta();

				if(izlaz==1 && mesto>0)
				{
					wait5sec();
					provera();

				 	mesto=mesto-1;
					provera_mesta();
					ispis();
				}

				if(ulaz==1 && mesto<15)
				{
					wait5sec();
					provera();					

				 	mesto=mesto+1;
					provera_mesta();
					ispis();
				}

				}

					//spustanje
			 		P2_0=1;
					P2_1=0;
					P2_2=0;
					P2_3=0;
					wait1sec();
					wait1sec();

					//spusteno
					P2_0=0;
					P2_1=0;
					P2_2=1;
					P2_3=0;	
						
				}

		//ulazak u parking
		if(ulaz==1 && mesto<15)
		{
			//podizanje
		 	P2_0=0;
			P2_1=1;
			P2_2=0;
			P2_3=0;
			wait1sec();
			wait1sec();
			//stiglo do vrha
			P2_0=0;
			P2_1=0;
			P2_2=0;
			P2_3=1;	

			wait5sec();
			provera();

			mesto=mesto+1;
			provera_mesta();
			ispis();

			//dokle god ima kola na izlazu ili ulazu drzati rampu podignutu da kola prolaze
			while(izlaz==1 || ulaz==1)
			{	
				//ostaje podignuto
			 	P2_0=0;
				P2_1=0;
				P2_2=0;
				P2_3=1;

				provera_mesta();

				if(izlaz==1 && mesto>0)
				{
					wait5sec();
					provera();

				 	mesto=mesto-1;
					provera_mesta();
					ispis();
				}

				if(ulaz==1 && mesto<15)
				{
					wait5sec();
					provera();					

				 	mesto=mesto+1;
					provera_mesta();
					ispis();
				}

				}

					//spustanje
			 		P2_0=1;
					P2_1=0;
					P2_2=0;
					P2_3=0;
					wait1sec();
					wait1sec();

					//spusteno
					P2_0=0;
					P2_1=0;
					P2_2=1;
					P2_3=0;	
						
				}
			}
		}