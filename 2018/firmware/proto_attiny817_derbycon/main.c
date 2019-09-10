/*
 * Prototype DerbyCon VIII (2018) Trevor Badge on ATTiny817
 * Author : Hyr0n
 * Special Thanks: April Wright for hooking up Zapp and Hyr0n tix to their very first DerbyCon!
 * Special Thanks: Tataylino for his demo on ATTiny817 UART http://tataylino.com/attiny817-uart-with-putty/
 * The badge works in a manner of 4 states (0..3) which must be achieved in a specific order
 * State 0 - Initially the lights blink RED/ORANGE slowly, requires that a trace be cut to proceed to State 1
 * State 1 - The lights blink BLUE/GREEN faster, requires that thermomister be cooled to freezing temp to proceed to State 2
 * State 2 - The lights blink PURPLE/YELLOW even faster, requires that the user type "t smashburger" at the console which is ROT8 of "keskztmjywj" printed on the badge to proceed to State 3
 * State 3 - The lights blink a full RGB rainbow at fastest speed, challenge is done
 */ 

#define F_CPU 20000000UL  // 20 MHz
#define NEWLINE_ON 1
#define NEWLINE_OFF 0
#define ENABLE 1
#define DISABLE 0
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

double FOOT_TEMP = 55.0; //THIS IS A PLACEHOLDER GLOBAL FOR THE TEMP UNTIL 816 INTEGRATION WITH THERMOMISTER
int STATE = 0;	
int x = 0;
char msg_rx[];

ISR(USART0_RXC_vect) { // This kicks off when any key is pressed
	char value = 0;
	value = USART0_RXDATAL;
	if(value != "")	{
		msg_rx[x] = value;
		USART_SendByte(msg_rx[x]);			//send back the character typed to display
		if(value != 8 && value != 127)		// if backspace/delete is pressed,
		{ x++; } else
		{ x--; }				
		if(value == '\r')					//if enter is pressed, process command
		{
			USART_SendByte('\n');			// new line
			check_cmd(msg_rx);				// check for command request
			x = 0;
			send_msg("AND!XOR> ", NEWLINE_OFF); // add a command line prompt
		}
	}
}

int load_state() {
	//TODO this should load the STATE variable from internal flash such that there is persistence
	return 0;
}

void save_state() {
	//TODO this should save the STATE variable to internal flash such that there is persistence
}

void check_cmd(char msg[]) {
	char cmd;
	char chk_space;
	cmd = msg[0];			// get first character
	chk_space = msg[1];		// get second character -should be space
	
	if((chk_space == ' ') || (cmd == 'h' || cmd == 'a' || cmd == 't')) {
		switch(cmd)	{
			case 'h':		// Help
				send_msg("\n\rAND!XOR DerbyCon VIII (2018) Trevor Badge \n\r h - Help (this menu) \n\r a - About \n\r t - Talk to Trevor", NEWLINE_ON);
			break;
			
			case 'a':		// About
				send_msg(" A badge for our first (and hopefully not last) DerbyCon.\r\n Big thanks to April Wright for bringing us out here! We Heart You!", NEWLINE_ON);
			break;
			
			case 't':		// Talk to Trevor
				if(STATE == 3){
					send_msg(" Yes. Your job is done. I can see the lights. All the blinky lights in the great beyond.\r\n", NEWLINE_OFF);
					send_msg(" For your reward, go to: https://goo.gl/Vuu1XP", NEWLINE_ON);
				}
				else if(STATE == 2){
					if(chk_space != ' ') //just "t" was typed					
						send_msg(" I am almost at peace...read me my last rights... (example: t SEKRETWORD)", NEWLINE_ON); 
					else if(msg[2]=='s' && msg[3]=='m' && msg[4]=='a' && msg[5]=='s' && msg[6]=='h' && msg[7]=='b' && msg[8]=='u' && msg[9]=='r' && msg[10]=='g' && msg[11]=='e' && msg[12]=='r'){
						//Check input for the word "smashburger" which should have ROT8 (for Derbycon 8) "keskztmjywj" printed on the back of the badge
						send_msg(" SMASHBURGER! Yes...that will do...if only...it were In N Out Burger.", NEWLINE_ON);
						STATE = 3; 
						save_state();
					}
					else
						send_msg(" I don't want that to be the last thing I hear!", NEWLINE_ON); 
				}
				else if(STATE == 1){
					send_msg(" ZOMG!!! MY FUCKING FOOT BURNZ! COOL IT DOWN! NEED...MILKSHAKE!!!", NEWLINE_ON); 
				}
				else{// STATE == 0
					send_msg(" OMG! MY FUCKING ARM HURTS! PLEASE CUT IT OFF!", NEWLINE_ON);			
				}
			break;			
			default:
				send_msg(" U IZ DRUNK! Type h to get list of commands!", NEWLINE_ON);
			}
	} else {send_msg(" U IZ DRUNK! Type h to get list of commands!", NEWLINE_ON);}
}

