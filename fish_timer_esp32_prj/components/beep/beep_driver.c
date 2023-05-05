#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "beep_driver.h"

#define GPIO_BUZZER GPIO_NUM_4
#define LEDC_OUTPUT_IO GPIO_BUZZER

void buzzer_driver_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY, // Set output frequency at 5 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    if (ledc_channel_config(&ledc_channel) == ESP_OK)
    {
        ESP_LOGI("hardware", "buzzer init-> GPIO_NUM_4");
    }
    else
    {
        ESP_LOGE("hardware", "buzzer init failed!");
    }
}

void buzzer_on(uint32_t freq_set)
{
    ESP_ERROR_CHECK(ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq_set));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 4095));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}

void buzzer_off(void)
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}
