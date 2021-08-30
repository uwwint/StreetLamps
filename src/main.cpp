#include <Arduino.h>
#include <FastLED.h>
#include <avr/sleep.h>


#include "light_simulation.h"

CRGB led1[1];
CRGB led2[1];
extern CRGBPalette16 bb_color;
uint8_t center=140;
uint8_t min=100;
uint8_t max=190;
LightSimulation ls1,ls2,ls3;


void setup() {
  FastLED.addLeds<APA102,13,11,BGR>(led1,1);
  FastLED.addLeds<APA102,9,8,BGR>(led1,1);
  FastLED.setBrightness(64);
  bb_color = HeatColors_p;
  Serial.begin(9600);

  ls1 = LightSimulation(micros(),&(led1[0]));
  ls2 = LightSimulation(micros(),&(led2[0]));

  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
  unsigned long int m=micros();
  ls1.update(m);
  ls2.update(m);
  FastLED.show();  
}