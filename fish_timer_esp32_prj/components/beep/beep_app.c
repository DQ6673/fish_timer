#include <stdio.h>
#include "argtable3/argtable3.h"
#include "driver/ledc.h"
#include "esp_console.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "beep_driver.h"

extern TaskHandle_t task_buzzer_handle;
QueueHandle_t beep_args_set_queue;
const char *buzzer_cmd_tag = "buzzer_cmd";

static struct{
    struct arg_int *duty_set;
    struct arg_int *freq_set;
    struct arg_end *end;
} buzzer_set_args;

int dutyset = 0;
int freqset = 0;

static int do_buzzer_set_cmd(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **)&buzzer_set_args);
    if (nerrors != 0)
    {
        arg_print_errors(stderr, buzzer_set_args.end, argv[0]);
        return 0;
    }

    dutyset = buzzer_set_args.duty_set->ival[0];
    freqset = buzzer_set_args.freq_set->ival[0];

    if (freqset > 0)
    {
        // send args to beep queue
        xQueueSend(beep_args_set_queue, (void *)&freqset, 0);
    }
    else
    {
        ESP_LOGE(buzzer_cmd_tag, "Set Wrong duty or frequency");
        return 0;
    }

    return 0;
}

static void register_buzzer_set(void)
{
    buzzer_set_args.duty_set = arg_int0("d", "duty", "<duty (0~8191)>", "Duty set to buzzer");
    buzzer_set_args.freq_set = arg_int1("f", "freq", "<frequency (Hz)>", "Frequency set to buzzer");
    buzzer_set_args.end = arg_end(1);

    const esp_console_cmd_t buzzer_set_cmd = {
        .command = "buzzer_set",
        .help = "Set buzzer args",
        .hint = NULL,
        .func = &do_buzzer_set_cmd,
        .argtable = &buzzer_set_args};

    ESP_ERROR_CHECK(esp_console_cmd_register(&buzzer_set_cmd));
}

void buzzer_init(void)
{
    beep_args_set_queue = xQueueCreate(10, sizeof(uint32_t));
    buzzer_driver_init();
    register_buzzer_set();
}