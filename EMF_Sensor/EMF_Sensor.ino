// :: EMF Sensor ::
// Original concept by Aaron ALAI - http://www.aaronalai.com/emf-detector

int NUMREADINGS = 15;
int SENSITIVITY = 15;
byte PROBE_PIN   = A7;
byte LEDS[] = {12,10,8,6,4,2};

byte LEDS_COUNT = sizeof(LEDS) / sizeof(byte);
int VALUE = 0;

//smoothing
int READINGS[15];
int INDEX = 0;
int TOTAL = 0;
int AVERAGE = 0;
int LEVEL = 0;


void setup() {
  for (int i = 0; i < NUMREADINGS; i++) { READINGS[i] = 0; }

  for (int i = 0; i < LEDS_COUNT; i++) { pinMode(LEDS[i], OUTPUT); }
  for (int i = 0; i < LEDS_COUNT; i++) { digitalWrite(LEDS[i], HIGH); delay(100); digitalWrite(LEDS[i], LOW); }
  
  Serial.begin(9600);
}

void loop() {
  VALUE = analogRead(PROBE_PIN); // Take reading
  
  if (VALUE >=1) {
    VALUE = constrain(VALUE, 1, SENSITIVITY);
    VALUE = map(VALUE, 1, SENSITIVITY, 1, 1023);
    
    TOTAL -= READINGS[INDEX];
    READINGS[INDEX] = VALUE;
    TOTAL += READINGS[INDEX];
    INDEX++;
    
    if (INDEX >= NUMREADINGS) { INDEX = 0; };
    
    AVERAGE = TOTAL / NUMREADINGS;
    
    LEVEL = map(AVERAGE, 1, 1023, 0, 6);
    
    for (int i = 0; i < LEVEL; i++) { digitalWrite(LEDS[i], HIGH); }
    for (int i = 0; i < LEDS_COUNT; i++) { digitalWrite(LEDS[i], LOW); }

    
    Serial.println(VALUE);
  }
}
