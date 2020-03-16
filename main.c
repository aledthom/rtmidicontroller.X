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

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Setup Registers                                                            */
/******************************************************************************/
void init_registers(){
    //setup registers for SD0SEL SPICON TRIS OSC etc.
    
    //32MHz internal oscillator with PLL
    OSCCON=0b11110000;
    
    //RA0+RA1 - encoder1 in RA2 - lcd reset out, RA3, RA4+RA5 encoder2 in
    //jumpers on RA0 RA1 and voltage pins for icsp
    ANSELA=0b00000000;
    TRISA=0b00110011;
    
    
    //RC0 - Clock out, RC1 - SDI in, RC2 SDO - out, RC3+4 chip selects - out, RC5 - lcd cmd/data out     
    //APFCONbits.SDOSEL = 0 //should default to 0 anyway at power on
    ANSELC=0b00000000;
    TRISC=0b00000010;
            
    //SSPEN- enable, SPI clock to Fosc/4 (8MHZ)
    SSP1CON1 = 0b00100000; 
    //TODO enable ssp1 interupt,
    
}



/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void) {
    init_registers();
    lcd_init();
    //fill_rectangle(0, 0, WIDTH, HEIGHT, COLOR_BLACK);
    return;
    
}
/*    Program space        used   4ECh (  1260) of   800h words   ( 61.5%)
    Data space           used     2h (     2) of    80h bytes   (  1.6%)
    EEPROM space         used     0h (     0) of   100h bytes   (  0.0%)
    Data stack space     used     0h (     0) of    70h bytes   (  0.0%)
    Configuration bits   used     2h (     2) of     2h words   (100.0%)
    ID Location space    used     0h (     0) of     4h bytes   (  0.0) */