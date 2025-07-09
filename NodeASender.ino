#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define CS_PIN 5
#define RST_PIN 14
#define DIO0_PIN 26

#define DHTPIN 4          
#define DHTTYPE DHT22     
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  dht.begin();

  LoRa.setPins(CS_PIN, RST_PIN, DIO0_PIN);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Gagal Mulai");
    while (1);
  }
}

void loop() {
  float suhu = dht.readTemperature();
  if (isnan(suhu)) {
    Serial.println("Gagal membaca sensor suhu!");
    return;
  }

  String pesan = "Suhu: " + String(suhu,1) + " C";
  Serial.println("Mengirim ke B: " + pesan);
  LoRa.beginPacket();
  LoRa.print(pesan);
  LoRa.endPacket();
  delay(2000); // jeda antar pesan
}
