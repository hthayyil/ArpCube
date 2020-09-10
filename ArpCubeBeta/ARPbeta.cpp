#include "ARPbeta.h"
//#include <ArduinoSTL.h>
//#include <vector>
ARP::ARP()
{


clear();

}

void ARP::addToVec(int index, int i ){
//zero
  if (index == 0){
    if (_s0 <4) {
      s0[_s0] = i;
      _s0++;
    }
    else {
      s0[0] = i;
      _s0 = 1;
    }}
    //one
  else if(index == 1) {
    if (_s1 <4) {
      s1[_s1] = i;
      _s1++;
    }
    else {
      s1[0] = i;
      _s1 = 1;
    } }
    //two
  else if (index == 2) {
     if (_s2 <4) {
        s2[_s2] = i;
        _s2++;
      }
      else {
        s2[2] = i;
        _s2 = 1;
      } }
  //three
  else if (index == 3) {
    if (_s3 <4) {
        s3[_s3] = i;
        _s3++;
      }
      else {
        s3[0] = i;
        _s3 = 1;
      } }
  //four
  else if (index == 4) {
    if (_s4 <4) {
      s4[_s4] = i;
      _s4++;
    }
    else {
      s4[_s4] = i;
      _s4 = 1;
    } }
  //five
  else if (index == 5) {
    if (_s5 <4) {
      s5[_s5] = i;
      _s5++;
    }
    else {
      s5[_s5] = i;
      _s5 = 1;
    } }
  //six
  else if (index == 6) {
    if (_s6 <4) {
      s6[_s6] = i;
      _s6++;
    }
    else {
      s6[0] = i;
      _s6 = 1;
    } }
  //seven
  else if (index == 7) {
    if (_s7 <4) {
      s7[_s7] = i;
      _s7++;
    }
    else {
      s7[0] = i;
      _s7 = 1;
    } }

}



int ARP::findNote(int index, int k){
  if (index == 0) {
    return s0[k];
  }
  else if (index == 1){
    return s1[k];
  }
  else if (index == 2){
    return s2[k];
  }
  else if (index == 3){
    return s3[k];
  }
  else if (index == 4){
    return s4[k];
  }
  else if (index == 5){
    return s5[k];
  }
  else if (index == 6){
    return s6[k];
  }

  else if (index == 7){
    return s7[k];
  }
return -1;
}//find Note



void ARP::clear(){
for (int k = 0; k < 4; k++){
  s0[k] = -1;
  s1[k] = -1;
  s2[k] = -1;
  s3[k] = -1;
  s4[k] = -1;
  s5[k] = -1;
  s6[k] = -1;
  s7[k] = -1;

}
  _s0 = 0;
  _s1 = 0;
  _s2 = 0;
  _s3 = 0;
  _s4 = 0;
  _s5 = 0;
  _s6 = 0;
  _s7 = 0;
}
