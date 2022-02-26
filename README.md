# ESP32-based temperature sensor

- Reads temperature and humidity data from a HTU21D, using [the HTU21D ESP-IDF component by lucadentella](https://github.com/lucadentella/esp32_htu21d)
- Publishes the temperature and humidity data to a MQTT broker every second. (The ESP32 is connected to the MQTT broker using TLS, and the ESP32 is authenticated to the MQTT broker using a client certificate)

The code quality is "duct taped together" level :)

## Known bugs

- Program crashes and reboots when the HTU21D is not connected
- Wi-Fi disconnects after 10-20 minutes for no apparent reason
- Unsuccessful message publishing (a.k.a. problem when sending temperature and/or humidity data) is not handled? (Have not tested this)
- The temperature and humidity sensor may be reinitialised unnecessarily

## Planned features

- Handle MQTT broker disconnection gracefully (don't reboot)
- Use the [NVS](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/api-reference/storage/nvs_flash.html) for configuration storage - e.g. MQTT broker hostname and port. (Currently, the MQTT broker details are hardcoded)
- Configure the temperature and humidity data sending interval using a ESP32-subscribed MQTT topic and message
- Store more than one Wi-Fi SSID and password (i.e. read [the example_connect() code](https://github.com/espressif/esp-idf/blob/v4.4/examples/protocols/README.md) and adapt [the Wi-Fi connection code](https://github.com/espressif/esp-idf/tree/v4.4/examples/wifi/getting_started/station) for my use).
- Turn on the onboard LED if there is a problem (e.g. not connected to Wi-Fi or the MQTT broker, no HTU21D, nonsense data, cannot send MQTT messages)
- Send a MQTT message when a nonsense data has been read from the sensor
- Rewrite in Rust
- Connect to Bluetooth and use Bluetooth tethering (Bluetooth PAN?) if Wi-Fi is not available

## How to build

- Install [ESP-IDF](https://github.com/espressif/esp-idf). This project uses [v4.4](https://github.com/espressif/esp-idf/releases/tag/v4.4). (Older versions of ESP-IDF may work, however I have not tested them.)
- Clone this repo and the HTU21D submodule. (The rust-esp32 component/submodule is not in use at this time)
- Edit `components/htu21d/htu21d.c` and add `conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;` before `i2c_param_config`. This fixes a bug I encountered, where I could not read the data from the HTU21D.
- Provide your own client certificate (`client.crt`), client key (`client.key`), and server certificate (`fullchain.crt`), and copy into `main/` as necessary. (See the [example code on ESP32 MQTT mutual (client and server) authentication](https://github.com/espressif/esp-idf/tree/v4.4/examples/protocols/mqtt/ssl_mutual_auth) for more information)
- Edit `main/mqtt_app_start_dummy.c`, substitute in your MQTT broker details, and save. Rename the file to `main/mqtt_app_start.c`
- Configure your Wi-Fi SSID and password with `idf.py menuconfig`, then `Example Connection Configuration`. If you get an error about idf.py not found, check that ESP-IDF is installed and that the PATH variable is correctly configured - see [the ESP-IDF documentation](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/get-started/index.html#step-4-set-up-the-environment-variables) for more information.
- Build and flash the program with `idf.py build flash`. 
