#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "ds18b20_driver.h"

float water_now = 0.00;

TaskHandle_t task_ds18b20_get_handle;
#define task_ds18b20_get_stackdepth 1024 * 2
#define task_ds18b20_get_priority 1

static void task_ds18b20_get(void *Param)
{
    ds18b20_init();
    for (;;)
    {
        ds18b20_requestTemperatures();
        float cTemp = ds18b20_get_temp();
        water_now = cTemp;
        // printf("Temperature: %0.2fC\n", cTemp);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void ds18b20_app_start(void)
{
    xTaskCreate(task_ds18b20_get,
                "task_ds18b20_get",
                task_ds18b20_get_stackdepth,
                NULL,
                task_ds18b20_get_priority,
                &task_ds18b20_get_handle);
}
