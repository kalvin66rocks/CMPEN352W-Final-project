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

#include <xc.h>
#include <stdio.h>
#include <pic18f26k22.h>

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

//---------------------------
//defines 
//---------------------------
#define _XTAL_FREQ   64000000   //not sure if you needed the equal sign I took it out

// define times
#define one_ms       63536
#define one_half_ms  62536 
#define two_ms       61536
#define seventeen_ms 31536
#define five_us    65525

//define pwm channels 
#define pwm1  
#define pwm2
#define pwm3 
#define pwm4 
//---------------------------
// type define 
//---------------------------
typedef unsigned char int8;
typedef unsigned int  int16;
typedef unsigned long int32;

//---------------------------
// functions
//---------------------------
void INIT_PIC (void);

//---------------------------
// globals
//---------------------------
int8 pwm_channel[4];
int8 duty_cycle[4];

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

//----------------------------------------------
// Main "function"
//----------------------------------------------
void main (void) {
    
    //local variables 
    char direction = 'f';
    int8 direction_state = 1;
    
	INIT_PIC();
    
    // set channels to respective pins
    //pwm_channel[0] = PORTAbits.RA0;
    //pwm_channel[1] = PORTAbits.RA1;
    //pwm_channel[2] = PORTAbits.RA2;
    //pwm_channel[3] = PORTAbits.RA3;
    
    // default angles 
    duty_cycle[0] = 45;
    duty_cycle[1] = 45;
    duty_cycle[2] = 45;
    duty_cycle[3] = 45;

	while(1){
    //put real logic here
    if (PIR3bits.RC2IF) {				    // wait for the receive flag to be set
			PIR3bits.RC2IF = 0;				// clear the flag for the next read
            
            // control switch 
			switch (RCREG2) {				// and do one of the following based on that key

			//--------------------------------------------
			// Reply with help menu
			//--------------------------------------------
			case '?':
                printf("\r\n");
				printf("-------------HELP------------\r\n");
				printf("?: help menu\r\n");
				printf("o: k\r\n");
				break;

			//--------------------------------------------
			// Reply with "k", used for PC to PIC test
			//--------------------------------------------
			case 'o':
				printf("o:	ok\r\n");
				break;
            //--------------------------------------------
            // increment/decrement duty cycle 
			// Reply with red duty cycle value 
			//--------------------------------------------
            case'i':
                printf("light should be on");
                LATCbits.LATC1 ^= 1; 
                break;
			//--------------------------------------------
			// If something unknown is hit, tell user
			//--------------------------------------------
			default:
				printf("Unknown key %c\r\n",RCREG2);
				break;

			} // end switch
			printf("> ");		// print a nice command prompt for the user
            
            // state switch switch 
            switch(direction){
                
            //--------------------------------------------
			// forward state 
			//--------------------------------------------
                case 'f':
                    
                    if(direction_state = 0){
                        duty_cycle[0] = 180;        // test angles 
                        duty_cycle[1] = 45;
                        duty_cycle[2] = 180;
                        duty_cycle[3] = 45;
                    }
                    else{
                        duty_cycle[0] = 45;        // test angles 
                        duty_cycle[1] = 180;
                        duty_cycle[2] = 45;
                        duty_cycle[3] = 180;
                    }
                   
            //--------------------------------------------
			// right state 
			//--------------------------------------------
                case 'r':
                    
                    if(direction_state = 0){
                        duty_cycle[0] = 180;        // test angles 
                        duty_cycle[1] = 45;
                        duty_cycle[2] = 180;
                        duty_cycle[3] = 45;
                    }
                    else{
                        duty_cycle[0] = 45;        // test angles 
                        duty_cycle[1] = 180;
                        duty_cycle[2] = 45;
                        duty_cycle[3] = 180;
                    }
            //--------------------------------------------
			// left state 
			//--------------------------------------------
                    
            //--------------------------------------------
			// backward state 
			//--------------------------------------------
                
                
            }
            
            
     } // end if 
    } //end while 
} // end main

