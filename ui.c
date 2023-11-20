/*
 * File:   ui.c
 * Author: aled
 *
 * Created on 26 March 2020, 23:40
 */


#include "ui.h"
#include "midimap.h"
#include "midisend.h"

char *midimap_short(char idx){
    switch(idx){
    
        case 0: return("Piano");
        case 1: return("Percussion");
        case 2: return("Organ");
        case 3: return("Guitar");
        case 4: return("Bass");
        case 5: return("Strings");
        case 6: return("Ensemble");
        case 7: return("Brass");
        case 8: return("Reed");
        case 9: return("Pipe");
        case 10: return("Synth Lead");
        case 11: return("Synth Pad");
        case 12: return("Synth Effects");
        case 13: return("Ethnic");
        case 14: return("Percussive");
        case 15: return("Sound FX");
        default: return("Error");
    }
}


/******************************************************************************/
/* Draw initial layout                                                        */
/******************************************************************************/
void init_ui(void){
#ifndef DEBUG
    //start with black background
    fill_rectangle(0, 0, WIDTH, HEIGHT, BACKGROUND);
#endif
    draw_patch(INITIAL_PATCH,0);    
    
    draw_fast_string(10,VOLUME_TEXT_Y,VOLUME_COLOUR,BACKGROUND,"    -    VOLUME    +  ");
    //volume goes 27-127, from x=10 to x=210 in 2 line steps
    //start at INITIAL_VOLUME
    fill_rectangle(10,VOLUME_Y_TOP,10+((INITIAL_VOLUME-27)*2),VOLUME_Y_BOTTOM, VOLUME_COLOUR);
    
}
 
//update volume level indicator
void draw_volume(char newvol, char dir){
    newvol -= 27;
    if(dir == UP) {
        fill_rectangle(8+(newvol*2),VOLUME_Y_TOP,10+(newvol*2),VOLUME_Y_BOTTOM, VOLUME_COLOUR);
    }
    else {
        fill_rectangle(10+(newvol*2),VOLUME_Y_TOP,12+(newvol*2),VOLUME_Y_BOTTOM, BACKGROUND);
    }
}

void draw_patch(char num, char selected){
    
    char drawstring[16];
    int strsize=0;
    
    //if (selected == 0) {
        //Update patch screen as we scroll throught the patches
        //draw patch group
        strcpy(drawstring,midimap_short(num>>3));
        strsize=strlen(drawstring);
        while(strsize<15){
            strcat(drawstring," ");
            strsize++;
        }
        draw_fast_string(60,6,ACTIVE,BACKGROUND,drawstring);
               
    
        //previous 2 patches
        strcpy(drawstring,"               ");
        if(num>1) strcpy(drawstring,midimap[num-2]);
        //add whitespace to end of string
        strsize=strlen(drawstring);
        while(strsize<15){
            strcat(drawstring," ");
            strsize++;
        }
        draw_fast_string(10,42,PATCHLIST,BACKGROUND, drawstring);
        
        if(num>0) strcpy(drawstring,midimap[num-1]);
        //add whitespace to end of string
        strsize=strlen(drawstring);
        while(strsize<15){
            strcat(drawstring," ");
            strsize++;
        }
        draw_fast_string(10,58,PATCHLIST,BACKGROUND, drawstring);
        
        //current patch selection
        strcpy(drawstring,midimap[num]);
        //add whitespace to end of string
        strsize=strlen(drawstring);
        while(strsize<15){
            strcat(drawstring," ");
            strsize++;
        }
        draw_fast_string(10,74,ACTIVE,BACKGROUND, drawstring);
        
        //next 2 patches (reverse order)
        strcpy(drawstring,"");
        if(num<126) strcpy(drawstring,midimap[num+2]);
        //add whitespace to end of string
        strsize=strlen(drawstring);
        while(strsize<15){
            strcat(drawstring," ");
            strsize++;
        }
        draw_fast_string(10,106,PATCHLIST,BACKGROUND, drawstring);
        if(num<127) strcpy(drawstring,midimap[num+1]);
        //add whitespace to end of string
        strsize=strlen(drawstring);
        while(strsize<15){
            strcat(drawstring," ");
            strsize++;
        }
        draw_fast_string(10,90,PATCHLIST,BACKGROUND, drawstring);   
    //} else {
        //Just update the colour to show new selected patch is active and send it  
    //    strcpy(drawstring,midimap[num]);
    //    draw_fast_string(10,74,ACTIVE,BACKGROUND, drawstring);
        midipatch(num);   //send current selected patch choice 
    //    }
    }


