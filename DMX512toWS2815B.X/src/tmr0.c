/*
 * File:   tmr0.c
 * Author: nishi
 *
 * Created on July 14, 2022, 4:41 PM
 */

#include <xc.h>
#include "tmr0.h"
#include "main.h"

volatile uint16_t tmr0_reload_val = TMR0_RELOAD_VAL;

static void (*Timer0_OverflowCallback)(void);
static void Timer0_DefaultOverflowCallback(void);

void TMR0_Initialize(void)
{

    /*
     * カウント時間: 10msec
     * クロックソース: Focs/4
     * TMR0 is synchronized to Fosc/4
     * TMR0 is an 16-bit timer
     * 
     * 65536 - (10msec / ((Fosc/4)-^1 * Prescaler * Postscaler))
     * = 65536 - 40000 = 25536 = 0x63C0
     */
    T0CON1bits.CKPS = 0x02; // Clock source select FOSC/4
    T0CON1bits.ASYNC = 0x00; // The input to the TMR0 counter is synchronized to Fosc/4
    T0CON1bits.CKPS = 0x01; // Prescaler 1:2
    
    TMR0H = (uint8_t)(tmr0_reload_val >> 8);
    TMR0L = (uint8_t)(tmr0_reload_val);
    
    Timer0_OverflowCallbackRegister(Timer0_DefaultOverflowCallback);
    
    T0CON0bits.MD16 = 0x01; // 16bit timer
    T0CON0bits.OUTPS = 0x01; // Postscaler 1:2
    T0CON0bits.EN = 0;
    
    TMR0_InterruptFlagClear();
    TMR0_InterruptEnable();
}

void TMR0_Realod(void)
{
    T0CON0bits.EN = 0;
    TMR0H = (uint8_t)(tmr0_reload_val >> 8);
    TMR0L = (uint8_t)(tmr0_reload_val);
    T0CON0bits.EN = 1;
}

void TMR0_WriteTimer(uint16_t writeValue)
{
    T0CON0bits.EN = 0;
    TMR0H = (uint8_t)(writeValue >> 8);
    TMR0L = (uint8_t)(writeValue);
    T0CON0bits.EN = 1;

    tmr0_reload_val = writeValue;
}

uint16_t TMR0_ReadTimer()
{
    return (uint16_t)((TMR0H << 8) | TMR0L);
}


void __interrupt(irq(IRQ_TMR0),base(8),high_priority) Timer0_OverflowISR()
{
    //Clear the TMR0 interrupt flag
    TMR0_InterruptFlagClear();
    TMR0H = (uint8_t)(tmr0_reload_val >> 8);
    TMR0L = (uint8_t)(tmr0_reload_val);
    
    if(Timer0_OverflowCallback)
    {
        Timer0_OverflowCallback();
    }
}


void Timer0_OverflowCallbackRegister(void (* CallbackHandler)(void))
{
    Timer0_OverflowCallback = CallbackHandler;
}

static void Timer0_DefaultOverflowCallback(void)
{
    //Add your interrupt code here or
    //Use Timer0_OverflowCallbackRegister function to use Custom ISR
    intvTmrFlg.fields.flag10msec = 0x01;
}