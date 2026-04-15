#ifndef __LED_H
#define __LED_H


typedef struct
{

	_Bool Led_Status;

} LED_INFO;

#define LED_ON		1

#define LED_OFF	  0


void LED_Init(void);
//void LED1_ON(void);
//void LED1_OFF(void);
//void LED1_Turn(void);

extern LED_INFO led_info;

void LED_Set(_Bool status);

#endif
