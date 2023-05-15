set(srcs "lcd_driver.c" "lcd_font.c" "lcd_app.c")

set(includes ".")

set(requires    "esp_lcd"
                "driver"
                )


idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )