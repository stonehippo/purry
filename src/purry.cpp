#include <Arduino.h>
#include "Volume3.h"
#include "Adafruit_VCNL4010.h"

#define speakerPin 9

void resetPurr();
uint16_t freq();
uint16_t purr();
long readProx();
bool checkEnable(long reading);

Adafruit_VCNL4010 proxSensor;

// cats typically purr between 24-50hz;
const uint16_t minFrequency = 24;
const uint16_t maxFrequency = 50;

const float START_RADIAN = 4.712; // Start at the mid-point of the sine wave (all the way off)
const float MAX_RADIAN = 10.995;
const float RADIAN_STEP = 0.001; // how many radians do we step the sine wave per loop?
const float SIN_OFFSET = 127.5; // the offset to map the sin values back to 0-255 for analogWrite

float volume = START_RADIAN;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  proxSensor.begin();
}


void loop() {
  long reading = readProx();
  if(checkEnable(reading)) {
    uint16_t f = freq();
    uint16_t p = purr();
    vol.tone(speakerPin, f, p);
    Serial.print(f);
    Serial.print(" ");
    Serial.print(p);
    Serial.print(" ");
  } else {
    resetPurr();
    vol.noTone();
  }
  Serial.println(reading);
}

uint16_t freq() {
  uint16_t f =  random(minFrequency, maxFrequency);
  return f;
}

void resetPurr() {
  volume = START_RADIAN;
}

// generate a sine wave for the volume
uint16_t purr() {
  if (volume < MAX_RADIAN) {
    volume = volume + (RADIAN_STEP * random(0,48));
  } else {
    resetPurr();
  }
  float p= sin(volume) * SIN_OFFSET + SIN_OFFSET;
  return long(p);
}

long readProx() {
  return proxSensor.readProximity();
}

bool checkEnable(long reading) {
  if (reading > 3000) {
    return true;
  } else {
    return false;
  }
}
