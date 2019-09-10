#include <atmel_start.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>

#include "simon.h"
#include "binary.h"
#include "state.h"
#include "temp.h"
#include "term.h"


#define TERM_WAITING 0
#define TERM_RUNNING 1
#define TERM_WAITING_TRIGGER 1000

#define TEMP_COLD 0
#define TEMP_AMBIENT 1
#define TEMP_HOT 2

//These are RAW ADC values and need calibration
#define TEMP_COLD_THRESHOLD 95 //ICE CUBE GIVES AN ADC READING OF 72
#define TEMP_HOT_THRESHOLD 170 //SOLDER IRON AT 130 DEGREES C GIVES AN ADC READING OF 190

//Local variable for overriding the temp state. As long as it's 0xFF, thermistor is used
static uint8_t m_temp_state_override = 0xFF;

uint8_t m_term_mode = TERM_WAITING;
uint16_t m_term_waiting_counter= 0;

volatile uint8_t buffer_size = 0;
char msg_rx[64];
volatile uint8_t x = 0;
static char prompt_norml[] = "AND!XOR# ";
static char prompt_ready[] = "";
volatile bool uart_ready = true;
volatile bool matt_damon = false; 

uint8_t term_mode_get(){
	return m_term_mode;	
}

/************************************************************************/
/* Helper function for debugging thermistor. Allow temp state to be     */
/* overriden with valid values                                          */
/************************************************************************/
static void temp_state_override(uint8_t value) {
	if (value <= TEMP_HOT) {
		m_temp_state_override = value;
	}
}

/************************************************************************/
/* Helper function that translates RAW ADC values from the thermistor   */
/* Into HOT, AMBIENT, or COLD values.                                   */
/************************************************************************/
static uint8_t temp_state_get(){
	if (m_temp_state_override != 0xFF) {
		return m_temp_state_override;
	}
	
	if (temp_get() < TEMP_COLD_THRESHOLD) {
		return TEMP_COLD;
	} else if (temp_get() > TEMP_HOT_THRESHOLD) {
		return TEMP_HOT;
	} else {
		return TEMP_AMBIENT;
	}
}

static void __print_binz(){
	int col = 0;
	for(int i=0; i<binz_len; i++){
		if(col == 12){
			col = 0;
			printf("\r\n");
		}
		printf("0x%02x ",pgm_read_byte(&(binz[i])));
		col++;
	}
	printf("\r\n\n");
}

static void __print_splash(){
	//Print ASCII Splash Screen
	printf("\r\n\n"
	"  AND!XOR DERBYCON IX\r\n"
	"       GOODBYE\r\n"
	"   ,--.       ,---.    \r\n"
	"  /    '.    /     \\  \r\n"
	"         \\  ;			\r\n"
	"          \\-|		    \r\n"
	"         (X X)      (/ \r\n"
	"         /'v'     ,-'	\r\n"
	" ,------/ >< \\---'	\r\n"
	"/)     ;  --  :		\r\n"
	"   ,---| ---- |--.		\r\n"
	"  ;    | ---- |   :	\r\n"
	" (|  ,-| ---- |-. |)	\r\n"
	"    | /| ---- |\\ |	\r\n"
	"    |/ | ---- | \\|	\r\n"
	"    \\  : ---- ;  |	\r\n"
	"     \\  \\ -- /  /	\r\n"
	"     ;   \\  /  :		\r\n"
	"    /   / \\/ \\  \\	\r\n"
	"   /)           (\\	\r\n"
	"\r\n");
}

