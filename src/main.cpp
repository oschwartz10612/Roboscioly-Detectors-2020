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

#define M 0.001581
#define B 3.963
 
int samples[NUMSAMPLES];
 
void setup(void) {
	Serial.begin(9600);
	adc->setReference(ADC_REFERENCE::REF_EXT, ADC_0);
	adc->setResolution(16);

	pinMode(RED, OUTPUT); //RED
	pinMode(GREEN, OUTPUT); //GREEN
	pinMode(BLUE, OUTPUT); //BLUE

	//LED's start in off condition
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

	float R;
	R = (adc->getMaxValue(ADC_0) / average)-1;
	R = SERIESRESISTOR / R;
	Serial.print("Thermistor resistance "); 
	Serial.println(R);

  float voltage;
  voltage = average * (4.0960/adc->getMaxValue(ADC_0));

  Serial.print("Voltage "); 
  Serial.println(voltage);
 
  float temp;
  //y=mx+b
  temp = (M*average);
  temp += B;

	Serial.print("Temperature "); 
	Serial.print(temp);
	Serial.println(" *C");

	// RED
	if (temp < UPPERRED && temp > LOWERRED) {
		digitalWrite(RED, HIGH);
	}
	// GREEN
	if (temp < UPPERGREEN && temp > LOWERGREEN) {
		digitalWrite(GREEN, HIGH);
	}
	// BLUE
	if (temp < UPPERBLUE && temp > LOWERBLUE) {
		digitalWrite(BLUE, HIGH);
	}

	adc->printError();
  
	delay(1000);
}
