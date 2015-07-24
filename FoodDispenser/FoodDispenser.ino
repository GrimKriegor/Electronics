//
// :: FoodDispenser ::
// My father needed a chicken feeder so he wouldn't need to drive everyday to feed...
// 
// 
//
// Grim Kriegor <grimkriegor@opmbx.org>
//
//
// Circuit suggestion:
//  * 
//

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <EEPROM.h>


//Instance preferences
const boolean      DEBUG = true;
const unsigned int QUANTITY_MIN = 1;
const unsigned int QUANTITY_MAX = 10;
const boolean      CONFIG_TIME_OF_SERVING = true;
const byte         CONFIG_TIME_OF_SERVING_HOURS[] = { 8, 10, 12, 14, 16 };

//Peripheral configuration
byte FEEDER = 2;
byte BUTTON_SERVE = 8;
byte BUTTON_SKIP = 9;
byte DIAL_QUANTITY = A0;
byte DIAL_TIME = A1;
byte LED[2] = {11,12};

//Variable initialization
byte ERROR_ALERT;
volatile boolean SERVED;
volatile byte QUANTITY;
volatile byte TIME_OF_SERVING;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* memory() reads or writes to the EEPROM.
 * Takes 2 arguments, _OPERATION (char) being read 'r' or write 'w' and _DATA (byte).
 * The EEPROM address is equal to the current month, returned by the RTC clock
 * in an attempt to do wear leveling 
 */
byte memory(char _OPERATION, byte _DATA=day()) {
  byte _EEPROM_ADDRESS = month();
  if      (_OPERATION == 'w') { EEPROM.update(_EEPROM_ADDRESS, _DATA); }
  else if (_OPERATION == 'r') { return EEPROM.read(_EEPROM_ADDRESS); }
}


/* serve() delivers food thru the FEEDER peripheral
 * QUANTITY, defined by defineServingQuantity() is just the duration the FEEDER stays active.
 * After each serving, SERVED becomes true and the current day is written to the EEPROM.
 */
void serve() {
  //Define serving quantity
  defineServingQuantity();
  
  //Open feeding valve for QUANTITY seconds
  digitalWrite(FEEDER, HIGH);
  delay(QUANTITY*1000);
  digitalWrite(FEEDER, LOW);

  //Define SERVED as true and write to EEPROM "month()" address
  SERVED = true;
  memory('w', day());
  if (DEBUG) Serial.println("Serving complete.");
}


/* skipServe() allows the user to skip the current day's serving
 * and writes to memory
 */
void skipServe() {
    if (SERVED) { SERVED = false; memory('w', 255); }
    else        { SERVED = true; memory('w', day()); }
}


/* defineServingQuantity() sets the quantity of the next serving
 * The value read from the DIAL_QUANTITY potentiometer gets translated to
 * a number of seconds between QUANTITY_MAX and QUANTITY_MIN
 */
void defineServingQuantity() {
  unsigned int _VALUE = analogRead(DIAL_QUANTITY);
  QUANTITY = map(_VALUE, 0, 1024, QUANTITY_MIN, QUANTITY_MAX);
}


/* defineServingTime() sets the time of the daily serving
 * If CONFIG_TIME_OF_SERVING is true in the config, the values from the DIAL_TIME potentiometer
 * will be converted to serving times defined by the CONFIG_TIME_OF_SERVING_HOURS[] byte array
 */
void defineServingTime() {
  if (CONFIG_TIME_OF_SERVING) {
    unsigned int _VALUE = analogRead(DIAL_TIME);
    if      (_VALUE < 300)                 { TIME_OF_SERVING = CONFIG_TIME_OF_SERVING_HOURS[0]; }
    else if (_VALUE > 300 && _VALUE < 400) { TIME_OF_SERVING = CONFIG_TIME_OF_SERVING_HOURS[1]; }
    else if (_VALUE > 300 && _VALUE < 400) { TIME_OF_SERVING = CONFIG_TIME_OF_SERVING_HOURS[2]; }
    else if (_VALUE > 300 && _VALUE < 400) { TIME_OF_SERVING = CONFIG_TIME_OF_SERVING_HOURS[3]; }
    else if (_VALUE > 500)                 { TIME_OF_SERVING = CONFIG_TIME_OF_SERVING_HOURS[4]; }
  } else { TIME_OF_SERVING = CONFIG_TIME_OF_SERVING_HOURS[0]; }
}

/* checkError() checks the Feeder for errors and returns the error code in a byte value
 *  Error 1 - Detected when the RTC resets its time (has a year lower than 2015)
 */
byte checkError() {
  byte _ERROR;
  if (year() < 2015) { _ERROR = 1; } //RTC needs battery change
  else               { _ERROR = 0; };
  return _ERROR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {
  /* Initialize Serial and Clock communication */
  if (DEBUG) Serial.begin(9600);
  setSyncProvider(RTC.get);

  /* Peripheral pin modes */
  pinMode(FEEDER, OUTPUT);
  pinMode(BUTTON_SERVE, INPUT_PULLUP);
  pinMode(BUTTON_SKIP, INPUT_PULLUP);
  pinMode(DIAL_QUANTITY, INPUT);
  pinMode(DIAL_TIME, INPUT);
  for (byte i=0; i<3; i++) { pinMode(LED[i], OUTPUT); }

  /* Greeting */
  if (DEBUG) Serial.println("\n\n\n\n\n\n\n\n\n--- FoodDispenser :: Initializing ---");

  /* Check EEPROM to see if there has been a serving today */
  if (memory('r') == day()) { 
    SERVED = true;
    if (DEBUG) Serial.println("There has already been a serving today."); 
  } else {
    SERVED = false;
    if (DEBUG) Serial.println("Daily serving awaiting.");
  }
}



void loop() {
  /* Prints current HH:MM:SS DD/MM/YYYY via Serial */
  if (DEBUG) Serial.print(hour()); Serial.print(":"); Serial.print(minute()); Serial.print(":"); Serial.print(second()); Serial.print(" "); Serial.print(day()); Serial.print("/"); Serial.print(month()); Serial.print("/"); Serial.println(year()); 
  
  /* Constantly tries to define TIME_OF_SERVING */
  defineServingTime();
  

  /* Manages the first indicator LED
   * This LED is supposed to stay always ON when the feeder is operating normally
   * In case of Error it will blink in a specific pattern
   * (see checkError() documentation)
   * Error 1 - Blink on and off in a regular pattern
   */
  byte ERROR_ALERT = checkError();
  if (ERROR_ALERT == 1) { digitalWrite(LED[1], HIGH); delay(500); digitalWrite(LED[1], LOW); }
//  else if () {}
  else { digitalWrite(LED[0], HIGH); }

  /* Manages the second indicator LED
   * If the daily serving has been dispensed it will turn the LED on
   */
  if (SERVED) { digitalWrite(LED[1], HIGH); }
  else        { digitalWrite(LED[1], LOW);  }

 
  /* Calls skipServe() by pressing BUTTON_SKIP */
  if (!digitalRead(BUTTON_SKIP)) {
    skipServe();
    delay(1000);
  }

  /* Calls serve() by pressing the BUTTON_SERVE */
  if (!digitalRead(BUTTON_SERVE)) {
    serve();
    delay(1000);
  }

  /* If there hasn't been a serving today and the TIME_OF_SERVING has passed
   * call serve() immediately
   */
  if (!SERVED && minute() >= TIME_OF_SERVING) { serve(); }
}
