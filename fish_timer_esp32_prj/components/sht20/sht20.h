#include "esp_err.h"

/// @brief reset the sht20
/// @param
void sht20_reset(void);

/// @brief start Temp measure
/// @param
void sht20_trigger_T(void);

/// @brief read sht20 Temp data
/// @param c_temp data save in this pointer
/// @return ESP_OK or error code
esp_err_t sht20_recive_T(float *c_temp);

/// @brief start RH measure
/// @param
void sht20_trigger_RH(void);

/// @brief read sht20 RH data
/// @param c_rh data save in this pointer
/// @return ESP_OK or error code
esp_err_t sht20_recive_RH(float *c_rh);
