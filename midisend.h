/* 
 * File:   midisend.h
 * Author: aled
 *
 * Created on 09 May 2020, 22:52
 */

#ifndef MIDISEND_H
#define	MIDISEND_H
    
#include "mytypes.h"
#include "system.h"
#include "ILI9225.h"

//chip select for VS1103 midi chip    
#define XDCS LATAbits.LATA2

// declarations
    
void midistart();
void midistop();
void midisend(char midi_byte);
void midivolume(char vol);
void midipatch(char patch);


#endif	/* MIDISEND_H */

