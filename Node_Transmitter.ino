#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define CS_PIN    5
#define RST_PIN   14
#define DIO0_PIN  26

#define DHTPIN 4          
#define DHTTYPE DHT22     
DHT dht(DHTPIN, DHTTYPE);

int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  dht.begin();

  LoRa.setPins(CS_PIN, RST_PIN, DIO0_PIN);

  if (!LoRa.begin(433E6)) {
    Serial.println("Gagal memulai LoRa!");
    while (1);
  }

  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3); 
  LoRa.setTxPower(14);

  Serial.println("LoRa P2P Transmitter Siap.");
}

void loop() {
  float suhu = dht.readTemperature();

  // Validasi pembacaan suhu
  if (isnan(suhu)) {
    Serial.println("Gagal membaca sensor suhu!");
    return;
  }

  String pesan = "Paket #" + String(counter) + " Suhu: " + String(suhu, 1) + " C";

  Serial.print("Mengirim: ");
  Serial.println(pesan);

  LoRa.beginPacket();
  LoRa.print(pesan);
  LoRa.endPacket();

  counter++;
  delay(2000);
}
