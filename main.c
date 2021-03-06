#include <avr/io.h>
#include <util/delay.h>
 
#define PIN_SCE   PORTD7
#define PIN_RESET PORTD6
#define PIN_DC    PORTD5
#define PIN_SDIN  PORTD4
#define PIN_SCLK  PORTD3

#define LCD_C     LOW
#define LCD_D     HIGH 
 
void data(uint8_t data);
void command(uint8_t data);
 
int main (void)
{
	DDRD |= 0b11111000; //Define LCD ports as output
	
	_delay_ms(200);
	PORTD &= ~(1<<PIN_RESET);
	_delay_ms(500);
	PORTD |= (1<<PIN_RESET);
	
	command(0x21);  // LCD Extended Commands.
	command(0xB6);  // Set LCD Vop (Contrast).//0xB1 
	command(0x04);  // Set Temp coefficent. //0x04
	command(0x13);  // LCD bias mode 1:48. //0x13
	command(0x0C);  // LCD in normal mode.
	command(0x20);
	command(0x80); //select X Address 0 of the LCD Ram
	command(0x40); //select Y Address 0 of the LCD Ram - Reset is not working for some reason, so I had to set these addresses
	command(0x0C);
	
	_delay_ms(500);
	while(1) {
		for(int i = 0; i<84*6; i++)
			data(0x31);
		
		
		/*
		PORTB |= (1<<PORTB5);
		_delay_ms(400);
		PORTB &= ~(1<<PORTB5);
		//_delay_ms(500);
		*/
	}
	return 0;
}

void data(uint8_t data){
	PORTD |= (1<<PIN_DC);
	PORTD &= ~(1<<PIN_SCE);
	PORTD &= ~(1<<PIN_SCLK);
	for(int i= 0; i < 8; i++){
		if(data > 0x7f){
			PORTD |= (1<<PIN_SDIN);
		}else{
			PORTD &= ~(1<<PIN_SDIN);
		}
		PORTD |= (1<<PIN_SCLK);
		PORTD &= ~(1<<PIN_SCLK);
		data = data<<1;
	}
	PORTD &= ~(1<<PIN_DC);
	PORTD |= (1<<PIN_SCE);
}

void command(uint8_t data){
	PORTD &= ~(1<<PIN_DC);
	PORTD &= ~(1<<PIN_SCE);
	PORTD &= ~(1<<PIN_SCLK);
	for(int i= 0; i < 8; i++){
		if(data > 0x7f){
			PORTD |= (1<<PIN_SDIN);
		}else{
			PORTD &= ~(1<<PIN_SDIN);
		}
		PORTD |= (1<<PIN_SCLK);
		PORTD &= ~(1<<PIN_SCLK);
		data = data<<1;
	}
	PORTD |= (1<<PIN_SCE);
}


// This block of code is converted from AdaFruit PCD8554 Library. My code above works fine, so I don't need this anymore.
// But still keeping in case
/*
void fastwrite(uint8_t d) {
  for(uint8_t bit = 0x80; bit; bit >>= 1) {
    PORTD &= ~(1<<PIN_SCLK);
    if(d & bit) PORTD |= (1<<PIN_SDIN);
    else        PORTD &= ~(1<<PIN_SDIN);
	PORTD |= (1<<PIN_SCLK);
  }
}

void command(uint8_t c) {
	PORTD &= ~(1<<PIN_DC);
	PORTD &= ~(1<<PIN_SCE);
	fastwrite(c);
	PORTD |= (1<<PIN_SCE);
}

void data(uint8_t c) {
	PORTD |= (1<<PIN_DC);
	PORTD &= ~(1<<PIN_SCE);
	fastwrite(c);
	PORTD |= (1<<PIN_SCE);
}
*/