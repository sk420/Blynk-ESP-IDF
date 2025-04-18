#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>  // for rand()
#include <time.h>    // for seeding
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "wifi_connect.h"
#define BLYNK_NO_BUILTIN
#include "BlynkApiEsp32.h"
#include "BlynkSocketEsp32.h"

#define BLYNK_TOKEN "4huy-J3UAzCmrBkz-z_Mprb5UR1a6t1z"

static BlynkTransportEsp32 _blynkTransport;
static BlynkSocket Blynk(_blynkTransport);

extern "C" void app_main(void)
{
    esp_log_level_set("wifi", ESP_LOG_ERROR);
    esp_log_level_set("wifi_init", ESP_LOG_ERROR);
    esp_log_level_set("phy_init", ESP_LOG_ERROR);
    esp_log_level_set("wifi_prov_scheme_ble", ESP_LOG_ERROR);
    esp_log_level_set("esp_netif_handlers", ESP_LOG_ERROR);
    esp_log_level_set("gpio", ESP_LOG_ERROR);

    wifi_init_sta("Wokwi-GUEST", "");


    Blynk.begin(BLYNK_TOKEN, "protocol.electrocus.com", 8080);

    while (true) {
        Blynk.run();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}