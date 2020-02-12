#include <Arduino.h>
#include <math.h>
#include "ADC.h"

ADC *adc = new ADC(); // adc object

//LEDS
#define LOWERRED 0
#define UPPERRED 10

#define LOWERGREEN 10
#define UPPERGREEN 30

#define LOWERBLUE 30
#define UPPERBLUE 100

#define RED 3
#define GREEN 2
#define BLUE 4

//SENSOR
#define OFFSET 0;
#define OFFSET2 0;

#define THERMISTORPIN A0         
#define NUMSAMPLES 50
#define VCONSTANT 3.3/adc->getMaxValue(ADC_0)
 
int samples[NUMSAMPLES];
float prev_voltage = 1000;

void setup(void) {
	Serial.begin(9600);
  adc->setResolution(16);
 // adc->setReference(ADC_REFERENCE::REF_EXT, ADC_0);


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

    // Serial.print("Average ADC: "); 
    // Serial.println(average);

    float voltage;
    voltage = average * VCONSTANT;

    float delta_voltage;
    delta_voltage = abs(voltage - prev_voltage);

    if(delta_voltage < 0.02)
    {
      float temperature;
      temperature = 13.40 - 59.53*voltage + 83.16*pow(voltage, 2) - 34.98*pow(voltage, 3) + 5.518*pow(voltage, 4);

      Serial.println("");
      Serial.println("System settled");
      Serial.print("Voltage: ");
      Serial.println(voltage, 5);
      Serial.print("Temperature: "); 
      Serial.println(temperature);

      //RED
      if (temperature <= UPPERRED && temperature >= LOWERRED) {
        Serial.println("RED");
        digitalWrite(RED, HIGH);
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, LOW);
      }
      // GREEN
      if (temperature <= UPPERGREEN && temperature >= LOWERGREEN) {
        Serial.println("GREEN");
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, LOW);
      }
      // BLUE
      if (temperature < UPPERBLUE && temperature >= LOWERBLUE) {
        Serial.println("BLUE");
        digitalWrite(BLUE, HIGH);
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, LOW);
      }
    }
    else
    {
      Serial.print(".");
    }
  
    prev_voltage = voltage;    

  delay(500);
}
