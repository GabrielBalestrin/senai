#include "WiFi.h"
#include "PubSubClient.h"
#include <DHTesp.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

#define DHT_PIN 15 // Definidno DHT22
DHTesp dhtSensor; // Definidno DHT22

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
 Serial.begin(115200);

 dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
 Serial.println("DHT22 objeto iniciado!");

 // conectando no wifi
 WiFi.begin(ssid, password);
 Serial.println("Conectando no Wi-Fi...");

 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
 }

 Serial.println("\nWi-Fi conectado!");

 // Conectando no MQTT broker
 mqttClient.setServer(mqtt_server, mqtt_port);
 String clientId = "ESP32Client-" + String(random(0xffff), HEX);
 if (mqttClient.connect(clientId.c_str())) {
  Serial.println("Conectado no MQTT broker.");
 } else {
  Serial.println("Falha na conexão o MQTT broker!");
 }
}

void loop() {
 TempAndHumidity data = dhtSensor.getTempAndHumidity();

 float temperature=0;
 temperature = data.temperature;

 float humidity=0;
 humidity = data.humidity;

 String message = "TEMPERATURA: " + String(temperature) + "°C, UMIDADE: " + String(humidity) + "%";
 mqttClient.publish("senai", message.c_str());
 Serial.println("Enviando dados para o MQTT broker...");
 delay(5000); // Wait 5 seconds before sending the next set of data
}