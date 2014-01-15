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
	{"Maandag  ", "Dinsdag  ", "Woensdag ", "Donderdag", "Vrijdag  ", "Zaterdag ", "Zondag   "};
const String receiverName[] = 
	{"< Ontvanger 1  >", "< Ontvanger 2  >", "< Ontvanger 3  >"};
const String menuStrings[] = 
	{"<    Alarm     >", "<     Klok     >", "<  Helderheid  >"};

int buttonState[] = {HIGH, HIGH, HIGH, HIGH};
int lastButtonState[] = {LOW,LOW,LOW,LOW};
int bs[] = {LOW,LOW,LOW,LOW};
int lastDebounceTime[] = {0, 0, 0, 0};
int debounceDelay = 1000;

RCSwitch transmitter = RCSwitch();

time_t alarm, currentTime;

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
int selectedMenuItem = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void serialPC() {
	if (Serial.available() > 1) {
		if (Serial.read() == 0x00) {
			switch (Serial.read()) {
				case 0x01:
					//Switch 1
					selectedReceiver = 0;
					switchReceiver();
					break;
				case 0x02:
					//Switch 2
					selectedReceiver = 1;
					switchReceiver();
					break;
				case 0x03:
					//Switch 3
					selectedReceiver = 2;
					switchReceiver();
					break;
				case 0x04:
					//Return switch state
					Serial.print(receiverState[0]);
					Serial.print(";");
					Serial.print(receiverState[1]);
					Serial.print(";");
					Serial.print(receiverState[2]);
					Serial.println(";");
					break;
				case 0x05:
					//Return alarm time
					Serial.print(hour(alarm));
					Serial.print(";");
					Serial.print(minute(alarm));
					Serial.print(";");
					Serial.print(second(alarm));
					Serial.println(";");
					break;
				case 0x06:
					//Set alarm
					break;
				case 0x07:
					//Set Time
					break;
			}
		}
	}
}

void checkBacklight() {
	if (currentMillis - lightMillis > lightTimeout) {
		analogWrite(pinBacklight, lightMin);
	} else {
		lightVal = map(analogRead(pinLightSensor), 1022, 0, lightMin, lightMax);
		lightVal = 255;
		analogWrite(pinBacklight, lightVal);
	}
}

void checkButtons() {
	buttonState[0] = digitalRead(pinButtonBack);
	buttonState[1] = digitalRead(pinButtonEnter);
	buttonState[2] = digitalRead(pinButtonPrevious);
	buttonState[3] = digitalRead(pinButtonNext);
	if (buttonState[0] == LOW || buttonState[1] == LOW || buttonState[2] == LOW || buttonState[3] == LOW) {
		lightMillis = currentMillis;
		Serial.println("button");
		delay(1000);
	}
}

//LARSZJWIK
void checkButton() {
  
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
	lcd.print(receiverName[selectedReceiver]);
	lcd.setCursor(0, 1);
	if (receiverState[selectedReceiver]) {
		lcd.print("  Ingeschakeld  ");
	} else {
		lcd.print("  Uitgeschakeld ");
	}
}

void printMenu() {
	lcd.setCursor(0, 0);
	lcd.print(menuStrings[selectedMenuItem]);
	lcd.setCursor(0, 1);
	lcd.print("     ");
	switch (selectedMenuItem) {
		case 0:
			if (hour(alarm) < 10) lcd.print(0);
			lcd.print(hour(alarm));
			lcd.print(":");
			if (minute(alarm) < 10) lcd.print(0);
			lcd.print(minute(alarm));
			break;
		case 1:
			if (hour(currentTime) < 10) lcd.print(0);
			lcd.print(hour(currentTime));
			lcd.print(":");
			if (minute(currentTime) < 10) lcd.print(0);
			lcd.print(minute(currentTime));
			break;
		case 2:
			lcd.print("     ");
			break;
	}
	lcd.print("      ");
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

	Serial.begin(9600);

	analogWrite(pinBacklight, 255);
	currentMillis = millis();
	lightMillis = currentMillis;

	lcd.clear();
}

void loop() {
	currentMillis = millis();
	currentTime = now();
	checkButton();
	checkBacklight();
	switch (mode) {
		case 0:
			printTime(hour(currentTime), minute(currentTime), second(currentTime), day(currentTime), month(currentTime), year(currentTime), weekday(currentTime));
			if (buttonState[2] == LOW || buttonState[3] == LOW) mode = 1;
			if (buttonState[1] == LOW) mode = 2;
			break;
		case 1:
			if (buttonState[0] == LOW) {
				mode = 0;
				break;
			}
			if (buttonState[1] == LOW) {
				switchReceiver();
			}
			if (buttonState[2] == LOW) {
				selectedReceiver--;
				if (selectedReceiver < 0) selectedReceiver = 2;
				Serial.println(selectedReceiver);
			}
			if (buttonState[3] == LOW) {
				selectedReceiver++;
				if (selectedReceiver > 2) selectedReceiver = 0;
				Serial.println(selectedReceiver);
			}
			printRemote();
			break;
		case 2:
			if (buttonState[0] == LOW) {
				mode = 0;
				break;
			}
			if (buttonState[1] == LOW) {

			}
			if (buttonState[2] == LOW) {
				selectedMenuItem--;
				if (selectedMenuItem < 0) selectedMenuItem = 2;
			}
			if (buttonState[3] == LOW) {
				selectedMenuItem++;
				if (selectedMenuItem > 2) selectedMenuItem = 0;
			}
			printMenu();
			break;
	}
}