//----------------------------------------------
// INIT_PIC
//----------------------------------------------
void INIT_PIC (void) {

	OSCCONbits.IRCF2  = 1;		  // Setup a 64Mhz internal
	OSCCONbits.IRCF1  = 1;
	OSCCONbits.IRCF0  = 1;
	OSCTUNEbits.PLLEN = 1;	
    
    // ---------------Setup the serial port------------------
	// Aiming for a baud rate of 9600
	// BAUD = FOSC/[64*(SPBRGH+1)]
	// SPBRGH = 64Mhz/ 9.6k/64 =  104
	TXSTA2bits.TXEN = 1;
	TXSTA2bits.SYNC = 0;
	TXSTA2bits.BRGH = 0;
	BAUDCON2bits.BRG16 = 0;
	RCSTA2bits.CREN = 1;
	SPBRG2 = 104;
	RCSTA2bits.SPEN = 1;    
    
    //pin config 
    TRISCbits.TRISC1 = 0;         // RC1 is GPIO output 
	TRISAbits.TRISA0 = 0;		  // RA0 is GPIO output
	TRISAbits.TRISA1 = 0;		  // RA1 is GPIO output
    TRISAbits.TRISA2 = 0;         // RA2 is GPIO output
    TRISAbits.TRISA3 = 0;         // RA3 is GPIO output 
    
    //Timer 0 
	T0CON = 0;					  // Funky power-on defaults, see page 159
	TMR0L = 0;
	T0CONbits.PSA    = 0;		  // Assign pre scalar to TMR0
	T0CONbits.T0PS2  = 0;		  // 001 = 1:4		101 = 1:64
	T0CONbits.T0PS1  = 1;		  // 010 = 1:8		110 = 1:128
	T0CONbits.T0PS0  = 0;		  // 011 = 1:16		111 = 1:256
	T0CONbits.T08BIT = 0;		  // 8-bit mode is legacy for older devices
	T0CONbits.TMR0ON = 1;		  // best to configure devices then turn them on
    TMR0 = seventeen_ms;          // set initial count value
    //Timer 1
    T1CONbits.TMR1ON  = 0;        //turn off timer 1
    T1CONbits.T1CKPS0 = 1;        //set the pre scaler
    T1CONbits.T1CKPS1 = 1;        //set the pre scaler 11 = 8 bit ';
    PIE1bits.TMR1IE   = 0;        //ensure that interrupts are disabled
    T1CONbits.TMR1ON  = 1;        //turn on timer 1
    

	INTCONbits.TMR0IF = 0;		  // Clear interrupt flag
	INTCONbits.TMR0IE = 1;		  // Enable TMR0 interrupt

	INTCONbits.GIE = 1;			  // Enable global interrupts


}

//-----------------------------------------------------------------------------
//Bit banging
//-----------------------------------------------------------------------------
void interrupt ISR(void) {
    
    INTCONbits.TMR0IF = 0;          // clear flag 
    LATCbits.LATC1 ^  = 1;			// toggle pin RC1
    
    //turn A0 - A3 on 
    LATAbits.LATA0 = 1;
    LATAbits.LATA1 = 1;
    LATAbits.LATA2 = 1;
    LATAbits.LATA3 = 1;
    
    
    // use timer 1 to delay 1ms 
    TMR1 = one_ms;
    while(PIR1bits.TMR1IF == 0);
    PIR1bits.TMR1IF = 0; 
         
    //this entire for loop will take 1ms
    for (int i = 0; i < 180; i++) {
        
        //adjust duty cycle of each pin 
        // duty RA0
        if (duty_cycle[0] < i)
             LATAbits.LATA0 = 0;
        else LATAbits.LATA0 = 1;
        //duty  RA1
        if (duty_cycle[1] < i)
             LATAbits.LATA1 = 0;
        else LATAbits.LATA1 = 1;
        //duty RA2
        if (duty_cycle[2] < i)
             LATAbits.LATA2 = 0;
        else LATAbits.LATA2 = 1;
        //duty RA3
        if (duty_cycle[3] < i)
             LATAbits.LATA3 = 0;
        else LATAbits.LATA3 = 1;
        
        // delay for 5us 
        PIR1bits.TMR1IF = 0;          // clear incase 
        TMR1 = five_us;           
        while(PIR1bits.TMR1IF == 0);    
        PIR1bits.TMR1IF = 0;
    }
   
    LATCbits.LATC1 ^= 1;			  // toggle pin so 
    TMR0 = seventeen_ms;
    
} // end tmr0_isr


//-----------------------------------------------------------------------------
// Helper function needed to point PRINTF to the second USART
//-----------------------------------------------------------------------------
void putch(char c) {
    
    while( ! TX2IF)
        continue;
    TX2REG = c;

}