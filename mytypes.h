

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MYTYPES_H
#define	MYTYPES_H

#include <xc.h> // include processor files - each processor file is guarded.  
//#define DEBUG

/* type encoder_inputs : for bit access to test inputs from rotary encoders */
typedef union {
    unsigned char value;
    struct {
        unsigned   enc1_1   :1;
        unsigned   enc1_2   :1;
        unsigned   enc2_1   :1;
        unsigned   enc2_2   :1;
        unsigned   b4       :1;
        unsigned   b5       :1;
        unsigned   b6       :1;
        unsigned   b7       :1;
    };
}  encoder_inputs;

// constants for volume/patch changes
#define UP   1
#define DOWN 2

//initial values for volume and patch numbers
#define INITIAL_VOLUME  107
#define INITIAL_PATCH   0


#endif	/* MYTYPES_H */

