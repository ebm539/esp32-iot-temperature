#include "read_temperature_and_humidity.h"
#include "mqtt_client.h"
#include "htu21d.h"


void read_temperature_and_humidity(esp_mqtt_client_handle_t client)
{
    int msg_id_temp;
    int msg_id_humid;
    char temperature[sizeof(float) * 4 + 1];
    char humidity[sizeof(float) * 4 + 1];
    while (true)
    {
        float temperature_float = ht21d_read_temperature();
        float humidity_float = ht21d_read_humidity();
        sprintf(temperature, "%f", temperature_float);
        sprintf(humidity, "%f", humidity_float);
        // TODO: handle failures of message publishing
        msg_id_temp = esp_mqtt_client_publish(client, "/sensor/temperature", temperature, 0, 1, 0);
        msg_id_humid = esp_mqtt_client_publish(client, "/sensor/humidity", humidity, 0, 1, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}