#include <SoftwareSerial.h>

#include <ARPbeta.h>

#include <ArduinoSTL.h>
#include <Wire.h>
#include "Adafruit_Trellis.h"
//#include "MIDIUSB.h"
#include <MIDI.h>
#include <MIDImap.h>

/*************************************************** 
VARIATION OF ARP CUBE SUPPORTS 4 NOTE ARPEGS
 ****************************************************/

/*************************************************** 


 ****************************************************/
//configuring BAUD rate for MID settings 
struct MySettings : public midi::DefaultSettings
{
  //static const long BaudRate = 115200;
  static const long BaudRate = 31250;
  
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, MySettings);


#define MIDICONTROLLER 0
#define ARPCUBE 1

// set the mode here, MIDICONTROLLER as default
int MODE = MIDICONTROLLER; 

//Call upon Trellis libary for button pad commands
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();

Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1);

//set the interrupt pin to A2
#define INTPIN 2
//set the number of trellis boards and number of keys we are using
#define NUMTRELLIS 2
#define numKeys (NUMTRELLIS * 16)

const byte rxPin = 0; // not used for the moment
const byte txPin = 1;

SoftwareSerial mySerial(rxPin, txPin);


// Connect Trellis Vin to 5V and Ground to ground.
// Connect the INT wire to pin #A2
// Connect I2C SDA pin to your Arduino SDA line PIN A4
// Connect I2C SCL pin to your Arduino SCL line PIN A5
// All Trellises share the SDA, SCL and INT pin
// OCTAVE POT - potPin is on analog pin 0
// TEMPO POT is on analog pin 1
//SWITCH is on digital Pin 6



//set midi commands
int velocity = 100;//velocity of MIDI notes, must be between 0 and 127
 //higher velocity usually makes MIDI instruments louder
int noteON = 144;//144 = 10010000 in binary, note on command
int noteOFF = 128;//128 = 10000000 in binary, note off command

//OCTAVE POT is on analog pin 0
int potPin = 0; //potentiometer is on A0
long potVal; //value to store potentiometer reading
long prevPotVal = 0;

//TEMPO POT is on analog pin 1
int tempoPin = 1; //potentiometer is on A0
long tempo; //value to store potentiometer reading
long prevTempo = 0;
//SWITCH is on digital pin 6
int switchPin = 6;

//boolean variable to make sure the interrupt only fires once
boolean wasPressed;

//Class objects- defined in the MIDImap and ARP libraries
MIDImap mp;
ARP arp;

void setup() {
   // Set MIDI baud rate:
   //9600 to interface with Hairless midi software 
   //Serial.begin(9600);
  // Serial.print("DEMO");

  //pinMode( rxPin, INPUT );
  pinMode( txPin, OUTPUT);
  //mySerial.begin( 31250 );
  Serial.begin(31250);
  //MIDI.begin();

  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  //digitalWrite(INTPIN, HIGH);
  pinMode(switchPin, INPUT);
  
 // attachInterrupt(digitalPinToInterrupt(INTPIN), addButton, FALLING);
  
  // begin() with the addresses of each panel in order
  trellis.begin(0x71, 0x70);  // only one

  // light up all the LEDs in order and play the lick
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();
    if (i == 2 || i == 4 || i == 5 || i == 7) {
      MIDImessage(noteON, 48+i, velocity);//turn note on
      delay(100);//hold note for 300ms
      MIDImessage(noteOFF, 48+i, velocity);//turn note off
     } else if (i == 8) {
        MIDImessage(noteON, 48+4, velocity);//turn note on
        delay(100);//hold note for 300ms
        MIDImessage(noteOFF, 48+4, velocity);//turn note off
      } else if (i == 13 || i == 15) {
        MIDImessage(noteON, 48+i-13, velocity);//turn note on
        delay(100);//hold note for 300ms
        MIDImessage(noteOFF, 48+i-13, velocity);//turn note off
      }
  }
  // then turn them all off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(30);
  }
}


void loop() {

  //read the value of the pin to determine which mode to be in
  if (digitalRead(switchPin) == HIGH) {
    MODE = MIDICONTROLLER;
    arp.clear();
  }
  else MODE = ARPCUBE;
  
  delay(30); // 30ms delay helps for latency
  //octave pot
  potVal = analogRead(potPin);

  
  //MIDICONTROLLER MODE
  if (MODE == MIDICONTROLLER) {    
    // If a button was just pressed or released
    if (trellis.readSwitches()) {
      // go through every button and check new status
      
      for (uint8_t i=0; i<numKeys; i++) {
        if (trellis.justPressed(i)) {
            MIDImessage(noteON, mp.noteCalcKeyC(i, potVal), velocity);//turn note on
            trellis.setLED(i);
        } 
         // if it was released, turn it off
         if (trellis.justReleased(i)) {
            MIDImessage(noteOFF, mp.noteCalcKeyC(i, potVal), velocity);//turn note off 
            trellis.clrLED(i);
        }
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  
  } //end midicontroller mode



 else if (MODE == ARPCUBE) {

  //read switches
      if (trellis.readSwitches()) {
      
      // go through every button and check new status
      for (uint8_t i=0; i<numKeys; i++) {
        if (trellis.justPressed(i)) {   
            arp.addToVec(i);
            trellis.setLED(i);
            MIDImessage(noteON, mp.noteCalcKeyC(i, potVal), velocity);//turn note on
            wasPressed = true;  
        } 
         // if it was released, turn it off
         if (trellis.justReleased(i)) {
            MIDImessage(noteOFF, mp.noteCalcKeyC(i, potVal), velocity);//turn note off 
            trellis.clrLED(i);
        }
   
      }
     
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }//end read switches

    if (wasPressed){
      arp.pressed();
      wasPressed = false; 
    }
   
    //this is where the arpeggiating takes place
    //if a note is in the arp midiNotes vector turn it on and off
   for (uint8_t k=0; k<arp.numNotes; k++) {
        for (int j = 0; j < arp.findVecLength(k); j ++) {
          if (but != -1) {
            int but = arp.findNote(k, j);
             MIDImessage(noteON, mp.noteCalcKeyC(but, potVal), velocity);//turn note on
            trellis.setLED(but);
          }
        }

          trellis.writeDisplay();
          delay(analogRead(tempoPin));
           for (int j = 0; j < arp.findVecLength(k); j ++) {
          int but = arp.findNote(k, j);
          if (but != -1) {
             MIDImessage(noteOFF, mp.noteCalcKeyC(but, potVal), velocity);//turn note off 
             trellis.clrLED(but);
          }
        }
           trellis.writeDisplay();   
          delay(analogRead(tempoPin));
    
   
   }
    
  
  
 }//end if ARPCUBE
  
  
}//end loop 


  //send MIDI message through USB port
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}
