#include <RCSwitch.h>
#include <LiquidCrystal.h>
#include <Time.h>

const int pinRX = 0;
const int pinTX = 1;
const int pinBacklight = 6;
const int pinButtonBack = 7;
const int pinButtonEnter = 8;
const int pinButtonPrevious = 9;
const int pinButtonNext = 10;
const int pinBuzzer = 13;
const int pinLightSensor = A0;
const String dayOfWeek[] =
	{"Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrijdag", "Zaterdag", "Zondag"};

int buttonStateBack = LOW;
int buttonStateEnter = LOW;
int buttonStatePrevious = LOW;
int buttonStateNext = LOW;
int buttonValBack = LOW;
int buttonValBackP = LOW;
int buttonValEnter = LOW;
int buttonValPrevious = LOW;
int buttonValPreviousP = LOW;
int buttonValNext = LOW;
int buttonValNextP = LOW;

long currentMillis;
int editHour, editMinute, editSecond, editDay, editMonth, editYear, editDOW;
int lightVal = 255;
int lightMin = 0;
int lightMax = 255;
long lightTimeout = 5000;
long lightMillis;
int mode = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void checkBacklight() {
	if (currentMillis - lightMillis > lightTimeout) {
		analogWrite(pinBacklight, lightMin);
	} else {
		lightVal = map(analogRead(pinLightSensor), 1022, 0, lightMin, lightMax);
		analogWrite(pinBacklight, lightVal);
	}
}

void checkButtons() {
	buttonStateBack = digitalRead(pinButtonBack);
	buttonStateEnter = digitalRead(pinButtonEnter);
	buttonStatePrevious = digitalRead(pinButtonPrevious);
	buttonStateNext = digitalRead(pinButtonNext);
	if (buttonStateBack == LOW || buttonStateEnter == LOW || buttonStatePrevious == LOW || buttonStateNext == LOW) delay(100);
}

void printTime(int h, int m, int s, int d, int mo, int y, int dow) {
	lcd.setCursor(0, 0);
	if (h < 10) lcd.print(0);
	lcd.print(h);
	lcd.print(":");
	if (m < 10) lcd.print(0);
	lcd.print(m);
	lcd.print(":");
	if (s < 10) lcd.print(0);
	lcd.print(s);
	lcd.setCursor(0, 1);
	lcd.print(String(d));
	lcd.print(" - ");
	lcd.print(String(mo));
	lcd.print(" - ");
	lcd.print(String(y));
}

void setup() {
	lcd.begin(16, 2);
	lcd.write("Even geduld...");

	pinMode(6, OUTPUT);
	pinMode(7, INPUT);
	digitalWrite(7, HIGH);
	pinMode(8, INPUT);
	digitalWrite(8, HIGH);
	pinMode(9, INPUT);
	digitalWrite(9, HIGH);
	pinMode(10, INPUT);
	digitalWrite(10, HIGH);

	analogWrite(pinBacklight, 255);
	currentMillis = millis();
	lightMillis = currentMillis;

	lcd.clear();
}

void loop() {
	currentMillis = millis();
	checkBacklight();
	printTime(hour(), minute(), second(), day(), month(), year(), weekday());
}