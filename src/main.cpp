#include <Arduino.h>
#include <math.h>
#include "ADC.h"

ADC *adc = new ADC(); // adc object

#define THERMISTORPIN A0         
#define NUMSAMPLES 50
#define VCONSTANT 4.096/adc->getMaxValue(ADC_0)
 
int samples[NUMSAMPLES];
 
void setup(void) {
	Serial.begin(9600);
  adc->setResolution(16);
  adc->setReference(ADC_REFERENCE::REF_EXT, ADC_0);
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

  delay(1000);
}
