/********************************************

This is the firmware for the MagicSwitchBoard
written by David Smith for the PIC16F684
Date: 9/9/2011

*********************************************/

#include <pic.h>
 
//FOSC_INTOSCIO -> use internal oscillator and let CLKIN & CLKOUT be digital I/O
//MCLRE_OFF -> use RA3 as digital input rather than MCLR
__CONFIG(FOSC_INTOSCIO & PWRTE_OFF & BOREN_OFF & CP_OFF & CPD_OFF & WDTE_OFF & MCLRE_OFF & IESO_OFF & FCMEN_OFF);


/////////////////DEFINE GLOBAL VARIABLES///////////////////

#define TRUE 1
#define FALSE 0
#define MEMORIZE 101
#define RECALLEXPIRE 105
#define TRICK1 0 
#define TRICK2 1
#define TRICK3 2

//typedef union
//{
//	unsigned char byte;
//	struct{
//		unsigned bit0:1;
//		unsigned bit1:1;
//		unsigned bit2:1;
//		unsigned bit3:1;
//		unsigned bit4:1;
//		unsigned bit5:1;
//		unsigned bit6:1;
//		unsigned bit7:1;
//	}bits;	
//} bit_field;
//
//bit_field sw_state_new = {FALSE}, sw_state_old = {FALSE}, led_state = {FALSE};

bit sw_state_new[8]={0}, sw_state_old[8]={0}, led_state[8]={0};

char table[4] = {-1, -1, -1, -1};

char mode = TRICK1, state = 0, NUM_LEDS = 4, NUM_SWTCHS = 4;

/////////////////FUNCTION DECLARATIONS///////////////////

void select_mode(void);
void reset_table(void);
void reset_timer(void);
void trick1(void);

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
	IOCA = 0x0F; //set RA0, RA1, RA2, RA3 as interrupt-on-change
	TRISA = 0x0F; //set RA0, RA1, RA2, RA3 as inputs

	//Set PORTC as output for LEDs
	PORTC = 0x00; //clear PORTC
	TRISC = 0x00; //set all as outputs
	
//	Initial blinking light show	
	unsigned int i;	
	unsigned int delay = 15000;
//
//	while(1==1)
//	{
//		NOP();
//		for(i=0; i<delay; i++);
//		NOP();
//		RC1 = RC1 ^ 1;
//	}	

	

	select_mode();
	while( *(unsigned char *) sw_state_new)
	{
		if(mode == 0)
		{
			NOP();
			for(i=0; i<delay; i++);
			NOP();
			RC0 = RC0 ^ 1;
		}
		else if(mode == 1)		
		{
			NOP();
			for(i=0; i<delay; i++);
			NOP();
			RC1 = RC1 ^ 1;
		}
		else if(mode == 2)
		{
			NOP();
			for(i=0; i<delay; i++);
			NOP();
			RC2 = RC2 ^ 1;
		}
		else if(mode == 3)
		{
			NOP();
			for(i=0; i<delay; i++);
			NOP();
			RC3 = RC3 ^ 1;
		}
	}
	
	switch(mode)
	{
		case TRICK1:
			mode0()
			break;
		default:
			while(TRUE)
			{
				NOP();
				for(i=0; i<delay; i++);
				NOP();
				RC0 = RC0 ^ 1;	
				RC1 = RC1 ^ 1;		
				RC2 = RC2 ^ 1;		
				RC3 = RC3 ^ 1;	
			}
			break;
	}
}

void interrupt sw_int(void)
{
//	set leds to led_array
	unsigned int j;
	*(unsigned char *) sw_state_old = *(unsigned char *) sw_state_new;
	NOP();
	for(j=0; j<500; j++);
	NOP();
	sw_state_new[0] = RA0;
	sw_state_new[1] = RA1;
	sw_state_new[2] = RA2;
	sw_state_new[3] = RA3;
	INTCONbits.RAIF = 0;
}

void select_mode(void)
{
	sw_state_new[0] = RA0;
	sw_state_new[1] = RA1;
	sw_state_new[2] = RA2;
	sw_state_new[3] = RA3;
	mode = *(unsigned char *) sw_state_new;
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
		elseif(timer > waitperiod)
			debouncing = false
	}
}
*/

void trick1(void)
{
//	if new switch input
//		reset timer
//		is input assigned to led
//			if not
//				assign switch to led
//				toggle led
//			if so
//				toggle led
	reset_table();
	char index, led_index, timer_enable = FALSE, timer_start = FALSE;
	
	while(TRUE)
	{
		if(newinput)
		{
			reset_timer();
			for(index=0;index<NUM_SWTCHS;index++)
			{
				if(sw_state_new[index]!=sw_state_old[index])
				{
					for(led_index=0;led_index<NUM_LEDS;led_index++)
					{
						if(table[led_index]==index)
						{
							led_state[led_index] = led_state[led_index]^1;
							break;
						}	
						else if(table[led_index]==-1)
						{
							table[led_index]=index;
							led_state[led_index] = led_state[led_index]^1;
							if(led_index==(NUM_LEDS-1))
								timer_enable=TRUE;
							break;
						}		
					}
				}
			}
		}
		if(timer_enable==TRUE && sw_state_new==0)
		{
			timer_start = TRUE;
		}
	}
}

void reset_table(void)
{
	char i=4;
	for(i=0;i<4;i++)
		table[i]=-1;
}	

void reset_timer(void)
{}	