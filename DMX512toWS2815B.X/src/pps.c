/*
 * File:   pps.c
 * Author: nishi
 *
 * Created on July 19, 2022, 1:37 PM
 */

#include <xc.h>
#include "pps.h"
#include "main.h"

void PPS_Initialize(void)
{
    /*
     * Peripheral Pin Select Module
     *
     * CONFIG2
     *   PPS1WAY = ON
     *     PPS設定はリセット後1回のみです
     *   PPS1WAY = OFF
     *     複数回PPS設定ができます
     * 
     *  # xxxPPS (Inpout)
     *   bit [7:6] 0
     *       [5:3] PORT
     *       [2:0] PIN
     * 
     *   PORT 000:PORTA, 001:PORTB, 010:PORTC
     *   PIN  000:Rx0 - 111:Rx7
     */

    INTCON0bits.GIE = 0;
    INTCON0bits.GIEL = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0; // PPS Unlock
    
    // PWM1 Output
    RC0PPS = 0x0A; // RC0: PWM1SP1 Output

    // PWM2 Output
    RC1PPS = 0x0C; // RC1: PWM2SP1 Output

    // PWM3 Output
    RC2PPS = 0x0E; // RC2: PWM3SP1 Output

    // EXTINT Input
    INT0PPS = 0x14; // EXT INT Input: RC4

    // CLC4
    RC6PPS = 0x04; // RC6: CLC4OUT
    
    // UART RX
    U1RXPPS = 0x15; // RC5: UART1 RX

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1; // PPS lock
}
