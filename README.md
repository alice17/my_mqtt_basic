# my_mqtt_basic
MQTT implementation for Arduino.

In this project Arduino gains sensor data from two temperature sensors DHT11 and Dallas DS18B20 connected to the board. Then, it publishes them at a regular interval to the MQTT broker via Ethernet Shield. [PubSubClient](https://github.com/knolleary/pubsubclient) library is used for managing the connection with the broker.
