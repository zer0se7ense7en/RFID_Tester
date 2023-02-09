#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PIN_RESET  9 // SPI Reset Pin
#define PIN_SS    10 // SPI Slave Select Pin

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

MFRC522 rfid(PIN_SS, PIN_RESET);
MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// '077', 128x64px
const unsigned char epd_bitmap_077 [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xc3, 0xff, 0xfc, 0x7f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xb2, 0xa6, 0x76, 0x56, 0x67, 0x64, 0x7f, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x55, 0xad, 0x65, 0x55, 0xd6, 0xd5, 0x7f, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0xf5, 0xa5, 0xed, 0xd6, 0x1e, 0xdd, 0x7f, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x2c, 0x55, 0x5c, 0x54, 0x0d, 0xc5, 0x7f, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xfe, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x07, 
	0xff, 0xf8, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x07, 
	0xff, 0xf0, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x0f, 
	0xff, 0xe0, 0x0c, 0x0f, 0xc0, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0f, 
	0xff, 0xc0, 0x1e, 0x07, 0x80, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x1f, 
	0xff, 0x80, 0x3e, 0x07, 0x81, 0xff, 0xe0, 0x0c, 0x0f, 0xff, 0x00, 0x70, 0x00, 0x00, 0xff, 0xff, 
	0xff, 0x00, 0x7e, 0x07, 0x03, 0xff, 0xe0, 0x08, 0x1f, 0xff, 0x00, 0x70, 0x00, 0x01, 0xff, 0xff, 
	0xfe, 0x00, 0x7e, 0x03, 0x03, 0xff, 0xc0, 0x08, 0x1f, 0xfe, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xfe, 0x00, 0xfe, 0x03, 0x07, 0xff, 0xc0, 0x18, 0x3f, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xfc, 0x01, 0xfe, 0x02, 0x07, 0xff, 0x80, 0x18, 0x3f, 0xfc, 0x00, 0xc0, 0x00, 0x0f, 0xff, 0xff, 
	0xf8, 0x01, 0xfe, 0x03, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xfc, 0x01, 0xc0, 0x00, 0x1f, 0xff, 0xff, 
	0xf8, 0x03, 0xfc, 0x03, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xf8, 0x01, 0x80, 0x01, 0xff, 0xff, 0xff, 
	0xf0, 0x03, 0xfc, 0x03, 0xff, 0xfe, 0x00, 0x7f, 0xff, 0xf0, 0x03, 0x8f, 0xc3, 0xff, 0xff, 0xff, 
	0xf0, 0x07, 0xfc, 0x03, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xe0, 0x07, 0x8f, 0xc7, 0xff, 0xff, 0xff, 
	0xf0, 0x07, 0xfc, 0x03, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xe0, 0x07, 0xcf, 0x87, 0xff, 0xff, 0xff, 
	0xe0, 0x07, 0xf8, 0x03, 0xff, 0xf0, 0x01, 0xff, 0xff, 0xc0, 0x0f, 0xcf, 0x8f, 0xff, 0xff, 0xff, 
	0xe0, 0x0f, 0xf8, 0x03, 0xff, 0xf0, 0x03, 0xff, 0xff, 0x80, 0x1f, 0xef, 0x8f, 0xff, 0xff, 0xff, 
	0xe0, 0x0f, 0xf8, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xff, 0x00, 0x3f, 0xff, 0x1f, 0xff, 0xff, 0xff, 
	0xc0, 0x1f, 0xf0, 0x03, 0xff, 0xc0, 0x0f, 0xff, 0xfe, 0x00, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 
	0xc0, 0x1f, 0xf0, 0x07, 0xff, 0x80, 0x1f, 0xff, 0xfc, 0x00, 0x7f, 0xfc, 0x3f, 0xff, 0xff, 0xff, 
	0xc0, 0x1f, 0xf0, 0x07, 0xff, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0xe0, 0x00, 0x7f, 0xff, 0xff, 0xff, 
	0xc0, 0x3f, 0xe0, 0x07, 0xfe, 0x00, 0x3f, 0xff, 0xf8, 0x01, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xc0, 0x3f, 0xe0, 0x07, 0xfe, 0x00, 0x7f, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xc0, 0x3f, 0xc0, 0x0f, 0xfc, 0x00, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xc0, 0x7f, 0xc0, 0x0f, 0xf8, 0x01, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xc0, 0x7f, 0x80, 0x1f, 0xf0, 0x01, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xc0, 0x7f, 0x80, 0x1f, 0xf0, 0x03, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xc0, 0x7f, 0x00, 0x3f, 0xe0, 0x07, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xc0, 0xfe, 0x00, 0x3f, 0xe0, 0x07, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xc0, 0xfe, 0x00, 0x7f, 0xc0, 0x0f, 0xff, 0xfe, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xc0, 0xfc, 0x00, 0xff, 0x80, 0x1f, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xe0, 0xf8, 0x00, 0xff, 0x80, 0x1f, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xe0, 0xf0, 0x01, 0xff, 0x00, 0x3f, 0xff, 0xfc, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x60, 0x03, 0xff, 0x00, 0x3f, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0x07, 0xff, 0x00, 0x7f, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf8, 0x00, 0x1f, 0xfe, 0x00, 0x7f, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xfc, 0x00, 0x3f, 0xfe, 0x00, 0x7f, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0x00, 0xff, 0xfe, 0x00, 0x7f, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char* epd_bitmap_allArray[1] = {
	epd_bitmap_077
};

void array_to_string(byte array[], unsigned int len, char buffer[])
{
   for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  oled.clearDisplay(); // clear display
  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.drawBitmap(0, 0, epd_bitmap_077, 128, 64, WHITE);
  oled.display();
}
 
void loop() {
  //oled.fillCircle(120, 56, 4, BLACK);
  oled.drawCircle(120, 56, 5, WHITE);
  oled.display();

  delay(50);

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  oled.fillCircle(120, 56, 5, WHITE);
  oled.display();

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    // NEW CARD DETECTED

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    oled.clearDisplay(); // clear display
    oled.drawRect(0,0,128,64,WHITE);
    oled.setTextSize(1);
    oled.setCursor(3, 3);        // position to display
    oled.println("Tag type:  (NEW TAG)");
    oled.setCursor(3,13);
    oled.setTextSize(2);
    oled.println(rfid.PICC_GetTypeName(piccType)); // text to display

    oled.setCursor(3,35);
    oled.setTextSize(1);
    oled.println("NUID in HEX:");
    oled.setCursor(3,45);
    oled.setTextSize(2);

    char strhex[32] = "";
    array_to_string(rfid.uid.uidByte, rfid.uid.size, strhex);
    oled.println(strhex);

    oled.display();               // show on OLED
  }
  else {
    // CARD READ PREVIOUSLY:
    oled.clearDisplay(); // clear display
    oled.drawRect(0,0,128,64,WHITE);
    oled.setTextSize(1);
    oled.setCursor(3, 3);        // position to display
    oled.println("Tag type: (SAME TAG)");
    oled.setCursor(3,13);
    oled.setTextSize(2);
    oled.println(rfid.PICC_GetTypeName(piccType)); // text to display

    oled.setCursor(3,35);
    oled.setTextSize(1);
    oled.println("NUID in HEX:");
    oled.setCursor(3,45);
    oled.setTextSize(2);

    char strhex[32] = "";
    array_to_string(rfid.uid.uidByte, rfid.uid.size, strhex);
    oled.println(strhex);

    oled.display();               // show on OLED
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
