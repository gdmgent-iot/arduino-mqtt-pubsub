#include "secrets.h"
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

// MQTT BROKER SETTINGS
const char* broker = "192.168.1.114";
const int port = 1883;
const char* topic = "technology/arduino/demo";

// CLIENTS
WiFiClient fifi;
MqttClient cutie(fifi);

// CONNECT TO MQTT BROKER
void connectToMqtt() {
  Serial.print("Connection to broker at: ");
  Serial.print(broker);
  Serial.print(" at port: ");
  Serial.println(port);

  while(!cutie.connect(broker, port)) {
    Serial.print(".");
    delay(500);
  }

  if(cutie.connected()) {
    Serial.println(""); // nieuwe lijn, zodat het niet achter laatste puntje komt
    Serial.println("Verbonden met de broker!");

    // subscribe to topic
    cutie.subscribe(topic);
    // stuur bericht
    cutie.beginMessage(topic);
    cutie.print("Hello from F-Rogers his Arduino");
    cutie.endMessage();
  }
}

// CONNECT TO THE WIFI NETWORK
void connectToWiFi() {
  int attempts = 0;

  // Probeer te verbinden, max. 5 keer
  while (WiFi.status() != WL_CONNECTED && attempts < 5) {
    Serial.print("Verbinden met netwerk: ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    delay(2000);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected :-)");
    Serial.println("IP-adres is: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Kan niet verbinden...");
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Booting the Arduino...");
  delay(2000);

  // define buttons
  pinMode(2, INPUT_PULLUP);
  // define LED
  pinMode(8, OUTPUT);

  connectToWiFi();
  connectToMqtt();
}

void loop() {
  // Optioneel: herverbinden als WiFi wegvalt
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi verloren, opnieuw verbinden...");
    connectToWiFi();
  }

  if (!cutie.connected()) {
    Serial.println("MQTT verbinding verloren, opnieuw verbinden...");
    connectToMqtt();
    return;
  }

  // when button pressed, send message
  publishWhenBtnPressed();
  // listen to messages
  toggleLED();
}

// put led on or off when message received
void toggleLED() {
  cutie.poll();
  while(cutie.available()) {
    String msg = cutie.readString();

    if (msg == "LED ON") {
      digitalWrite(8, HIGH);
    } else if (msg == "LED OFF") {
      digitalWrite(8, LOW);
    }
  }
}

void publishWhenBtnPressed() {
  int btnState = digitalRead(2);
  if (btnState == LOW) {
    Serial.println("Button pressed, sending message...");
    cutie.beginMessage(topic);
    cutie.print("FRED");
    cutie.endMessage();
    delay(1000); // ff wachten, anders stuurt ie 10 berichten
  }
}