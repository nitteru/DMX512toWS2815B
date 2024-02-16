/*
 * File:   tmr4.c
 * Author: nishi
 *
 * Created on July 15, 2022, 11:14 AM
 */

#include <xc.h>
#include "tmr4.h"
#include "main.h"

volatile uint8_t tmr4_reload_val = TMR4_RELOAD_VAL;

static void (*Timer4_OverflowCallback)(void);
static void Timer4_DefaultOverflowCallback(void);

void TMR4_Initialize(void)
{
    /*
     * Clock Souce: Fosc/4
     * TMR4 Period: 125usec (8kHz)
     * Postscaler 1:1
     * Timer Prescaler Output is not synchronized to FOSC/4
     *   Setting PSYNC requires that the output of the prescaler is slower than FOSC/4.
     * Rising edge
     * ON bit is not synchronized to timer clock input
     * Free-running, Period Pulse, Software gate, Timer control Start: ON=1, Stop: ON=0
     * Reset source: TxINPPS
     */

    // Set default overflow callback
    Timer4_OverflowCallbackRegister(Timer4_DefaultOverflowCallback);

    T4TMR = 0x00;
    T4HLT = 0x00;
    T4CLKCON = 0x01;
    T4RST = 0x00;
    
    T4PR = 0xFA; // Period 125usec (8kHz)
    T4CON = 0x90;  // Prescaler 1:2
    
    TMR4_InterruptFlagClear();
    TMR4_InterruptEnable();

    //T4CONbits.ON = 0x01;  // TMR ON/OFFは再生処理で制御する
}

void TMR4_Realod(void)
{
    T4TMR = tmr4_reload_val;
}

void TMR4_WriteTimer(uint8_t writeValue)
{
    T4TMR = writeValue;
    tmr4_reload_val = writeValue;
}

uint8_t TMR4_ReadTimer()
{
    return T4TMR;
}

void __interrupt(irq(IRQ_TMR4),base(8)) Timer4_ISR()
{
    // clear the TMR4 interrupt flag
     TMR4_InterruptFlagClear();

    if(Timer4_OverflowCallback)
    {
        Timer4_OverflowCallback();
    }
}

void Timer4_OverflowCallbackRegister(void (* InterruptHandler)(void)){
    Timer4_OverflowCallback = InterruptHandler;
}

static void Timer4_DefaultOverflowCallback(void){
    // add your TMR4 interrupt custom code
    // or set custom function using Timer4_OverflowCallbackRegister()

}