idf_component_register(SRCS "device.c" 
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_timer
                                  esp_wifi)