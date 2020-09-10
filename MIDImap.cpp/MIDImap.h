#ifndef MIDImap_h
#define MIDImap_h
#include <ArduinoSTL.h>
#include "string.h"

class MIDImap {

public:
  MIDImap();
  int octaveCalculator(long potValue);
  int mapNote(int i);
  int noteCalc(int i, long potVal, long keyPot);
  std::string keyCalculator(long potValue);
  std::string printKey(long potValue);
  int mapNoteKey(int i, std::string key);
};

#endif
