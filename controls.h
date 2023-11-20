

/* 
 * File:        controls.h 
 * Author:      Aled
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONTROLS_H
#define	CONTROLS_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include "system.h"
#include "ILI9225.h"    /* for display functions */
#include "mytypes.h"
#include "midisend.h"


// declarations


extern char volume;    //midi volume global
extern char patch;     //current midi patch global
extern char tmr_count;

void patch_change(char var);
void volume_change(char var);
void patch_tmr_done(void); 


#endif	/* CONTROLS_H */

