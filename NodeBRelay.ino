#include <SPI.h>
#include <LoRa.h>

#define CS_PIN 5
#define RST_PIN 14
#define DIO0_PIN 26

void setup() {
  Serial.begin(9600);
  LoRa.setPins(CS_PIN, RST_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Gagal Mulai");
    while (1);
  }
  Serial.println("Relay siap.");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String pesanMasuk = "";
    while (LoRa.available()) {
      pesanMasuk += (char)LoRa.read();
    }
    Serial.println("Pesan diterima dari A: " + pesanMasuk);
    
    // Relay ke C
    String pesanRelay = pesanMasuk + " -> relay";
    LoRa.beginPacket();
    LoRa.print(pesanRelay);
    LoRa.endPacket();
    Serial.println("Diteruskan ke C: " + pesanRelay);
  }
}
