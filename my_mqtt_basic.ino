#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "DHT.h"

// dht11 and ds18b20 pins
#define DHTTYPE DHT11
#define DHTPIN 2
#define INTERVAL 5000       // interval in ms
#define ONE_WIRE_BUS 8

// RGB led
#define RPIN 3
#define GPIN 5
#define BPIN 6

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dsensor(&oneWire);
DHT dht(DHTPIN, DHTTYPE);

byte mac[] = { 0x90, 0xA2, 0xDA, 0x03, 0x00, 0xC3 };
IPAddress myip;    
IPAddress server(192, 168, 1, 10);
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (mqttClient.connect("arduinoClient")) {
      Serial.println("connected");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      
      setPinColor(255, 0, 0); // red light
      delay(5000);
    }
  }
}

void setPinColor(int red, int green, int blue){
  analogWrite(RPIN, red);
  analogWrite(GPIN, green);
  analogWrite(BPIN, blue);  
}

void publishTemp(){
  // publish temperature data

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  dsensor.requestTemperatures();
  float td = dsensor.getTempCByIndex(0);

  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    //mqttClient.publish("temp", "error");
    return;
  }

  Serial.print("Publishing DHT11 temperature: ");
  Serial.println(t);
  mqttClient.publish("sensors/dht11/temp", String(t).c_str());

  Serial.print("Publishing DHT11 humidity: ");
  Serial.println(h);
  mqttClient.publish("sensors/dht11/humidity", String(h).c_str());

  Serial.print("Publishing DS18B20 temperature: ");
  Serial.println(td);
  mqttClient.publish("sensors/dallas/temp", String(td).c_str());

  setPinColor(0, 255, 0); // green light 
}

void setup(){
  Serial.begin(9600);

  mqttClient.setServer(server, 1883);   // set mosquitto server
  
  IPAddress myip(192, 168, 1, 2);
  Ethernet.begin(mac, myip);
  //myip = Ethernet.localIP();
  
  Serial.print("My ip: ");  
  Serial.println(myip);
  Serial.print("Server: ");
  Serial.println(server);
  delay(1500);

  Serial.println("Starting sensors...");
  dht.begin();
  dsensor.begin();

  pinMode(RPIN, OUTPUT);
  pinMode(GPIN, OUTPUT);
  pinMode(BPIN, OUTPUT);  
}

void loop(){
  if (!mqttClient.connected()) {
    setPinColor(255, 0, 0); // red light
    reconnect();
  }

  mqttClient.loop(); 
  publishTemp();

  delay(INTERVAL);
}
