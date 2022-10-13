#include <Arduino.h>
#include "HX711.h"
#include "max6675.h"
#include <SPI.h>
#include <SD.h>

int fire = 6;
int off_key = A3;
int start_key = 1234;
int start = 10;
int warning = 5;
int soPin = A0;// SO=Serial Out
int csPin = A1;// CS = chip select CS pin
int sckPin = A2;// SCK = Serial Clock pin
int LOADCELL_DOUT_PIN = 2; // load cell
int LOADCELL_SCK_PIN = 3; // load cell
double load = 0; // load variable newton
double temp = 0; // temp value
int command = 0; 
HX711 scale; // scale object
MAX6675 robojax(sckPin, csPin, soPin); // temp object
File myFile; // SD card file

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  while (!Serial1) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (!SD.begin(53)) {
    Serial1.println("SD card initialization failed!");
    while (1);
  }
  pinMode(fire, OUTPUT);
  pinMode(start_key, OUTPUT);
  pinMode(warning, OUTPUT);
  pinMode(start, OUTPUT);
  digitalWrite(start, LOW);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(36.059);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0
  
  Serial1.println("Enter PIN to continue");
  while(Serial1.available()==0){}
  int start_input = Serial1.parseInt();  
  if(start_input==start_key){  
    Serial1.println("Starting the motor...");
    digitalWrite(start, HIGH);
    pinMode(9, OUTPUT);
    digitalWrite(9,HIGH);
    delay(300); 
  }else{
    Serial1.println("Check the passcode and try again!");
    while(1);
  }
    
  
}

void loop() {
  
  load = scale.get_units()/1000UL*9.81;
  temp = (robojax.readCelsius()-1036)*1.99-0.45;  
  Serial1.print(load);
  Serial1.println(" N");
  delay(200); 
  // Serial1.print("Temprerature: ");
  // Serial1.print(temp);
  // Serial1.println(" C");
  if(temp>=500){
    Serial1.print("Temprerature: ");
    Serial1.print(temp);
    Serial1.println(" C");    
    digitalWrite(fire, HIGH);
    Serial1.println("Fire safety system activated!");
    for(int i=0; i==0; i=i){
    Serial1.println("Temperature is too high!");
    digitalWrite(warning, HIGH);
    delay(500);
    digitalWrite(warning, LOW);
    delay(500);
     }
  }
   myFile = SD.open("load.txt", FILE_WRITE);
   if (myFile) {
    myFile.print("F= ");
    myFile.print(load);
    myFile.println(" N");
    // close the file:
    myFile.close();
    // Serial1.println("load saved to SD card");
  } else {
    // if the file didn't open, print an error:
    Serial1.println("error opening load.txt");
  }  
  if(Serial1.available()>0){
    command = Serial1.parseInt();    
  }
  if (command == 1){
    Serial1.print("Temprerature: ");
    Serial1.print(temp);
    Serial1.println(" C");
  }  
}
