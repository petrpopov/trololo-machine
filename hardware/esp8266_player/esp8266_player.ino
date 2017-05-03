#include "EEPROMParams.h"
#include <aJSON.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <OneButton.h>
#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>
#include <WiFiManager.h>

#define HTTP_DEBUG

#define MAGIC_ADDR 0
#define MAGIC_VALUE 42
#define RX 13
#define TX 12
#define BADUM_PIN 14
#define FAIL_PIN 5
#define SOUND_LEVEL_PIN A0

#define BADUM_NUMBER 1
#define FAIL_NUMBER 2

const String apiHost = "http://51.15.69.129";
uint32_t chipId;
const String chipName = "Moscow";
const String apiSecret = "asaf3rfalf3k23fmaskfj3";
const String badumPath = "/api/badum";
const String failPath = "/api/fail";

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

  chipId = ESP.getChipId();

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
  checkConfigCounter();
}

//rest api
const char* getBodyForCounter(unsigned long counter) {
  aJsonObject *body = aJson.createObject();
  aJson.addStringToObject(body, "counter", String(counter).c_str());
  aJson.addStringToObject(body, "chipId", String(chipId).c_str());
  aJson.addStringToObject(body, "chipName", chipName.c_str());
  aJson.addStringToObject(body, "apisecret", apiSecret.c_str());
  return aJson.print(body);
}

const String getBadumApiUrl() {
  return apiHost + badumPath;
}

const String getFailApiUrl() {
  return apiHost + failPath;
}

void postMethod(unsigned long counter, const String host) {
  HTTPClient http;
  http.begin(host);
  http.addHeader("Content-Type", "application/json"); 

  const char *payload = getBodyForCounter(counter);
  int httpCode = http.POST((uint8_t *)payload, strlen(payload));

  String answer = http.getString();                  //Get the response payload
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(answer);    //Print request response payload
  http.end(); 
}

void sendRestBadum(unsigned long counter) {
  Serial.print("Posting to /api/badum counter ");
  Serial.println(counter);
  
  postMethod(counter, getBadumApiUrl());
}

void sendRestFail(unsigned long counter) {
  Serial.print("Posting to /api/fail counter ");
  Serial.println(counter);
  
  postMethod(counter, getFailApiUrl());
}

//callbacks
void checkConfigCounter() {
  long nowTime = millis();
  if(badumLongPress && badumPressInitTime != 0 && nowTime - badumPressInitTime >= configLimitTime) {
    stopConfigCounter();
  }
  delay(10);
}

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
    paramsWriter->writeSoundParams(params);
    playSound(BADUM_NUMBER); 

    if(WiFi.status() == WL_CONNECTED) {
      sendRestBadum(params->badumCounter);
    }
  }
}

void playFail() {
  long nowTime = millis();
  if(nowTime - lastPlayStartTime > deltaTime) {
    lastPlayStartTime = nowTime;
    params->failCounter++;
    paramsWriter->writeSoundParams(params);
    playSound(FAIL_NUMBER);

    if(WiFi.status() == WL_CONNECTED) {
      sendRestFail(params->failCounter);
    }
  }
}

void playSound(int number) {
  mp3_play(number);
}



