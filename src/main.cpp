#include <Arduino.h>
#include <math.h>

//LEDS
#define LOWERRED -30
#define UPPERRED 22

#define LOWERGREEN 23 
#define UPPERGREEN 30

#define LOWERBLUE 31
#define UPPERBLUE 50

#define BLUE 2
#define RED 3
#define GREEN 4

//SENSOR
#define OFFSET 0;
#define OFFSET2 0;

#define THERMISTORPIN A0         
#define THERMISTORNOMINAL 10000      
#define TEMPERATURENOMINAL 25   
#define NUMSAMPLES 50
#define SERIESRESISTOR 1000
 
int samples[NUMSAMPLES];
 
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

    Serial.print("Avarage ADC: "); 
    Serial.println(average);

    float R;
    R = (1024 / average)-1;
    R = SERIESRESISTOR / R;
    Serial.print("Thermistor resistance: "); 
    Serial.println(R);

    float voltage;
    voltage = average * (4.0960/1024);

    Serial.print("Voltage "); 
    Serial.println(voltage);
  
    float temp;
    //y=mx+b
    temp = 626.5 * voltage;
    temp = temp - 2482;
    if(temp > 50) {
      temp = temp - OFFSET2;
    } else {
      temp = temp + OFFSET;
    }
    

    // Serial.print("Temperature from voltage EQ: "); 
    // Serial.print(temp);
    // Serial.println(" *C");

    // //T=-104.9+0.0004889R-(5.13*10-10)R2+(1.85*10-16)R3
    // float tempVal;

    // temp = 0.0004889 * R;
    // tempVal = 5.13e-10 * pow(R, 2);
    // temp = temp - tempVal;
    // tempVal = 1.85e-16 * pow(R, 3);
    // temp = temp + tempVal;
    // temp = temp - 104.9;
    // if(temp > 50) {
    //   temp = temp - OFFSET2;
    // } else {
    //   temp = temp + OFFSET;
    // }

    // Serial.print("Temperature from resistance EQ: "); 
    // Serial.print(temp);
    // Serial.println(" *C");

    //RED
    if (temp < UPPERRED && temp > LOWERRED) {
      Serial.println("RED");
      digitalWrite(RED, HIGH);
      digitalWrite(GREEN, LOW);
      digitalWrite(BLUE, LOW);
    }
    // GREEN
    if (temp < UPPERGREEN && temp > LOWERGREEN) {
      Serial.println("GREEN");
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
      digitalWrite(BLUE, LOW);
    }
    // BLUE
    if (temp < UPPERBLUE && temp > LOWERBLUE) {
      Serial.println("BLUE");
      digitalWrite(BLUE, HIGH);
      digitalWrite(RED, LOW);
      digitalWrite(GREEN, LOW);
    }

  delay(1000);
}
