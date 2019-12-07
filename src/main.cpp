#include <Arduino.h>
#include <math.h>
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
#define SERIESRESISTOR 10000

#define A 1.139
#define B 2.324
#define C 0.938
 
//int samples[NUMSAMPLES];
 
void setup(void) {
  Serial.begin(9600);
  adc->setResolution(16);

  pinMode(RED, OUTPUT); //RED
  pinMode(GREEN, OUTPUT); //GREEN
  pinMode(BLUE, OUTPUT); //BLUE
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}
 
void loop(void) {
  //uint8_t i;
  float sensorValue;
 
  //for (i=0; i< NUMSAMPLES; i++) {
   //samples[i] = adc->analogRead(THERMISTORPIN, ADC_0);
   //delay(10);
  //}
  sensorValue = adc->analogRead(THERMISTORPIN, ADC_0);
  //average = 0;
  //for (i=0; i< NUMSAMPLES; i++) {
    // average += samples[i];
  //}
  
  //average /= NUMSAMPLES;

  double voltage;

  voltage = sensorValue * (4.0960/adc->getMaxValue(ADC_0));
  //(4.096 * adc->analogRead(THERMISTORPIN, ADC_0)) / 65536;
  //Serial.println(adc->analogRead(THERMISTORPIN, ADC_0));
  //print(voltage);

  Serial.print("Voltage "); 
  Serial.println(voltage);
 
   float R;
   R = (adc->getMaxValue(ADC_0) / sensorValue)-1;
   R = SERIESRESISTOR / R;
   Serial.print("Resitance "); 
   Serial.println(R);

  // //RED
  // if (steinhart < UPPERRED && steinhart > LOWERRED) {
  //   digitalWrite(RED, HIGH);
  // }
  // //GREEN
  // if (steinhart < UPPERGREEN && steinhart > LOWERGREEN) {
  //   digitalWrite(GREEN, HIGH);
  // }
  // //BLUE
  // if (steinhart < UPPERBLUE && steinhart > LOWERBLUE) {
  //   digitalWrite(BLUE, HIGH);
  // }

  adc->printError();
  
  delay(1000);
}

// Adapted from Thermistor Example #3 from the Adafruit Learning System guide on Thermistors 
// https://learn.adafruit.com/thermistor/overview by Limor Fried, Adafruit Industries
// MIT License - please keep attribution and consider buying parts from Adafruit