void USART_Init(void) {
	USART0_CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_RXCIE_bm;
	USART0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
	USART0_CTRLA = (1<<USART_RXCIE_bp)|(0<<USART_TXCIE_bp);
	PORTB_DIRSET = (0<<PIN3_bp) | (1<<PIN2_bp);
	USART0_BAUD = 1388;					//Calculate baud rate setting - BAUD = 64*CLK_PER/S*fBAUD = 64*3.33MHz/16*9600 = 1388 ~ i.e. Client connects at 9600 Baud over Serial
	USART0_STATUS = USART_RXCIF_bm | USART_TXCIF_bm;
}

void USART_SendByte(uint8_t u8Data) { 
	// Send Character
	USART0_TXDATAL = u8Data;
	while((USART0_STATUS &(1<<USART_TXCIF_bp)) == 0);
	USART0_STATUS = (1<<USART_TXCIF_bp);			// clear flag
}

void send_msg(char msg[], char new_line) { 
	for(int i = 0; i < strlen(msg); i++) {
		USART_SendByte(msg[i]);			// send value
	}
	if(new_line == ENABLE) {
		USART_SendByte('\n');			// new line
		USART_SendByte('\r');			// return	
		USART_SendByte('\n');			// new line
		USART_SendByte('\r');			// return	
	}
}

int main(void) {
	USART_Init();					// Initialize USART
	sei();							// Enable all interrupts
	STATE = load_state();			// Load state from internal flash
	
	//TODO Remove at integration - this is 817 development board specific
	PORTC.DIRSET = PIN0_bm;			// Configure Yellow LED0 pin as output 
	PORTC.DIRCLR = PIN5_bm; 		// Configure SW0 User Button as input ~ THIS SIMULATE CUTTING THE TRACE
	
    while (1) //USE THIS LOOP TO CONTROL LED COLORS AND CHECK FOR UNLOCKS (TRACE CUT, TEMPERATURE)
    {	
		//TODO Modify at integration - this is 817 development board specific
		//THIS SIMULATES THE TRACE BEING CUT
		if ((!(PORTC.IN & (PIN5_bm)))&&(STATE==0)) {
			//BUTTON IS BEING PUSHED
			//THIS IS PERMANENT ONCE THIS VARIABLE IS SET WE DONT GO BACK
			STATE = 1;
			save_state();
		}
		
		//TODO Modify at integration - this is 817 development board specific
		//THIS SIMULATES THE TERMPERATURE CHECK
		if ((!(PORTC.IN & (PIN5_bm)))&&(STATE==1)) {
			//BUTTON IS BEING PUSHED
			//THIS IS PERMANENT ONCE THIS VARIABLE IS SET WE DONT GO BACK
			//THIS WILL DECREMENT TEMPERATURE BY 5 DEGREES EVERY PUSH TO SIMULATE COOLING
			FOOT_TEMP -= 5.0;
			if(FOOT_TEMP <= 35.0){
				STATE = 2;				
			}
			save_state();
		}
		
		//Note: Check and modification for state 3 is handled through terminal logic

		//TODO Modify at integration - this is 817 development board specific
		//THIS IS MY BLING THE UBER YELLOW LED		
		//IDEA IS EACH STATE HAS VASTLY DIFFERENT RGB BLINKS SO WE CAN TELL WHO HAS DONE WHAT BY LOOKING AT THEM
		if (STATE == 0){
			//STATE 0 IS SLOW AND POSSIBLY JUST 2 COLORS - RED FADE IN FADE OUT, THEN ORANGE FADE IN FADE OUT
			//Nothing has happened yet awaiting the trace to be cut
			PORTC.OUTSET = PIN0_bm; /* Yellow LED0 on */
			_delay_ms(500);
			PORTC.OUTCLR = PIN0_bm; /* Yellow LED0 off */
			_delay_ms(500);			
		}
		else if (STATE == 1){
			//STATE 1 IS FASTER AND USES JUST 2 COLORS - BLUE FADE IN FADE OUT, THEN GREEN FADE IN FADE OUT
			//Trace has been cut, awaiting the foot to be chilled
			PORTC.OUTSET = PIN0_bm; /* Yellow LED0 on */
			_delay_ms(150);
			PORTC.OUTCLR = PIN0_bm; /* Yellow LED0 off */
			_delay_ms(150);
		}
		else if (STATE == 2){
			//STATE 2 IS FASTER AND USES JUST 2 COLORS - PURPLE FADE IN FADE OUT, THEN YELLOW FADE IN FADE OUT
			//Trace has been cut, foot has been chilled, awaiting the secret word
			PORTC.OUTSET = PIN0_bm; /* Yellow LED0 on */
			_delay_ms(75);
			PORTC.OUTCLR = PIN0_bm; /* Yellow LED0 off */
			_delay_ms(75);
		}
		else {//STATE == 3
			//STATE 3 IS FASTEST WITH RAINBOW RGB CYCLE
			//Everything has been completed
			PORTC.OUTSET = PIN0_bm; /* Yellow LED0 on */
			_delay_ms(25);
			PORTC.OUTCLR = PIN0_bm; /* Yellow LED0 off */
			_delay_ms(25);
		}
    }
}


