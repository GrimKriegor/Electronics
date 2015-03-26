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

#include <LEDController.h>
#include <Wire.h>
#include <Timer.h>


//Configure
const byte LED_PIN = 3; // Info LED pin
const byte BUZZER_PIN = 11; // Info Buzzer pin
const byte BUTTON_PIN = 2; // Button pin
const long INTENSITY_THRESHOLD = 500; // Level of activity to be considered as moderate to vigorous
const int MAX_SITTING_TIME = 1; // Number of minutes the program will wait before setting off the alarm


//Declare variables
long MAX_SITTING_TIME_SECS = MAX_SITTING_TIME * 60;
int INTENSITY;
byte SETTING;
byte READING_NUMBER;
long ACTIVITY_ARRAY[] = {};
const int MPU=0x68;  // I2C address of the MPU-6050
long AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
LEDController LED(LED_PIN);
LEDController BUZZER(BUZZER_PIN);
Timer t;

//Appends current activity level to the array
void sumActivity() {
  ACTIVITY_ARRAY[READING_NUMBER] = INTENSITY;
  READING_NUMBER++;

  Serial.print("INTENSITY READING = ");
  Serial.println(INTENSITY);
  Serial.println(SETTING);
  BUZZER.onOff(100); //DEBUG
  SETTING = 2;
}

//Averages the array and checks if the level of activity is sufficient
void checkActivity() {
  int ACTIVITY_ARRAY_SUM = 0;
  int ACTIVITY_ARRAY_SIZE = sizeof(ACTIVITY_ARRAY) / sizeof(int);
  for (int i=0; i <= ACTIVITY_ARRAY_SIZE; i++) { ACTIVITY_ARRAY_SUM += ACTIVITY_ARRAY[i]; }
  int ACTIVITY_ARRAY_AVERAGE = ACTIVITY_ARRAY_SUM / ACTIVITY_ARRAY_AVERAGE;
  
  Serial.print("ACTIVITY AVERAGE = ");
  Serial.println(ACTIVITY_ARRAY_AVERAGE);
  
  if (ACTIVITY_ARRAY_AVERAGE > INTENSITY_THRESHOLD) {
    sit();
  } else {
    alarm();
  }
}

void sit() {
  Serial.println("Sitting.");
  SETTING = 0;
  BUZZER.onOff(50); BUZZER.onOff(50); BUZZER.onOff(50); BUZZER.onOff(100);
  t.after(MAX_SITTING_TIME_SECS*1000, alarm);
}

void alarm() {
  Serial.println("Alarm!");
  SETTING = 1;
  LED.onOff(100);
  BUZZER.onOff(100);
}

void walk() {
  Serial.println("Walking.");
  unsigned long ACTIVITY_ARRAY[] = {};
  SETTING = 2;
  READING_NUMBER = 0;
  //t.every(5000, sumActivity, 12);
  t.after(60000, checkActivity);
  BUZZER.onOff(100); BUZZER.onOff(200);
}
  

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  sit();
}


void loop() {

    t.update();
  
    if (SETTING == 0) {
      LED.cycleDim(1000,0,20);
    }
  
    if (SETTING == 1) {
      if (! digitalRead(2)) { SETTING = 2; walk();}
      BUZZER.onOff(100);
      LED.onOff(100);
    }
    
    if (SETTING == 2) {
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
      
      ACTIVITY_ARRAY[READING_NUMBER] = INTENSITY;
      READING_NUMBER++;

      LED.dim(map(INTENSITY,10,500,0,100));
      Serial.println(INTENSITY);
    }
  

}
