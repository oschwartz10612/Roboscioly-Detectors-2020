#include <Arduino.h>
#include <math.h>
#include "ADC.h"

ADC *adc = new ADC(); // adc object

//LEDS
#define LOWERRED 55
#define UPPERRED 500000

#define LOWERGREEN 20 
#define UPPERGREEN 50

#define LOWERBLUE 0
#define UPPERBLUE 20

#define RED 3
#define GREEN 5
#define BLUE 7

//SENSOR
#define OFFSET 0;
#define OFFSET2 0;

#define THERMISTORPIN A0         
#define NUMSAMPLES 50
#define VCONSTANT 5.01/adc->getMaxValue(ADC_0)
 
int samples[NUMSAMPLES];
 
void setup(void) {
	Serial.begin(9600);
  adc->setResolution(16);
}
 
void loop(void) {

    uint8_t i;
    float average;
  
    for (i=0; i< NUMSAMPLES; i++) {
      samples[i] = adc->analogRead(THERMISTORPIN, ADC_0);
      delay(10);
    }

    average = 0;
    for (i=0; i< NUMSAMPLES; i++) {
      average += samples[i];
    }
  
    average /= NUMSAMPLES;

    float voltage;
    voltage = average * VCONSTANT;

    Serial.println(voltage, 25);

  delay(500);
}
