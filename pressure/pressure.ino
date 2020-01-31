#include "ms5540c.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // I2C Address: 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
 
}
void loop() {
  // put your main code here, to run repeatedly:
  static MS5540C snr;

  snr.calibration();
  snr.measure();
  //Serial.println(snr.temp);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.print(snr.temp);
  display.print("C\n");
  display.print(snr.pcompreal);
  display.print("mbr\n");
  display.print(snr.pcomphg);
  display.print("mmhg");
  display.display(); 
  
  //Serial.println(snr.pcompreal);
  //Serial.println(snr.pcomphg);
  delay(1000);
  
}
