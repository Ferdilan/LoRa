#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CS_PIN    5
#define RST_PIN   14
#define DIO0_PIN  26

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long waktuTerima = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(CS_PIN, RST_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("Gagal memulai LoRa!");
    while (true);
  }


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED gagal diinisialisasi"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("LoRa RX Siap...");
  display.display();

  Serial.println("LoRa P2P Receiver Siap.");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {

    String pesan = "";
    while (LoRa.available()) {
      pesan += (char)LoRa.read();
    }

    waktuTerima = millis();
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    Serial.println("=================================");
    Serial.println("Pesan   : " + pesan);
    Serial.print("RSSI    : "); Serial.print(rssi); Serial.println(" dBm");
    Serial.print("SNR     : "); Serial.print(snr, 2); Serial.println(" dB");
    Serial.print("Waktu   : "); Serial.print(waktuTerima); Serial.println(" ms");
    Serial.println();

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Pesan: ");
    display.println(pesan);
    display.print("RSSI: ");
    display.print(rssi);
    display.println(" dBm");
    display.print("SNR : ");
    display.print(snr, 1);
    display.println(" dB");
    display.display();
  }
}
