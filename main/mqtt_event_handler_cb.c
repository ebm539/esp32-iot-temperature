#include "mqtt_event_handler_cb.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include "init_htu21d.h"
#include "htu21d.h"
#include "read_temperature_and_humidity.h"
#include "esp_system.h"

static const char *TAG = "CB_READ_TEMP";

esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            // initialise HTU21D
            // TODO: check if already initialised (use global variable? pass through function?)
            int status = init_htu21d();
            // this prints on Wi-Fi/MQTT disconnect...
            // TODO: fix printing htu21d status on MQTT disconnect
            // printf("htu21d init status: %d", status);

            // Some magic voodoo to calculate the necessary size of the char array
            // (Publishing the integer as-is did work, but caused a warning on compile)
            char status_str[sizeof(int) * 4 + 1];
            sprintf(status_str, "%d", status);
            msg_id = esp_mqtt_client_publish(client, "/sensor/status", status_str, 0, 1, 1);
            if (status != HTU21D_ERR_OK) {
                // TODO: handle missing HTU21D better
                abort();
            }
            // go to read_temperature_and_humidity.c
            read_temperature_and_humidity(client);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            esp_restart();
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            esp_restart();
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}
