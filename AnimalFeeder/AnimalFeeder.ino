//
// :: AnimalFeeder ::
//
// ...
//
// Grim Kriegor <grimkriegor@opmbx.org>
//
//
// Circuit suggestion:
//  * 
//

#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h> 
#include <EEPROM.h>


//Peripheral configuration
byte FEEDER_VALVE = 2;
byte SERVE_NOW_BUTTON = 7;
byte SERVE_SKIP_BUTTON = 8;
byte SERVING_QUANTITY_DIAL = A0;
byte SERVING_TIME_DIAL = A1;
byte LED[3] = {11,12,13};

//Variable initialization
byte ERROR_ALERT = 0;
volatile boolean TODAYS_SERVING = false;
volatile byte SERVING_QUANTITY = 1;
volatile byte SERVING_TIME = 37;



void setup() {
  //Initialize Serial and Clock communication
  Serial.begin(9600);
  setSyncProvider(RTC.get);

  //Peripheral pin modes
  pinMode(FEEDER_VALVE, OUTPUT);
  pinMode(SERVE_NOW_BUTTON, INPUT_PULLUP);
  pinMode(SERVE_SKIP_BUTTON, INPUT_PULLUP);
  pinMode(SERVING_QUANTITY_DIAL, INPUT);
  pinMode(SERVING_TIME_DIAL, INPUT);
  for (byte i=0; i<3; i++) { pinMode(LED[i], OUTPUT); }

  //Check EEPROM to see if there has been a serving today
  if (EEPROM.read(month()) == day()) { 
    TODAYS_SERVING = true;
    Serial.println("No servings today!"); 
  }
}

void loop() {
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.println(year()); 

  if (!TODAYS_SERVING && minute() == SERVING_TIME) { Serial.println("POOP!"); feed(); }
}




void feed() {
  //Open feeding valve for SERVING_QUANTITY seconds
  digitalWrite(FEEDER_VALVE, HIGH);
  delay(SERVING_QUANTITY*1000);
  digitalWrite(FEEDER_VALVE, LOW);

  //Define TODAYS_SERVING as true and write to EEPROM "month()" address
  TODAYS_SERVING = true;
  EEPROM.update(month(), day());
}


void defineServingQuantity() {
  unsigned int _VALUE = analogRead(SERVING_QUANTITY_DIAL);
  SERVING_QUANTITY = map(_VALUE,0,1024,1,5);
}


void defineServingTime() {
  unsigned int _VALUE = analogRead(SERVING_TIME_DIAL);
  if      (_VALUE < 300) {}
  else if (_VALUE > 300 && _VALUE < 400) {}
  else if (_VALUE > 300 && _VALUE < 400) {}
  else if (_VALUE > 300 && _VALUE < 400) {}
  else if (_VALUE > 500) {}
}




