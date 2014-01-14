const int pinBacklight = 6;
const int pinButtonBack = 7;
const int pinButtonEnter = 8;
const int pinButtonPrevious = 9;

int buttonState[] = {0,0,0,0};
int lastButtonState[] = {0,0,0,0};
long lastDebounceTime[] = {0,0,0,0};
long debounceDelay = 50; 

void setup() {
  pinMode(pinBacklight, INPUT);
  pinMode(pinButtonBack, INPUT);
  pinMode(pinButtonEnter, INPUT);
  pinMode(pinButtonPrevious, INPUT);
  
  /*
  digitalWrite(pinBacklight, HIGH);
  digitalWrite(pinButtonBack, HIGH);
  digitalWrite(pinButtonEnter, HIGH);
  digitalWrite(pinButtonPrevious, HIGH);
  */
  
  Serial.begin(9600);
}

void doStuff(int button) {
  Serial.print("B");
  Serial.print(button);
  Serial.println(" on");
  
  // DO STUFF DIE JE NU IN JE LOOP DOET - DIE CASE ZJWIK
}
  
void checkButtons() {
  int reading[] = {0,0,0,0};

  reading[0] = digitalRead(pinBacklight);
  reading[1] = digitalRead(pinButtonBack);
  reading[2] = digitalRead(pinButtonEnter);
  reading[3] = digitalRead(pinButtonPrevious);

  for(int i = 0; i<4;i++) {
    if (reading[i] != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    } 

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading[i] != buttonState[i]) {
        buttonState[i] = reading[i];
        if (buttonState[i] == HIGH) { //VERANDEREN NAAR LOW VOOR DIE SKEERE BUTTONS VAN HABETS
          doStuff(i);
        }
      }
    }
    lastButtonState[i] = reading[i];
  }
}

void loop() {
  checkButtons();
}












