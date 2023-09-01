/*
 * File:   tmr4.c
 * Author: Teruyuki
 *
 * Created on 2023/06/01, 11:31
 */


#include <xc.h>
#include "tmr4.h"
#include "main.h"

uint8_t tmr4_reload_val = TMR4_RELOAD_VAL;

void TMR4_Initialize(void)
{
    PIE2bits.TMR4IE = 0x00;
    PIR2bits.TMR4IF = 0x00;

    /*
     * Clock Souce: Fosc/4
     * TMR4 Period: 64usec (15.625kHz)
     * PWM3,4,5 Clock Source
     */

    T4CONbits.TMR4ON = 0x00;
    T4CONbits.T4OUTPS = 0x00; // Postscaler 1:1, PWMには反映されない

    TMR4 = 0x00; // Counter
#ifdef OSC_CLK_32MHZ
    T4CONbits.T4CKPS = 0x01; // Prescaler 1:4
    PR4 = 0xFF; // Period 64usec (7.8125kHz), Resolution 10bit
    //PR4 = 0x7F; // Period 32usec (15.625kHz), Resolution 9bit
    //PR4 = 0x3F; // Period 16usec (31.25kHz), Resolusion 8bit
#elif defined OSC_CLK_16MHZ
    T4CONbits.T4CKPS = 0x00; // Prescaler 1:1
    PR4 = 0xFF; // Period 64usec (15.625kHz), Resolution 10bit
    //PR4 = 0x7F; // Period 32usec (31.25kHz), Resolution 9bit
    //PR4 = 0x3F; // Period 16nsec (62.5kHz), Resolusion 8bit
#elif defined OSC_CLK_8MHZ
    T4CONbits.T4CKPS = 0x00; // Prescaler 1:1
    T4PR = 0xFF; // Period 128usec (7.8125kHz), Resolution 10bit
    //PR4 = 0x7F; // Period 64usec (15.625kHz), Resolution 9bit
    //PR4 = 0x3F; // Period 32usec (31.25kHz), Resolusion 8bit
#endif
}

void TMR4_Realod(void)
{
    TMR4 = tmr4_reload_val;
}

void TMR4_WriteTimer(uint8_t writeValue)
{
    TMR4 = writeValue;
    tmr4_reload_val = writeValue;
}

uint8_t TMR4_ReadTimer()
{
    return TMR4;
}

