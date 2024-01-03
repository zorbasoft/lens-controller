/*
    Name:       LensController.ino
    Created:	20.12.2023 22:04:51
    Author:     PAWELK-MOB\PAWEL
*/
#include "StepperController.h"
#define LED_PIN 13

StepperController stepperZoom(4, 5, 6, 7, 8, 9);
StepperController stepperFocus(10, 11, 12, 12, 14, 15);

void setup()
{
	Serial.begin(115200);
	while (!Serial);
	pinMode(LED_PIN, OUTPUT);
	stepperZoom.initialize();
	digitalWrite(LED_PIN, 1);
	delay(1000);
	digitalWrite(LED_PIN, 0);
}

void loop()
{
	//delayMicroseconds(500);
	stepperZoom.loop();
	//stepperFocus.loop();
	//digitalWrite(LED_PIN, !digitalRead(LED_PIN));
	
}
