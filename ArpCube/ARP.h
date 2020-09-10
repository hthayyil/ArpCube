#ifndef ARP_h
#define ARP_h
#include <ArduinoSTL.h>
#include <vector>
class ARP{
  //array holding the notes in the pattern

  public:
    std::vector<std::vector<int>> midiNotes;
    int numNotes;
    std::vector<int> tempVec;
    int tempNumNotes;
    int numNotes;
    ARP();
    pressed(int i);
    addNote(int i);
    removeNote(int i);
    clear();
};

#endif
