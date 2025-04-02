/**
 * @file main.c
 * @author Patrycja Nazim
 * @date January 2025
 * @brief File containing the main function.
 * @ver 0.1
 */

#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "peripherals_.h"
#include "UART_COMMUNICATION.h"
#include <stdio.h>
#include <string.h>

#define RED_LED_POS 8 
#define BUZZER_POS 13
#define BUTTON_POS 3
#define BUTTON2_POS 4
#define MAX_LEN 100  // Maximum input size

char rx_sentence[MAX_LEN];  // Buffer for storing user input
uint8_t rx_index = 0;  // Index in buffer
uint8_t rx_READY = 0;  // Flag when sentence is ready
char schowek;
volatile char received_char = '\0';
volatile int char_received_flag = 0;

/**
 * @brief Delay function for simple timing.
 * @param n Number of milliseconds to delay.
 */
void delay_ms(int n) {
    volatile int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3500; j++) {}
}

/**
 * @brief UART0 interrupt handler. Reads the received string and sets a flag.
 */

void UART0_IRQHandler() {
    if (UART0->S1 & UART0_S1_RDRF_MASK) {  // Data received, receiver is full and is the source of the interrupt and flag is raised
        schowek = UART0->D;  // Read UART data into UART0_D register location
        if (schowek == '\n') {  // User finished input
            rx_sentence[rx_index] = '\0';  // Null-terminate string
            rx_READY = 1;  // Signal main loop
            rx_index = 0;  // Reset buffer index
        } 
        else if (rx_index < MAX_LEN - 1) {  
            rx_sentence[rx_index++] = schowek;  // Store character (one at a time from schowek into rx_sentence[rx_index])
        }
    }
}

// Morse code table
const char* morse[27] = {
    ".-",        // A
    "-...",      // B
    "-.-.",      // C
    "-..",       // D
    ".",         // E
    "..-.",      // F
    "--.",       // G
    "....",      // H
    "..",        // I
    ".---",      // J
    "-.-",       // K
    ".-..",      // L
    "--",        // M
    "-.",        // N
    "---",       // O
    ".--.",      // P
    "--.-",      // Q
    ".-.",       // R
    "...",       // S
    "-",         // T
    "..-",       // U
    "...-",      // V
    ".--",       // W
    "-..-",      // X
    "-.--",      // Y
    "--..",      // Z
    " "          // SPACE
};


/**
 * @brief The interupt function

 *
 * @return NULL
 */
void PORTB_IRQHandler(void) {
        if (PORTB->ISFR & (1 << BUTTON_POS)) {  // Check if interrupt is from Button Pin
					delay_ms(20); // for debouncing 
        if ((FPTB->PDIR & (1 << BUTTON_POS)) == 0) {  // Button is pressed
	//				  code <<= 1; // Shift left
					  UART0_SendString("-\r\n"); // Send "-" to Termite
            Turn_on_led();  // Turn ON the LED (active low)
				  	Turn_on_buzzer();  // Turn ON Buzzer (Button Released)

        } else {  // Button is released
						delay_ms(300);
            Turn_off_led();  // Turn OFF the LED (active low)
					  Turn_off_buzzer();  // Turn OFF Buzzer (Button Pressed)
        }
        PORTB->ISFR = (1 << BUTTON_POS);  // Clear the interrupt flag
    }else if (PORTB->ISFR & (1 << BUTTON2_POS)){
			delay_ms(20); // for debouncing 
			if ((FPTB->PDIR & (1 << BUTTON2_POS)) == 0) {  // Button is pressed
		//		    code = (code << 1) | 1; // Shift left and add 1
				    UART0_SendString(".\r\n"); // Send "-" to Termite
            Turn_on_led();  // Turn ON the LED (active low)
				  	Turn_on_buzzer();  // Turn ON Buzzer (Button Released)
							
        } else {  // Button is released
					 delay_ms(100);
            Turn_off_led();  // Turn OFF the LED (active low)
					  Turn_off_buzzer();  // Turn OFF Buzzer (Button Pressed)
        }
				PORTB->ISFR = (1 << BUTTON2_POS);  // Clear the interrupt flag
}
}

 

/**
 * @brief Main function.
 */
int main(void) {
	    __enable_irq();  // Enable global interrupts
    // Enable UART0 and configure it
    UART0_Init();
	  BuzzerINIT(BUZZER_POS);
    RedLed_init();
    Button_init(BUTTON_POS);
	  Button_init(BUTTON2_POS);

    // Infinite loop
    while (1) {
				if (rx_READY){
				UART0_SendString("\r\nYour input: ");
				UART0_SendString(rx_sentence);
				UART0_SendString("\r\n Morse: ");
				UART0_SendString("\r\n");
				rx_READY = 0;	
				
				
						for (int i = 0; rx_sentence[i] != '\0'; i++) {
							
							if (rx_sentence[i] >= 'a' && rx_sentence[i] <= 'z') { 
                rx_sentence[i] -= 32;  // Convert to uppercase
            }
				
            // Check if it's a valid letter or space
            if ((rx_sentence[i] >= 'A' && rx_sentence[i] <= 'Z') || rx_sentence[i] == ' ') {
                uint8_t index = (rx_sentence[i] == ' ') ? 26 : (rx_sentence[i] - 65); // index is either space or index of lookup table for given letter (65 is first index, A letter)
                const char* morse_code = morse[index]; // takes value from lookup table							
							  Turn_on_led();  // Turn on LED							
							  rx_sentence[i] = UART0->D;  // Read UART data
                UART0_SendString(morse_code);

					  for (int i = 0; morse_code[i] != '\0'; i++) {
                if (morse_code[i] == '.') {
                    Turn_on_buzzer(); // Buzzer ON
                    delay_ms(200);  // Short beep for dot
                } 
                else if (morse_code[i] == '-') {
                    Turn_on_buzzer(); // Buzzer ON
                    delay_ms(600);  // Long beep for dash
                }

                Turn_off_buzzer(); // Buzzer OFF
                delay_ms(200);  // Short pause between symbols
            }
						  Turn_off_led();  // Turn off LED
							delay_ms(600);
							
         
					}

					}

            }
        } 
    }
	