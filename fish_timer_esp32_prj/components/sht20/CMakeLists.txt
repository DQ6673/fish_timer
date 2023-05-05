set(srcs "sht20.c")

set(includes ".")

set(requires    "user_iic"
                )


idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${requires}
                       )