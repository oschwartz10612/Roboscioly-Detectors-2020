#include <Arduino.h>
#include <math.h>

//LEDS
#define LOWERRED 55
#define UPPERRED 500000

#define LOWERGREEN 20 
#define UPPERGREEN 50

#define LOWERBLUE 0
#define UPPERBLUE 20

#define RED 2
#define GREEN 4
#define BLUE 6

//SENSOR
#define OFFSET 0;
#define OFFSET2 0;

#define THERMISTORPIN A0         
#define NUMSAMPLES 50
#define VCONSTANT 5/1023
 
int samples[NUMSAMPLES];
float prev_average = 1000;

 
void setup(void) {
	Serial.begin(9600);

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
      samples[i] = analogRead(THERMISTORPIN);
      delay(10);
    }

    average = 0;
    for (i=0; i< NUMSAMPLES; i++) {
      average += samples[i];
    }
  
    average /= NUMSAMPLES;

    //Serial.print("Average ADC: "); 
    //Serial.println(average);

    float voltage;
    voltage = average * VCONSTANT;

    float delta_voltage;
    delta_voltage = abs(average - prev_average);

    //Temperature function T(v) = (4.506)v^2-(2.392)v-(0.007079)
    
    //Serial.print("Temperature: ");
    //Serial.println(temperature);

    if(delta_voltage < 0.03)
    {
      float temperature;
      temperature = ((4.506) * pow(voltage, 2)) + ((-2.392) * voltage) + 1.992921;

      Serial.println("");
      Serial.println("System settled");
      Serial.print("Voltage: ");
      Serial.println(voltage, 5);
      Serial.print("Temperature: "); 
      Serial.println(temperature);

      //RED
      if (temperature < UPPERRED && temperature > LOWERRED) {
        Serial.println("RED");
        digitalWrite(RED, HIGH);
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, LOW);
      }
      // GREEN
      if (temperature < UPPERGREEN && temperature > LOWERGREEN) {
        Serial.println("GREEN");
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, LOW);
      }
      // BLUE
      if (temperature < UPPERBLUE && temperature > LOWERBLUE) {
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
  
    prev_average = average;

    

  delay(500);
}
