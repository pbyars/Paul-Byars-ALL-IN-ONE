#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

const int buttonPin = 2;
int buttonState = LOW;
int previousBS = LOW;     //BS -> buttonState
int stp = 10;
int dir = 9;
int EN = 8;

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, HIGH);

  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}

void loop() {
  //buttonPress();
  readTags();
  delay(2000);
}

void buttonPress(){
  buttonState = digitalRead(buttonPin);

  if(buttonState != previousBS){
    if(buttonState == LOW){
      Serial.println("Button pressed");
      digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
      digitalWrite(EN, LOW);
      for(int i = 0; i < 1000; i++) {  //Loop the forward stepping enough times for motion to be visible
        digitalWrite(stp, HIGH); //Trigger one step forward
        delay(1);
        digitalWrite(stp, LOW); //Pull step pin low so it can be triggered again
        delay(1);
      }
      digitalWrite(EN, HIGH);
    }
    else{
      Serial.println("Button is not pressed");
    }

    previousBS = buttonState;
  }
}

void readTags(){
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()){
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag: ");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++){
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println(content);

  //writeTags1(content);
  //writeTags2(content);
  printTags();
}

void writeTags1(String content){
  int eeAddress = 0;
  String newContent = "";

  Serial.println("Writing to the EEPROM");
  EEPROM.put(eeAddress, content);
  Serial.println(content);
  Serial.println("Reading from the EEPROM");
  EEPROM.get(eeAddress, newContent);
  Serial.println(newContent);

}

void writeTags2(String content){
  int eeAddress = 12;
  String newContent = "";

  Serial.println("Writing to the EEPROM");
  EEPROM.put(eeAddress, content);
  Serial.println(content);
  Serial.println("Reading from the EEPROM");
  EEPROM.get(eeAddress, newContent);
  Serial.println(newContent);

}

void printTags(){
  String content1, content2 = "";
  EEPROM.get(0, content1);
  Serial.println(content1);
  EEPROM.get(12, content2);
  Serial.println(content2);
}
