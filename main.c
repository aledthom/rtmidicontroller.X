/*
 * File:   main.c
 * Author: aled
 *
 * Created on 14 March 2020, 16:40
 */


/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */


#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "ILI9225.h"
#include "mytypes.h"        /* type declarations */
#include "controls.h"
#include "ui.h"


/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

char tmr_count; //counter for TMR 
char volume;    //midi volume
char patch;     //current midi patch
bool update_ui;
//char new_patch; //selected patch waiting to be activated

/******************************************************************************/
/* Setup Registers                                                            */
/******************************************************************************/
void init_registers(){
    //setup registers for SD0SEL SPICON TRIS OSC etc.
    
    //32MHz internal oscillator with PLL
    OSCCON=0b11110000;
    
    //WPUx latched pull-ups, setup TMR0 prescale 1:256 and clock source off)
    OPTION_REG = 0b00110111;
    
    //RA0+RA1 - encoder1 in RA2 - midi Chip select, RA3-mclr/vpp, RA4+RA5 encoder2 in
    //jumpers on RA0 RA1 and voltage pins for icsp
    ANSELA=0x00;
    TRISA=0b00110011;
    //TRISA=0x00;
    WPUA=0b00110011;
    
    
    //RC0 - Clock out, RC1 - SDI in, RC2 - SDO- out, RC3-lcd cmd/data out, RC4,chip selects - lcd, RC5 - lcd reset     
    //APFCONbits.SDOSEL = 0 //should default to 0 anyway at power on
    ANSELC=0x00;
    TRISC=0x00; //no inputs needed
    TRISCbits.TRISC1=1;
            
    //SSPEN- enable, SPI clock Idle high, to Fosc/4 
    //TODO Fosc/4 too fast for vs1103b? SSP1CON1 = 0b00010000;
    SSP1CON1 = 0b00010000;//
    SSPSTAT &= 0x3F;
    //TODO remove this SSPCON1bits.CKP=1 for LCD
    SSPCON1bits.CKP=0;
    //TODO DEBUG change back to SSPSTATbits.CKE = 0;//for lcd
    SSPSTATbits.CKE = 1;
    APFCON0bits.SDOSEL = 0;
    PIE1bits.SSP1IE = 1;    //enable ssp interrupt
    INTCONbits.PEIE = 1;    //enable peripheral interrupts
    SSP1CON1bits.SSPEN =1;  //enable serial peripherals
    //ei();
    LATA2=1;
    
    
    
}

/******************************************************************************/
/* Interrupt routine                                                          */
/******************************************************************************/
// ISR (TMR interrupt for patch change selection)
void __interrupt() isr(void){
    // TMR0 with 1:256 prescaler, preload of 6 gives 125Hz interrupt frequency
    // 100 counts for ~0.8 seconds (allow some cycles for isr handling?)
    
    /* Determine which flag generated the interrupt */
    if(INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF; /* Clear Interrupt Flag */
        if(tmr_count == 100) patch_tmr_done();
        tmr_count++;
    }
}

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void) {
    //initial values
    
    volume=INITIAL_VOLUME;
    patch=INITIAL_PATCH;
    update_ui=false;
    
    char changed;
    
    init_registers();
    lcd_init();
    init_ui();
    
    
//    //get initial encoder values
//    encoder_inputs new_encoders;
//    encoder_inputs old_encoders;
//    old_encoders.value = (PORTA & 0xF); //keep first 4 bits only
//    
    
    char encoder1=0, encoder2=0;
    
    
    /*Main loop*/
    while(1){
        //check if screen needs to be updated (new patch selected)
//        if(update_ui==true) {
//            draw_patch(patch,1);
//            update_ui=false;
//        }
    
        
        //delay_us(100);
        
        
    /*encoder test v2 
     * shift bits left 2 at a time and add new encoder reading
     * valid movements are at 11111010 and 00000101 - clockwise
     * and                    00001010 and 11110101 - anti clockwise
     * 
     */
        
        //check encoder 1
          changed=PORTA & 0b00000011;
          encoder1=(encoder1<<2)|changed;
          switch(encoder1){
            case 0xFA:volume_change(UP);break;
            case 0x05:volume_change(UP);break;
            case 0xF5:volume_change(DOWN);break;
            case 0x0A:volume_change(DOWN);break;   
        }
        
        //check encoder 2
        changed= (PORTA  >>4) & 0b00000011;
        encoder2=(encoder2<<2)|changed;
        switch(encoder2){
            case 0xFA:patch_change(UP);break;
            case 0x05:patch_change(UP);break;
            case 0xF5:patch_change(DOWN);break;
            case 0x0A:patch_change(DOWN);break;            
        }
        }
        
     
//        
//        /*test rotary encoder inputs on RA0-3*/
//        //check if input has changed
//        new_encoders.value = (PORTA & 0xF);           //keep first 4 bits only
//        changed = new_encoders.value ^ old_encoders.value;  //XOR leaves only bits which changed
//        
//        // If the encoders have changed we need to work out which direction they turned
//        // logic; if newA == oldB, encoder turned clockwise
//        // if newA != oldB, encoder turned counterclockwise
//        // example; (AB) 00 01 11 10 00 01 11 10 00
//        if( changed ){
//            if(changed >= 4){
//                //encoder2 (patch selection) changed pins 2 and 3
//                if(new_encoders.enc2_2 == old_encoders.enc2_1) patch_change(UP);
//                else patch_change(DOWN);
//            }else{
//                //encoder1 volume) changed pins 0 and 1
//                if(new_encoders.enc1_2 == old_encoders.enc1_1) volume_change(UP);
//                else volume_change(DOWN);
//            }
//            old_encoders = new_encoders;   // save for next time
//        }
        
//find which encoder changed
// Value changes up or down as encoder is turned.
// encoder 2 outputs are PORTB.F1 and PORTB.F0
// logic; if newA == oldB, encoder turned clockwise
// if newA != oldB, encoder turned counterclockwise
// example; (AB) 00 01 11 10 00 01 11 10 00
   
    //}
    //return;
}