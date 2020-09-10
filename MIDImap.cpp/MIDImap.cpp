
#include "MIDImap.h"
#include "string.h"
MIDImap::MIDImap()
{
  //initializations?

}
//NoteCalc:returns the MIDI note based on pot offset for octave , key pot, and key pressed
int MIDImap::noteCalc(int i, long potVal, long keyPot){

  if (keyCalculator(keyPot) == "CHROM")
    return (mapNote(i) + octaveCalculator(potVal));
  else return (mapNoteKey(i, keyCalculator(keyPot)) + octaveCalculator(potVal));
}

//OCTAVECALCULATOR :reads in potentiometer value and returns a value for the MIDI 'C' note to base
//calculations of all other notes
//Note: didn't split equally because many notes are inaudible on the higher and lower freq ranges 
int MIDImap::octaveCalculator(long potValue){
  // C-1 ,0 1  octaves  notes 0 - 32
  if (potValue <= 180){
     return 0;
  }
  //C 1, 2, 3 octave
  if (potValue > 180 && potValue <= 380){
    return 24;
  }
  //C 3, 4, 5 octave
  if (potValue > 380 && potValue <= 650 ){
    return 48;
  }
  //C 5, 6, 7 octave
  if (potValue > 650 && potValue <= 950){
    return 72;
  }
  //C 7, 8, 9 octave
  if (potValue > 950 && potValue <= 1023 ){
    return 96;
  }
  return 0;

}

std::string MIDImap::keyCalculator(long potValue){
  if (potValue <= 78){
     return "CHROM";
  }
  else if (potValue > 78 && potValue <= 156){
    return "C";
  }
  else if (potValue > 156 && potValue <= 234){
    return "D";
  }
  else if (potValue > 234 && potValue <= 310){
    return "E";
  }
  else if (potValue > 310 && potValue <= 386 ){
    return "F";
  }
  else if (potValue > 386&& potValue <= 462 ){
    return "G";
  }
  else if (potValue > 462 && potValue <= 538 ){
    return "A";
  }
  else if (potValue > 538 && potValue <= 614 ){
    return "B";
  }
  else if (potValue > 614 && potValue <= 690 ){
    return "CSHARP";
  }
  else if (potValue > 690 && potValue <= 766 ){
    return "DSHARP";
  }
  else if (potValue > 766 && potValue <= 842 ){
    return "FSHARP";
  }
  else if (potValue > 842 && potValue <= 918 ){
    return "GSHARP";
  }
  else if (potValue > 918 && potValue <= 1023 ){
    return"ASHARP";
  }
  else return "CHROM";

}//end keyCalculator

std::string MIDImap::printKey(long potValue){
  if (potValue <= 78){
     return "KEY: CHROMATIC";
  }

  if (potValue > 78 && potValue <= 156){
    return "KEY: C/Am      ";
  }

  if (potValue > 156 && potValue <= 234){
    return "KEY: D/Bm      ";
  }

  if (potValue > 234 && potValue <= 310){
    return "KEY: E/C#m     ";
  }

  if (potValue > 310 && potValue <= 386 ){
    return "KEY: F/Dm      ";
  }
  if (potValue > 386 && potValue <= 462 ){
    return "KEY: G/Em      ";
  }
  if (potValue > 462 && potValue <= 538 ){
    return "KEY: A/F#m      ";
  }
  if (potValue > 538 && potValue <= 614 ){
    return "KEY: B/G#m      ";
  }
  if (potValue > 614 && potValue <= 690 ){
    return "KEY: C#/A#m      ";
  }
  if (potValue > 690 && potValue <= 766 ){
    return "KEY: D#/Cm       ";
  }
  if (potValue > 766 && potValue <= 842 ){
    return "KEY: F#/D#m      ";
  }
  if (potValue > 842 && potValue <= 918 ){
    return "KEY: G#/Fm       ";
  }
  if (potValue > 918 && potValue <= 1023 ){
    return"KEY: A#/Gm        ";
  }
  else return "KEY: CHROMATIC";

}

//MIDImap: this takes the location of a button on the trellis and maps it to a L-R grid system
//which will be translated to a MIDI note number based on the octave
//MAPS NOTES CHROMATICALLY
int MIDImap::mapNote(int i){
  if (i == 0) return 7;
  if (i == 1) return 15;
  if (i == 2) return 23;
  if (i == 3) return 31;
  if (i == 4) return 6;
  if (i == 5) return 14;
  if (i == 6) return 22;
  if (i == 7) return 30;
  if (i == 8) return 5;
  if (i == 9) return 13;
  if (i == 10) return 21;
  if (i == 11) return 29;
  if (i == 12) return 4;
  if (i == 13)return 12;
  if (i == 14) return 20;
  if (i == 15) return 28;
  if (i == 16) return 3;
  if (i == 17) return 11;
  if (i == 18) return 19;
  if (i == 19) return 27;
  if (i == 20) return 2;
  if (i == 21) return 10;
  if (i == 22) return 18;
  if (i == 23) return 26;
  if (i == 24) return 1;
  if (i == 25) return 9;
  if (i == 26) return 17;
  if (i == 27) return 25;
  if (i == 28) return 0;
  if (i == 29) return 8;
  if (i == 30) return 16;
  else return 24;
}

//MapNoteKey: Maps notes based on the key selected by the KEY POT
int MIDImap::mapNoteKey(int i, std::string key){

//NOTES FOR: c MAJ/ A min
  int cNotes[32] = {
    12, 24, 36, 48,
   11, 23, 35, 47,
   9, 21, 33, 45,
    7, 19, 31, 43,
    5,17, 29, 41,
    4, 16, 28, 40,
     2, 14, 26, 38,
     0, 12,24, 36};

     if (key == "C") return cNotes[i];
     else if (key == "D") {return cNotes[i] +2;}
     else if (key == "E") {return cNotes[i] +4;}
     else if (key == "F") {return cNotes[i] +5; }
     else if (key == "G"){return cNotes[i] + 7; }
     else if (key == "A") {return cNotes[i] + 9; }
     else if (key == "B") {return cNotes[i] +11; }
     else if (key == "CSHARP") {return cNotes[i] +1; }
     else if (key == "DSHARP") {return cNotes[i] +3; }
     else if (key == "FSHARP") {return cNotes[i] +6; }
     else if (key == "GSHARP") {return cNotes[i] +8; }
     else if (key == "ASHARP") {return cNotes[i] +10; }
  return cNotes[i];

}
