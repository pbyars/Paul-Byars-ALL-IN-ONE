#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define SS_PIN 10
#define RST_PIN 9
#define RELAY_PIN A5 // the Arduino pin connects to relay

MFRC522 rfid(SS_PIN, RST_PIN);    // Create MFRC522 instance. 

const int buttonPin = 2;
int buttonState = LOW;
int previousBS = LOW;     //BS -> buttonState
int stp = 10;
int dir = 9;
int EN = 8;

//--------------------------ALL ALLOWED CARDS------------------------
byte card[4] = {0x25, 0xEB, 0xD3, 0x2B};
byte keychain[4] = {0x69, 0x40, 0xF6, 0x73};
byte paulManeCard[7] = {0x04, 0x82, 0x39, 0xF2, 0x45, 0x5C, 0x80};
byte kristieManeCard[7] = {0x04, 0x2C, 0x37, 0xF2, 0x45, 0x5C, 0x80};
//-------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, HIGH);

  SPI.begin();      // initialize  SPI bus
  rfid.PCD_Init();   // initialize rfid reader
  pinMode(RELAY_PIN, OUTPUT); // initialize pin as an output
  digitalWrite(RELAY_PIN, HIGH); // lock the door
  
  Serial.println("Approximate your card to the reader...");
}

void loop() {
  checkTags();
}

void buttonPress(){
  buttonState = digitalRead(buttonPin);

  if (buttonState != previousBS) {
    if (buttonState == LOW) {
      Serial.println("Button pressed");
      digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
      digitalWrite(EN, LOW);
      for (int i = 0; i < 1000; i++) {  //Loop the forward stepping enough times for motion to be visible
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

void unlockDoor() {
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  digitalWrite(EN, LOW);
  
  for (int i = 0; i < 1000; i++) {  //Loop the forward stepping enough times for motion to be visible
    digitalWrite(stp, HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp, LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  
  digitalWrite(EN, HIGH);
  Serial.println(" Door is unlocked!");
}

void checkTags() {
  if (rfid.PICC_IsNewCardPresent()){ // new tag is available
    if (rfid.PICC_ReadCardSerial()){ // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      //----------------------RFID CHECKS----------------------------------
      if (rfid.uid.uidByte[0] == card[0] && 
          rfid.uid.uidByte[1] == card[1] && 
          rfid.uid.uidByte[2] == card[2] && 
          rfid.uid.uidByte[3] == card[3]) {
        Serial.print("Access is GRANTED for user with white card.");
        unlockDoor();
        digitalWrite(RELAY_PIN, LOW);  // unlock the door for 2 seconds
        delay(2000);
        //digitalWrite(RELAY_PIN, HIGH); // lock the door
      }
      
      else if (rfid.uid.uidByte[0] == keychain[0] && 
               rfid.uid.uidByte[1] == keychain[1] && 
               rfid.uid.uidByte[2] == keychain[2] && 
               rfid.uid.uidByte[3] == keychain[3]) {
        Serial.print("Access is GRANTED for user with blue keychain.");
        unlockDoor();
        digitalWrite(RELAY_PIN, LOW);  // unlock the door for 2 seconds
        delay(2000);
        //digitalWrite(RELAY_PIN, HIGH); // lock the door
      }

      else if (rfid.uid.uidByte[0] == paulManeCard[0] && 
               rfid.uid.uidByte[1] == paulManeCard[1] && 
               rfid.uid.uidByte[2] == paulManeCard[2] && 
               rfid.uid.uidByte[3] == paulManeCard[3] &&
               rfid.uid.uidByte[4] == paulManeCard[4] &&
               rfid.uid.uidByte[5] == paulManeCard[5] &&
               rfid.uid.uidByte[6] == paulManeCard[6]) {
        Serial.print("Access is GRANTED for user with Paul's Mane Card.");
        unlockDoor();
        digitalWrite(RELAY_PIN, LOW);  // unlock the door for 2 seconds
        delay(2000);
        //digitalWrite(RELAY_PIN, HIGH); // lock the door
      }

      else if (rfid.uid.uidByte[0] == kristieManeCard[0] && 
               rfid.uid.uidByte[1] == kristieManeCard[1] && 
               rfid.uid.uidByte[2] == kristieManeCard[2] && 
               rfid.uid.uidByte[3] == kristieManeCard[3] &&
               rfid.uid.uidByte[4] == kristieManeCard[4] &&
               rfid.uid.uidByte[5] == kristieManeCard[5] &&
               rfid.uid.uidByte[6] == kristieManeCard[6]) {
        Serial.print("Access is GRANTED for user with Kristie's Mane Card.");
        unlockDoor();
        digitalWrite(RELAY_PIN, LOW);  // unlock the door for 2 seconds
        delay(2000);
        //digitalWrite(RELAY_PIN, HIGH); // lock the door
      }
      //---------------------------------------------------------------------
      
      else {
        Serial.print("Access is DENIED for user with UID:");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
      }

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
}