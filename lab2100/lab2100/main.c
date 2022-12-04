/*
 * lab2100.c
 *
 * Created: 10/05/2022 6:28:44 p. m.
 * Author : Keiry Mariana
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LCD_4.c"
#include "USART_VT2.C"


float  Xi_1;
float  Sum_Xi_1;
float  Vrms;
float  Vrms_true;
float  Cont;


int main(void)
{
	    DDRH=0xFF;
		
	    lcdinit();
	    InitUSART(19200);
	
		
		ADMUX= 0x60;                      // 1 en al REFS0 Y ADLAR
		ADCSRA=  0x8F;             //ADEN   ADIF = 128 prescalerS
		ADCSRA |= (1<<ADSC);           // Start conversion
		
// 		
		asm("LDI R16,1<<2|0<<1|0<<0	    ; PRESCALER DE 256");
		asm("STS 0x81,R16				; AQUÍ INICIA EL TIMER");	// 0x81 = TCCR1B
		asm("LDI R16,0x98				; SE PRECARGA EL TIMER 1");
		asm("STS 0x85,R16				; CON 0x9840");		// 0x85 = TCNT1H
		asm("LDI R16,0x40				; VALOR NECESARIO PARA");
		asm("STS 0x84,R16				; DESBORDAR A 1,66 ms");		// 0x84 = TCNT1L
		asm("LDI R16,1<<0				; HABILITACIÓN DE INT");
		asm("STS 0x6F,R16				; DE TIMER 1");				// 0x6F = TIMSK1
		
       //SMCR = 1;						// Habilita el modo Sleep para reducir el consumo de potencia");
		
		sei();

  
    while (1)
    {
       //asm("SLEEP");
	   asm("NOP");
    }
}
//  
 ISR(TIMER1_OVF_vect
 ){
	     	asm("LDI R16,0x98");
	     	asm("STS 0x85,R16");		// 0x85 = TCNT1H
	     	asm("LDI R16,0x40");
	     	asm("STS 0x84,R16");		// 0x84 = TCNT1L
		
		 
		  Cont = Cont + 1;
		  if (Cont == 15)
		  {
			  Vrms = Sum_Xi_1/15;
			  Vrms_true = sqrt(Vrms);

			  Cont = 0;
			  Sum_Xi_1 = 0;

		  }
		  
		ADCSRA |= (1<<ADSC);           //Cuando finalice conversion inice otra conversion
		
		convertir_U(Vrms_true);
		EnviarU(32);
		
				posicion(0,0);
				lcd_print("VOLTAJE RMS:");

				posicion(0,1);
				convertir_ASCII(Vrms_true);
 }

  ISR(ADC_vect){
	     // Fin de conversion
	   	   Xi_1 =ADCH*0.02;     // *5/255
		   Sum_Xi_1 = Sum_Xi_1 + (Xi_1*Xi_1);
   
		          
  }


