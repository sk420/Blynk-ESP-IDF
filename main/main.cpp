#include <stdio.h>
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wifi_connect.h"

// #define BLYNK_DEBUG
#define BLYNK_PRINT stdout
#define BLYNK_TOKEN "-------------------------" // Replace with your Blynk token

#include "BlynkEspIDF.h"
#include <BlynkWidgets.h>

BlynkTimer timer;
WidgetTerminal terminal(V0);

BLYNK_WRITE(V0)
{
  std::string receivedCommand = param.asStr();
     
  terminal.print("Received Command: ");
  terminal.println(receivedCommand);

  if (receivedCommand == "help")
  {
    terminal.println("Available Commands:");
    terminal.println("- cpu: Get current cpu temperature");
    terminal.println("- status: Check device status");
    terminal.println("- restart: Restart the device");
    terminal.println("- clear: Clear the terminal");
  }
  else if (receivedCommand == "restart")
  {
    terminal.println("System Restarting");
    terminal.flush();
    vTaskDelay(pdMS_TO_TICKS(100)); // Wait for a second before restarting
    esp_restart();
  }
  else if (receivedCommand == "clear")
  {
    terminal.clear();
  }
  else
  {
    terminal.println("Unknown Command. Type 'help' for assistance.");
  }

  terminal.flush();
}

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
    wifi_begin();
    vTaskDelay(pdMS_TO_TICKS(10));

    Blynk.begin(BLYNK_TOKEN, "server", 80);

    terminal.clear();
    terminal.println("Hello from ESP32-IDF ");
    terminal.println("This is a test message.");
    terminal.flush();

    while (true)
    {
        Blynk.run();
        timer.run();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}