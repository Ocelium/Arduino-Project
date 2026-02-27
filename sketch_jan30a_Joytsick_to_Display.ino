//Rayen, Thomas, Thomas, Rickey

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
Adafruit_SSD1306 myDisplay(128,64);

const int xPin = A0;
const int yPin = A1;

void setup() {
  Serial.begin(9600);
  myDisplay.begin(SSD1306_SWITCHCAPVCC,0x3C);

  //Testing the display
  myDisplay.clearDisplay();
  myDisplay.setCursor(0, 0);
  myDisplay.setTextSize(1);
  myDisplay.setTextColor(1);
  myDisplay.print("Hello World!");
  myDisplay.display();
  delay(1000);
}

void loop() {
  
  int xValue = analogRead(xPin);
  int yValue = analogRead(xPin);

  myDisplay.setTextSize(2);
  myDisplay.setTextColor(1);

  myDisplay.print("X:");
  myDisplay.println(xValue);
  myDisplay.print("Y:");
  myDisplay.print(yValue);

  myDisplay.display();

  delay(100);

  myDisplay.clearDisplay();
  myDisplay.setCursor(0, 0);
  
}