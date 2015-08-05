#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =  "XXXX";   // cannot be longer than 32 characters!
const char *pass =  "XXXX";   //

#define BUFFER_SIZE 100

const int buttonPin = 0;
int buttonState = 0;

WiFiClient wclient;
PubSubClient client(wclient, "m11.cloudmqtt.com", XXXX);

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
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }

    if (client.connected())
      client.loop();
  }
  ButtonPress();
}

void ButtonPress(){
  buttonState = digitalRead(buttonPin);
  if(buttonState == LOW){
    client.publish("/test/buttonPressed", "1");
    Serial.println("Button Pressed...");    
  }
  
}

