
#include <SoftwareSerial.h>
#include <MIDImap.h>
#include <ArduinoSTL.h>
#include <LiquidCrystal.h> 
#include <Wire.h>
#include "Adafruit_Trellis.h"
//#include "MIDIUSB.h"
#include <MIDI.h>
#include <ARPbeta.h>




//DEFINE MODES:
#define MIDICONTROLLER 0
#define ARPCUBE 1
#define POLY 2

// Define MODE: MIDICONTROLLER MODE is set as default
int MODE = MIDICONTROLLER; 

//Call upon Trellis libary for button pad commands
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();

//Inititalize TRELLIS
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1);

//set the interrupt pin to A2
#define INTPIN 2
//set the number of trellis boards and number of keys we are using
#define NUMTRELLIS 2
#define numKeys (NUMTRELLIS * 16)

/*
 Connect Trellis Vin to 5V and Ground to ground.
 Connect the INT wire to pin #A2
 Connect I2C SDA pin to your Arduino SDA line PIN A4
 Connect I2C SCL pin to your Arduino SCL line PIN A5
 All Trellises share the SDA, SCL and INT pin
 OCTAVE POT - potPin is on analog pin 0
 TEMPO POT is on analog pin 1
 SWITCH is on digital Pin 6
 KEY POT is on pin A2
*/


//SET MIDI COMMANDS
int velocity = 100;//velocity of MIDI notes, must be between 0 and 127
 //higher velocity usually makes MIDI instruments louder
int noteON = 144;//144 = 10010000 in binary, note on command
int noteOFF = 128;//128 = 10000000 in binary, note off command

//OCTAVE POT is on analog pin 0
int potPin = 0; //potentiometer is on A0
long potVal; //value to store potentiometer reading
long prevPotVal = 0;

//TEMPO POT is on analog pin 1
int tempoPin = 1;
long tempo; //value to store potentiometer reading
long prevTempo = 0;
long BPM;

//SWITCH is on digital pin 6
int switchPin = 6;

//BUTTON is on pin 7
int button = 7;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int prevButtonState = LOW;


//POTENTIOMETER to indicate key is on pin A2
int keyPot = 2;
long keyVal;

volatile int arpSeq[32];
bool wasPressed = false;
//volatile int polySeq[8][4];

//used for poly mode
int noteOrderPoly[8][4] = 
{ {28, 29, 30, 31},
  {24, 25, 26, 27},
  {20, 21, 22, 23},
  {16, 17, 18, 19},
  {12, 13, 14, 15},
  {8,   9, 10, 11},
  {4,   5,  6,  7},
  {0,   1,  2,  3} };

//Maps trellis matrix button order
int noteOrder[32] = {
  28, 24, 20, 16, 12 ,8, 4 ,0, 
  29, 25, 21, 17, 13, 9, 5,1,
  30, 26, 22, 18, 14, 10, 6, 2,
  31, 27, 23, 19, 15, 11, 7, 3
};

//Class objects- defined in the MIDImap and ARP libraries
MIDImap mp;
ARP arp;

//Set Contrast: used to surpass need for POT on LCD
int Contrast=100;
LiquidCrystal lcd(13, 12, 8, 9, 10, 11);  

//INITIALIZE SOFTWARE SERIAL: to use DIN Socket
const byte rxPin = 0; // not used for the moment
const byte txPin = 1;
SoftwareSerial mySerial(rxPin, txPin);

//ClearSequence: Resets the ARP sequence
void clearSequence(){
   for(int i=0; i<32; i++){
      arpSeq[i] = -1;
   }
}


  //send MIDI message through USB port
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}

void setup() {
   // Set MIDI baud rate:
   //9600 to interface with Hairless midi software 
   pinMode(button, INPUT);
   
   pinMode( txPin, OUTPUT);
   
   //initialize contrast for LCD
   analogWrite(5,Contrast);
   
   lcd.begin(16, 2);
   lcd.print("ARPCUBE");
   
   Serial.begin(31250);
  // Serial.begin(9600);
   clearSequence();
  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);

  pinMode(switchPin, INPUT);
 keyVal = analogRead(keyPot);

