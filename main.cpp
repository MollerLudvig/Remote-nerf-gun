#include <Arduino.h>
#include <IRremote.h>


const int RECV_PIN = 11;
const int MOTOR = 13;
IRrecv irrecv(RECV_PIN);

String result;
String pressedButton;
String inputBuffer = "";

unsigned long lastButtonPress = 0;
const unsigned long inputTimeout = 1000;


String translateIR()

{

  result = String(irrecv.decodedIRData.decodedRawData, HEX);
  String output;

  switch(irrecv.decodedIRData.decodedRawData)

  {
  case 0xBA45FF00: output = "POWER"; break;
  case 0xB847FF00: output = "FUNC/STOP"; break;
  case 0xB946FF00: output = "VOL+"; break;
  case 0xBB44FF00: output = "FAST BACK"; break;
  case 0xBF40FF00: output = "PAUSE"; break;
  case 0xBC43FF00: output = "FAST FORWARD"; break;
  case 0xF807FF00: output = "DOWN"; break;
  case 0xEA15FF00: output = "VOL-"; break;
  case 0xF609FF00: output = "UP"; break;
  case 0xE619FF00: output = "EQ"; break;
  case 0xF20DFF00: output = "ST/REPT"; break;
  case 0xE916FF00: output = "0"; break;
  case 0xF30CFF00: output = "1"; break;
  case 0xE718FF00: output = "2"; break;
  case 0xA15EFF00: output = "3"; break;
  case 0xF708FF00: output = "4"; break;
  case 0xE31CFF00: output = "5"; break;
  case 0xA55AFF00: output = "6"; break;
  case 0xBD42FF00: output = "7"; break;
  case 0xAD52FF00: output = "8"; break;
  case 0xB54AFF00: output = "9"; break;
  case 0x0: output = "REPEAT"; break;

  default: 
    Serial.println(" other button   ");
  }

  return output;
}

bool isValidNumber(String str) {
  if (str.length() == 1 && isDigit(str[0])) {
    return true;
  } else {
    return false;
  }
}

void processInputBuffer() {
  if (inputBuffer.length() > 0) {
    int timeToShoot = inputBuffer.toInt();
    digitalWrite(MOTOR, HIGH);
    delay(150 + timeToShoot * 480);
    digitalWrite(MOTOR, LOW);
    inputBuffer = "";
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(RECV_PIN, INPUT);
  irrecv.enableIRIn();
  pinMode(MOTOR, OUTPUT);
}

void loop(){
  if (irrecv.decode()){
        pressedButton = translateIR();
        Serial.println(pressedButton);

        if (isValidNumber(pressedButton)) {
          inputBuffer += pressedButton;
          lastButtonPress = millis();
        }

        irrecv.resume();
  }

    if (millis() - lastButtonPress > inputTimeout) {
      processInputBuffer();
  }
}

// #include <Arduino.h>

// void setup() {
//   pinMode(13, OUTPUT);
//   digitalWrite(13, LOW);
// }

// void loop() {
//   digitalWrite(13, HIGH);
//   delay(2000);
//   digitalWrite(13, LOW);
//   delay(2000);
// }