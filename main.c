/********************************************

This is the firmware for the MagicSwitchBoard
written by David Smith for the PIC16F684
Date: 9/9/2011

*********************************************/

#include <pic.h>
 
//FOSC_INTOSCIO -> use internal oscillator and let CLKIN & CLKOUT be digital I/O
__CONFIG(FOSC_INTOSCIO & PWRTE_OFF & BOREN_OFF & CP_OFF & CPD_OFF & WDTE_OFF & MCLRE_OFF & IESO_OFF & FCMEN_OFF);

void main()
{
	//leave OSCCON at default for 4MHz

	//Setup Interrupts
	INTCONbits.RAIF = 0; //clear PORTA I-O-C flag
	INTCONbits.GIE = 1; //set Global Interrupt Enable
	INTCONbits.RAIE = 1; //set PORTA External Interrupt Enable	

	//Set PORTA as input for switches
	// note that RA3 is always an input.
	PORTA = 0x00; //clear PORTA
	ANSEL = 0x00; //PORTA & PORTC assigned as digital I/O
	CMCON0 = 0x07;
	//WPUA = 0x00; //Disable weak pull-ups, our switch setup doesn't need them
	IOCA = 0b000001; //set RA0 as interrupt-on-change
	TRISA = 0x01; //set RA0 as inputs

	//Set PORTC as output
	PORTC = 0x00; //clear PORTC
	TRISC = 0x00; //set all as outputs
	


	
	//Initial blinking light show	
	unsigned int i;	
	unsigned int delay = 15000;

	while(1==1)
	{
		NOP();
		for(i=0; i<delay; i++);
		NOP();
		RC1 = RC1 ^ 1;
	}		
}

void interrupt sw_int(void)
{
//	Psuedocode
//	If button interrupt
	if(INTCONbits.RAIF == 1)
	{
		button_debounce(); //wait for bouncing to stop
		INTCONbits.RAIF = 0;  //reset int_flag
		
//		for each changed button
//			if table miss
//				count++
//				set in sw_array for bouncing check later
//				set in led_array equal to button value
//				set table.swnum equal to count
//			if table hit
//				set sw_array
//				set in led_array equal to button value
//		if sw_array all low
//			enable/reset timer
//		elseif sw_array not all low
//			disable timer
//
//	if timer interrupt
//		reset sw_array, led_array, table	
	

	set leds to led_array
	unsigned int j;
	NOP();
	for(j=0; j<500; j++);
	NOP();
	RC3 = PORTAbits.RA0;
	INTCONbits.RAIF = 0;
}

/*
void button_debounce(void)
{
	debouncing = true
	while(debouncing)
	{
		if(old_array != new_array)
			reset timer
		elseif(timer < waitperiod)
			timer++
		elseif(timer >waitperiod)
			debouncing = false
	}
}
*/