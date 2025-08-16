#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define CS_PIN    5
#define RST_PIN   14
#define DIO0_PIN  26

#define DHTPIN 4          
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  dht.begin();

  LoRa.setPins(CS_PIN, RST_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("Gagal memulai LoRa!");
    while (true);
  }

  Serial.println("LoRa P2P Transmitter Siap.");
}

void loop() {
  float kelembapan = dht.readHumidity();

  if (isnan(kelembapan)) {
    Serial.println("Gagal membaca sensor Kelembapan!");
    return;
  }

  String pesan = "ID:A | Kelembapan:" + String(kelembapan, 1) + " %";

  LoRa.beginPacket();
  LoRa.print(pesan);
  LoRa.endPacket();

  Serial.println("=================================");
  Serial.println("Mengirim: " + pesan);
  Serial.print("Waktu   : ");
  Serial.println(millis());
  Serial.println();

  delay(2000);
}
