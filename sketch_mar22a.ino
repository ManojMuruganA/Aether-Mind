#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define DHTPIN 14
#define DS18B20 2  
#define REPORTING_PERIOD_MS 1000

float temperature, humidity, BPM = 0, SpO2 = 0, bodytemperature;


const char* ssid = "your_SSID";  
const char* password = "your_Password";  

DHT dht(DHTPIN, DHTTYPE);
PulseOximeter pox;
uint32_t tsLastReport = 0;
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);

ESP8266WebServer server(80);

bool poxInitialized = false;

void setup() {
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  delay(100);

  dht.begin();
  sensors.begin();

  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected!");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");

  Serial.print("Initializing pulse oximeter...");
  if (pox.begin()) {
    Serial.println("MAX30100 Initialized Successfully");
    poxInitialized = true;
  } else {
    Serial.println("FAILED to initialize MAX30100");
    poxInitialized = false;
  }
}

void loop() {
  server.handleClient();

  if (poxInitialized) {
    pox.update();
  }

  sensors.requestTemperatures();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    bodytemperature = sensors.getTempCByIndex(0);

    temperature = t;
    humidity = h;

    if (poxInitialized) {
      BPM = pox.getHeartRate();
      SpO2 = pox.getSpO2();
    } else {
      BPM = 0;
      SpO2 = 0;
    }

    Serial.println("\n--- Health Data ---");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());  // Show the IP every update cycle

    Serial.print("Room Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Room Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Body Temperature: ");
    Serial.print(bodytemperature);
    Serial.println(" °C");

    if (poxInitialized) {
      Serial.print("BPM: ");
      Serial.println(BPM > 0 ? BPM : 0);

      Serial.print("SpO2: ");
      Serial.println(SpO2 > 0 ? SpO2 : 0);
    } else {
      Serial.println("BPM: N/A (Oximeter Failed)");
      Serial.println("SpO2: N/A (Oximeter Failed)");
    }

    Serial.println("--------------------\n");

    tsLastReport = millis();
  }
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(temperature, humidity, poxInitialized ? BPM : 0, poxInitialized ? SpO2 : 0, bodytemperature));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float temperature, float humidity, float BPM, float SpO2, float bodytemperature) {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<title>Patient Health Monitoring</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  
  // Auto-refresh every 5 seconds
  html += "<meta http-equiv='refresh' content='5'>";
  
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center;}";
  html += "h1 { color: #4CAF50; }";
  html += "div { margin: 20px; padding: 20px; border: 1px solid #ccc; }";
  html += "p { font-size: 18px; }";
  html += "</style>";
  
  html += "</head>";
  html += "<body>";
  html += "<h1>Health Monitoring System</h1>";
  html += "<div>";
  html += "<p><strong>Room Temperature:</strong> " + String(temperature) + " °C</p>";
  html += "<p><strong>Room Humidity:</strong> " + String(humidity) + " %</p>";
  html += "<p><strong>Body Temperature:</strong> " + String(bodytemperature) + " °C</p>";

  if (poxInitialized) {
    html += "<p><strong>Heart Rate:</strong> " + (BPM > 0 ? String(BPM) : "N/A") + " BPM</p>";
    html += "<p><strong>SpO2:</strong> " + (SpO2 > 0 ? String(SpO2) : "N/A") + " %</p>";
  } else {
    html += "<p><strong>Heart Rate:</strong> N/A (Oximeter Failed)</p>";
    html += "<p><strong>SpO2:</strong> N/A (Oximeter Failed)</p>";
  }

  html += "</div>";
  html += "</body>";
  html += "</html>";

  return html;
}
