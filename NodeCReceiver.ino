#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CS_PIN 5
#define RST_PIN 14
#define DIO0_PIN 26

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(CS_PIN, RST_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Gagal Mulai");
    while (1);
  }
  Serial.println("Node C (Receiver) siap.");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED gagal diinisialisasi"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Node C - Ready");
  display.display();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String pesan = "";
    while (LoRa.available()) {
      pesan += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    unsigned long waktu = millis();
    
    Serial.println("=================================");
    Serial.println("Waktu: " + String(waktu) + " ms");
    Serial.println("Pesan diterima: " + pesan);
    Serial.println("RSSI: " + String(rssi) + " dBm");
    Serial.println("SNR : " + String(snr, 2) + " dB");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Pesan:");
    display.setCursor(0, 10);
    display.print(pesan);
    display.setCursor(0, 30);
    display.print("RSSI: ");
    display.print(rssi);
    display.print(" dBm");
    display.setCursor(0, 40);
    display.print("SNR : ");
    display.print(snr, 1);
    display.print(" dB");
    display.display();
  }
}
