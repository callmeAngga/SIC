#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

const char* ssid = "BERIMAN_ATAS";
const char* password = "07021972";
const char* serverName = "http://192.168.1.218:2207";

#define DHTTYPE DHT11
#define DHT_PIN 27

DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {
  delay(2000);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C, Kelembaban: ");
  Serial.print(humidity);
  Serial.println(" %");

  String jsonPayload = "{\"temperature\":";
  jsonPayload += temperature;
  jsonPayload += ",\"humidity\":";
  jsonPayload += humidity;
  jsonPayload += "}";

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Kode Respon HTTP: ");
      Serial.println(httpResponseCode);
      Serial.print("Respon: ");
      Serial.println(response);
    } else {
      Serial.print("Kode Kesalahan: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Terputus");
  }
}