//set default tempo to 120 BPM 60k/120
 tempo = 500; 
 
  
 // attachInterrupt(digitalPinToInterrupt(INTPIN), addButton, FALLING);
  
  // begin() with the addresses of each panel in order
  trellis.begin(0x71, 0x70);  // only one

  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();
    if (i == 2 || i == 4 || i == 5 || i == 7) {
      delay(100);//hold note for 300ms
     } else if (i == 8) {
        delay(100);//hold note for 300ms
      } else if (i == 13 || i == 15) {      
        delay(100);//hold note for 300ms
      }
  }
  
  // then turn them all off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(30);
  }
}//end setup


void loop() {

  //read the value of the pin to determine which mode to be in
  if (digitalRead(switchPin) == HIGH) {
    MODE = MIDICONTROLLER;
    arp.clear();
    clearSequence();
    lcd.setCursor(0,0);
    lcd.print("MIDI MODE          ");
    lcd.setCursor(0, 1); 
    keyVal = analogRead(keyPot);
    lcd.print(mp.printKey(keyVal).c_str());
  }
   else { 
        MODE = ARPCUBE;
////   lcd.setCursor(0,0);
////    lcd.print("ARPMODE BPM: ");
////     lcd.print(tempo*60000);
////    lcd.setCursor(0, 1); 
////    keyVal = analogRead(keyPot);
////    lcd.print(mp.printKey(keyVal).c_str());
}
  
  delay(30); // 30ms delay helps for latency
  
  //octave pot
   potVal = analogRead(potPin);


  if (keyVal!= analogRead(keyPot)){
      //lcd.clear();
     lcd.setCursor(0, 1); 
     keyVal = analogRead(keyPot);
     lcd.print(mp.printKey(keyVal).c_str());
  }


  
  //MIDICONTROLLER MODE
 if  (MODE == MIDICONTROLLER) {
    
        
    // If a button was just pressed or released
    if (trellis.readSwitches()) {
      // go through every button and check new status
      
      for (uint8_t i=0; i<numKeys; i++) {
        if (trellis.justPressed(i)) {
            
            MIDImessage(noteON, mp.noteCalc(i, potVal, keyVal), velocity);//turn note on
            trellis.setLED(i);
        } 
         // if it was released, turn it off
         if (trellis.justReleased(i)) {
            MIDImessage(noteOFF, mp.noteCalc(i, potVal, keyVal), velocity);//turn note off 
            trellis.clrLED(i);
        }
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  
  } //end midicontroller mode


//ARP MODE

while (MODE ==ARPCUBE) {
      lcd.setCursor(0,0);
      lcd.print("ARPMODE BPM: ");
      lcd.print(tempo*60000);
      lcd.setCursor(0, 1); 
      keyVal = analogRead(keyPot);
      lcd.print(mp.printKey(keyVal).c_str());
  
  for (uint8_t k=0; k<numKeys; k++) {
  
  //read Mode switch 
    if (digitalRead(switchPin) == HIGH) {
      MODE = MIDICONTROLLER;
       break;
    }
    
  if (digitalRead(button) == HIGH) {
     MODE = POLY;
     break;   
  }

  
   //reading the TEMPO POT
    if(prevTempo != analogRead(tempoPin)){
     prevTempo = analogRead(tempoPin);
     BPM = map(prevTempo, 0, 1023, 40, 800);
     //tempo indiciates BPM in ms
     tempo = 60000/BPM;
     lcd.setCursor(0,0);
     lcd.print("ARPMODE Bpm: ");
     lcd.print(BPM);
     lcd.print("    ");    
  }

  //reading the KEY POT
   if (keyVal!= analogRead(keyPot)){
    //lcd.clear();
     lcd.setCursor(0, 1); 
     keyVal = analogRead(keyPot);
    lcd.print(mp.printKey(keyVal).c_str());
  }
  potVal = analogRead(potPin);
  
   if (trellis.readSwitches()) {
    for (uint8_t i=0; i<numKeys; i++) {
      int temp[4];
        if (trellis.justPressed(i)) {
            trellis.setLED(i);
             arpSeq[k] = i;
             MIDImessage(noteON, mp.noteCalc(i, potVal,keyVal), velocity);//turn note on        
        } 
         // if it was released, turn it off
         else if (trellis.justReleased(i)) {
            MIDImessage(noteOFF, mp.noteCalc(i, potVal, keyVal), velocity);//turn note off 
            trellis.clrLED(i);
        }
         
      }
      trellis.writeDisplay();
   }//end if readswitches
      // tell the trellis to set the LEDs we requested
      trellis.setLED(noteOrder[k]);
      trellis.writeDisplay();
      
      if (arpSeq[k] != -1)  MIDImessage(noteON, mp.noteCalc(arpSeq[k], potVal,keyVal), velocity);//turn note on     
     delay(tempo);
     
      if (arpSeq[k] != -1)  MIDImessage(noteOFF, mp.noteCalc(arpSeq[k], potVal,keyVal), velocity);//turn note on  
      
    trellis.clrLED(noteOrder[k]);
    trellis.writeDisplay(); 
      
    }//cycle thru arp

  
}//end while ARP

 while (MODE == POLY) {
  
    lcd.setCursor(0,0);
    lcd.print("POLYMODE BPM: ");
    lcd.print(tempo*60000);
    lcd.setCursor(0, 1); 
    keyVal = analogRead(keyPot);
    lcd.print(mp.printKey(keyVal).c_str());

  for (int i = 0 ; i < 8; i++){

     //read Mode switch 
    if (digitalRead(button) == LOW) {
       //Serial.print("Read BUTTON in POLU");
     
       MODE = ARPCUBE;
       break;
      }
    //read Mode switch 
    if (digitalRead(switchPin) == HIGH) {
        MODE = MIDICONTROLLER;
       break;
     }

         //reading the KEY POT
    if (keyVal!= analogRead(keyPot)){
    //lcd.clear();
     lcd.setCursor(0, 1); 
     keyVal = analogRead(keyPot);
     lcd.print(mp.printKey(keyVal).c_str());
   }
     //reading the TEMPO POT
    if(prevTempo != analogRead(tempoPin)){
     prevTempo = analogRead(tempoPin);
     BPM = map(prevTempo, 0, 1023, 40, 800);
     //tempo indiciates BPM in ms
     tempo = 60000/BPM;
     lcd.setCursor(0,0);
     lcd.print("POLYMODE BPM: ");
     lcd.print(BPM);
     lcd.print("    ");    
  }

   
   potVal = analogRead(potPin);
  

     if (trellis.readSwitches()) {
        for (uint8_t q=0; q<numKeys; q++) {
        if (trellis.justPressed(q)) {
            trellis.setLED(q);
            arp.addToVec(i, q);
             MIDImessage(noteON, mp.noteCalc(q, potVal,keyVal), velocity);//turn note on        
         } 
         // if it was released, turn it off
         else if (trellis.justReleased(q)) {
            MIDImessage(noteOFF, mp.noteCalc(q, potVal, keyVal), velocity);//turn note off 
            trellis.clrLED(q);
         }
         
      }
      trellis.writeDisplay();
   }//end if readswitches

  
    
    for (int p = 0; p <4; p ++){
         trellis.setLED(noteOrderPoly[i][p]);
     }

      //PLAY THE NOTES Se
      for (int j = 0; j < 4; j ++) {
   
            int but1 = arp.findNote(i, j);
            if (but1 != -1) MIDImessage(noteON, mp.noteCalc(but1, potVal, keyVal), velocity);
      }
    
     trellis.writeDisplay();
      delay(tempo);
     
      //if (arpSeq[k] != -1)  MIDImessage(noteOFF, mp.noteCalc(arpSeq[k], potVal,keyVal), velocity);//turn note on  

    for (int j = 0; j < 4; j ++) {

         int but2 = arp.findNote(i, j);
         if (but2 != -1)  MIDImessage(noteOFF, mp.noteCalc(but2, potVal, keyVal), velocity);//turn note off          
       }
      
    for (int p = 0; p <4; p ++){
         trellis.clrLED(noteOrderPoly[i][p]);
     }
     trellis.writeDisplay(); 

    
   }//end for 0 - 8

   
  
 }//end if MODE = POLY
  
}//end loop 
