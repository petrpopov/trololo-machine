#ifndef eepromparams_h
#define eepromparams_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

struct SoundParams {
    public:
        unsigned long badumCounter;
        unsigned long failCounter;
        unsigned long updateDate;
};

class EEPROMParams
{
    public:
        EEPROMParams();
        EEPROMParams(int magicAddress, int magicValue);
        int magicAddress;
        int magicValue;
        void writeSoundParams(SoundParams *params);
        void readSoundParams(SoundParams *params);
        void writeLong(int address, unsigned long value);
        unsigned long readLong(int address);
};

#endif
