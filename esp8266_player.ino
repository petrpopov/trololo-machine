#include <OneButton.h>
#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>
#include <WiFiManager.h>

#define RX 13
#define TX 12
#define BADUM_PIN 14
#define FAIL_PIN 16
#define SOUND_LEVEL_PIN A0

#define BADUM_NUMBER 1
#define FAIL_NUMBER 2

int oldVolume = 0;
int newVolume = 0;
unsigned long oldTime = 0;
unsigned long newTime = 0;
unsigned long deltaTime = 800;

//SoftwareSerial for separate controlling mp3 player and bootloader
SoftwareSerial mp3(RX, TX);

//declare buttons
OneButton badumButton(BADUM_PIN, true);
OneButton failButton(FAIL_PIN, true);

void setup() {
  //setup button pins
  pinMode(BADUM_PIN, INPUT);
  pinMode(FAIL_PIN, INPUT);
  pinMode(SOUND_LEVEL_PIN, INPUT);

  //setup Serial
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("\n Starting");

  //Setup mp3 player
  mp3.begin(9600);
  mp3_set_serial(mp3);
  delay(200);
  mp3_set_volume(20);
  delay(200);

  //setup buttons for controlling device
  badumButton.attachClick(playBadum);
  failButton.attachClick(playFail);
}

void loop() {
  
  badumButton.tick();
  failButton.tick();
  processSoundLevel();
  
  oldTime = newTime;
  delay(10);
}


//buttons and controls
void processBadumButton() {
  //read the badum button
  //HIGH LEVEL 1 is unpressed, LOW level is pressed
  int badumKey = digitalRead(BADUM_PIN);
 
  if(badumKey == 0) {
    newTime = millis();
    
    if(newTime - oldTime >= deltaTime) {
      Serial.println("Button pressed");
//      params->badumCounter++;
//      paramsWriter->writeSoundParams(params);
      playBadum();
    }
  }
}

void processFailButton() {
  //read the fail button
  int failKey = digitalRead(FAIL_PIN);

  if(failKey == 0) {
    newTime = millis();

    if(newTime - oldTime >= deltaTime) {
//      params->badumCounter++;
//      paramsWriter->writeSoundParams(params);
      playFail();
    }
  }
}

void processSoundLevel() {
  //read the volume level
  int valread = analogRead(SOUND_LEVEL_PIN);
  newVolume = map(valread, 1, 1024, 0, 30);

  if(newVolume != oldVolume) {
    mp3_set_volume(newVolume); 
    oldVolume = newVolume;
  }
}

//sound functions
void playBadum() {
  playSound(BADUM_NUMBER);
}

void playFail() {
  playSound(FAIL_NUMBER);
}

void playSound(int number) {
  mp3_play(number);
}



