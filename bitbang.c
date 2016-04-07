//--------------------------------------------------------------------
//- Name:	kevin Brenneman Michael Stumpf
//- Date:	Spring 2014
//-
//- Purp: 		figure out Bit bang for pwm for final project
//-
//- Academic Integrity Statement: I certify that, while others may have
//- assisted me in brain storming, debugging and validating this program,
//- the program itself is my own work. I understand that submitting code
//- which is the work of other individuals is a violation of the course
//- Academic Integrity Policy and may result in a zero credit for the
//- assignment, or course failure and a report to the Academic Dishonesty
//- Board. I also understand that if I knowingly give my original work to
//- another individual that it could also result in a zero credit for the
//- assignment, or course failure and a report to the Academic Dishonesty
//- Board.
//------------------------------------------------------------------------
#include <p18F26k22.h>

/* Set up the configuration bits */
#pragma config FOSC = INTIO67
#pragma config PLLCFG = ON			// Multiply the internal oscillator speed by 4
#pragma config PRICLKEN = OFF		// Primary clock enabled
#pragma config FCMEN = OFF			// Fail-Safe Clock Monitor enabled
#pragma config IESO = OFF			// Oscillator Switchover mode disabled
#pragma config PWRTEN = ON			// Power up timer enabled
#pragma config BOREN = OFF			// Brown-out Reset disabled in hardware and software 
#pragma config BORV = 285			// VBOR set to 2.85 V nominal 
#pragma config WDTEN = OFF			// Watch dog timer is always disabled. SWDTEN has no effect. 
#pragma config WDTPS = 1			// 1:1 Watchdog timer postscalar 			
#pragma config CCP2MX = PORTC1		// CCP2 input/output is multiplexed with RC1  
#pragma config PBADEN = OFF			// PORTB<5:0> pins are configured as digital I/O on Reset 
#pragma config CCP3MX = PORTB5		// P3A/CCP3 input/output is multiplexed with RB5  
#pragma config HFOFST = OFF			// HFINTOSC output and ready status are delayed by the oscillator stable status  
#pragma config T3CMX = PORTC0		// T3CKI is on RC0  
#pragma config P2BMX = PORTC0		// P2B is on RC0 
#pragma config MCLRE = EXTMCLR		// MCLR pin enabled, RE3 input pin disabled  
#pragma config STVREN = OFF			// Stack full/underflow will not cause Reset 
#pragma config LVP = OFF			// Single-Supply ICSP disabled 
#pragma config XINST = OFF			// Instruction set extension and Indexed Addressing mode disabled (Legacy mode) 
#pragma config DEBUG = ON			// Block 0 (000800-003FFFh) code-protected 


#define _XTAL_FREQ = 64000000

typedef unsigned char int8;
typedef unsigned int  int16;
typedef unsigned long int32;


/*
//----------------------------------------------------
// Set the vector for the low priority interrupt
//----------------------------------------------------
void high_isr(void);
#pragma code high_vector=0x08
void interrupt_at_high_vector(void) {
	_asm GOTO high_isr _endasm
}
#pragma code 
*/
void INIT_PIC (void);



int8 pwm_channel[4];
int8 duty_cycle[4];

//----------------------------------------------
// Main "function"
//----------------------------------------------
void main (void) {
	INIT_PIC();
    
    pwm_channel[0] = PORTAbits.RA0;
    pwm_channel[1] = PORTAbits.RA1;
    pwm_channel[2] = PORTAbits.RA2;
    pwm_channel[3] = PORTAbits.RA3;
    
    duty_cycle[0] = 0;
    duty_cycle[1] = 45;
    duty_cycle[2] = 90;
    duty_cycle[3] = 180;

	while(1);
    //put real logic here

} // end main

//----------------------------------------------
// INIT_PIC
//----------------------------------------------
void INIT_PIC (void) {

	OSCCONbits.IRCF2 = 1;		// Setup a 64Mhz internal
	OSCCONbits.IRCF1 = 1;
	OSCCONbits.IRCF0 = 1;
	OSCTUNEbits.PLLEN = 1;	

	TRISAbits.TRISA3 = 1;		// Upper button is a digital
	ANSELAbits.ANSA3 = 0;

	TRISCbits.TRISC1 = 0;		// RC1 is GPIO output
	TRISCbits.TRISC2 = 0;		// RC2 is GPIO output

	T0CON = 0;					// Funky power-on defaults, see page 159
	TMR0L = 0;
	T0CONbits.PSA = 0;			// Assign prescalar to TMR0
	T0CONbits.T0PS2 = 0;		// 001 = 1:4		101 = 1:64
	T0CONbits.T0PS1 = 1;		// 010 = 1:8		110 = 1:128
	T0CONbits.T0PS0 = 0;		// 011 = 1:16		111 = 1:256
	T0CONbits.T08BIT = 1;		// 8-bit mode is legacy for older devices
	T0CONbits.TMR0ON = 1;		// best to configure devices then turn them on
    
    T2CONbits.TMR2ON =0;        //turn off timer 2
    T2CONbits.T2CKPS0 = 1;      //set the prescaler
    T2CONbits.T2CKPS1 = 0;      //set the prescaler
    PIE1bits.TMR2IE =0;         //ensure that interrupts are disabled
    T2CONbits.TMR2ON =1;        //turn on timer 2
    

	INTCONbits.TMR0IF = 0;		// Clear interrupt flag
	INTCONbits.TMR0IE = 1;		// Enable TMR0 interrupt

	INTCONbits.GIE = 1;			// Enable global interrupts


}

//-----------------------------------------------------------------------------
//interrupt vector
//-----------------------------------------------------------------------------
//#pragma interrupt high_isr
void interrupt ISR(void) {
    static int8 state =0;
    INTCONbits.TMR0IF =0;

    if(state == 0){
    // have all the pins go high
    for (int j = 0; j < 4; j++) {
        pwm_channel[j] = 1;
    }
    //wait 1ms
    state = 1;
    }
    
    
    if(state ==1)
    {
    //__delay_ms(1.0);

    //this entire for loop will take 2 ms
    for (int i = 0; i < 181; i++) {
        for (int j = 0; j < 4; j++) {
            if (duty_cycle[j] < i)
                pwm_channel[j] = 0;
            else pwm_channel[j] = 1;
        }
        
        //delay 11us  
    }
    state =2;
    }
    if (state ==2)
    {
    WRITETIMER0(31536);
    state =0;
    }

} // end tmr0_isr
