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
	{ "Ma", "Di", "Wo", "Do", "Vr", "Za", "Zo" };
const String receiverName[] = 
	{ "< Ontvanger 1  >", "< Ontvanger 2  >", "< Ontvanger 3  >" };
const String menuStrings[] = 
	{ "<    Alarm     >", "<    Klok      >", "<    Scherm    >" };
const String alarmMenuStrings[] =
	{ "     Alarm      ", "   Alarm uur    ", "  Alarm minuut " };
const String timeMenuStrings[] =
	{ "      Jaar      ", "     Maand      ", "      Dag       ", "      Uur       ", "     Minuut     ", "    Seconde     " };

boolean buttonState[] = { false, false, false, false };
boolean lastButtonState[] = { false, false, false, false };
int debounceDelay = 50;

RCSwitch transmitter = RCSwitch();

time_t currentTime, editTime;
int alarmHour = 0;
int alarmMinute = 0;
boolean alarmEnabled = false;
boolean alarmTrigger = false;

int editVal[] = { 0, 0, 0, 0, 0, 0 };

int lightVal = 255;
int lightMin = 0;
int lightMax = 255;

int mode = 0;
int modeAlarm = 0;
int modeTime = 0;

int selectedMenuItem = 0;
int selectedReceiver = 0;

boolean receiverState[] = {false, false, false};

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
					Serial.print(alarmHour);
					Serial.print(";");
					Serial.print(alarmMinute);
					Serial.print(";");
					Serial.print("0");
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
	if (mode == 6) {
		lightVal = 255;
	} else {
		lightVal = map(analogRead(pinLightSensor), 0, 1022, lightMin, lightMax);
		Serial.print(analogRead(pinLightSensor));
		Serial.print(" - ");
		Serial.println(lightVal);
		analogWrite(pinBacklight, lightVal);
	}
}

int convertPin(int i) {
	switch (i) {
		case 0: return pinButtonBack;
		case 1: return pinButtonEnter;
		case 2: return pinButtonPrevious;
		case 3: return pinButtonNext;
	}
}

void checkButton() {
	if (digitalRead(pinButtonBack) == LOW || digitalRead(pinButtonEnter) == LOW || digitalRead(pinButtonPrevious) == LOW || digitalRead(pinButtonNext) == LOW) delay(debounceDelay);

	for (int i = 0; i <= 3; i++) {
		buttonState[i] = false;
		if (digitalRead(convertPin(i)) == LOW) buttonState[i] = true;
		if (buttonState[i]) {
			if (lastButtonState[i]) {
				buttonState[i] = false;
			} else {
				lastButtonState[i] = true;
			}
		} else {
			lastButtonState[i] = false;
		}
	}
}

