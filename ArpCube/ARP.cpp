#include "ARP.h"
#include <ArduinoSTL.h>
#include <vector>
ARP::ARP()
{
  //clear the notes in the midi vector
  midiNotes.clear();
  numNotes = 0;

}
//this is for the original arpeg
ARP::pressed(int i){
  //if a pad was pressed and it is already in the pattern remove it, else add it to the arpeg
  if(std::find(midiNotes.begin(), midiNotes.end(), i) != midiNotes.end()) {
    this->removeNote(i);
} else {
    this->addNote(i);
}
}


 ARP::addNote(int i){
   midiNotes.push_back(i);
   numNotes ++;
}

ARP::removeNote(int i){
  midiNotes.erase(std::remove(midiNotes.begin(), midiNotes.end(), i), midiNotes.end());
  numNotes--;
}

ARP::clear(){
  midiNotes.clear();
  numNotes = 0;
}
