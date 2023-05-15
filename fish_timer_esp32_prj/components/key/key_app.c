#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "key_driver.h"

TaskHandle_t task_key_handle;
#define task_key_stackdepth 1024 * 2
#define task_key_priority 1

static void task_key_handler(void *Param)
{
    for (;;)
    {

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void ds18b20_app_start(void)
{
    key_driver_init();

    xTaskCreate(task_key_handler,
                "task_key_handler",
                task_key_stackdepth,
                NULL,
                task_key_priority,
                &task_key_handle);
}
