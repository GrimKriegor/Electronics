//
// :: Movement Candle ::
//
// Simple candle contraption that turns on
// whenever it detects motion in the dark
//
// Grim Kriegor <grimkriegor@opmbx.org>
//
//
// Circuit suggestion:
// * ATTiny85
// * 2 LED (red, yellow) connected to PWM pins 0 and 1 and ground via 47ohm resistor
// * Pyroelectric sensor connected to pin 3
// * Light sensitive resistor connected to analog pin 2, 5V and ground via 1Kohm resistor
// * Potentiometer connected to analog pin 1
//


//ATTiny85 pin configuration
byte IR_PIN = 3;              //Pyroelectric sensor
byte POTENTIOMETER_PIN = A1;  //Sensitivity potentiometer
byte LIGHTSENSOR_PIN = A2;    //Light sensitive resistor (LSR)
byte LED1 = 0;
byte LED2 = 1;


//LED flicker intensity, min and max
byte INTENSITY_MAX = 100;
byte INTENSITY_MIN = 50;
int  TIMEOUT = 10; //For how many seconds shall the candle flicker when motion is detected?

//If set to true, the LSR can be used to ajust light sensitivity
bool CONFIGURABLE = true;

//Se......
void setup() {
  pinMode(IR_PIN, INPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {

  //If CONFIGURABLE is set to true, the value read from the potentiometer will be used to
  //define the light sensitivity, otherwise it will be a fixed value
  unsigned int SENSITIVITY;
  if (CONFIGURABLE) { SENSITIVITY = map(analogRead(A1),0,1023,0,50); }
  else { SENSITIVITY = 50; }


  //If motion is detected via pyroelectric sensor, and its dark, turn candle on
  if ( digitalRead(IR_PIN) && analogRead(LIGHTSENSOR_PIN) < SENSITIVITY) {
    
    //During this period the 2 LEDs will flicker like a candle
    int COUNT = 0;
    while (COUNT < TIMEOUT*10) {
      
      //The LEDs will constantly and randomly change intensity between INTENSITY_MIN and INTENSITY_MAX
      analogWrite(LED1, (random(INTENSITY_MIN, INTENSITY_MAX)));
      delay(random(0,5));
      analogWrite(LED2, (random(INTENSITY_MIN, INTENSITY_MAX)));
      delay(random(80,150));

      COUNT++;
    }

    //When no motion is detected, both LEDs default to off
    digitalWrite(LED1, LOW); 
    digitalWrite(LED2, LOW);
    
  } 
}

