/*
 *
 * File:   pwn5.c
 * Author: nishi
 *
 * Created on July 19, 2022, 11:40 AM
 */

#include <xc.h>
#include "pwm3.h"
#include "main.h"

void PWM3_Initialize(void)
{
    PWM3CONbits.EN = 0;

    PWM3ERS = 0x00; // External Reset disable
    PWM3LDS = 0x00; // Auto-load disable
    
    PWM3CLK = 0x02; // Clock Source: FOSC DIV 1:1, 16MHz
    PWM3CPRE = 0x00; // PWM clock is prescaled by n+1 (0 + 1 = 4, 1:1), 16MHz
    
    PWM3PRH = 0x03; // 16MHz^-1 * 1024, 15.625kHz周期
    PWM3PRL = 0xFF;

    PWM3PIPOS = 0x00; // Period interrupt occurs n+1 period events, 割り込み未使用
    
    /*
     * Reload not enabled or reload complete
     * External Reset input is active-high
     * Stop counter at the end of the period. Output goes to the Inactive state
     */
    PWM3CON = 0x00;
    
    /*
     * PWMx_SaP2_out is high true
     * PWMx_SaP1_out is high true
     * PWMx Slice “a” Push-Pull mode is not enabled (プッシュプル:周期毎の交互出力)
     * Left Aligned mode
     */
    PWM3S1CFG = 0x00;
    
    // 初期デューティ設定 (この設定では10bitの範囲)
    // PWM3CONbits.ENをセットした後はLDビットをセットすること｡次のタイマーリロードで反映される｡
    PWM3S1P1H = 0x00; // Left Aligned: PWMx_SaP1_out is high n PWMx clock periods at beginning of PWMx period
    PWM3S1P1L = 0x00;
    PWM3S1P2H = 0x00; // Left Aligned: PWMx_SaP1_out is high n PWMx clock periods at beginning of PWMx period
    PWM3S1P2L = 0x00;

    /*
     * There are no PWMx period and parameter value transfers pending
     */
    PWMLOADbits.MPWM3LD = 0x00;
    
    PWMEN = 0x00; // PWM同時ON/OFFはここでは行わない
    
    PWM3SaP1_InterruptFlagClear();
    PWM3SaP2_InterruptFlagClear();
    PWM3PERIOD_InterruptFlagClear();

    PWM3SaP1_InterruptDisable(); // 割り込み未使用
    PWM3SaP2_InterruptDisable();
    PWM3_InterruptDisable();
    PWM3PERIOD_InterruptDisable();
    
    //PWM3CONbits.EN = 1; // PWMはメインループ中でON/OFFを制御
}

void PWM3_SetDuty(uint16_t dutyValue)
{
    dutyValue = 0x03FF & dutyValue;
    PWM3S1P1 = dutyValue;
    // PWM3S1P2の扱いは検討中
}

uint16_t PWM3_GetDuty(void)
{
    uint16_t ret = 0;
    ret = PWM3S1P1;

    return ret;
    // retを直接returnできる?
}

void PWM3_Start(void)
{
    PWM3CONbits.EN = 0;
    PWM3S1P1 = 0x0000;
    PWM3CONbits.EN = 1;
}

void PWM3_Stop(void)
{
    PWM3CONbits.EN = 0;
    PWM3S1P1 = 0x0000;

}

void PWM3_Restart(void)
{
    PWM3CONbits.EN = 1;
}

void PWM3_Pasue(void)
{
    PWM3CONbits.EN = 0;
}