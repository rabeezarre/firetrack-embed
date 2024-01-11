#include <SPI.h>
#include <RH_RF95.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <time.h>
#include <WiFiUdp.h>
#include <NTPClient.h>


// Singleton instance of the radio driver
RH_RF95 rf95(27, 33);

// WiFi credentials
const char* ssid = "rabee";
const char* password = "isthebest";

// JSON endpoint
const char* serverName = "http://firetrack.cleverapps.io/api/sensorData";

// UDP instance
WiFiUDP ntpUDP;

// NTP Client setup
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // 0 is the time offset, 60000 is the update interval in milliseconds

void setup() {
  Serial.begin(9600);
  while (!Serial);

  initLoRa();
  connectToWiFi();

  // Start NTP client
  timeClient.begin();
}

void loop() {
  handleLoRaMessages();
}

void initLoRa() {
  if (!rf95.init()) {
    Serial.println("LoRa init failed");
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void handleLoRaMessages() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      Serial.print("got request: ");
      Serial.println((char*)buf);

      sendLoRaReply();
      if (WiFi.status() == WL_CONNECTED) {
        String jsonData = String((char*)buf); 
        String timestamp = getCurrentTimestamp();
        String updatedJsonData = addTimestampToJson(jsonData, timestamp);
        Serial.println(updatedJsonData);
        sendJsonToApi(updatedJsonData);
      } else {
        Serial.println("WiFi Disconnected");
      }

    } else {
      Serial.println("recv failed");
    }
  }
}

void sendLoRaReply() {
  uint8_t data[] = "data was received, thank you!";
  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();
  Serial.println("Sent a reply");
}

// Function to get current timestamp
String getCurrentTimestamp() {
  timeClient.update();
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = gmtime(&rawtime);

  char timestamp[30];
  snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02dT%02d:%02d:%02d.891281", 
           ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday, 
           ti->tm_hour, ti->tm_min, ti->tm_sec);

  return String(timestamp);
}


// Function to add timestamp to JSON
String addTimestampToJson(const String &jsonData, const String &timestamp) {
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonData);
  doc["timestamp"] = timestamp;
  String updatedJsonData;
  serializeJson(doc, updatedJsonData);
  return updatedJsonData;
}

// Function to send JSON to API
void sendJsonToApi(const String &jsonData) {
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
