set(srcs "key_driver.c" "key_app.c")

set(includes ".")

set(requires    "driver"
                "user_timer"
                )


idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )