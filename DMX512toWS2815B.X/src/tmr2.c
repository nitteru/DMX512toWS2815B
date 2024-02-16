/*
 * File:   tmr2.c
 * Author: nishi
 *
 * Created on July 15, 2022, 11:12 AM
 */

#include <xc.h>
#include "tmr2.h"
#include "main.h"

volatile uint8_t tmr2_reload_val = TMR2_RELOAD_VAL;

static void (*Timer2_OverflowCallback)(void);
static void Timer2_DefaultOverflowCallback(void);

void TMR2_Initialize(void)
{
    /*
     * Clock Souce: Fosc/2
     * TMR2 Period: 125usec (8kHz)
     * Postscaler 1:1
     * Timer Prescaler Output is not synchronized to FOSC/2
     *   Setting PSYNC requires that the output of the prescaler is slower than FOSC/2.
     * Rising edge
     * ON bit is not synchronized to timer clock input
     * Free-running, Period Pulse, Software gate, Timer control Start: ON=1, Stop: ON=0
     * Reset source: TxINPPS
     */

    // Set default overflow callback
    Timer2_OverflowCallbackRegister(Timer2_DefaultOverflowCallback);

    T2TMR = 0x00;
    T2HLT = 0x00;
    T2CLKCON = 0x01;
    T2RST = 0x00;
    
    T2PR = 0xFA; // Period 125usec (8kHz)
    T2CON = 0x90;  // Prescaler 1:2
    
    TMR2_InterruptFlagClear();
    TMR2_InterruptEnable();

    //T2CONbits.ON = 0x01;  // TMR ON/OFFは再生処理で制御する
}

void TMR2_Realod(void)
{
    T2TMR = tmr2_reload_val;
}

void TMR2_WriteTimer(uint8_t writeValue)
{
    T2TMR = writeValue;
    tmr2_reload_val = writeValue;
}

uint8_t TMR2_ReadTimer()
{
    return T2TMR;
}

void __interrupt(irq(IRQ_TMR2),base(8)) Timer2_ISR()
{
    // clear the TMR2 interrupt flag
     TMR2_InterruptFlagClear();

    if(Timer2_OverflowCallback)
    {
        Timer2_OverflowCallback();
    }
}

void Timer2_OverflowCallbackRegister(void (* InterruptHandler)(void)){
    Timer2_OverflowCallback = InterruptHandler;
}

static void Timer2_DefaultOverflowCallback(void){
    // add your TMR2 interrupt custom code
    // or set custom function using Timer2_OverflowCallbackRegister()

}