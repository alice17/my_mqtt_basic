# my_mqtt_basic
MQTT implementation for Arduino.

In this project Arduino gains sensor data from two temperature sensors DHT11 and Dallas DS18B20 connected to the board. It publishes them at a regular interval to the MQTT broker via Ethernet Shield. Clients can connect to the broker and receive data.

[PubSubClient](https://github.com/knolleary/pubsubclient) library is used for managingin order to manage the connection with the broker.
