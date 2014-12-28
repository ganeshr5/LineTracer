// Line Tracer code

#include <avr/io.h>
#define F_CPU 8000000L
#include <util/delay.h>
#include "lcd.h"
#include "mydefs.h"

int sensor;
#define lmtrpos PIND4
#define lmtrneg PIND3
#define rtmtrpos PIND5
#define rtmtrneg PIND6
#define sbit(a,b) a|=(1<<b);
#define clrbit(a,b) a&=(0<<b);

unsigned int get_ana_val(unsigned char channel)
{
  ADMUX=channel;
  //ADCSRA|=(1<<ADEN);
  set(ADCSRA,ADEN);
  //ADCSRA|=(1<<ADSC);
  set(ADCSRA,ADSC);
  while(chk(ADCSRA,ADSC));
  return(ADC);
}

unsigned char get_digi_val(unsigned char channel)
{
  unsigned int temp;
  temp=get_ana_val(channel);
  if(temp<600)
  return(1);
  else
  return(0);
}

int sense()
{
  int els,ls,ers,rs,ms;
  els=get_digi_val(5);
  ls=get_digi_val(2);
  ers=get_digi_val(1);
  rs=get_digi_val(0);
  ms=get_digi_val(3);
  sensor=(ers<<4)|(rs<<3)|(ms<<2)|(ls<<1)|(els);
  return sensor;		
}

void delay(int time)
{
  int i;
  for(i=0;i<time;i++)
  {
    _delay_ms(1);
  }
}


void timer1_init()
{	
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM10);
	TCCR1B |= (1<<CS12) | (1<<CS10);
	OCR1A = 0;   //PD5  // right side
	OCR1B = 0;   //PD4  // left side
}

void foreward(int speed)
{
 	clrbit(PORTD,lmtrneg);
 	OCR1A=speed;
 	clrbit(PORTD,rtmtrneg);
 	OCR1B=speed;
}

void backward(int speed)
{
  sbit(PORTD,lmtrneg);
 	OCR1A=255-speed;
 	sbit(PORTD,rtmtrneg);
 	OCR1B=255-speed;
}

void stop()
{
  PORTD=0x00;
  OCR1A=0;
  OCR1B=0;
}

void left_motor(int dir,int speed)
{
  
 if(dir==1)
 {
   clrbit(PORTD,lmtrneg);
   OCR1B=speed;
 }
 else if(dir==-1)
  {
    sbit(PORTD,lmtrneg);
    OCR1B=speed;
  }
 else
 {
  clrbit(PORTD,lmtrneg);
  OCR1B=0;
 }
}

void right_motor(int dir,int speed)
{
  
 if(dir==1)
 {
   clrbit(PORTD,rtmtrneg);
   OCR1A=speed;
 }
 else if(dir==-1)
 {
    sbit(PORTD,rtmtrneg);
    OCR1A=speed;
  }
 else
 {
  clrbit(PORTD,rtmtrneg);
  OCR1A=0;
 }

}


void display_sensors()
{
 unsigned int sen0,sen1,sen2,sen3,sen4;

 sen0=get_digi_val(5);
 LCDvar(0,0,sen0,4); 

 sen1=get_digi_val(2);
 LCDvar(6,0,sen1,4);

 sen2=get_digi_val(3);
 LCDvar(11,0,sen2,4);

 sen3=get_digi_val(0);
 LCDvar(0,1,sen3,4);

 sen4=get_digi_val(1);
 LCDvar(6,1,sen4,4);

}

/*void read_sensors()
{
 int i;
 for(i=0;i<5;i++)
 {
    sensors[i]=get_ana_val(i);
    if(sensors[i]<512)  sensors[i]=1;
    else sensors[0];
 }

}*/

void main()

{
 DDRD=0xFF;
 DDRA=0x00;
 timer1_init();
 InitLCD(0);

 while(1)
 {

	display_sensors();
  sense();
  switch(sensor)
  {
   case 0:backward(30);
          delay(100);
          break;
   
   case 1:right_motor(1,50);
          //left_motor(-1,10);
          delay(100);
          break;
   
   case 2:right_motor(1,30);
          // left_motor(-1,10);
          delay(100);
          break;
   
   case 3:right_motor(1,30);
          // left_motor(-1,10);
		      delay(100);
          break;
   
   case 4:foreward(30);
          delay(100);
          break;
   	
	case 6:right_motor(1,30);
          //left_motor(1,10);
		      delay(100);
          break;

	case 7:right_motor(1,30);
    		   delay(100);
			     break;

	case 8://right_motor(1,10);
	         left_motor(1,30);
		       delay(100);
		       break;
 	
  case 12://right_motor(-1,10);
	         left_motor(1,30);
		       break;   
	
	case 14:foreward(30);
			     break;
  	
  case 16://right_motor(-1,10);
           left_motor(1,30);
		       break;
   
  case 31:foreward(30);
   			  break;
   
  default:backward(30);
  			  break;
  		
    }
   }
}
 
