#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_console.h"
#include "esp_vfs_fat.h"
// #include "cmd_system.h"
#include "myiic.h"
#include "sht20.h"
#include "timer.h"
#include "esp_timer.h"
#include "lcd.h"
#include "lcd_app.h"
#include "ds18b20.h"
#include "beep_app.h"
#include "lvgl.h"
#include "lv_examples/src/lv_demo_benchmark/lv_demo_benchmark.h"

float temp_now = 0.00;
float water_now = 0.00;
// 任务参数
TaskHandle_t task_sht20_get_handle;
#define task_sht20_get_stackdepth 1024 * 2
#define task_sht20_get_priority 1

TaskHandle_t task_lcd_handler_handle;
#define task_lcd_handler_stackdepth 1024 * 3
#define task_lcd_handler_priority 1

TaskHandle_t task_ds18b20_get_handle;
#define task_ds18b20_get_stackdepth 1024 * 2
#define task_ds18b20_get_priority 1

TaskHandle_t task_buzzer_handle;
#define task_buzzer_stackdepth 1024 * 2
#define task_buzzer_priority 1

extern QueueHandle_t beep_args_set_queue;

#define MOUNT_PATH "/data"
#define HISTORY_PATH MOUNT_PATH "/history.txt"

#define LV_TICK_PERIOD_MS 2
#define DISP_BUF_SIZE (LV_HOR_RES_MAX * 40)
/**********************************************************************/

/**********************************************************************/
static void initialize_filesystem(void)
{
	static wl_handle_t wl_handle;
	const esp_vfs_fat_mount_config_t mount_config = {
		.max_files = 4,
		.format_if_mount_failed = true};
	esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(MOUNT_PATH, "storage", &mount_config, &wl_handle);
	if (err != ESP_OK)
	{
		ESP_LOGE("filesystem", "Failed to mount FATFS (%s)", esp_err_to_name(err));
		return;
	}
}

static void lv_tick_task(void *arg)
{
	lv_tick_inc(LV_TICK_PERIOD_MS);
}

void disp_driver_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
	lcd_flush(area->x1, area->y1, area->x2, area->y2, (void *)color_map);
	lv_disp_flush_ready(drv);
}

static void my_lv_app(void)
{
	lv_demo_benchmark();
}

static void task_lcd_handler(void *Param)
{
	lcd_init();
	lv_init();

	lv_color_t *buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
	assert(buf1 != NULL);
	/* Use double buffered when not working with monochrome displays */
	lv_color_t *buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
	assert(buf2 != NULL);

	static lv_disp_buf_t disp_buf;
	uint32_t size_in_px = DISP_BUF_SIZE;
	lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.flush_cb = disp_driver_flush;
	// disp_drv.rotated = 1;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);

	/* Create and start a periodic timer interrupt to call lv_tick_inc */
	const esp_timer_create_args_t periodic_timer_args = {
		.callback = &lv_tick_task,
		.name = "periodic_gui",
	};
	esp_timer_handle_t periodic_timer;
	ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
	ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

	// lv_demo_benchmark();
	my_lv_app();

	printf("sb\r\n");
	for (;;)
	{
		// display_temp(temp_now - 2);
		// display_water(water_now);
		// display_time();
		// display_note();

		// vTaskDelay(10 / portTICK_PERIOD_MS);
		lv_task_handler();
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

static void task_sht20_get(void *Param)
{
	sht20_reset();
	vTaskDelay(20 / portTICK_PERIOD_MS);
	float current_RH = 0;

	for (;;)
	{
		sht20_trigger_T();
		vTaskDelay(100 / portTICK_PERIOD_MS);
		while (sht20_recive_T(&temp_now) != ESP_OK)
			vTaskDelay(10 / portTICK_PERIOD_MS);

		sht20_trigger_RH();
		vTaskDelay(35 / portTICK_PERIOD_MS);
		while (sht20_recive_RH(&current_RH) != ESP_OK)
			vTaskDelay(10 / portTICK_PERIOD_MS);

		// ESP_LOGI("runing", "tempreture is %.2fC, humidity is %.2f%%", temp_now, current_RH);
		sht20_reset();
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

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

static void task_buzzer(void *Param)
{
	buzzer_init();
	uint32_t freq_set = 0;

	for (;;)
	{
		// xQueueReceive(beep_args_set_queue, (void *)&freq_set, portMAX_DELAY);
		buzzer_on(3500);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		buzzer_off();
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void app_main(void)
{
	// clock_timer_init();
	// ESP_ERROR_CHECK(i2c_master_init());
	// initialize_filesystem();
	// esp_console_repl_t *repl = NULL;
	// esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
	// repl_config.history_save_path = HISTORY_PATH;
	// repl_config.prompt = "dpm>";
	// esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
	// ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));

	// // sht20
	// if (xTaskCreate(task_sht20_get,
	// 				"task_sht20_get",
	// 				task_sht20_get_stackdepth,
	// 				NULL,
	// 				task_sht20_get_priority,
	// 				&task_sht20_get_handle) != pdPASS)
	// 	ESP_LOGE("start_info", "sht20 任务创建失败");

	// // ds18b20 task
	// if (xTaskCreate(task_ds18b20_get,
	// 				"task_ds18b20_get",
	// 				task_ds18b20_get_stackdepth,
	// 				NULL,
	// 				task_ds18b20_get_priority,
	// 				&task_ds18b20_get_handle) != pdPASS)
	// 	ESP_LOGE("start_info", "ds18b20 任务创建失败");

	// // lcd task
	if (xTaskCreate(task_lcd_handler,
					"task_lcd_handler",
					task_lcd_handler_stackdepth,
					NULL,
					task_lcd_handler_priority,
					&task_lcd_handler_handle) != pdPASS)
		ESP_LOGE("start_inf", "iic 任务创建失败");

	// // buzzer task
	// if (xTaskCreate(task_buzzer,
	// 				"task_buzzer",
	// 				task_buzzer_stackdepth,
	// 				NULL,
	// 				task_buzzer_priority,
	// 				&task_buzzer_handle) != pdPASS)
	// 	ESP_LOGE("start_info", "buzzer 任务创建失败");

	// ESP_ERROR_CHECK(esp_console_start_repl(repl));
}
