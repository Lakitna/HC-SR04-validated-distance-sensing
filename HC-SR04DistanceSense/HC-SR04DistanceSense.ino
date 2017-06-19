// Code reads the value of a HC-SR04 ultrasonic distance sensor using
// the NewPing library https://github.com/PaulStoffregen/NewPing
// These sensors are notoriously flacky, so I wrote a few validations
// on the sensor output. All so the connected servo doesn't update
// unnecessarily.
// 
// Code by Sander van Beek, 2017

//#include <Arduino.h>
#include <NewPing.h>
#include <Servo.h>


// Sensor settings
#define SEN_TRIGGER_PIN  12   // Sensor trigger pin
#define SEN_ECHO_PIN     11   // Sensor echo pin
#define SEN_MAX_DISTANCE 200  // Sensor max distance in cm
#define SEN_MIN_DISTANCE 3    // Sensor min distance in cm, everything below will be regarded as infinite

// Servo settings
#define SER_PIN          9    // Servo com pin

// Scale settings
#define SCALED_MIN_RANGE 0    // Scaled min value
#define SCALED_MAX_RANGE 180  // Scaled max value
#define SCALE_CURVE      1.5  // Scaling curve. 0 = linear, 10 = max, -10 = min

// Validation settings
#define INF_LOOPS        5    // Amount of values that have to be infinite before setting output to infinite
#define LOOPS            3    // Amount of values that have to be (almost) the same before setting output
#define POS_VARIANCE     3    // Minimum variance (plus and min) in cm between which values have to be to pass test
#define SCALED_VARIANCE  2    // Minumum variance (plus and min) between which scaled values have to be to pass test
#define SEN_MAX_CHANGE   50   // Maximum variance (plus and min) in cm between which values have to be to pass test




// Some empty vars
byte position_prev[INF_LOOPS];
byte scaled_prev;
boolean update;
byte count;
Servo ser;
NewPing sonar(SEN_TRIGGER_PIN, SEN_ECHO_PIN, SEN_MAX_DISTANCE);

void setup() {
  // Start serial
  Serial.begin(115200);

  // Servo setup
  ser.attach(SER_PIN);
}

void loop() {
  // Wait a bit
  delay(100);

  // Run update function and write the output to the servo.
  ser.write( senseUpdate( sonar ) );
}
