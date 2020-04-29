void tono (int8 who){
    
   if( who == 0 ){
     
      generate_tone(500,125);
      delay_ms(20);
   }
   if( who == 1 ){

      generate_tone(E_NOTE[0],100);
      generate_tone(B_NOTE[0],80);
   }
   if( who == 2 ){

      generate_tone(600,125);
      delay_ms(100);
      generate_tone(600,125);
   }
   if( who == 3 ){

      generate_tone(600,225);
      delay_ms(400);
      generate_tone(600,225);
      delay_ms(400);
      generate_tone(600,225);
      delay_ms(400);
      generate_tone(600,225);
   }
   if(who == 4 ){

      generate_tone(600,1000);

   }

 }

unsigned int aabs(signed int e )
{
   if(e > 0) return e;
   else return abs(e);
}

int8 buttons()
{
   
  tmp = read_adc();
  
       if( tmp >= 114 && tmp <= 226 && flag1==0 )  { flag = 1 ; flag1=1; tono(0);}
  else if( tmp >= 284 && tmp <= 396 && flag1==0 )  { flag = 2 ; flag1=1; tono(0);}
  else if( tmp >= 454 && tmp <= 566 && flag1==0 )  { flag = 3 ; flag1=1; tono(0);}
  else if( tmp >= 624 && tmp <= 736 && flag1==0 )  { flag = 4 ; flag1=1; tono(0);}
  else if( tmp >= 794 && tmp <= 906 && flag1==0 )  { flag = 5 ; flag1=1; tono(1);}
  
  else { 
         flag = 0; 
         if(tmp>1000)flag1 = 0;
       }

  return flag;
}

void read_temp()
{
   b        =  spi_xfer(0);
   open_   =  bit_test(b,2);
   b        &= 0b0111111111111000;
   b        =  b>>3;
   b        /= 4;
   delay_ms(1);
}

void control(int16 t)
{
   read_temp();
   int8 x = b;
   q= x-t;
   q=aabs(q);
   if(q >= 1 && x>t) output_low(PIN_A1);
   else if(q >= 5 && t>x) output_high (PIN_A1);
}

void begin()
{

v =0;
q =0;
counter=0;
seconds=0;
minutes=0;
tmp=0;
flag=0;
flag1=0;
b=0;
open_=0;
   lcd_putc('\f');
   printf(lcd_putc,"  OVEN CONTROL  ");
   lcd_gotoxy(1,2);
   printf(lcd_putc,"   **V-1.0**    ");
   delay_ms(2000);
}

void mode ()
{
   while(1)
   {
      lcd_putc('\f');
      printf(lcd_putc," FIX TEMPERATURE ");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"PRE-HEATING: %lu",pre_temperature);
      int8 d= buttons();
      if (d == 1)pre_temperature++;
      else if (d == 2)pre_temperature--;
      else if (d == 3)pre_temperature-=10;
      else if (d == 4)pre_temperature+=10;
      else if (d == 5) break;
      if(pre_temperature >600)pre_temperature=0;
      else if(pre_temperature <0)pre_temperature=600;
      delay_ms(100);
   }
   
   while(1)
   {
      lcd_putc('\f');
      printf(lcd_putc,"FIX TIME (MIN) ");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"HEATING: %lu",heating);
      int8 d= buttons();
      if (d == 1)heating++;
      else if (d == 2)heating--;
      else if (d == 3)heating-=10;
      else if (d == 4)heating+=10;
      else if (d == 5) break;
      if(heating >600)heating=0;
      else if(heating <0)heating=600;
      delay_ms(100);
   }
   
   while(1)
   {
      lcd_putc('\f');
      printf(lcd_putc," FIX TEMPERATURE ");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"HEATING: %lu",temperature);
      int8 d= buttons();
      if (d == 1)temperature++;
      else if (d == 2)temperature--;
      else if (d == 3)temperature-=10;
      else if (d == 4)temperature+=10;
      else if (d == 5) break;
      if(temperature >600)temperature=0;
      else if(temperature <0)temperature=600;
      delay_ms(100);
   }
}

void run ()
{
   read_temp();
   if( pre_temperature > 0 )
   {   
      while(1)
      {
         control(pre_temperature);   
         lcd_putc('\f');
         printf(lcd_putc,"  PRE->HEATING  ");
         lcd_gotoxy(1,2);
         printf(lcd_putc," %lu%cC -> %lu%cC ",b,1,pre_temperature,1);          
         if( q >=1 && b>pre_temperature) break;
         if(buttons() == 5){output_low(PIN_A1);break;}
         delay_ms(200);
      }
       while(1)
       {
         lcd_putc('\f');
         printf(lcd_putc,"  PRESS  ENTER  ");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"  TO  CONTINUE  "); 
         if(buttons() == 5)break;
         tono(4);
         if(buttons() == 5)break;
         delay_ms(1000);
         if(buttons() == 5)break;
         delay_ms(1000);
         if(buttons() == 5)break;
         delay_ms(1000);
         if(buttons() == 5)break;
         delay_ms(1000);
         if(buttons() == 5)break;
         delay_ms(1000);
         if(buttons() == 5)break;
         delay_ms(1000);
         if(buttons() == 5)break;
         delay_ms(1000);
         if(buttons() == 5)break;
         delay_ms(1000);
         if(buttons() == 5)break;
         delay_ms(1000);
         if(buttons() == 5)break;
         delay_ms(1000);
       }
   }
   
   if (temperature>0 && heating>0)
   {
      counter=0;
      seconds=0;
      minutes=0;
      while(1)
      {
         if(minutes < heating )
         {
            control(temperature);
            lcd_putc('\f');
            printf(lcd_putc,"    HEATING     ");
            lcd_gotoxy(1,2);
            printf(lcd_putc,"%lu%cC -> %lu%cC ",b,1,temperature,1);  
            if(buttons() == 5){output_low(PIN_A1);break;}
            delay_ms(200);
         }
         else break;
         
      }
       
   }
   
}

void end ()
{
   while(1)
   {
      output_low(PIN_A1);
      read_temp();
      lcd_putc('\f');
      printf(lcd_putc,"    FINISH     ");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"     %lu%cC     ",b,1); 
      tono(3);
      if(buttons() == 5)break;
      delay_ms(1000);
      if(buttons() == 5)break;
      delay_ms(1000);
      if(buttons() == 5)break;
      delay_ms(1000);
      if(buttons() == 5)break;
      delay_ms(1000);
      if(buttons() == 5)break;
      delay_ms(1000);
      if(buttons() == 5)break;
      delay_ms(1000);
      if(buttons() == 5)break;
      delay_ms(1000);
      if(buttons() == 5)break;
      delay_ms(1000);
      if(buttons() == 5)break;
      delay_ms(1000);
      if(buttons() == 5)break;
      delay_ms(1000);
   }
   
}
