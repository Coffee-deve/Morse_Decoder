#include "MKL05Z4.h"
#define CLK_DIS 					0x00
#define MCGFLLCLK 				0x01
#define OSCERCLK					0x02
#define MCGIRCLK					0x03


void UART0_Init(void);
void UART0_SendString(const char* str);

void write_data_from_receiver_into(char schowek_);