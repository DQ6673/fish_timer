#include "myiic.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"

#define I2C_SDA_GPIO_NUM GPIO_NUM_18
#define I2C_SCL_GPIO_NUM GPIO_NUM_19
#define I2C_FREQ_HZ (100 * 1000)
#define I2C_PORT_NUM I2C_NUM_0
#define I2C_WAIT_MS (500)

#define ACK (0)
#define NACK (1)

esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_PORT_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_GPIO_NUM,
        .scl_io_num = I2C_SCL_GPIO_NUM,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = I2C_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

esp_err_t i2c_send(uint8_t dev_addr, uint8_t *dat, size_t dat_len)
{
    esp_err_t send_sta = ESP_OK;

    i2c_cmd_handle_t iiccmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(iiccmd));
    // write device address
    ESP_ERROR_CHECK(i2c_master_write_byte(iiccmd, dev_addr, ACK));
    // write data
    ESP_ERROR_CHECK(i2c_master_write(iiccmd, dat, dat_len, ACK));
    ESP_ERROR_CHECK(i2c_master_stop(iiccmd));
    send_sta = i2c_master_cmd_begin(I2C_PORT_NUM, iiccmd, I2C_WAIT_MS);

    i2c_cmd_link_delete(iiccmd);

    return send_sta;
}

esp_err_t i2c_read(uint8_t dev_addr, uint8_t *dat, size_t dat_len)
{
    esp_err_t read_sta = ESP_OK;

    i2c_cmd_handle_t iiccmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(iiccmd));
    // write device address
    ESP_ERROR_CHECK(i2c_master_write_byte(iiccmd, dev_addr + 1, ACK));
    // write data
    ESP_ERROR_CHECK(i2c_master_read(iiccmd, dat, dat_len - 1, ACK));
    ESP_ERROR_CHECK(i2c_master_read(iiccmd, dat + dat_len - 1, 1, NACK));
    ESP_ERROR_CHECK(i2c_master_stop(iiccmd));
    read_sta = i2c_master_cmd_begin(I2C_PORT_NUM, iiccmd, I2C_WAIT_MS);

    i2c_cmd_link_delete(iiccmd);

    return read_sta;
}