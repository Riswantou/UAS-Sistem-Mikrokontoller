#define BLYNK_TEMPLATE_ID "TMPL6cdYlTKa4"
#define BLYNK_TEMPLATE_NAME "ESP32 DHT22"
#define BLYNK_AUTH_TOKEN "8oxxRdSXgLRYAgtaNp8M7t_eFRdnT_dz"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST"; // Nama WiFi
char pass[] = "";            // Password WiFi

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address & size LCD

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Membaca sensor");

  // Set interval pengiriman data setiap 2.5 detik
  timer.setInterval(2500L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca sensor DHT!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error     ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    return;
  }

  // Kirim data ke Blynk
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V0, t);

  // Tampilkan di Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" C, Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Tampilkan ke LCD
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(t, 1);
  lcd.print(" C   ");

  lcd.setCursor(0, 1);
  lcd.print("Lembap: ");
  lcd.print(h, 1);
  lcd.print(" %   ");
}
