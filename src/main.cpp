#include <Arduino.h>
#include "ADC.h"

ADC *adc = new ADC(); // adc object

//LEDS
#define LOWERRED 10
#define UPPERRED 20

#define LOWERGREEN 20 
#define UPPERGREEN 30

#define LOWERBLUE 30
#define UPPERBLUE 40

#define RED 2
#define GREEN 3
#define BLUE 4

//SENSOR
#define OFFSET 7

#define THERMISTORPIN A0         
#define THERMISTORNOMINAL 10000      
#define TEMPERATURENOMINAL 25   
#define NUMSAMPLES 5
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000
 
int samples[NUMSAMPLES];
 
void setup(void) {
  Serial.begin(9600);
  adc->setReference(ADC_REFERENCE::REF_EXT, ADC_0);
  adc->setResolution(16);

  pinMode(RED, OUTPUT); //RED
  pinMode(GREEN, OUTPUT); //GREEN
  pinMode(BLUE, OUTPUT); //BLUE
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
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

  Serial.print("Avarage ADC "); 
  Serial.println(average);
 
  average = adc->getMaxValue(ADC_0) / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C

  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");

  //RED
  if (steinhart < UPPERRED && steinhart > LOWERRED) {
    digitalWrite(RED, HIGH);
  }
  //GREEN
  if (steinhart < UPPERGREEN && steinhart > LOWERGREEN) {
    digitalWrite(GREEN, HIGH);
  }
  //BLUE
  if (steinhart < UPPERBLUE && steinhart > LOWERBLUE) {
    digitalWrite(BLUE, HIGH);
  }

  adc->printError();
  
  delay(1000);
}

// Adapted from Thermistor Example #3 from the Adafruit Learning System guide on Thermistors 
// https://learn.adafruit.com/thermistor/overview by Limor Fried, Adafruit Industries
// MIT License - please keep attribution and consider buying parts from Adafruit