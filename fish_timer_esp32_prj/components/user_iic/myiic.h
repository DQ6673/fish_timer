#include "esp_err.h"

esp_err_t i2c_master_init(void);
esp_err_t i2c_send(uint8_t dev_addr, uint8_t *dat, size_t dat_len);
esp_err_t i2c_read(uint8_t dev_addr, uint8_t *dat, size_t dat_len);
