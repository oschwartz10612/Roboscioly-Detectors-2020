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

#define BLUE 2
#define RED 3
#define GREEN 4

//SENSOR
#define OFFSET 18;

#define THERMISTORPIN A0         
#define THERMISTORNOMINAL 10000      
#define TEMPERATURENOMINAL 25   
#define NUMSAMPLES 50
#define SERIESRESISTOR 10000
 
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
  if (Serial.available() > 0) {
    char t = Serial.read();
    Serial.flush();

    digitalWrite(RED, LOW);
	  digitalWrite(GREEN, LOW);
	  digitalWrite(BLUE, LOW);

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

    Serial.print("Avarage ADC: "); 
    Serial.println(average);

    float R;
    R = (adc->getMaxValue(ADC_0) / average)-1;
    R = SERIESRESISTOR / R;
    Serial.print("Thermistor resistance: "); 
    Serial.println(R);

    float voltage;
    voltage = average * (4.0960/adc->getMaxValue(ADC_0));

    Serial.print("Voltage "); 
    Serial.println(voltage);
  
    float temp;
    //y=mx+b
    temp = 626.5 * voltage;
    temp = temp - 2482;
    temp = temp + OFFSET;

    Serial.print("Temperature from voltage EQ: "); 
    Serial.print(temp);
    Serial.println(" *C");

    //T=-104.9+0.0004889R-(5.13*10-10)R2+(1.85*10-16)R3
    float tempVal;

    temp = 0.0004889 * R;
    tempVal = 5.13e-10 * pow(R, 2);
    temp = temp - tempVal;
    tempVal = 1.85e-16 * pow(R, 3);
    temp = temp + tempVal;
    temp = temp - 104.9;
    temp = temp + OFFSET;

    Serial.print("Temperature from resistance EQ: "); 
    Serial.print(temp);
    Serial.println(" *C");

    //RED
    if (temp < UPPERRED && temp > LOWERRED) {
      Serial.println("RED");
      digitalWrite(RED, HIGH);
    }
    // GREEN
    if (temp < UPPERGREEN && temp > LOWERGREEN) {
      Serial.println("GREEN");
      digitalWrite(GREEN, HIGH);
    }
    // BLUE
    if (temp < UPPERBLUE && temp > LOWERBLUE) {
      Serial.println("BLUE");
      digitalWrite(BLUE, HIGH);
    }

    adc->printError();
  }
  delay(100);
}
