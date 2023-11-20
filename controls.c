
/*
 * File:   controls.c
 * Author: aled
 *
 * Created on 23 March 2020, 18:20
 * 
 *      functions to be called when inputs are recieved in main loop or
 *      by TMR interrupt
 *      send updates to screen and sends midi commands
 * 
 */


/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */
#include <stdbool.h>       /* For true/false definition */
#include "controls.h"
#include "ui.h"


extern bool update_ui;


/*
 * volume_change(x) , x can be UP or DOWN
 * 
 * increment or decrement volume if its (0-127)
 * TODO update display to show new volume
 */
void volume_change(char dir) {
    if(dir == UP) {
        volume = MIN(volume,126);
        volume++;
        draw_volume(volume,UP);
        midivolume(volume);     //send new volume setting
    }
    else {
        volume = MAX(volume,1);
        volume--;
        draw_volume(volume,DOWN);
        midivolume(volume);     //send new volume setting
    }
}



/*
 * patch_change(x) , x can be UP or DOWN
 * 
 * patch is selected with the rotary encoder, selection is made after 0.8
 * seconds counted using timer0
 * 
 * increment or decrement patch number (0-127)
 * 
 * TODO start (or re-start) TMR countdown with preload
 */
void patch_change(char dir) {
    
    if(dir == UP) {
        patch = MIN(patch,126);
        patch++;
        draw_patch(patch,0);
    }
    else {
        patch = MAX(patch,1);
        patch--;
        draw_patch(patch,0);
    }
    update_ui=true;
    
//    //reset TMR0 rollover counter, enable TMR0 interrupt and start it if not already running
//    tmr_count=0;
//    INTCONbits.TMR0IE = 1;
//    //TODO enable interrupts ei();
//    OPTION_REGbits.TMR0CS = 0; 
}

/*
 * TODO patch_tmr_done()
 * 
 * called when TMR countdown for patch change finishes
 * update display to reflect new patch choice is active
 * TODO update screen
 */
void patch_tmr_done() {
    //no more interrupts from TMR0, and stop TMR0 running
    INTCONbits.TMR0IE = 0;
    di();
    OPTION_REGbits.TMR0CS = 1;
    update_ui=true;
    //re-entrant ? draw_patch(patch,1);
    
}   
