#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =  "XXXX";   // cannot be longer than 32 characters!
const char *pass =  "XXXX";   //

const char *mqtt_server = "XXXX";
const int mqtt_port = XXXX;
const char *mqtt_user = "XXXX";
const char *mqtt_pass = "XXXX";

#define BUFFER_SIZE 100

bool LedState = false;


void callback(const MQTT::Publish& pub) {
  LedState = !LedState;
  digitalWrite(0, LedState);  
}

WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);

void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  pinMode(0, OUTPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect("arduinoClient2")
                         .set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.subscribe("/test/buttonPressed");
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }

    if (client.connected())
      client.loop();
  }
}
