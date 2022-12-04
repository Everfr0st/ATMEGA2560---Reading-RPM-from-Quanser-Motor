
#include <avr/io.h>
#include <util/delay.h>
#define LCD_PORT PORTH
#define LCD_DDR  DDRH
#define LCD_RS 0                 
#define LCD_RW 1                       // Mandado a tierra para sol escritura
#define LCD_EN 2

void lcdcommand(unsigned char cmnd)
{
	LCD_PORT = (LCD_PORT & 0x0f)|(cmnd & 0xf0);		// Se utilizan 240 bits para los datos y 15 para control
	LCD_PORT &= ~ (1<<LCD_RS);						// RS en 0 para los comandos 
	LCD_PORT |= (1<<LCD_EN);						/* EN = 1 FOR H TO L PULSE */
	_delay_us(1);									/* WAIT FOR MAKE ENABLE WIDE */    //bt especifico 
	LCD_PORT &= ~(1<<LCD_EN);						/* EN = 0 FOR H TO L PULSE */
	_delay_us(100);									/* WAIT FOR MAKE ENABLE WIDE */
	
	LCD_PORT = (LCD_PORT & 0x0f)|(cmnd << 4);		/* SEND COMMAND TO DATA PORT */
	LCD_PORT |= (1<<LCD_EN);						/* EN = 1 FOR H TO L PULSE */
	_delay_us(1);									/* WAIT FOR MAKE ENABLE WIDE */
	LCD_PORT &= ~(1<<LCD_EN);						/* EN = 0 FOR H TO L PULSE */
	_delay_us(100);									/* WAIT FOR MAKE ENABLE WIDE */
}

void lcddata(unsigned char data)
{
	LCD_PORT = (LCD_PORT & 0x0f)|(data & 0xf0);		/* SEND DATA TO DATA PORT */
	LCD_PORT |= (1<<LCD_RS);						/* RS = 1 FOR DATA */
	LCD_PORT |= (1<<LCD_EN);						/* EN=0 FOR H TO L PULSE */
	_delay_us(1);									/* WAIT FOR MAKE ENABLE WIDE */
	LCD_PORT &= ~(1<<LCD_EN);						/* EN = 0 FOR H TO L PULSE */
	
	
	LCD_PORT = (LCD_PORT & 0x0f)|(data << 4);		/*    la coneXion es de 4 bits  */
	LCD_PORT |= (1<<LCD_EN);						/* EN=0 FOR H TO L PULSE*/
	_delay_us(1);									/* WAIT FOR MAKE ENABLE WIDE*/
	LCD_PORT &= ~(1<<LCD_EN);						/* EN = 0 FOR H TO L PULSE*/
	_delay_us(100);									/* WAIT FOR MAKE ENABLE WIDE*/
}

void lcdinit()
{
	LCD_DDR = 0xFF;
	LCD_PORT &= ~(1<<LCD_EN);                      // LCD_EN = 0
	_delay_ms(50);									/* WAIT FOR SOME TIME */
	lcdcommand(0x33);
	lcdcommand(0x32);								/* SEND $32 FOR INIT OT 0X02 */
	lcdcommand(0x28);								/* INIT. LCD 2 LINE, 5 X 7 MATRIX */
	lcdcommand(0x0C);								/* DISPLAY ON CURSOR ON */ 
	lcdcommand(0x01);								/* LCD CLEAR */
	_delay_us(2000);
}

void posicion(unsigned char x, unsigned char y)
{
	unsigned char firstcharadd[]={0x80, 0xC0};
	lcdcommand(firstcharadd[y] + x);
}

void lcd_print(const char *str)
{
	uint8_t i;
	// Que pasa si el compilador traduce una cadena de caracteres
	while ( (i = *str++) )
	 {
		lcddata(i);
		
	}
}




void lcd_clear()
{
	lcdcommand(0x01);
	_delay_ms(2);
}




void convertir_ASCII(float numero)
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
	
	while ( dig[j] != '\0' )   //
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
		
		if (dig[i]<=57&&dig[i]>=46)    //Numero de 8 a 9
		{
			
			lcddata(dig[i]);
		}
		
	}



}