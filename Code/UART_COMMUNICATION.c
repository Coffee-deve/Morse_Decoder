#include "UART_COMMUNICATION.h"

void write_data_from_receiver_into(char schowek_){
	 schowek_ = UART0->D; 
}


void UART0_Init(void)
{
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;							// Enable clock for UART0
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;							// Enable clock for Port B
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(MCGFLLCLK);		        // Clock MCGFLLCLK=41943040Hz (CLOCK_SETUP=0)
	PORTB->PCR[1] = PORT_PCR_MUX(2);							//PTB1=TX_D
	PORTB->PCR[2] = PORT_PCR_MUX(2);							//PTB2=RX_D
	
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );		// Disable TX & RX
	UART0->BDH = 0;
	UART0->BDL =91;		// For CLOCK_SETUP=0 BR=28800	BDL=91	:	CLOCK_SETUP=1	BR=230400		BDL=13
	UART0->C4 &= ~UART0_C4_OSR_MASK;
	UART0->C4 |= UART0_C4_OSR(15);	// For CLOCK_SETUP=0 BR=28800	OSR=15	:	CLOCK_SETUP=1	BR=230400		OSR=15
	UART0->C5 |= UART0_C5_BOTHEDGE_MASK;	// Both edge sampling
	UART0->C2 |= UART0_C2_RIE_MASK;		// Enable IRQ
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);		// Enable TX & RX
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	
}
/**
 * @brief Send a string via UART0.
 * @param str The string to send.
 */
void UART0_SendString(const char* str) {
    while (*str) {
        while (!(UART0->S1 & UART0_S1_TDRE_MASK));  // Wait until TX is ready
        UART0->D = *str++;                          // Send the character
    }
}



