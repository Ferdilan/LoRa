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
  LoRa.setPins(CS_PIN, RST_PIN, DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Gagal Mulai");
    while (1);
  }
  Serial.println("Receiver C siap.");

  // if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
  //   Serial.println(F("OLED gagal diinisialisasi"));
  //   while (1);
  // }

  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setTextColor(WHITE);
  // display.setCursor(0,0);
  // display.println("LoRa RX Siap...");
  // display.display();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String pesan = "";
    while (LoRa.available()) {
      pesan += (char)LoRa.read();
    }
    Serial.println("Pesan diterima: " + pesan);
    // display.clearDisplay();
    // display.setCursor(0, 0);
    // display.print("Pesan: ");
    // display.println(pesan);
    // display.display();
  }
}
