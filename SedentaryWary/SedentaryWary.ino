//
// :: SedentaryWary ::
//
// * The current scientific literature clearly advises against increasced sitting time
// * A 1-2 minute break is highly recommended between each 30 min sitting session
// * This device will notify you once your 30 minute session is over and
//   check if you had your break via accelerometry ... <WIP>
//
// Grim Kriegor <grimkriegor@opmbx.org>
//
//
// Circuit suggestion:
//  * MPU-6050 (accelerometer+gyroscope) connected via SDA and SCL
//  * Indicator LED connected to a PWM enabled pin (ex. Pin 3)
//  * Indicator piezo buzzer (ex. Pin 11)
//  * Input button (PULL_UP) (ex. Pin 2)
//

#include <LEDController.h> // https://github.com/GrimKriegor/Electronics/tree/master/LEDController
#include <Wire.h>          // http://arduino.cc/en/Reference/Wire
#include <Timer.h>         // http://playground.arduino.cc/Code/Timer


//Configure
const byte LED_PIN = 5; // Info LED pin
const byte BUZZER_PIN = 11; // Info Buzzer pin
const byte BUTTON_PIN = 2; // Button pin
const int MPU = 0x68;  // I2C address of the MPU-6050

const byte DIP[] = {}; //Config


// INTENSITY_THRESHOLD, SITTING_TIME, WALKING_TIME, ALARM_TIMEOUT
const int configSettings[5][4] = {
  {200, 1, 5, 5}, //Debug Mode
  {250, 30, 1, 10}, //Normal setting
  {200, 30, 2, 20}, //Super Supreeme
  {100, 30, 3, 10},
  {0, 0, 0, 0}
};
int INTENSITY_THRESHOLD; // Level of activity to be considered as moderate to vigorous
int SITTING_TIME; // Number of minutes the program will wait before setting off the alarm
int WALKING_TIME; // Minutes walking
int ALARM_TIMEOUT;  //Seconds till alarm timeout


//Declare variables
int INTENSITY; //Intensity is the "mean" of acceleration from the 3 axis
int STAGE; //This variable selectively enables the appropriate block during loop(), depending on the current state
byte READING_NUMBER; //Index number of each reading
boolean SOUND = true;
int ACTIVITY_ARRAY[150]; //This array will store all the activity readings during the Walking setting, 5 min max
long AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; //Raw values from the MPU-6050
LEDController LED(LED_PIN);
LEDController BUZZER(BUZZER_PIN);
Timer timer;
Timer timer_aux;




//Checks the DIP switches, sets the SETTING and assigns the variables... WIP
void initConfigure() {
  
  byte SETTING = 1;
  
  INTENSITY_THRESHOLD = configSettings[SETTING][0]; 
  SITTING_TIME = configSettings[SETTING][1];
  WALKING_TIME = configSettings[SETTING][2]; 
  ALARM_TIMEOUT = configSettings[SETTING][3];
  
  sit(); //Initialize the cycle
}


//Appends current activity level to the ACTIVITY_ARRAY[]
void sumActivity() {
  
  ACTIVITY_ARRAY[READING_NUMBER] = labs(INTENSITY);
  READING_NUMBER++;

  Serial.print("Intensity Reading = ");
  Serial.println(INTENSITY);
}


//Averages the ACTIVITY_ARRAY[] and checks if the level of activity is sufficient (>= INTENSITY_THRESHOLD)
// * If so, changes to sitting mode and starts the cycle all over
// * If not, activates the alarm again and gives the user another chance of reaching the activity level
void checkActivity() {
  
  int ACTIVITY_ARRAY_SUM = 0;
  int ACTIVITY_ARRAY_READINGS = (WALKING_TIME*60)/2;
  int ACTIVITY_ARRAY_SIZE = sizeof(ACTIVITY_ARRAY) / sizeof(int);
  for (int i=0; i < ACTIVITY_ARRAY_SIZE; i++) { ACTIVITY_ARRAY_SUM = ACTIVITY_ARRAY_SUM + ACTIVITY_ARRAY[i]; }
  int ACTIVITY_ARRAY_AVERAGE = ACTIVITY_ARRAY_SUM / ACTIVITY_ARRAY_READINGS;
  
  Serial.print("Average = ");
  Serial.println(ACTIVITY_ARRAY_AVERAGE);
  
  if (ACTIVITY_ARRAY_AVERAGE > INTENSITY_THRESHOLD) {
    Serial.println("WELL DONE!");
    sit();
  } else {
    Serial.println("INSUFICIENT ACTIVITY AVERAGE, PLEASE TRY AGAIN!");
    alarm();
  }
}


// Disables the alarm sound
void alarmTimeout() {
  Serial.println("Alarm timeout...");
  if (STAGE == 1) { SOUND = false; }
}


//Initializes Sitting idle mode
// * Waits for SITTING_TIME, usually 30 mins
void sit() {
  Serial.println("-- Sitting Stage --");
  Serial.print("Waiting "); Serial.print(SITTING_TIME); Serial.println(" minutes.");
  STAGE = 0;
  BUZZER.onOff(50); BUZZER.onOff(50); BUZZER.onOff(50); BUZZER.onOff(100);
  timer.after(SITTING_TIME*60000, alarm);
}


//Initializes Alarm mode
// * User must press the button to initialize Walking mode
void alarm() {
  Serial.println("-- Alarm Stage --");
  STAGE = 1;
  SOUND = true;
  LED.onOff(100);
  BUZZER.onOff(100);
  delay(1000);
  timer_aux.after(ALARM_TIMEOUT*1000, alarmTimeout);
}


// Initializes Walking mode
// * Timer set to take a reading of the movement intensity every 2 s
// * Timer set to average and compare the readings to the INTENSITY_THRESHOLD
void walk() {
  STAGE = 2;
  Serial.println("-- Walking Stage --");
  Serial.print("Please exercise during the next "); Serial.print(WALKING_TIME); Serial.println(" minutes.");
  READING_NUMBER = 0;
  timer.every(2000, sumActivity, (WALKING_TIME*60)/2);
  timer.after(WALKING_TIME*60000+3000, checkActivity);
  BUZZER.onOff(100); BUZZER.onOff(200);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  initConfigure();
}


void loop() {
  
    timer.update();
    timer_aux.update();
  
    if (STAGE == 0) {
      LED.cycleDim(1000,0,10);
      delay(5000);
    }
  
    if (STAGE == 1) {
      if (! digitalRead(2)) { walk(); }
      if (SOUND == true) { BUZZER.onOff(100); }
      LED.onOff(100);
    }
    
    if (STAGE == 2) {
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
      AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
      AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
      
      INTENSITY = (labs(GyX)+labs(GyY)+labs(GyZ))/100;

      LED.dim(map(INTENSITY,10,500,0,100));
      delay(500);
    }
}
