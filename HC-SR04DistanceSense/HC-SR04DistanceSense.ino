#include <Arduino.h>
#include <NewPing.h>
#include <Servo.h>
  
// Sensor settings
#define SEN_TRIGGER_PIN  12   // Sensor trigger pin
#define SEN_ECHO_PIN     11   // Sensor echo pin
#define SEN_MAX_DISTANCE 200  // Sensor max distance in cm
#define SEN_MIN_DISTANCE 3    // Sensor min distance in cm

// Servo settings
#define SER_PIN          9    // Servo com pin
#define SER_MIN_RANGE    0    // Servo min value in degrees
#define SER_MAX_RANGE    180  // Servo max value in degrees

// Validation settings
#define INF_LOOPS        5    // Amount of values that have to be infinite before setting output to infinite
#define LOOPS            3    // Amount of values that have to be (almost) the same before setting output
#define POS_VARIANCE     3    // Minimum Range (plus and min) in cm between which values have to be to pass test
#define SER_VARIANCE     2    // Minumum Range (plus and min) in degrees between which values have to be to pass test
#define POS_MAX_CHANGE   50   // Maximum range (plus and min) in cm between which values have to be to pass test

#define SCALE_CURVE      1.5  // Scaling curve. 0 = linear, 10 = max, -10 = min

// Timing settings
#define MAIN_DELAY       100  // Main delay of sketch, determines update rate



// Some empty vars
byte position_prev[INF_LOOPS];
byte ser_prev;
boolean update;
byte count;
Servo ser;
NewPing sonar(SEN_TRIGGER_PIN, SEN_ECHO_PIN, SEN_MAX_DISTANCE);

void setup() {
  // Start serial
  Serial.begin(115200);
  Serial.print("\nSetup: ");

  // Set onboard LED as output
  pinMode(13, OUTPUT);

  // Servo setup
  ser.attach(SER_PIN);
  
  Serial.println("done");
}

void loop() {
  // Wait a bit
  delay(MAIN_DELAY);

  // Run update function
  senseUpdate( sonar );
}
