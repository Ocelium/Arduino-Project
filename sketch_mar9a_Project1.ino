#include <SPI.h>
#include <SD.h>
//I did not add camera to this

const int motionSensor = 0,
const int chipSelect = 10;
File myFile;

void setup() {

  Serial.begin(9600);

  //SD CARD PREP
  //
  while (!Serial);
  pinMode(chipSelect, OUTPUT);

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true);
  }
  Serial.println("initialization done.");
  //
  //SD CARD PREP

  //Motion Sensor Prep
  pinMode(motionSensor, INPUT);

 
}

void loop() {

  //SD CARD READ AND WRITE
  //
  //
  //open file named people_data.txt
  myFile = SD.open("people_data.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to people_data.txt...");

    //Motion sensor detect on or off (to change sensitivity, potentiometers on back of module)
    if (digitialRead(motionSensor) ==) {
      //turn camera on for 10s, 5fps
    }
    else {
      //keep camera off
    }


    myFile.print();
    
    // close the file:
    myFile.close();
    Serial.println("done.");

  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  //Read it now
  myFile = SD.open("people_data.txt");
  if (myFile) {
    Serial.println("people_data.txt");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  delay(200);
  //
  //
  //SD CARD READ AND WRITE
}