#include <WiFi.h>
#include <ThingSpeak.h>

#define WIFI_SSID "Saran"
#define WIFI_PASS "1234567890_-!"

#define THINGSPEAK_API_KEY "7PNCCFSIA73SIUPM"
#define THINGSPEAK_CHANNEL_ID "2566578"

const int pulsePin = A0;
const int samplingInterval = 1000; // Sampling interval in milliseconds
const int numReadings = 10; // Number of readings to average

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(pulsePin, INPUT);
  connectToWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  int heartRate = getHeartRate();
  Serial.print("Heart rate: ");
  Serial.println(heartRate);

  uploadToThingSpeak(heartRate);
  delay(samplingInterval);
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

int getHeartRate() {
  int sum = 0;
  for (int i = 0; i < numReadings; i++) {
    sum += analogRead(pulsePin);
    delay(10); // Sampling rate of 100 Hz
  }
  int average = sum / numReadings;
  int heartRate = map(average, 0, 4095, 0, 200); // Adjust according to your sensor and environment
  return heartRate;
}

void uploadToThingSpeak(int heartRate) {
  unsigned long channelID = atol(THINGSPEAK_CHANNEL_ID);
  ThingSpeak.writeField(channelID, 1, heartRate, THINGSPEAK_API_KEY);
  Serial.println("Data sent to ThingSpeak successfully!");
}
