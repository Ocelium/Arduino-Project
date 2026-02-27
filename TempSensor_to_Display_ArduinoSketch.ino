//Rayen, Thomas, Thomas, Rickey

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
Adafruit_SSD1306 myDisplay(128,64);


// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  myDisplay.begin(9600);
  myDisplay.begin(SSD1306_SWITCHCAPVCC,0x3C);

  //Testing the display
  myDisplay.clearDisplay();
  myDisplay.setCursor(0, 0);
  myDisplay.setTextSize(1);
  myDisplay.setTextColor(1);
  myDisplay.print("Hello World!");
  myDisplay.display();
  delay(1000);


  

  // Initialize device.
  dht.begin();
  myDisplay.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  myDisplay.println(F("------------------------------------"));
  myDisplay.println(F("Temperature Sensor"));
  myDisplay.print  (F("Sensor Type: ")); myDisplay.println(sensor.name);
  myDisplay.print  (F("Driver Ver:  ")); myDisplay.println(sensor.version);
  myDisplay.print  (F("Unique ID:   ")); myDisplay.println(sensor.sensor_id);
  myDisplay.print  (F("Max Value:   ")); myDisplay.print(sensor.max_value); myDisplay.println(F("°C"));
  myDisplay.print  (F("Min Value:   ")); myDisplay.print(sensor.min_value); myDisplay.println(F("°C"));
  myDisplay.print  (F("Resolution:  ")); myDisplay.print(sensor.resolution); myDisplay.println(F("°C"));
  myDisplay.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  myDisplay.println(F("Humidity Sensor"));
  myDisplay.print  (F("Sensor Type: ")); myDisplay.println(sensor.name);
  myDisplay.print  (F("Driver Ver:  ")); myDisplay.println(sensor.version);
  myDisplay.print  (F("Unique ID:   ")); myDisplay.println(sensor.sensor_id);
  myDisplay.print  (F("Max Value:   ")); myDisplay.print(sensor.max_value); myDisplay.println(F("%"));
  myDisplay.print  (F("Min Value:   ")); myDisplay.print(sensor.min_value); myDisplay.println(F("%"));
  myDisplay.print  (F("Resolution:  ")); myDisplay.print(sensor.resolution); myDisplay.println(F("%"));
  myDisplay.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  myDisplay.setTextSize(2);
  myDisplay.setTextColor(1);

  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    myDisplay.println(F("Error reading temperature!"));
  }
  else {
    myDisplay.print(F("Temp: "));
    myDisplay.print(event.temperature);
    myDisplay.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    myDisplay.println(F("Error reading humidity!"));
  }
  else {
    myDisplay.print(F("Humidity: "));
    myDisplay.print(event.relative_humidity);
    myDisplay.println(F("%"));
  }

  myDisplay.display();

  delay(100);

  myDisplay.clearDisplay();
  myDisplay.setCursor(0, 0);
}
