/*
 * File:   tmr6.c
 * Author: nishi
 *
 * Created on July 15, 2022, 11:16 AM
 */

#include <xc.h>
#include "tmr6.h"
#include "main.h"

volatile uint8_t tmr6_reload_val = TMR6_RELOAD_VAL;

static void (*Timer6_OverflowCallback)(void);
static void Timer6_DefaultOverflowCallback(void);

void TMR6_Initialize(void)
{
    /*
     * Clock Souce: Fosc/6
     * TMR6 Period: 125usec (8kHz)
     * Postscaler 1:1
     * Timer Prescaler Output is not synchronized to FOSC/6
     *   Setting PSYNC requires that the output of the prescaler is slower than FOSC/6.
     * Rising edge
     * ON bit is not synchronized to timer clock input
     * Free-running, Period Pulse, Software gate, Timer control Start: ON=1, Stop: ON=0
     * Reset source: TxINPPS
     */

    // Set default overflow callback
    Timer6_OverflowCallbackRegister(Timer6_DefaultOverflowCallback);

    T6TMR = 0x00;
    T6HLT = 0x00;
    T6CLKCON = 0x01;
    T6RST = 0x00;
    
    T6PR = 0xFA; // Period 125usec (8kHz)
    T6CON = 0x90;  // Prescaler 1:2
    
    TMR6_InterruptFlagClear();
    TMR6_InterruptEnable();

    //T6CONbits.ON = 0x01;  // TMR ON/OFFは再生処理で制御する
}

void TMR6_Realod(void)
{
    T6TMR = tmr6_reload_val;
}

void TMR6_WriteTimer(uint8_t writeValue)
{
    T6TMR = writeValue;
    tmr6_reload_val = writeValue;
}

uint8_t TMR6_ReadTimer()
{
    return T6TMR;
}

void __interrupt(irq(IRQ_TMR6),base(8)) Timer6_ISR()
{
    // clear the TMR6 interrupt flag
     TMR6_InterruptFlagClear();

    if(Timer6_OverflowCallback)
    {
        Timer6_OverflowCallback();
    }
}

void Timer6_OverflowCallbackRegister(void (* InterruptHandler)(void)){
    Timer6_OverflowCallback = InterruptHandler;
}

static void Timer6_DefaultOverflowCallback(void){
    // add your TMR6 interrupt custom code
    // or set custom function using Timer6_OverflowCallbackRegister()

}