static void __check_cmd(char msg[]) {
	char cmd;
	cmd = msg[0];			// get first character

	//Process the buffer
	if((msg[1] == ' ') || (cmd == 'h' || cmd == 'a' || cmd == 't')) {
		switch(cmd)	{
			case 'h':		// Help
			printf("\n\rAND!XOR DerbyCon IX (2019) Trevor Badge \n\r h - Help (this menu) \n\r a - About \n\r t - Talk to Trevor\r\n\n");
			break;
			
			case 'a':		// About
			printf(" A free badge for our last DerbyCon.\r\n\n");
			break;
			
			case 't':		// Talk to Trevor
			//Check for factory reset switch
			if(msg[2]=='r' && msg[3]=='e' && msg[4]=='s' && msg[5]=='e' && msg[6]=='t'){
				//#t reset forces a factory reset of the STATE back to 0
				printf(" Remember when things were simple, when we first started hacking? Pepperidge Farm remembers.\r\n\n");
				if(state_get() == 3)
					trevor_touch_hug_set(false);
				simon_level_reset();
				state_set(0);
				state_save();
				for (uint8_t i=2; i<=6; i++){msg[i]=0;} //Clear the Rx Buffer
				break;
			}

			//TOGGLE GOD MODE I.E. MATT DAMON
			if(msg[2]=='M' && msg[3]=='A' && msg[4]=='T' &&	msg[5]=='T' &&	msg[6]=='D' &&	msg[7]=='A' && msg[8]=='M' && msg[9]=='O' && msg[10]=='N' &&
			msg[11]=='I' &&	msg[12]=='S' &&	msg[13]=='A' &&	msg[14]=='U' &&	msg[15]=='N' &&	msg[16]=='I' && msg[17]=='C' &&	msg[18]=='O' &&	msg[19]=='R' &&	msg[20]=='N' && (msg[1] == ' ')) {
				if(matt_damon){
					matt_damon = false;
					printf(" MATT DAMON MODE DISABLED!\r\n\n");
				}
				else{
					matt_damon = true;
					printf(" MATT DAMON MODE ENABLED!\r\n\n");
				}
				for (uint8_t i=2; i<=20; i++){msg[i]=0;} //Clear the Rx Buffer
				break;
			}

			//DEBUG COMMAND FOR CHANGING STATE
			if(msg[2]=='s' && (msg[4]=='0' || msg[4]=='1' || msg[4]=='2' || msg[4]=='3') && (matt_damon) && (msg[1] == ' ')){
				printf(" Current State: %d\r\n", state_get());
				printf(" New State: %d\r\n\n", (msg[4]-48));
				state_set(msg[4]-48);//Convert the ASCII into an actual number
				state_save();
				for (uint8_t i=2; i<=4; i++){msg[i]=0;} //Clear the Rx Buffer
				break;
			}
			
			//DEBUG COMMAND FOR CHANGING TEMP
			if(msg[2]=='t' && (msg[4]=='0' || msg[4]=='1' || msg[4]=='2') && (matt_damon) && (msg[1] == ' ')){
				printf(" Current Temp RAW ADC value: %d\r\n", temp_get());
				printf(" Current Temp State: %d\r\n", temp_state_get());
				printf(" New Temp State: %d\r\n\n", (msg[4]-48));
				temp_state_override(msg[4]-48); //Convert the ASCII into an actual number
				for (uint8_t i=2; i<=4; i++){msg[i]=0;} //Clear the Rx Buffer
				break;
			}

			if(state_get() == 3){//STATE 3 ~ FINISHED
				printf(" Well friends. That's it for ol' Trevor.\r\n");
				printf(" At least they added SOCKS5 to metasploit. Be sure to ice Grifter!\r\n");
				printf(" https://www.youtube.com/watch?v=oOXcol_fFGQ\r\n\n");
			}
			else if(state_get() == 2){//STATE 2 ~ GET CLOSE
				printf(" Time to say goodbye. Gonna miss you guys.\r\n");
				printf(" Before I go, could I get an awkward hug? Hold me close, awkwardly close, and coddle me long time.\r\n\n");
			}
			else if(state_get() == 1){//STATE 1 ~ PURGATORY / HEAVEN / HELL
				//If cold
				if((temp_state_get() == 0)&&(msg[1] != ' ')){
					printf(" Wow it's cold up here and the air is thin...in the Heavenly skies!\r\n");
					printf(" There's an angel St Will Caruana, with a halo, doing cloud donuts on a rascal scooter...\r\n");
					printf(" He just keeps laughing and saying I'm a piece of shit who will never get past the gates.\r\n");	
					printf(" But Will did say I could spend my time cracking the sekret password...so I have less pain in hell?\r\n");	
					printf(" Let's ask him what it is! (example: t ask).\r\n\n");
					break; //Just in case temperature changes while this is printing
				}
				else if((temp_state_get() == 0)&&(msg[2]=='a' && msg[3]=='s' && msg[4]=='k' && msg[1] == ' ')){
					printf(" Will opens his mouth and hex fly out...\r\n\n");	
					//_delay_us(10000);
					__print_binz();
					for (uint8_t i=2; i<=4; i++){msg[i]=0;} //Clear the Rx Buffer
					break; //Just in case temperature changes while this is printing
				}
				//else if ambient temp
				else if((temp_state_get() == 1)&&(msg[1] != ' ')){
					printf(" Weird. I'm in a null space void, purgatory?\r\n");
					printf(" I've heard its hot in hell and cool in heaven.\r\n");
					printf(" What do I know? I'm just a dead cockroach.\r\n\n");
					break; //Just in case temperature changes while this is printing
				}
				//else if hot
				else if((temp_state_get() == 2)&&(msg[1] != ' ')){
					printf(" Wow it's hot down here...in HELL!\r\n");
					printf(" There's a demon Will Caruana, wearing a HackForSatan badge, doing lava donuts on a rascal scooter...\r\n");
					printf(" He keeps laughing at me and says there's an eternity of elevator phones being shoved in my butt.\r\n");
					printf(" However to minimize the pain I have to tell him the password? (example: t SEKRETWORD)\r\n\n");
					break; //Just in case temperature changes while this is printing
				}
				else if((temp_state_get() == 2)&&(msg[1]==' ' && msg[2]=='R' && msg[3]=='O' && msg[4] == 'U' && msg[5] == 'N' && msg[6] == 'D' && msg[7] == 'E' && msg[8] == 'R' && msg[9] == 'S')){
					printf(" I think that's the right password!\r\n");
					printf(" Demon Will is just flipping me the bird and telling me to say goodbye to my loved one(s)...namely YOU!\r\n\n");
					state_set(2);
					state_save();
					for (uint8_t i=2; i<=9; i++){msg[i]=0;} //Clear the Rx Buffer
					break; //Just in case temperature changes while this is printing
				}
			}
			else if (msg[1] != ' '){//STATE 0 ~ SIMON
				printf(" I'm heading towards the light...but the light is too bright!\r\n");
				printf(" All of life's memories flashing before my eyes...repeat them back to me so I don't #trevorforget!\r\n\n");
			}
			else printf(" U IZ DRUNK! Type h to get list of commands!\r\n\n");
			break;
			default: printf(" U IZ DRUNK! Type h to get list of commands!\r\n\n");
		}
	} else {printf(" U IZ DRUNK! Type h to get list of commands!\r\n\n");}
	printf(prompt_norml);
}

