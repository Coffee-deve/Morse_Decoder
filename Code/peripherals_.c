#include "peripherals_.h"
void Turn_on_led(void){
	PTB->PCOR = (1 << RED_LED_POS);
}
void Turn_on_buzzer(void){
	PTB->PSOR = (1 << BUZZER_POS);
}
void Turn_off_led(void){
	PTB->PSOR = (1 << RED_LED_POS);
}
void Turn_off_buzzer(void){
	PTB->PCOR = (1 << BUZZER_POS);
}

void BuzzerINIT(int PORTB_PIN_NUMBER){
		// Configure PORTB Pin 13 for Buzzer
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;       // Enable clock for GPIO B
    PORTB->PCR[BUZZER_POS] |= PORT_PCR_MUX(1); // Set Pin 9 as GPIO
    PTB->PDDR |= (1 << PORTB_PIN_NUMBER);           // Set Pin 9 as output
    PTB->PCOR = (1 << PORTB_PIN_NUMBER);            // Ensure Buzzer is OFF initially
}
void RedLed_init(void){
	    // configure portb pin 8 as red led output
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;        // Enable clock for PORTB
    PORTB->PCR[RED_LED_POS] = PORT_PCR_MUX(1); // Set pin as GPIO
    PTB->PDDR |= (1 << RED_LED_POS);           // Set pin as output
    PTB->PSOR = (1 << RED_LED_POS);            // Turn off LED
}

void Button_init(int PORTB_PIN_NUMBER){
			// configure portb pin X as button input
 	PORTB->PCR[PORTB_PIN_NUMBER] |= PORT_PCR_MUX(1)| PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xB);  /* MUX config. Set Pin 13 of PORT B as GPIO, interrupt on falling edge */  	 
  PTB->PDDR &= ~(1UL << PORTB_PIN_NUMBER); /* Set pin 13 of GPIO B as input */ 
	NVIC_ClearPendingIRQ(31);
	NVIC_EnableIRQ(31);
  NVIC_SetPriority(31, 0);
	
}