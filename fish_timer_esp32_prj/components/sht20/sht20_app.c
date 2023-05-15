#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "sht20_driver.h"

float temp_now = 0.00;

TaskHandle_t task_sht20_get_handle;
#define task_sht20_get_stackdepth 1024 * 2
#define task_sht20_get_priority 1

static void task_sht20_get(void *Param)
{
    sht20_reset();
    vTaskDelay(20 / portTICK_PERIOD_MS);

    for (;;)
    {
        sht20_trigger_T();
        vTaskDelay(100 / portTICK_PERIOD_MS);
        while (sht20_recive_T(&temp_now) != ESP_OK)
            vTaskDelay(10 / portTICK_PERIOD_MS);

        // ESP_LOGI("runing", "tempreture is %.2fC, humidity is %.2f%%", temp_now, current_RH);
        sht20_reset();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void sht20_app_start(void)
{
    xTaskCreate(task_sht20_get,
                "task_sht20_get",
                task_sht20_get_stackdepth,
                NULL,
                task_sht20_get_priority,
                &task_sht20_get_handle);
}