void printHome() {
	lcd.setCursor(0, 0);
	//lcd.print("13:37:33   Alarm");
	//lcd.print("Do 15      00:00");
	if (hour(currentTime) < 10) lcd.print(0);
	lcd.print(hour(currentTime));
	lcd.print(":");
	if (minute(currentTime) < 10) lcd.print(0);
	lcd.print(minute(currentTime));
	lcd.print(":");
	if (second(currentTime) < 10) lcd.print(0);
	lcd.print(second(currentTime));
	lcd.print("   ");
	if (alarmEnabled) {
		lcd.print("Alarm");
	} else {
		lcd.print("     ");
	}
	lcd.setCursor(0, 1);
	lcd.print(dayOfWeek[weekday(currentTime)]);
	lcd.print(" ");
	lcd.print(day(currentTime));
	if (day(currentTime) < 10) lcd.print(" ");
	lcd.print("      ");
	if (alarmEnabled) {
		if (alarmHour < 10) lcd.print(0);
		lcd.print(alarmHour);
		lcd.print(":");
		if ((alarmMinute) < 10) lcd.print(0);
		lcd.print(alarmMinute);
	} else {
		lcd.print("     ");
	}
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
			if (alarmEnabled) {
				if (alarmHour < 10) lcd.print(0);
				lcd.print(alarmHour);
				lcd.print(":");
				if ((alarmMinute) < 10) lcd.print(0);
				lcd.print(alarmMinute);
			} else {
				lcd.print(" Uit    ");
			}
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

void printMenuAlarm() {
	lcd.setCursor(0, 0);
	lcd.print(alarmMenuStrings[modeAlarm]);
	lcd.setCursor(0, 1);
	if (modeAlarm == 0) {
		lcd.print("    < ");
		if (alarmEnabled) {
			lcd.print("Aan");
		} else {
			lcd.print("Uit");
		}
		lcd.print(" >     ");
	} else {
		lcd.print("  <  ");
		if (alarmHour < 10) lcd.print(0);
		lcd.print(alarmHour);
		lcd.print(":");
		if ((alarmMinute) < 10) lcd.print(0);
		lcd.print(alarmMinute);
		lcd.print("  >    ");
	}
}

void printMenuTime() {
	lcd.setCursor(0, 0);
	lcd.print(timeMenuStrings[modeTime]);
	//lcd.print("<2014 - 12 - 24>");
	//lcd.print("<   13:37:33   >");
	lcd.setCursor(0, 1);
	if (modeTime < 3) {
		lcd.print("<");
		lcd.print(year(currentTime));
		lcd.print(" - ");
		if (month(currentTime) < 10) lcd.print(" ");
		lcd.print(month(currentTime));
		lcd.print(" - ");
		if (day(currentTime) < 10) lcd.print(" ");
		lcd.print(day(currentTime));
		lcd.print(">");
	} else {
		lcd.print("<   ");
		if (hour(currentTime) < 10) lcd.print(0);
		lcd.print(hour(currentTime));
		lcd.print(":");
		if (minute(currentTime) < 10) lcd.print(0);
		lcd.print(minute(currentTime));
		lcd.print(":");
		if (second(currentTime) < 10) lcd.print(0);
		lcd.print(second(currentTime));
		lcd.print("   >");
	}
}

void printAlarm() {
	lcd.setCursor(0, 0);
	lcd.print("Het alarm  ");
	if (alarmHour < 10) lcd.print("0");
	lcd.print(alarmHour);
	lcd.print(":");
	if (alarmMinute < 10) lcd.print("0");
	lcd.print(alarmMinute);
	lcd.setCursor(0 ,1);
	lcd.print(" gaat af!       ");
}

void switchReceiver() {
	lcd.setCursor(0, 1);
	lcd.write(" Even geduld... ");
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

boolean month31(int m) {
	switch (m) {
		case 1: return true;
		case 3: return true;
		case 5: return true;
		case 7: return true;
		case 8: return true;
		case 10: return true;
		case 12: return true;
		default: return false;
	}
}

void checkEditValues() {
	// (yr % 4 == 0 && yr % 100 != 0 || yr % 400 == 0)
	//
	/*
	0 year
	1 month
	2 day

	3 hour
	4 minute
	5 second
	 */
	if (editVal[0] < 1970) editVal[0] = 2014;
	
	if (editVal[1] < 1) editVal[1] = 12;
	if (editVal[1] > 12) editVal[1] = 1;

	boolean isLeapYear = ((editVal[0] % 4 == 0) && (editVal[0] % 100 != 0) || (editVal[0] % 400 == 0));
	if (isLeapYear && editVal[1] == 2 && editVal[2] < 1) editVal[2] = 29;
	if (isLeapYear && editVal[1] == 2 && editVal[2] > 29) editVal[2] = 1;

	if (!isLeapYear && editVal[1] == 2 && editVal[2] < 1) editVal[2] = 28;
	if (!isLeapYear && editVal[1] == 2 && editVal[2] > 28) editVal[2] = 1;

	boolean mo31 = month31(editVal[1]);
	if (!isLeapYear && mo31 && editVal[2] < 1) editVal[2] = 31;
	if (!isLeapYear && mo31 && editVal[2] > 31) editVal[2] = 1;

	if (!isLeapYear && !mo31 && editVal[2] < 1) editVal[2] = 30;
	if (!isLeapYear && !mo31 && editVal[2] > 30) editVal[2] = 1;

	if (editVal[3] < 0) editVal[3] = 23;
	if (editVal[3] > 23) editVal[3] = 0;
	
	if (editVal[4] < 0) editVal[4] = 59;
	if (editVal[4] > 59) editVal[4] = 0;

	if (editVal[5] < 0) editVal[5] = 59;
	if (editVal[5] > 59) editVal[5] = 0;

	if (alarmHour < 0) alarmHour = 23;
	if (alarmHour > 23) alarmHour = 0;
	if (alarmMinute < 0) alarmMinute = 59;
	if (alarmMinute > 59) alarmMinute = 0;
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

	transmitter.enableTransmit(13);

	Serial.begin(9600);

	lcd.clear();
}

void loop() {
	currentTime = now();
	if (alarmEnabled && !alarmTrigger && alarmHour == hour(currentTime) && alarmMinute == minute(currentTime)) {
		mode = 6;
		transmitter.switchOn("10010", "10000");
		transmitter.switchOn("10010", "01000");
		transmitter.switchOn("10010", "00100");
		for (int i = 0; i <= 2; i++) receiverState[i] = true;
		alarmTrigger = true;
	}
	checkButton();
	checkBacklight();
	switch (mode) {
		case 0:
		//START SCREEN
			printHome();
			if (buttonState[2] || buttonState[3]) mode = 1;
			if (buttonState[1]) mode = 2;
			break;
		case 1:
		//REMOTE SCREEN
			if (buttonState[0]) {
				mode = 0;
				break;
			}
			if (buttonState[1]) {
				switchReceiver();
			}
			if (buttonState[2]) {
				selectedReceiver--;
				if (selectedReceiver < 0) selectedReceiver = 2;
			}
			if (buttonState[3]) {
				selectedReceiver++;
				if (selectedReceiver > 2) selectedReceiver = 0;
			}
			printRemote();
			break;
		case 2:
		//MAIN MENU
			if (buttonState[0]) {
				mode = 0;
				break;
			}
			if (buttonState[1]) {
				switch (selectedMenuItem) {
					case 0: mode = 3; break;
					case 1: mode = 4; break;
					case 2: mode = 5; break;
				}
			}
			if (buttonState[2]) {
				selectedMenuItem--;
				if (selectedMenuItem < 0) selectedMenuItem = 1;
			}
			if (buttonState[3]) {
				selectedMenuItem++;
				if (selectedMenuItem > 1) selectedMenuItem = 0;
			}
			printMenu();
			break;
		case 3:
		//ALARM MENU
			if (buttonState[0]) {
				mode = 2;
				modeAlarm = 0;
			}
			if (buttonState[1]) {
				modeAlarm++;
				if (modeAlarm > 2) modeAlarm = 0;
			}
			if (buttonState[2]) {
				switch (modeAlarm) {
					case 0:
						if (alarmEnabled) {
							alarmEnabled = false;
						} else {
							alarmEnabled = true;
						}
						break;
					case 1:
						alarmHour--;
						break;
					case 2:
						alarmMinute--;
						break;
				}
			}
			if (buttonState[3]) {
				switch (modeAlarm) {
					case 0:
						if (alarmEnabled) {
							alarmEnabled = false;
						} else {
							alarmEnabled = true;
						}
						break;
					case 1:
						alarmHour++;
						break;
					case 2:
						alarmMinute++;
						break;
				}
			}
			checkEditValues();
			printMenuAlarm();
			break;
		case 4:
		//TIME MENU
			editVal[0] = year(currentTime);
			editVal[1] = month(currentTime);
			editVal[2] = day(currentTime);
			editVal[3] = hour(currentTime);
			editVal[4] = minute(currentTime);
			editVal[5] = second(currentTime);
			if (buttonState[0]) {
				mode = 2;
				modeTime = 0;
			}
			if (buttonState[1]) {
				modeTime++;
				if (modeTime > 5) modeTime = 0;
			}
			if (buttonState[2]) {
				editVal[modeTime]--;
				checkEditValues();
			}
			if (buttonState[3]) {
				editVal[modeTime]++;
				checkEditValues();
			}
			setTime(editVal[3], editVal[4], editVal[5], editVal[2], editVal[1], editVal[0]);
			printMenuTime();
			break;
		case 5:
		//SCREEN MENU
			if (buttonState[0]) {

			}
			if (buttonState[1]) {

			}
			if (buttonState[2]) {

			}
			if (buttonState[3]) {
				
			}
			break;
		case 6:
		//ALARM
			if (buttonState[0]) {
				mode = 0;
				alarmEnabled = 0;
				alarmTrigger = false;
			}
			if (buttonState[1]) {

			}
			if (buttonState[2]) {

			}
			if (buttonState[3]) {

			}
			printAlarm();
			break;
	}
}