void term_write_byte(volatile uint8_t c) {
	// Send Character
	while (!(USART0.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART0.TXDATAL = c;
	USART0_STATUS = (1<<USART_TXCIF_bp);			// clear flag
}
	
void term_handler() {
	char value = 0;
		
	if (m_term_mode == TERM_WAITING) {
		//Tick counter to periodically show waiting message
		m_term_waiting_counter++;
		//If we overflowed the counter, print something to terminal and wait some more
		if (m_term_waiting_counter > TERM_WAITING_TRIGGER) {
			m_term_waiting_counter = 0;
			printf("\n\rReady - Press 'c' for terminal");
		}
	}
		
	//If there's data ready for us, process it
	if (USART_0_is_rx_ready())
	{
		//Get a value off the terminal
		value = USART_0_read();
			
			
		if (m_term_mode == TERM_WAITING) {
				
			if (value =='c') {
				m_term_mode = TERM_RUNNING;
				printf("\n\n\n\r");
				__print_splash();
				printf(prompt_norml);
			}

			} else {
			msg_rx[x] = value;
			putchar(msg_rx[x]);			//send back the character typed to display
			if(value != 8 && value != 127)		// if backspace/delete is pressed,
			{ x++; } else
			{ x--; }
			if(value == '\r')					//if enter is pressed, process command
			{
				putchar('\n');			// new line
				__check_cmd(msg_rx);				// check for command request
				x = 0;
				printf(prompt_ready); // add a command line prompt
			}
		}
	}
}