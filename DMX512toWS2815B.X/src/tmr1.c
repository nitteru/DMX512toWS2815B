/*
 * File:   tmr1.c
 * Author: nishi
 *
 * Created on July 15, 2022, 10:44 AM
 */

#include <xc.h>
#include "tmr1.h"
#include "main.h"

volatile uint16_t tmr1_reload_val = TMR1_RELOAD_VAL;
uint8_t tmr1OverflowFlag = 0; // TMR1オーバーフローフラグ

static void (*Timer1_OverflowCallback)(void);
static void Timer1_DefaultOverflowCallback(void);

void TMR1_Initialize(void)
{
    /*
     * Clock source: Fosc/4
     * Prescaler 1:1
     * Enables register read/write of Timer in one 16-bit operation
     * Gate Controlは使用しない
     */
    Timer1_OverflowCallbackRegister(Timer1_DefaultOverflowCallback);
    
    T1GCON = 0x00;
    T1GATE = 0x00;

    T1CLK = 0x01;

    TMR1H = (uint8_t)(tmr1_reload_val >> 8);
    TMR1L = (uint8_t)(tmr1_reload_val);
    
    T1CON = 0x03;
    
    TMR1_InterruptFlagClear();    
    TMR1_InterruptEnable(); // Enable interrupt
}

void TMR1_Realod(void)
{
    TMR1_Stop();
    TMR1H = (uint8_t)(tmr1_reload_val >> 8);
    TMR1L = (uint8_t)(tmr1_reload_val);
    TMR1_Start();
}

void TMR1_WriteTimer(uint16_t writeValue)
{
    TMR1_Stop();
    TMR1H = (uint8_t)(writeValue >> 8);
    TMR1L = (uint8_t)(writeValue);
    TMR1_Start();

    tmr1_reload_val = writeValue;
}

uint16_t TMR1_ReadTimer()
{
    return (uint16_t)((TMR1H << 8) | TMR1L);
}

void __interrupt(irq(TMR1),base(8)) Timer1_OverflowISR()
{

    // Clear the TMR1 interrupt flag
    TMR1_InterruptFlagClear();
    TMR1_Realod();
    
    if(Timer1_OverflowCallback)
    {
        Timer1_OverflowCallback();
    }
}

void Timer1_OverflowCallbackRegister(void (* CallbackHandler)(void))
{
    Timer1_OverflowCallback = CallbackHandler;
}

static void Timer1_DefaultOverflowCallback(void)
{
    //Add your interrupt code here or
    //Use Timer1_OverflowCallbackRegister function to use Custom ISR
}
