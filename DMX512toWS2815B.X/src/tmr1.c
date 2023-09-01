/*
 * File:   tmr1.c
 * Author: Teruyuki
 *
 * Created on 2023/06/01, 14:29
 */


#include <xc.h>
#include "tmr1.h"

uint16_t tmr1_reload_val = TMR1_RELOAD_VAL;

void TMR1_Initialize(void) {

    /*
     * 各クロックでのCapture分解能 (Prescaler 1:1)
     * 
     * 32MHz: (32MHz / 4 / 1)^-1 = 125nsec
     * 16MHz: (16MHz / 4 / 1)^-1 = 250nsec
     *  8MHz: ( 8MHz / 4 / 1)^-1 = 500nsec
     * 
     * 2usec分解能を達成するためにPSを1:8設定とする
     */

    T1CONbits.TMR1ON = 0x00;
    PIE1bits.TMR1IE = 0x00;
    PIE1bits.TMR1GIE = 0x00;
    PIR1bits.TMR1IF = 0x00;
    PIR1bits.TMR1GIF = 0x00;
    
    T1GCON = 0x00;
    
    T1CONbits.TMR1CS = 0x00; // Timer1 clock source is instruction clock (FOSC/4)
    T1CONbits.T1CKPS = 0x03; // 1:8 Prescale value
    T1CONbits.T1OSCEN = 0x00; // Dedicated secondary oscillator circuit disabled
    T1CONbits.nT1SYNC = 0x00; // Synchronize asynchronous clock input with system clock (FOSC)
}

void TMR1_Realod(void)
{
    TMR1L = (uint8_t)tmr1_reload_val;
    TMR1H = (uint8_t)(tmr1_reload_val >> 8);
}

void TMR1_WriteTimer(uint16_t writeValue)
{
    tmr1_reload_val = writeValue;
    TMR1L = (uint8_t)tmr1_reload_val;
    TMR1H = (uint8_t)(tmr1_reload_val >> 8);
}

uint16_t TMR1_ReadTimer(void)
{
    return (uint16_t)(((uint16_t)TMR1H << 8) | (uint16_t)TMR1L);
}