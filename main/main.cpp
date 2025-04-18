#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wifi_connect.h"

// #define BLYNK_DEBUG
#define BLYNK_PRINT stdout
#define BLYNK_NO_BUILTIN

#include "BlynkApiEsp32.h"
#include "BlynkSocketEsp32.h"

#define BLYNK_TOKEN "4huy-J3UAzCmrBkz-z_Mprb5UR1a6t1z"

static BlynkTransportEsp32 _blynkTransport;
static BlynkSocket Blynk(_blynkTransport);

#include <BlynkWidgets.h>

BlynkTimer tmr;

BLYNK_WRITE(V1)
{
    printf("Got a value: %s\n", param.asString());
}

extern "C" void app_main(void)
{
    esp_log_level_set("wifi", ESP_LOG_ERROR);
    esp_log_level_set("wifi_init", ESP_LOG_ERROR);
    esp_log_level_set("phy_init", ESP_LOG_ERROR);
    esp_log_level_set("wifi_prov_scheme_ble", ESP_LOG_ERROR);
    esp_log_level_set("esp_netif_handlers", ESP_LOG_ERROR);
    esp_log_level_set("gpio", ESP_LOG_ERROR);

    wifi_init_sta("Wokwi-GUEST", "");

    vTaskDelay(pdMS_TO_TICKS(1000));

    Blynk.begin(BLYNK_TOKEN, "protocol.electrocus.com", 8080);
    tmr.setInterval(1000, []()
                    { Blynk.virtualWrite(V0, BlynkMillis() / 1000); });
    while (true)
    {
        Blynk.run();
        tmr.run();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}