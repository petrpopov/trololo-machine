#include "EEPROMParams.h"
#include <OneButton.h>
#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>
#include <WiFiManager.h>

#define MAGIC_ADDR 0
#define MAGIC_VALUE 42
#define RX 13
#define TX 12
#define BADUM_PIN 14
#define FAIL_PIN 16
#define SOUND_LEVEL_PIN A0

#define BADUM_NUMBER 1
#define FAIL_NUMBER 2

//this is for controlling volume
int oldVolume = 0;
int newVolume = 0;

//this is for controlling pause between playing files
unsigned long lastPlayStartTime = 0;
unsigned long deltaTime = 1500;

//this is for controlling entering the wifi config mode
boolean badumLongPress = false;
int badumCounter = 0;
long badumPressInitTime = 0;
const long configLimitTime = 10000;
const int configPressLimit = 3;

//eeprom params custom writer for saving play counters
SoundParams *params = new SoundParams();
EEPROMParams *paramsWriter = new EEPROMParams(MAGIC_ADDR, MAGIC_VALUE);

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

  //read counters params
  paramsWriter->readSoundParams(params);

  //setup Serial
  Serial.begin(115200);
  Serial.println("Starting");

  //Setup mp3 player
  mp3.begin(9600);
  mp3_set_serial(mp3);
  delay(20);
  mp3_set_volume(20);

  //setup buttons for controlling device
  badumButton.attachClick(pressBadumButton);
  badumButton.attachLongPressStop(startConfigCounter);
  failButton.attachClick(playFail);
}

void loop() {
  badumButton.tick();
  failButton.tick();
  processSoundLevel();

  long nowTime = millis();
  if(badumLongPress && badumPressInitTime != 0 && nowTime - badumPressInitTime >= configLimitTime) {
    stopConfigCounter();
  }
  delay(10);
}

//callbacks
void startConfigCounter() {
  if(badumLongPress == true) {
    Serial.println("Already started configuration init mode");
    return;
  }

  Serial.println("Starting confifuration init mode");
  badumLongPress = true;
  badumPressInitTime = millis();
}

void stopConfigCounter() {
  Serial.println("Time is out, stop configuration init mode");
  flushConfigInitParams();
}

void flushConfigInitParams() {
  badumLongPress = false;
  badumPressInitTime = 0;
  badumCounter = 0;
}

void startWifiManager() {
  Serial.println("Starting wifi manager");

  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(300);
  if(!wifiManager.autoConnect("trololowifi")) {
    Serial.println("Connection failed");
  }

  Serial.println("Connected");
}

void pressBadumButton() {
  if(badumLongPress == true) {
    badumCounter++;
    Serial.println("Configuration init mode short press");

    if(badumCounter >= configPressLimit) {
      Serial.println("Config init done, entering configurations");
      Serial.println("Starting WiFi Manager");
      flushConfigInitParams();
      startWifiManager();
    }
  }
  else {
    playBadum();
  }
}

//buttons and controls
void processSoundLevel() {
  //read the volume level
  int valread = analogRead(SOUND_LEVEL_PIN);
  newVolume = map(valread, 1, 1024, 0, 30);

  if(newVolume != oldVolume) {
    mp3_set_volume(newVolume); 
    oldVolume = newVolume;
  }
}

void playBadum() {
  long nowTime = millis();
  if(nowTime - lastPlayStartTime > deltaTime) {
    lastPlayStartTime = nowTime;
    params->badumCounter++;
    Serial.println(params->badumCounter);
    paramsWriter->writeSoundParams(params);
    playSound(BADUM_NUMBER); 
  }
}

void playFail() {
  long nowTime = millis();
  if(nowTime - lastPlayStartTime > deltaTime) {
    lastPlayStartTime = nowTime;
    params->failCounter++;
    paramsWriter->writeSoundParams(params);
    playSound(FAIL_NUMBER);
  }
}

void playSound(int number) {
  mp3_play(number);
}



