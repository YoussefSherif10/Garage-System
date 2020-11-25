
#include <avr/io.h>
#include "BIT_MATH.h"
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include"util/delay.h"

#define no_free_slots 0
#define max_free_slots 90 

/*global variables*/
unsigned short free_slots_number = 90 ;	

/*functions declarations*/
void SEVEN_SEGMENT(void);

int main(void)
{
	/*configurations*/
	
	SET_BIT(DDRC , 0);
	SET_BIT(DDRC , 1);
	SET_BIT(DDRC , 2);
	SET_BIT(DDRC , 3);			// set PC0 --> PC1 as output
	
	SET_BIT(DDRD ,  0) ;			// set PD0 as output
	SET_BIT(PORTD , 2);
	SET_BIT(PORTD , 3);			//PD2 , PD3 are gate sensors , work with pull up resistance
	
	SET_BIT(DDRA , 0);
	SET_BIT(DDRA , 1);
	SET_BIT(DDRA , 2);			
	SET_BIT(DDRA , 3);
	SET_BIT(DDRA , 4);
	SET_BIT(DDRA , 5);			//set PA0 , PA5 as output
	
	SET_BIT(DDRB , 0);
	SET_BIT(DDRB , 1);
	SET_BIT(DDRB , 2);
	SET_BIT(DDRB , 3);			//set PD0 --> PD1 as output
	
	SET_BIT(GICR , INT0);		//enable external interrupt 0
	SET_BIT(GICR , INT1);		//enable external interrupt 1
	sei();						//enable interrupt
	
	SEVEN_SEGMENT();
	
    while (1) 
    {
		
	}
    
}

/*seven segment function definition*/
void SEVEN_SEGMENT(void)
{
	PORTB = 0 ;					//disable first digit
	PORTC = 0 ;					//disable second digit
	PORTB = free_slots_number % 10 ;	//get first digit
	PORTC |= free_slots_number / 10 ;	//get second digit
}

/*entrance gate*/
ISR(INT0_vect)
{
	if(free_slots_number > no_free_slots)
	{
		SET_BIT(PORTA , 2);		//power the motor
		SET_BIT(PORTA , 0);
		CLR_BIT(PORTA , 1);     //rotate in open direction
		_delay_ms(3000);		//wait for 3 second
		CLR_BIT(PORTA , 2);		//close the motor
		_delay_ms(500);			//wait half a second 
		SET_BIT(PORTA , 2);		//power the motor
		CLR_BIT(PORTA , 0);
		SET_BIT(PORTA , 1);		//rotate in close direction
		_delay_ms(3000);		//wait 3 seconds
		CLR_BIT(PORTA , 2);		//shut down the motor
		free_slots_number-- ;	//decrement the number of the cars
	}
	SEVEN_SEGMENT();
	
	if(free_slots_number == no_free_slots)
	{
		SET_BIT(PORTD , 0);	 //turn on the LED
	}
}

/*exit gate*/
ISR(INT1_vect)
{
	if(free_slots_number < max_free_slots)
	{
		SET_BIT(PORTA , 3);		//power the motor
		SET_BIT(PORTA , 4);
		CLR_BIT(PORTA , 5);		//rotate in open direction
		_delay_ms(3000);		//wait for 3 second
		CLR_BIT(PORTA , 3);		//shut down the motor
		_delay_ms(500);			//wait half a second
		SET_BIT(PORTA , 3);		//power the motor
		CLR_BIT(PORTA , 4);		
		SET_BIT(PORTA , 5);		//rotate in close direction
		_delay_ms(3000);		//wait for 3 seconds
		CLR_BIT(PORTA , 3);		//shut down the motor
		free_slots_number++ ;	//increment the number of the cars
	}
	SEVEN_SEGMENT();
	CLR_BIT(PORTD , 0);			//turn off the LED
}

