/*
 * File:   interrupts.c
 * Author: Teruyuki
 *
 * Created on 2023/05/19, 15:04
 */


#include <xc.h>
#include "interrupts.h"
#include "main.h"

void (*INT0_InterruptHandler)(void); // EXT_INT0割り込みハンドラの関数ポインタ

void INT0_CallBack(void)
{
    // Add your custom callback code here
    if(INT0_InterruptHandler)
    {
        INT0_InterruptHandler();
    }
}

void Interrupt_Initialize(void)
{
    INTCON0bits.IPEN = 1; // Interrupt Priority enable

    uint8_t state = (uint8_t)INTCON0bits.GIE;
    INTCON0bits.GIE = 0;
    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCKbits.IVTLOCKED = 0x00; // unlock IVT

    // IVTBASE 0x000008
    IVTBASEU = 0;
    IVTBASEH = 0;
    IVTBASEL = 8;

    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCKbits.IVTLOCKED = 0x01; // lock IVT

    INTCON0bits.GIE = state;
    
    // 割り込みフラグクリア
    // 各割り込み初期化
    // Callbackの登録
     // Clear the interrupt flag
    // Set the external interrupt edge detect
    EXT_INT0_InterruptFlagClear();   
    EXT_INT0_risingEdgeSet();    
    // Set Default Interrupt Handler
    INT0_SetInterruptHandler(INT0_DefaultInterruptHandler);
    EXT_INT0_InterruptEnable();   
}

void __interrupt(irq(default),base(8)) Default_ISR()
{
}

void __interrupt(irq(IRQ_INT0),base(8)) INT0_ISR()
{
    EXT_INT0_InterruptFlagClear();

    // Callback function gets called everytime this ISR executes
    INT0_CallBack();    
}

void INT0_SetInterruptHandler(void (* InterruptHandler)(void)){
    INT0_InterruptHandler = InterruptHandler;
}

void INT0_DefaultInterruptHandler(void){
    // add your INT0 interrupt custom code
    // or set custom function using INT0_SetInterruptHandler()

}