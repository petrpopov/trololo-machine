//http://playground.arduino.cc/Code/EEPROMReadWriteLong
#include "eepromparams.h"
#include <EEPROM.h>

EEPROMParams::EEPROMParams()
{

}

EEPROMParams::EEPROMParams(int magicAddress, int magicValue)
{
    this->magicAddress = magicAddress;
    this->magicValue = magicValue;

    EEPROM.begin(12);
    int savedValue = EEPROM.read(magicAddress);
    if(savedValue != magicValue) {
        //erase
        EEPROM.write(magicAddress, magicValue);
        EEPROM.write(magicAddress+1, 0);
        EEPROM.write(magicAddress+2, 0);
        EEPROM.write(magicAddress+3, 0);
        EEPROM.write(magicAddress+4, 0);
        EEPROM.end();
    }
}

void EEPROMParams::writeSoundParams(SoundParams *params)
{
    int address = this->magicAddress+1;
    this->writeLong(address, params->badumCounter);
    this->writeLong(address + 4, params->failCounter);
    this->writeLong(address + 8, params->updateDate);
    EEPROM.commit();
}

void EEPROMParams::readSoundParams(SoundParams *params)
{
    int address = this->magicAddress+1;
    params->badumCounter = this->readLong(address);
    params->failCounter = this->readLong(address + 4);
    params->updateDate = this->readLong(address + 8);
}

void EEPROMParams::writeLong(int address, unsigned long value)
{
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
}

unsigned long EEPROMParams::readLong(int address)
{
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

