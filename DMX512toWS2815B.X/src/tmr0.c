/*
 * File:   tmr0.c
 * Author: Teruyuki
 *
 * Created on 2023/05/30, 14:25
 */


#include <xc.h>
#include "tmr0.h"

uint8_t tmr0_reload_val = TMR0_RELOAD_VAL;

void TMR0_Initialize(void)
{
    INTCONbits.T0IE = 0x00;

    OPTION_REGbits.T0SE = 0x01; // Icnrement on high-to-low transition on T0CKI pin
    OPTION_REGbits.T0CS = 0x00; // Clock source is Fosc/4
    OPTION_REGbits.PSA = 0x00; // Prescaler is assigned to the Timer0 module
    
    // PSはWDTのものと独立
#ifdef OSC_CLK_32MHZ
    OPTION_REGbits.PS = 0x07; // PS 1:256
#elif defined OSC_CLK_16MHZ
    OPTION_REGbits.PS = 0x06; // PS 1:128
#elif defined OSC_CLK_8MHZ
    OPTION_REGbits.PS = 0x05; // PS 1:64
#endif
    
    TMR0 = tmr0_reload_val;
    //INTCONbits.T0IE = 0x01; // Enable interrupt
}

void TMR0_Realod(void)
{
    uint8_t gieMode = INTCONbits.T0IE;
    INTCONbits.T0IE = 0;
    TMR0 = tmr0_reload_val;
    INTCONbits.T0IE = gieMode;
}

void TMR0_WriteTimer(uint8_t writeValue)
{
    uint8_t gieMode = INTCONbits.T0IE;
    INTCONbits.T0IE = 0;
    TMR0 = writeValue;
    INTCONbits.T0IE = gieMode;

    tmr0_reload_val = writeValue;
}

uint8_t TMR0_ReadTimer()
{
    return TMR0;
}
