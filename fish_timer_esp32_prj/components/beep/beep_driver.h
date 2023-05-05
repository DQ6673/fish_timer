#ifndef _BEEP_DRIVER_H_
#define _BEEP_DRIVER_H_

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY (5000)           // Frequency in Hertz. Set frequency at 5 kHz



void buzzer_driver_init(void);
void buzzer_on(uint32_t freq_set);
void buzzer_off(void);

#endif



