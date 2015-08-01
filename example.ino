/*
 Basic MQTT example with Authentication

  - connects to an MQTT server, providing username
    and password
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =	"XXXXXXXX";		// cannot be longer than 32 characters!
const char *pass =	"XXXXXXXX";		//

// Update these with values suitable for your network.
IPAddress server(172, 16, 0, 2);

#define BUFFER_SIZE 100

void callback(const MQTT::Publish& pub) {
  Serial.print(pub.topic());
  Serial.print(" => ");
  if (pub.has_stream()) {
    uint8_t buf[BUFFER_SIZE];
    int read;
    while (read = pub.payload_stream()->read(buf, BUFFER_SIZE)) {
      Serial.write(buf, read);
    }
    pub.payload_stream()->stop();
    Serial.println("");
  } else
    Serial.println(pub.payload_string());  
}

WiFiClient wclient;
PubSubClient client(wclient, "m11.cloudmqtt.com", XXXXX);

void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
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
      if (client.connect(MQTT::Connect("arduinoClient")
                         .set_auth("XXXX", "XXXX"))) {
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.publish("/test/outTopic", "hello world");
        client.subscribe("/test/outTopic");
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }

    if (client.connected())
      client.loop();
  }
}
