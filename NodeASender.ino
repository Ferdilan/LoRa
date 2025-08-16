#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define CS_PIN 5
#define RST_PIN 14
#define DIO0_PIN 26

#define DHTPIN 4          
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);

String idNode = "A";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  dht.begin();

  LoRa.setPins(CS_PIN, RST_PIN, DIO0_PIN);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Gagal Mulai");
    while (1);
  }

  Serial.println("Transmitter (Node A) Siap");
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  if (isnan(kelembapan)) {
    Serial.println("Gagal membaca sensor Kelembapan!");
    return;
  }

  String pesan = "ID:" + idNode + " Kelembapan: " + String(kelembapan,1) + " %";
  Serial.println("Mengirim ke B: " + pesan);

  LoRa.beginPacket();
  LoRa.print(pesan);
  LoRa.endPacket();
  delay(2000);
}
