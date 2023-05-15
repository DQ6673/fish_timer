#include <stdio.h>
#include "driver/ledc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "buzzer_driver.h"

static const char *TAG = "buzzer app";

#define BUZZER_FREQ_HZ 3500

TaskHandle_t task_buzzer_handle;
#define task_buzzer_stackdepth 1024 * 2
#define task_buzzer_priority 1

static void task_buzzer_handler(void *Param)
{
    buzzer_init();

    for (;;)
    {
        // xQueueReceive(beep_args_set_queue, (void *)&freq_set, portMAX_DELAY);
        // buzzer_on(3500);
        // vTaskDelay(100 / portTICK_PERIOD_MS);
        // buzzer_off();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void buzzer_switch(uint8_t dat)
{
    if (dat == BEEP_ON)
    {
        buzzer_on(BEEP_FREQ_HZ)
    }
    else if (dat == BEEP_OFF)
    {
        buzzer_off();
    }
}

void buzzer_app_start(void)
{
    beep_args_set_queue = xQueueCreate(10, sizeof(uint32_t));
    buzzer_driver_init();
    // buzzer task
    xTaskCreate(task_buzzer_handler,
                "task_buzzer_handler",
                task_buzzer_stackdepth,
                NULL,
                task_buzzer_priority,
                &task_buzzer_handle);
}
