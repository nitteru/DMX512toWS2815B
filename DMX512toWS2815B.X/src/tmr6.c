/*
 * File:   tmr6.c
 * Author: Teruyuki
 *
 * Created on 2023/06/01, 11:31
 */


#include <xc.h>
#include "tmr6.h"
#include "main.h"

uint8_t tmr6_reload_val = TMR6_RELOAD_VAL;

void TMR6_Initialize(void)
{
    PIE2bits.TMR6IE = 0x00;
    PIR2bits.TMR6IF = 0x00;

    /*
     * Clock Souce: Fosc/4
     * DACデータ更新用 8kHz, 125usec周期
     */

    T6CONbits.TMR6ON = 0x00;
    T6CONbits.T6OUTPS = 0x00; // Postscaler 1:1, PWMには反映されない

    TMR6 = 0x00; // Counter
#ifdef OSC_CLK_32MHZ
    T6CONbits.T6CKPS = 0x01; // Prescaler 1:4
    //PR6 = 0xFF; // Period 64usec (7.8125kHz)
    //PR6 = 0x7F; // Period 32usec (15.625kHz)
    PR6 = 0x3F; // Period 16usec (31.25kHz)
#elif defined OSC_CLK_16MHZ
    T6CONbits.T6CKPS = 0x01; // Prescaler 1:4
    PR6 = 0x7D; // Period 125usec (8kHz)
    //PR6 = 0x7F; // Period 32usec (31.25kHz)
    //PR6 = 0x3F; // Period 16nsec (62.5kHz)
#elif defined OSC_CLK_8MHZ
    T6CONbits.T6CKPS = 0x00; // Prescaler 1:1
    T6PR = 0xFF; // Period 128usec (7.8125kHz)
    //PR6 = 0x7F; // Period 64usec (15.625kHz)
    //PR6 = 0x3F; // Period 32usec (31.25kHz)
#endif
}

void TMR6_Realod(void)
{
    TMR6 = tmr6_reload_val;
}

void TMR6_WriteTimer(uint8_t writeValue)
{
    TMR6 = writeValue;
    tmr6_reload_val = writeValue;
}

uint8_t TMR6_ReadTimer()
{
    return TMR6;
}

