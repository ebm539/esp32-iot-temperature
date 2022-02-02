#include "mqtt_event_handler.h"
#include "mqtt_event_handler_cb.h"
#include "mqtt_client.h"
#include "esp_log.h"
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD("mqtt_event_handler.c", "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}