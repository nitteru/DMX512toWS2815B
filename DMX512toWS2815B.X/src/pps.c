/*
 * File:   pps.c
 * Author: Teruyuki
 *
 * Created on 2023/05/29, 17:20
 */


#include <xc.h>

void PPS_Initialize(void) {
    uint8_t gieMode = INTCONbits.GIE;
    
    INTCONbits.GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0; // PPS Unlock
    
    // Peripheral input
    INTPPS = 0x15; // EXT_INT input is RC5
    CCP1PPS = 0x15; // RC5
    RXPPS = 0x13; // RC3
    SSPCLKPPS = 0x10; // RC0
    SSPDATPPS = 0x11; // RC1
    
    // Output source
    //RA1PPS = 0x00; // RA1 output source is LATA1
    RA5PPS = 0x0F; // PWM4OUT
    RA4PPS = 0x0E; // PWM3OUT
    RC4PPS = 0x0D; // CCP2 PWM
    //RC4PPS = 0x00; // LATC4
    RC2PPS = 0x14; // TX
    RC1PPS = 0x11; // SDA
    RC0PPS = 0x10; // SCL
    
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1; // PPS lock
    INTCONbits.GIE = gieMode;
}