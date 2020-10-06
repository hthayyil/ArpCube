# ArpCube
**Arduino Code and schematics for the ArpCube Midi Controller created by Hanna Thayyil**


The ArpCube is an Arduino powered MIDI controller. The cube can be plugged into a computer to process the button controls into MIDI, 
or it can be plugged into any device that understands MIDI. 
The ArpCube can act as a standalone MIDI trigger or it can be used as an arpeggiator. 
When in arpeggiator mode, the ArpCube allows a user to step through a selected sequence of notes based on the input buttons. 

## Modes

**MIDI Mode**
When in MIDI mode, pressing a button will send it’s corresponding MIDI On signal out once, and releasing a button will send a MIDI Off signal. 
Pressing and holding a button will sustain the note until the button is released. 
When in MIDI Mode the user can use the KEY potentiometer to choose which key the notes are mapped to. 
The chosen key will be displayed on the LCD screen.
The user can also use the OCTAVE potentiometer to transpose the notes on each key up or down octaves.


**Mono Arp Mode** 
In Mono mode the cube steps through a sequence of 32 steps. 
Each step is represented by a button, and when a step is reached the button light will turn on. 
When the arpeggiator reaches a step and the user presses any of the 32 buttons, that step will save the note that was just pressed and replay that note when it reaches that step. 
Every step can only support one note, so if a step already has a note saved and the user presses another note, the previously programmed note will be erased and the new note will be saved. 
The user can transpose octave and keys using the OCTAVE and KEY Potentiometer, and the user can also control the BPM using the BPM Potentiometer.
The BPM, and key will be displayed on the LCD screen.

**Poly Arp Mode** 
In Poly Mode the cube steps through a sequence of 8 steps. 
Each step is represented by a column on the button matrix. Each step can hold up to 4 notes. 
When the sequence is on a step and a user presses a key that note will be added to that step. 
When the step has reached 4 notes stored, and the user adds another note, all old notes will be cleared and the new note will be stored. 
The user can transpose octave and keys using the OCTAVE and KEY pot, and control BPM using the BPM pot.
The BPM, and key will be displayed on the LCD screen.
