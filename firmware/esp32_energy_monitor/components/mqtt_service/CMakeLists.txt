idf_component_register(SRCS "mqtt.c" 
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES mqtt
                                  lib
                                  esp_event
                                  esp_netif)