#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void InitUSART(int velocidad);
void EnviarU(uint8_t dato);
void convertir_U(float numero);


void convertir_U(float numero)
{
	
	
	uint8_t dig[5]={};
	//Dígito Centena
	dig[0]=(numero/100);
	//Dígito Decena
	dig[1]=((numero-dig[0]*100)/10);
	// Digito unidad
	dig[2]=((numero-dig[0]*100-dig[1]*10));
	// Decimal
	dig[4]=((numero-dig[0]*100-dig[1]*10-dig[2])*10);

	
	dig[0]+=48;
	dig[1]+=48;
	dig[2]+=48;
	
	dig[3]=46;

	dig[4]+=48;

	
	uint8_t j=0;
	
	while ( dig[j] != '\0' )   // Va aumentar hasta que llegue al final del arreglo - en C 
	{
		j++;
	}
	
	if(numero<1){       //Lo de los espacios
		dig[0]=32;
		dig[1]=32;
		}else{
		if(numero<10){
			dig[0]=32;
			dig[1]=32;
			}else{
			if(numero<100){
				dig[0]=32;
			}
		}
		
	}

	for(int i=0;i<j;i++){
		
		if (dig[i]<=57&&dig[i]>=46)
		{
			
			EnviarU(dig[i]);
		}
		
	}
}

void InitUSART(int velocidad)
{
	//Habilitar
	UCSR3B|=1<<TXEN3;  
	UCSR3B|=1<<RXEN3;        //Para la recepcion hay que habilitarlo

	//modo
	UCSR3C|=0<<6;//ASINCRONO   // Or - mascara a ese registro en modo asincrono, no señal de reloj
	
	//paridad 
	UCSR3C|=0<<4;              // saber si esta recibiendo lo mismo que se esta enviando
	
	UCSR3C|=0<<3;               // Bit de parada, separar bits 
	
	//Numero de bits
	UCSR3C|=3<<1;              // CONFIGURACIÓN A 8 BITS
	
	// Habilitar la escritura
	UCSR3B|=1<<RXCIE3;          // la bandera si se ha leido o no
	UCSR3A|=0<<U2X3;            // velocidad  
	
	//velocidad
	UBRR3=(F_CPU/8/velocidad)-1;                 
	
	

}


void EnviarU(uint8_t dato)
{
	while(!(UCSR3A & (1<<UDRE3)));  // esperar para enviar otro - 0 vacio
	UDR3=dato;
}


