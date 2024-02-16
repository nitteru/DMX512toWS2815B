/*
 * File:   pwn1.c
 * Author: nishi
 *
 * Created on July 19, 2022, 11:40 AM
 */

#include <xc.h>
#include "pwm1.h"
#include "main.h"

void PWM1_Initialize(void)
{
    PWM1CONbits.EN = 0;

    PWM1ERS = 0x00; // External Reset disable
    PWM1LDS = 0x00; // Auto-load disable
    
    PWM1CLK = 0x02; // Clock Source: FOSC DIV 1:1, 16MHz
    PWM1CPRE = 0x00; // PWM clock is prescaled by n+1 (0 + 1 = 4, 1:1), 16MHz
    
    PWM1PRH = 0x03; // 16MHz^-1 * 1024, 15.625kHz周期
    PWM1PRL = 0xFF;

    PWM1PIPOS = 0x00; // Period interrupt occurs n+1 period events, 割り込み未使用
    
    /*
     * Reload not enabled or reload complete
     * External Reset input is active-high
     * Stop counter at the end of the period. Output goes to the Inactive state
     */
    PWM1CON = 0x00;
    
    /*
     * PWMx_SaP2_out is high true
     * PWMx_SaP1_out is high true
     * PWMx Slice “a” Push-Pull mode is not enabled (プッシュプル:周期毎の交互出力)
     * Left Aligned mode
     */
    PWM1S1CFG = 0x00;
    
    // 初期デューティ設定 (この設定では10bitの範囲)
    // PWM1CONbits.ENをセットした後はLDビットをセットすること｡次のタイマーリロードで反映される｡
    PWM1S1P1H = 0x00; // Left Aligned: PWMx_SaP1_out is high n PWMx clock periods at beginning of PWMx period
    PWM1S1P1L = 0x00;
    PWM1S1P2H = 0x00; // Left Aligned: PWMx_SaP1_out is high n PWMx clock periods at beginning of PWMx period
    PWM1S1P2L = 0x00;

    /*
     * There are no PWMx period and parameter value transfers pending
     */
    PWMLOADbits.MPWM1LD = 0x00;
    
    PWMEN = 0x00; // PWM同時ON/OFFはここでは行わない
    
    PWM1SaP1_InterruptFlagClear();
    PWM1SaP2_InterruptFlagClear();
    PWM1PERIOD_InterruptFlagClear();

    PWM1SaP1_InterruptDisable(); // 割り込み未使用
    PWM1SaP2_InterruptDisable();
    PWM1_InterruptDisable();
    PWM1PERIOD_InterruptDisable();
    
    //PWM1CONbits.EN = 1; // PWMはメインループ中でON/OFFを制御
}

void PWM1_SetDuty(uint16_t dutyValue)
{
    dutyValue = 0x03FF & dutyValue;
    PWM1S1P1 = dutyValue;
    // PWM1S1P2の扱いは検討中
}

uint16_t PWM1_GetDuty(void)
{
    uint16_t ret = 0;
    ret = PWM1S1P1;

    return ret;
    // retを直接returnできる?
}

void PWM1_Start(void)
{
    PWM1CONbits.EN = 0;
    PWM1S1P1 = 0x0000;
    PWM1CONbits.EN = 1;
}

void PWM1_Stop(void)
{
    PWM1CONbits.EN = 0;
    PWM1S1P1 = 0x0000;

}

void PWM1_Restart(void)
{
    PWM1CONbits.EN = 1;
}

void PWM1_Pasue(void)
{
    PWM1CONbits.EN = 0;
}