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
const String receiverName[] = 
	{"Ontvanger 1", "Ontvanger 2", "Ontvanger 3"};

int buttonStateBack = HIGH;
int buttonStateEnter = HIGH;
int buttonStatePrevious = HIGH;
int buttonStateNext = HIGH;
int buttonValBack = HIGH;
int buttonValBackP = HIGH;
int buttonValEnter = HIGH;
int buttonValPrevious = HIGH;
int buttonValPreviousP = HIGH;
int buttonValNext = HIGH;
int buttonValNextP = HIGH;

RCSwitch transmitter = RCSwitch();

long currentMillis;
int editHour, editMinute, editSecond, editDay, editMonth, editYear, editDOW;
int lightVal = 255;
int lightMin = 0;
int lightMax = 255;
long lightTimeout = 5000;
long lightMillis;
int mode = 0;
int selectedReceiver = 0;
boolean receiverState[] = {false, false, false};

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
	if (buttonStateBack == LOW || buttonStateEnter == LOW || buttonStatePrevious == LOW || buttonStateNext == LOW) {
		delay(100);
		lightMillis = currentMillis;
	}
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
	lcd.print("          ");
	lcd.setCursor(0, 1);
	lcd.print(String(d));
	lcd.print(" - ");
	lcd.print(String(mo));
	lcd.print(" - ");
	lcd.print(String(y));
	lcd.print("    ");
}

void printRemote() {
	lcd.setCursor(0, 0);
	lcd.print(" ");
	lcd.print(receiverName[selectedReceiver]);
	lcd.print("        ");
	lcd.setCursor(0, 1);
	lcd.print(booleanToString(receiverState[selectedReceiver]));
	lcd.print("        ");
}

String booleanToString(boolean input) {
	if (!input) {
		return "Uitgeschakeld";
	} else {
		return "Ingeschakeld";
	}
}

void switchReceiver() {
	if (!receiverState[selectedReceiver]) {
		switch (selectedReceiver) {
			case 0:
				transmitter.switchOn("10010", "10000");
				break;
			case 1:
				transmitter.switchOn("10010", "01000");
				break;
			case 2:
				transmitter.switchOn("10010", "00100");
				break;
		}
		receiverState[selectedReceiver] = true;
	} else {
		switch (selectedReceiver) {
			case 0:
				transmitter.switchOff("10010", "10000");
				break;
			case 1:
				transmitter.switchOff("10010", "01000");
				break;
			case 2:
				transmitter.switchOff("10010", "00100");
				break;
		}
		receiverState[selectedReceiver] = false;
	}
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
	checkButtons();

	transmitter.enableTransmit(13);

	analogWrite(pinBacklight, 255);
	currentMillis = millis();
	lightMillis = currentMillis;

	lcd.clear();
}

void loop() {
	currentMillis = millis();
	time_t t = now();
	checkButtons();
	checkBacklight();
	switch (mode) {
		case 0:
			printTime(hour(t), minute(t), second(t), day(t), month(t), year(t), weekday(t));
			if (buttonValNext == LOW || buttonStatePrevious == LOW) mode = 1;
			break;
		case 1:
			if (buttonStateBack == LOW) {
				mode = 0;
				break;
			}
			if (buttonStateEnter == LOW) {
				switchReceiver();
			}
			if (buttonStatePrevious == LOW) {
				selectedReceiver--;
				if (selectedReceiver < 0) selectedReceiver = 2;
			}
			if (buttonStateNext == LOW) {
				selectedReceiver++;
				if (selectedReceiver > 2) selectedReceiver = 0;
			}
			printRemote();
			break;
	}
}