/*
 * File:   ccp1.c
 * Author: nishi
 *
 * Created on July 19, 2022, 11:51 AM
 */

#include <xc.h>
#include "ccp1.h"
#include "main.h"
#include "tmr1.h"

static void (*CCP1_CallBack)(void);

static void CCP1_DefaultCallBack(void) {

}

void CCP1_Initialize(void)
{
    /*
     * PWMモードで初期化
     * PPS設定はPPS_Initialize() (pps.c)でまとめて初期化
     */
    CCP1_InterruptDisable(); // PWMモードで割り込みは使わない

    /*
     * EN: Disable
     * OUT: Read-only
     * FMT: Right aligned format (PWM mode)
     * MODE: PWM Operation
     */
    CCP1CON = 0b00001100;
    
    /*
     * Pin selected by CCP1PPS
     */
    CCP1CAP = 0x00;
    
    /*
     * CCPR1[15:10]: Not used
     * CCPR1H[1:0]: Accesses the two Most Significant bits CCPR[9:8]
     * CCPR1L: Accesses the eight Least Significant bits CCPR[7:0]
     */
    CCPR1H = 0x00; // サンプルコードはMSB側が2だったけど本当?
    CCPR1L = 0x02;
    
    /*
     * PWMモードでのCCPIFは出力がHigh→Lowに切り替わるタイミング
     */
    
    CCP1_SetCallBack(CCP1_DefaultCallBack);
    CCP1_InterruptFlagClear();
    
    CCPTMRS0bits.C1TSEL = 0x01; // Timers Selection: TMR2
    T2TMR = 0x00;
    T2CONbits.ON = 0; // 先にスタートしておく
    
}

void __interrupt(irq(IRQ_CCP1),base(8),high_priority) CCP1_CaptureISR()
{
    // Clear the CCP1 interrupt flag
    CCP1_InterruptFlagClear();
    
    //tmr1Count = CCPR1;
    
    // Return 16-bit captured value
    CCP1_CallBack();
}

void CCP1_SetCallBack(void (*customCallBack)(void)){
    CCP1_CallBack = customCallBack;
}

