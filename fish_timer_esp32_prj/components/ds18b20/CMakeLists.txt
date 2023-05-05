set(srcs "ds18b20.c")

set(includes ".")

set(requires    "driver"
                "user_timer"
                )


idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )