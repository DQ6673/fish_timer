#include "lcd_driver.h"
#include <stdio.h>
#include <string.h>
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"

#define LCD_CS_GPIO_NUM GPIO_NUM_10
#define LCD_SCLK_GPIO_NUM GPIO_NUM_6
#define LCD_MOSI_GPIO_NUM GPIO_NUM_7
#define LCD_MISO_GPIO_NUM (-1)
#define LCD_DC_GPIO_NUM GPIO_NUM_2

// #ifndef LCD_DEBUG
// #define LCD_DC_GPIO_NUM GPIO_NUM_2
// #else
// #define LCD_DC_GPIO_NUM GPIO_NUM_5
// #endif

#define LCD_RST_GPIO_NUM (-1)

#define LCD_H_RES 320
#define LCD_V_RES 172

#define LCD_SPI_HOST SPI2_HOST
#define LCD_SPI_MODE 3
#define LCD_PIXEL_CLOCK_HZ (80 * 1000 * 1000)
#define LCD_PIXEL_BITS 16
#define LCD_CMD_BITS 8
#define LCD_PARAM_BITS 8
#define LCD_X_GAP 0
#define LCD_Y_GAP 34
#define LCD_BUF_MAX (LCD_H_RES * LCD_V_RES / 4 * 2)

// 定义 lcd 总控制面板句柄
esp_lcd_panel_handle_t lcd_panel_handle = NULL;
uint8_t test_color[LCD_BUF_MAX] = {0};
/// @brief 初始化 lcd
/// @param
void lcd_init(void)
{
    // spi gpio config
    spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_SCLK_GPIO_NUM,
        .mosi_io_num = LCD_MOSI_GPIO_NUM,
        .miso_io_num = LCD_MISO_GPIO_NUM,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_BUF_MAX,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    // spi speed config
    esp_lcd_panel_io_handle_t lcd_io_handle = NULL;
    esp_lcd_panel_io_spi_config_t lcd_io_config = {
        .dc_gpio_num = LCD_DC_GPIO_NUM,
        .cs_gpio_num = LCD_CS_GPIO_NUM,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = LCD_SPI_MODE,
        .trans_queue_depth = 10,
        //.on_color_trans_done = example_notify_lvgl_flush_ready,
        //.user_ctx = &disp_drv,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &lcd_io_config, &lcd_io_handle));

    // lcd panel config
    esp_lcd_panel_dev_config_t panel_dev_config = {
        .reset_gpio_num = LCD_RST_GPIO_NUM,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = LCD_PIXEL_BITS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(lcd_io_handle, &panel_dev_config, &lcd_panel_handle));

    // lcd st7789 init
    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(lcd_panel_handle, LCD_X_GAP, LCD_Y_GAP));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(lcd_panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(lcd_panel_handle, false, true));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(lcd_panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_panel_handle, false));

    memset(test_color, 0, sizeof(test_color));
    for (int i = 0; i < 4; i++)
    {
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(lcd_panel_handle,
                                                  0,
                                                  LCD_V_RES / 4 * i,
                                                  LCD_H_RES,
                                                  LCD_V_RES / 4 * (i + 1),
                                                  test_color));
    }
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_panel_handle, true));
}

extern uint8_t bgd_pic_map[];

// / @brief 执行颜色填充
// / @param
void bgd_pic_fullfill(void)
{
    for (int i = 0; i < 4; i++)
    {
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(lcd_panel_handle,
                                                  0,
                                                  LCD_V_RES / 4 * i,
                                                  LCD_H_RES,
                                                  LCD_V_RES / 4 * (i + 1),
                                                  (bgd_pic_map + (i * LCD_H_RES * LCD_V_RES / 4 * 2))));
    }
}

/// @brief 填充函数，方便外部调用，省去 lcd_panel_handle 参数
/// @param x_start
/// @param y_start
/// @param x_end
/// @param y_end
/// @param data
void lcd_draw_icon(int x_start, int y_start, int x_end, int y_end, const void *data)
{
    if ((x_end - x_start) * (y_end - y_start) > LCD_BUF_MAX)
    {
        ESP_LOGE("LCD错误", "数据量过大");
    }

    else
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(lcd_panel_handle, x_start, y_start, x_end, y_end, data));
}

void lcd_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void *colordata)
{
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(lcd_panel_handle, x1, y1, x2 + 1, y2 + 1, colordata));
}