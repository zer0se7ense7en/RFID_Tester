#include <Arduino.h>
#include <SPI.h>


#define PIN_RESET  9 // SPI Reset Pin
#define PIN_SS    10 // SPI Slave Select Pin

MFRC522 mfrc522(PIN_SS, PIN_RESET);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print("Detected UID:");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0x0" : " 0x");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    mfrc522.PICC_HaltA();
    delay(1000);
    }
}