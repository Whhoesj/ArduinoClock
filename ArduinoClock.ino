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

boolean buttonStateBack = false;
boolean buttonStateEnter = false;
boolean buttonStatePrevious = false;
boolean buttonStateNext = false;

int editHour, editMinute, editSecond, editDay, editMonth, editYear, editDOW;
int lightVal = 255;
int mode = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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
}

void loop() {

}
