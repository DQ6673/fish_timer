#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "timer.h"
#include "esp_timer.h"
#include "myiic.h"

#include "sht20_app.h"
#include "lcd_app.h"
#include "ds18b20_app.h"
#include "buzzer_app.h"
#include "key_app.h"


// 任务参数



/**********************************************************************/


void app_main(void)
{
	clock_timer_init();
	ESP_ERROR_CHECK(i2c_master_init());

	key_app_start();
	ds18b20_app_start();
	sht20_app_start();
	buzzer_app_start();
	lcd_app_start();
	// sht20

	// ds18b20 task

}
