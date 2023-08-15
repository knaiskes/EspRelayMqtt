#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";

const char* mqttServer = "";
const int mqttPort = 1883;

const char* mqttUsername = "";
const char* mqttPassword = "";

const char* mqttTopic = ""; // Example: relays/topic
const char* mqttOnConnectMsg = "Successfully connected to MQTT broker";
char mqttPingTopic[50];
const char* mqttTopicPingSuffix = "/ping";

unsigned long lastMqttMessageTime = 0;
const unsigned long mqttPingInterval = 20000; // 20 seconds

const int relay_pin = D3;

void callback(char* mqttTopic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(mqttServer, mqttPort, callback, espClient);

void setup() {
  strcpy(mqttPingTopic, mqttTopic);
  strcat(mqttPingTopic, mqttTopicPingSuffix);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    if (client.connect("ESP8266Client", mqttUsername, mqttPassword)) {
      client.publish(mqttTopic, mqttOnConnectMsg);
    } else {
      delay(2000);
    }
  }

  client.subscribe(mqttTopic);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  unsigned long currentTime = millis();
  if (currentTime - lastMqttMessageTime >= mqttPingInterval) {
    sendPing();
    lastMqttMessageTime = currentTime; // Update the last message time
  }
  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client", mqttUsername, mqttPassword)) {
      client.subscribe(mqttTopic);
      client.publish(mqttTopic, mqttOnConnectMsg);
    } else {
      delay(2000);
    }
  }
}

void callback(char* mqttTopic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (message == "true") {
    digitalWrite(relay_pin, LOW);
  } else if (message == "false") {
    digitalWrite(relay_pin, HIGH);
  }
}

void sendPing() {
  if (client.connected()) {
    client.publish(mqttPingTopic, "ping");
  }
}
