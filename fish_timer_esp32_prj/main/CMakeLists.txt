set(srcs "main.c")

set(includes ".")

set(requires    "sht20"
                "user_iic"
                "user_timer"
                "user_lcd"
                "ds18b20"
                "driver"
                "esp_timer"
                "beep"
                "console"
                "fatfs"
                "lvgl"
                "lv_examples"
                )


idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )