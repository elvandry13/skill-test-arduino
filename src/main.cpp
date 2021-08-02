/**
 * @file main.cpp
 * @author Elvandry Ghiffary (elvandry13@gmail.com)
 * @brief Skill Test for Arduino Firmware
 * @version 0.1
 * @date 2021-07-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <Arduino.h>
#include <SoftwareSerial.h>

// Button pins
#define BUTTON_UP 8
#define BUTTON_DOWN 9

// Motor driver pins
#define RPWM 5
#define LPWM 6
#define L_EN 7
#define R_EN 4

byte buff[1];
int pwm = 0;
bool datasent = false;
bool laststate1 = false;
bool laststate2 = false;

SoftwareSerial Serial2(2, 3); // RX, TX

void setup()
{
	// Serial0 for flash firmware and serial debugging
	Serial.begin(115200);

	// Serial2 for serial communication
	Serial2.begin(115200);

	// Setup button pins
	pinMode(BUTTON_UP, INPUT);
	pinMode(BUTTON_DOWN, INPUT);

	// Setup motor driver pins
	for (int i = 4; i < 8; i++)
	{
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
	}
}

void loop()
{
	// Read button pin, store to buffer
	if (!digitalRead(BUTTON_UP) && laststate1 == false)
	{
		pwm++;
		Serial.println(pwm);
		laststate1 = true;
		datasent = false;
	}
	else if (digitalRead(BUTTON_UP))
	{
		laststate1 = false;
	}

	if (!digitalRead(BUTTON_DOWN) && laststate2 == false)
	{
		pwm--;
		Serial.println(pwm);
		laststate2 = true;
		datasent = false;
	}
	else if (digitalRead(BUTTON_DOWN))
	{
		laststate2 = false;
	}

	// Send buffer to ESP32, kelipatan 10
	if ((pwm != 0) && ((pwm % 10) == 0) && (datasent == false))
	{
		buff[0] = pwm;
		Serial2.write(buff, 1);
		datasent = true;
	}

	// Limit 0-255
	if (pwm >= 255 ) pwm = 255;
	if (pwm < 0 ) pwm = 0;
	
	// Send pwm to motor DC
	digitalWrite(R_EN, HIGH);
	digitalWrite(L_EN, HIGH);
	analogWrite(RPWM, 0);
	analogWrite(LPWM, pwm);
}
