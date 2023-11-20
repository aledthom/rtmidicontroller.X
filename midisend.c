/*
 * File:   midisend.c
 * Author: aled
 *
 * Created on 23 March 2020, 18:20
 * 
 *  Send MIDI commands via SPI bus to VS1103 chip
 * 
 * TODO midisend() main function
 * TODO volume control
 * TODO patch number control
 */

#include <xc.h>
#include "midisend.h"

__bit midi_enabled=0;
//TODO debug variable remove-
__bit note_on=0;

/* midistart()
 *      set intial conditions for midi command transfer by SPI
 */
void midistart(){
    //TODO stop SPI, set clock edge, start SPI
    //ckp=0 cke=1  - idle low, read data on rising edge
    //TODO DEBUG switch to midi spi settings-
    //SSPCON1bits.CKP=0;
    //SSPSTATbits.CKE=1;
    midi_enabled=1;
}

/* midistop()
 *      end midi transmission- set SPI for LCD control again
 */
void midistop(){
    //TODO stop SPI, set clock edge back to normal, start SPI
    //SSPCON1bits.CKP=1;
    //SSPSTATbits.CKE=0;
    midi_enabled=0;
}

/* TODO midisend( char midibyte ) 
 *      send a midi byte via the SPI interface
 *      must be sent as a 16 bit word (0x00 first)
 *      and correct CS line must be set
 * 
 */
void midisend(char midi_byte){
     //if(!midi_enabled) { midistart(); }
     XDCS = 0;      //enable XDCS line     
     spi_write(0);  //send 0x00
     XDCS = 1;
     delay_us(1);
     XDCS = 0;
     spi_write(midi_byte);  //send byte
     XDCS = 1;      //disable XDCS line
}

 
 /*midi_volume(char dir)
  *     send midi command to set volume
  *     midi command is 1011nnnn 0ccccccc 0vvvvvvv
  *     nnnn    = channel number 1-16 (0-15 binary)
  *     ccccccc = control function  (0b00000111 or 7 for volume control)
  *     0vvvvvv = value 0-127          
  */  
void midivolume(char vol){
    midistart();
//    //TODO debug note on/off
//    if(note_on){
//        midisend(0b1000000);//note off
//        midisend(69);//A 440Hz
//        midisend(127);
//        note_on=0;
//        }else{
//        midisend(0b10010000);//note on
//        midisend(69);//A 440Hz
//        midisend(127);
//        note_on=1;
//    }
//    
    midisend(0b10110000); //control message for channel 1
    midisend(7);        //volume change request
    midisend(vol);      //new volume level
    midistop();
}
 
/*  midi_patch(char patch)
 *      send midi command to select new patch number
 *      midi command is 1100nnnn 0ppppppp
 *      where nnnn=midi channel 1-16 (0-15 binary) and ppppppp=patch number
 */ 
void midipatch(char patch){
    midistart();
    midisend(0b11000000);  //request patch change on channel 1
    midisend(patch);
    
    //TODO DEBUG section
//    delay_us(10);
//    midisend(0b10000000);//note off
//    midisend(69);//A 440Hz
//    midisend(127);
//    midisend(0b10010000);//note on
//    midisend(69);//A 440Hz
//    midisend(127);
//    midistop();
}
