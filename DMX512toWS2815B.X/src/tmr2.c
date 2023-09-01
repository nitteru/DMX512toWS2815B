/*
 * File:   tmr2.c
 * Author: Teruyuki
 *
 * Created on 2023/06/01, 11:31
 */


#include <xc.h>
#include "tmr2.h"
#include "main.h"

uint8_t tmr2_reload_val = TMR2_RELOAD_VAL;

void TMR2_Initialize(void)
{
    PIE1bits.TMR2IE = 0x00;
    PIR1bits.TMR2IF = 0x00;

    /*
     * Clock Souce: Fosc/4
     * TMR2 Period: 64usec (15.625kHz)
     * PWM3,4,5 Clock Source
     */

    T2CONbits.TMR2ON = 0x00;
    T2CONbits.T2OUTPS = 0x00; // Postscaler 1:1, PWMには反映されない

    TMR2 = 0x00; // Counter
#ifdef OSC_CLK_32MHZ
    T2CONbits.T2CKPS = 0x01; // Prescaler 1:4
    //PR2 = 0xFF; // Period 64usec (7.8125kHz), Resolution 10bit
    //PR2 = 0x7F; // Period 32usec (15.625kHz), Resolution 9bit
    PR2 = 0x3F; // Period 16usec (31.25kHz), Resolusion 8bit
#elif defined OSC_CLK_16MHZ
    T2CONbits.T2CKPS = 0x00; // Prescaler 1:1
    //PR2 = 0xFF; // Period 64usec (15.625kHz), Resolution 10bit
    PR2 = 0x7F; // Period 32usec (31.25kHz), Resolution 9bit
    //PR2 = 0x3F; // Period 16nsec (62.5kHz), Resolusion 8bit
#elif defined OSC_CLK_8MHZ
    T2CONbits.T2CKPS = 0x00; // Prescaler 1:1
    T2PR = 0xFF; // Period 128usec (7.8125kHz), Resolution 10bit
    //PR2 = 0x7F; // Period 64usec (15.625kHz), Resolution 9bit
    //PR2 = 0x3F; // Period 32usec (31.25kHz), Resolusion 8bit
#endif
}

void TMR2_Realod(void)
{
    TMR2 = tmr2_reload_val;
}

void TMR2_WriteTimer(uint8_t writeValue)
{
    TMR2 = writeValue;
    tmr2_reload_val = writeValue;
}

uint8_t TMR2_ReadTimer()
{
    return TMR2;
}

