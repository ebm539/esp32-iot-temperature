#include "read_temperature_and_humidity.h"
#include "mqtt_client.h"
#include "htu21d.h"

void read_temperature_and_humidity(esp_mqtt_client_handle_t client) {
    // int msg_id_temp;
    // int msg_id_humid;
    // I don't know why these 2 lines are necessary for determining how big to make char array ("string"), but they are
    char temperature[sizeof(float) * 4 + 1];
    char humidity[sizeof(float) * 4 + 1];
    while (true) {
		float temperature_float = ht21d_read_temperature();
		float humidity_float = ht21d_read_humidity();
		// check if values are plausible, if they are not, skip rest of the while loop for this iteration
		// plausible: temperature above absolute zero, neither value not -999 as defined in `components/htu21d/htu21d.c`
		// the delay is purely arbitary
		// TODO: print warning/send MQTT message informing me that a reading didn't make sense
		// readings are -999 when there is no raw data, which is generally when the sensor has been disconnected after init
		if (temperature_float < -274) {
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			continue;
		}
		if (humidity_float < 0) {
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			continue;
		}
        // convert temperature and humidity values into strings for MQTT publish
        sprintf(temperature, "%f", temperature_float);
        sprintf(humidity, "%f", humidity_float);
        // TODO: handle failures of message publishing
        // msg_id_temp = esp_mqtt_client_publish(client, "/sensor/temperature", temperature, 0, 1, 0);
        // msg_id_humid = esp_mqtt_client_publish(client, "/sensor/humidity", humidity, 0, 1, 0);
        esp_mqtt_client_publish(client, "/sensor/temperature", temperature, 0, 1, 0);
        esp_mqtt_client_publish(client, "/sensor/humidity", humidity, 0, 1, 0);
        // TODO: make the delay configurable using MQTT
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
