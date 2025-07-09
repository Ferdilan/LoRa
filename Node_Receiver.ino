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

unsigned long waktuTerima;
int counter = 0;
int receivedPackets = 0;
int totalPackets = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(CS_PIN, RST_PIN, DIO0_PIN);

  if (!LoRa.begin(433E6)) {
    Serial.println("Gagal memulai LoRa!");
    while (1);
  }

  LoRa.setSpreadingFactor(7); // SF tetap
  LoRa.setSignalBandwidth(125E3); // 125 kHz BW
  LoRa.setTxPower(14);  

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
  Serial.println("time_ms,paket,rssi_dbm,snr_db"); // header CSV
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    totalPackets++;

    String isiPesan = "";
    while (LoRa.available()) {
      isiPesan += (char)LoRa.read();
    }

    waktuTerima = millis();
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    // Cetak data dalam format CSV
    Serial.print(waktuTerima);
    Serial.print(",");
    Serial.print(isiPesan);
    Serial.print(",");
    Serial.print(rssi);
    Serial.print(",");
    Serial.println(snr, 2);


     // Cetak ke OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Pesan: ");
    display.println(isiPesan);
    display.print("RSSI: ");
    display.print(rssi);
    display.println(" dBm");
    display.print("SNR : ");
    display.print(snr, 1);
    display.println(" dB");
    display.print("Paket: ");
    display.println(totalPackets);
    display.display();
  }
}
