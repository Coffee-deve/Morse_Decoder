#include "MKL05Z4.h"
#define RED_LED_POS 8 
#define BUTTON_POS 3
#define BUZZER_POS 13
#define BUTTON_POS 3
#define BUTTON2_POS 4

void Turn_on_button1(void);
void Turn_on_button2(void);
void Turn_on_led(void);
void Turn_on_buzzer(void);
void Turn_off_button1(void);
void Turn_off_button2(void);
void Turn_off_led(void);
void Turn_off_buzzer(void);
void BuzzerINIT(int PORTB_PIN_NUMBER);
void RedLed_init(void);
void Button_init(int PORTB_PIN_NUMBER);