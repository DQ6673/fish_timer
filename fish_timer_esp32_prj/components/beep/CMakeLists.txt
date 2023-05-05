set(srcs "beep_driver.c" "beep_app.c")

set(includes ".")

set(requires    "driver"
                "console"
                )


idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )