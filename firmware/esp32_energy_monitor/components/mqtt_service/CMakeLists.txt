idf_component_register(SRCS "mqtt.c" 
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES device
                                  mqtt
                                  lib
                                  esp_event
                                  sntp_time
                                  esp_netif)