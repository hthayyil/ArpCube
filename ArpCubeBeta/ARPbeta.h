#ifndef ARPbeta_h
#define ARPbeta_h
//#include <ArduinoSTL.h>
//#include <vector>
class ARP{
  //array holding the notes in the pattern

  public:
    //std::vector<std::vector<int>>midiNotes;
    //int numNotes;
    int s0[4];
    int _s0;
    int s1[4];
     int _s1;
    int s2[4];
    int _s2;
    int s3[4];
    int _s3;
    int s4[4];
    int _s4;
    int s5[4];
    int _s5;
    int s6[4];
    int _s6;
    int s7[4];
    int _s7 = 0;
    //std::vector<int> tempVec;
    //int tempNumNotes;
    ARP();
    //void pressed(int index);
    void addToVec(int index, int i);
  //  void addNote(int index, std::vector<int> i);
    void clear();
    int findNote(int index, int k);
  //  int findVecLength(int k);
};

#endif
