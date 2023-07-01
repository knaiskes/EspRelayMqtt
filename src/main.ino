#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";

const char* mqtt_server = "";
const int mqtt_port = 1883;

const char* mqtt_username = "";
const char* mqtt_password = "";

const char* topic = "";

const int relay_pin = D3;

void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(mqtt_server, mqtt_port, callback, espClient);

void setup() {
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
    } else {
      delay(2000);
    }
  }

  client.subscribe(topic);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      client.subscribe(topic);
    } else {
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
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
