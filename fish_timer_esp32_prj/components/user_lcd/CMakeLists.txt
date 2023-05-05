set(srcs "lcd.c" "lcd_font.c" "lcd_app.c")

set(includes ".")

set(requires    "esp_lcd"
                "driver"
                "main"
                )


idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )