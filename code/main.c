#include <18F2550.h>
#device ADC=10

#include <math.h>


#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#fuses CPUDIV1        //No System Clock Postscaler
#fuses NOUSBDIV       //USB clock source comes from primary oscillator
#fuses INTRC_IO       //Internal RC Osc, no CLKOUT
#fuses NOFCMEN        //Fail-safe clock monitor disabled
#fuses NOIESO         //Internal External Switch Over mode disabled
#fuses PUT            //Power Up Timer
#fuses NOBROWNOUT     //No brownout reset
#fuses NOVREGEN       //USB voltage regulator disabled
#fuses NOWDT          //No Watch Dog Timer
#fuses NOPBADEN       //PORTB pins are configured as digital I/O on RESET
#fuses NOMCLR         //Master Clear pin used for I/O
#fuses NOLVP          //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#fuses NODEBUG        //No Debug mode for ICD
#fuses PROTECT        //Code protected from reads
#fuses NOCPD          //No EE protection
#fuses NOWRT          //Program memory not write protected
#fuses NOWRTD         //Data EEPROM not write protected

#use delay(internal=8000000)
#use FIXED_IO( A_outputs=PIN_A2,PIN_A1 )
#use RS232(UART1,baud=9600,parity=N,bits=8)

#include <main.h>
#USE SPI (MASTER, CLK=PIN_C4, DI=PIN_C2, DO=PIN_C1, ENABLE=PIN_C5, BAUD=38400, MODE=1, BITS=16, ENABLE_ACTIVE=0, STREAM=SPI_1, MSB_FIRST)



#define KEYBOARD_PIN PIN_A0

#define LCD_ENABLE_PIN PIN_B2
#define LCD_RS_PIN PIN_B0
#define LCD_RW_PIN PIN_B1
#define LCD_DATA4 PIN_B3
#define LCD_DATA5 PIN_B4
#define LCD_DATA6 PIN_B5
#define LCD_DATA7 PIN_B6

int v =0;
unsigned int8 q =0;
int8 counter=0;
int8 seconds=0;
int16 minutes=0;
int16 tmp=0;
int8 flag=0;
int8 flag1=0;
int16 b=0;
int8 open_=0;
int16 preheating=0;
int16 pre_temperature=0;
int16 heating=0;
int16 temperature=0;

char ptr [8] = {  0b00001111,
                  0b00001001,
                  0b00001111,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000};
#INT_TIMER1
void  TIMER1_isr(void) 
{
   set_timer1(3035);
   
   if(counter < 3)counter++;
   else 
   {
      counter=0;
      if(seconds<59)seconds++;
      else 
      {
         seconds=0;
         minutes++;
      }
   }
   
}


#include <lcd.c>
#include "tones.c"
#include "funciones.c"


void main()
{
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);      //262 ms overflow
   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);

   set_timer1(3035);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);

   lcd_init();
   set_adc_channel(0);
   lcd_set_cgram_char(1, ptr);
   
   while(TRUE)
   {
      read_temp();
      begin();
      mode();
      run();
      end();
      delay_ms(1000);
   }

}
