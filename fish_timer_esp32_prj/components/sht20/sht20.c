#include "sht20.h"
#include "myiic.h"
#include "esp_log.h"

#define SHT20_ADDRESS (0X80)
#define SHT20_COMMAND_RESET (0XFE)
#define SHT20_COMMAND_TRIGGER_T (0XF3)
#define SHT20_COMMAND_TRIGGER_RH (0XF5)

void sht20_reset(void)
{
    uint8_t sht20_dat[1] = {SHT20_COMMAND_RESET};
    i2c_send(SHT20_ADDRESS, sht20_dat, sizeof(sht20_dat));
}

void sht20_trigger_T(void)
{
    uint8_t sht20_dat[1] = {SHT20_COMMAND_TRIGGER_T};
    i2c_send(SHT20_ADDRESS, sht20_dat, sizeof(sht20_dat));
}

void sht20_trigger_RH(void)
{
    uint8_t sht20_dat[1] = {SHT20_COMMAND_TRIGGER_RH};
    i2c_send(SHT20_ADDRESS, sht20_dat, sizeof(sht20_dat));
}

esp_err_t sht20_recive_T(float *c_temp)
{
    float current_T = 0;
    uint8_t sht20_dat[3] = {0};
    esp_err_t sht_sta = ESP_OK;
    sht_sta = i2c_read(SHT20_ADDRESS, sht20_dat, sizeof(sht20_dat));

    if (sht_sta != ESP_OK)
        return sht_sta;
    else
    {
        current_T = ((float)sht20_dat[0] * 256 + (float)sht20_dat[1]) * 175.72 / 65536 - 46.85;
        *c_temp = current_T;
    }
    return sht_sta;
}

esp_err_t sht20_recive_RH(float *c_rh)
{
    float current_RH = 0;
    uint8_t sht20_dat[3] = {0};
    esp_err_t sht_sta = ESP_OK;
    sht_sta = i2c_read(SHT20_ADDRESS, sht20_dat, sizeof(sht20_dat));

    if (sht_sta != ESP_OK)
        return sht_sta;
    else
    {
        current_RH = ((float)sht20_dat[0] * 256 + (float)(sht20_dat[1]&0XFC)) * 125 / 65536 - 6;
        *c_rh = current_RH;
    }
    return sht_sta;
}
