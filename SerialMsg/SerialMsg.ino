


void setup() {
  Serial.begin(9600);

}

void loop() {
  readSerial();
}

void processSerialData(String msg) {
  if(msg == "refresh") {
    Serial.println("0;0;0");  
  }
  else if(msg == "toggle1") {
    Serial.println("0;0;1");  
  }
  else if(msg == "toggle2") {
    Serial.println("0;1;0");  
  }
  else if(msg == "toggle3") {
    Serial.println("0;1;1");  
  }
}

void readSerial() {
  String startChar = "#";
  String endChar = "%";
  String receivedChar;
  String msg;
  if(Serial.available() >0) {

    while(receivedChar != endChar && Serial.available()) {
      char incomingByte = Serial.read();
      receivedChar = String(incomingByte);
      msg = msg + incomingByte;

    }
    int l =  msg.length();
    if(msg.substring(0,1) == "#" && msg.substring(l-1,l) == "%") {

      msg = msg.substring(1,l-1);
      processSerialData(msg);
    }
  }
  delay(20);
}






