#include "sntp_time.h"

#include "esp_sntp.h"
#include "esp_log.h"
#include <time.h>


void init_sntp(void) {
    ESP_LOGI("NTP", "Inicializando SNTP...");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    
    // Usamos un servidor público de Google o pool.ntp.org
    esp_sntp_setservername(0, "pool.ntp.org"); 
    esp_sntp_init();

    // Esperar a que la hora se sincronice (esto toma unos pocos segundos)
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int intentos = 0;
    
    while (timeinfo.tm_year < (2026 - 1900) && intentos < 10) {
        ESP_LOGI("NTP", "Esperando sincronización de hora...");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
        intentos++;
    }
    
    // Configurar tu zona horaria local (Colombia/Bogota)
    setenv("TZ", "COT5", 1);
    tzset();
}