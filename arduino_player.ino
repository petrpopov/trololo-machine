#include <EEPROMParams.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#define MAGIC_ADDR 0
#define MAGIC_VALUE 42
#define BADUM_NUMBER 1
#define FAIL_NUMBER 2

#define SOUND_LEVEL_PIN A1
#define BADUM_PIN 12
#define FAIL_PIN 13

int oldVolume = 0;
int newVolume = 0;
unsigned long oldTime = 0;
unsigned long newTime = 0;
unsigned long deltaTime = 800;

SoftwareSerial wifi(8, 9);
SoftwareSerial playerSerial(10, 11); //RX, TX  for player instead of default 0, 1
SoundParams *params = new SoundParams();
EEPROMParams *paramsWriter = new EEPROMParams(MAGIC_ADDR, MAGIC_VALUE);

String essid = "kathmandu";
String password = "39cc56a12e";

void setup() {
  Serial.begin(9600);
  playerSerial.begin (9600);
  wifi.begin(115200);
  
  mp3_set_serial (playerSerial); 
  delay(200);
  mp3_set_volume(25);
  delay(200);
  mp3_set_EQ(2);

  //pins and config
  pinMode(BADUM_PIN, INPUT);
  pinMode(FAIL_PIN, INPUT);
  pinMode(SOUND_LEVEL_PIN, INPUT);
  
  //read params
  paramsWriter->readSoundParams(params);
}

void connect() 
{
  wifi.println("AT");
  Serial.println(wifi.readString());

  wifi.println("AT+RST");
  Serial.println(wifi.readString());

  String cmd = "AT+CWJAP=\"" +essid+"\",\"" + password + "\"";
  wifi.println(cmd);
  delay(4000);
  Serial.println(wifi.readString());

  wifi.println("AT+CIFSR");
  Serial.println(wifi.readString());
}

void loop() {

  while(wifi.available()) {
    Serial.println(wifi.readString());
  }
  
  processBadumButton();
  processFailButton();

  processSoundLevel();
  
  oldTime = newTime;
}

//buttons and controls
void processBadumButton() 
{
  //read the badum button
  boolean badumKey = !digitalRead(BADUM_PIN);
 
  if(badumKey) {
    newTime = millis();
    
    if(newTime - oldTime >= deltaTime) {
      params->badumCounter++;
      paramsWriter->writeSoundParams(params);
      wifi.print("{");
      wifi.println(params->badumCounter);
      playBadum();
    }
  }
}

void processFailButton() 
{
  //read the fail button
  boolean badumKey = !digitalRead(FAIL_PIN);
 
  if(badumKey) {
    newTime = millis();
    
    if(newTime - oldTime >= deltaTime) {
      params->failCounter++;
      paramsWriter->writeSoundParams(params);
      wifi.print("}");
      wifi.println(params->failCounter);
      playFail();
    }
  }
}

void processSoundLevel() 
{
  //read the volume level
  int valread = analogRead(SOUND_LEVEL_PIN);
  newVolume = map(valread, 0, 1023, 0, 30);

  if(newVolume != oldVolume) {
    mp3_set_volume(newVolume); 
    oldVolume = newVolume;
  }
}

//sound functions
void playBadum() 
{
  playSound(BADUM_NUMBER);
}

void playFail() 
{
  playSound(FAIL_NUMBER);
}

void playSound(int number)
{
  mp3_play(number);
}

