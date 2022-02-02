#include "mqtt_app_start.h"
#include "mqtt_event_handler.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "esp_log.h"

#include <stdio.h>
#include <stdint.h>

// This is example code!
// Edit the MQTT broker hostname and port before using for your own project

extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");
extern const uint8_t client_key_pem_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_pem_end[] asm("_binary_client_key_end");
extern const uint8_t server_cert_pem_start[] asm("_binary_fullchain_crt_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_fullchain_crt_end");

void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtts://mqtt-tls.example.com:8883",
        .client_cert_pem = (const char *)client_cert_pem_start,
        .client_key_pem = (const char *)client_key_pem_start,
        .cert_pem = (const char *)server_cert_pem_start,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}