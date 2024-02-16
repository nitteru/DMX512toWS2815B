/*
 * File:   pwn4.c
 * Author: nishi
 *
 * Created on July 19, 2022, 11:40 AM
 */

#include <xc.h>
#include "pwm2.h"
#include "main.h"

void PWM2_Initialize(void)
{
    PWM2CONbits.EN = 0;

    PWM2ERS = 0x00; // External Reset disable
    PWM2LDS = 0x00; // Auto-load disable
    
    PWM2CLK = 0x02; // Clock Source: FOSC DIV 1:1, 16MHz
    PWM2CPRE = 0x00; // PWM clock is prescaled by n+1 (0 + 1 = 4, 1:1), 16MHz
    
    PWM2PRH = 0x03; // 16MHz^-1 * 1024, 15.625kHz周期
    PWM2PRL = 0xFF;

    PWM2PIPOS = 0x00; // Period interrupt occurs n+1 period events, 割り込み未使用
    
    /*
     * Reload not enabled or reload complete
     * External Reset input is active-high
     * Stop counter at the end of the period. Output goes to the Inactive state
     */
    PWM2CON = 0x00;
    
    /*
     * PWMx_SaP2_out is high true
     * PWMx_SaP1_out is high true
     * PWMx Slice “a” Push-Pull mode is not enabled (プッシュプル:周期毎の交互出力)
     * Left Aligned mode
     */
    PWM2S1CFG = 0x00;
    
    // 初期デューティ設定 (この設定では10bitの範囲)
    // PWM2CONbits.ENをセットした後はLDビットをセットすること｡次のタイマーリロードで反映される｡
    PWM2S1P1H = 0x00; // Left Aligned: PWMx_SaP1_out is high n PWMx clock periods at beginning of PWMx period
    PWM2S1P1L = 0x00;
    PWM2S1P2H = 0x00; // Left Aligned: PWMx_SaP1_out is high n PWMx clock periods at beginning of PWMx period
    PWM2S1P2L = 0x00;

    /*
     * There are no PWMx period and parameter value transfers pending
     */
    PWMLOADbits.MPWM2LD = 0x00;
    
    PWMEN = 0x00; // PWM同時ON/OFFはここでは行わない
    
    PWM2SaP1_InterruptFlagClear();
    PWM2SaP2_InterruptFlagClear();
    PWM2PERIOD_InterruptFlagClear();

    PWM2SaP1_InterruptDisable(); // 割り込み未使用
    PWM2SaP2_InterruptDisable();
    PWM2_InterruptDisable();
    PWM2PERIOD_InterruptDisable();
    
    //PWM2CONbits.EN = 1; // PWMはメインループ中でON/OFFを制御
}

void PWM2_SetDuty(uint16_t dutyValue)
{
    dutyValue = 0x03FF & dutyValue;
    PWM2S1P1 = dutyValue;
    // PWM2S1P2の扱いは検討中
}

uint16_t PWM2_GetDuty(void)
{
    uint16_t ret = 0;
    ret = PWM2S1P1;

    return ret;
    // retを直接returnできる?
}

void PWM2_Start(void)
{
    PWM2CONbits.EN = 0;
    PWM2S1P1 = 0x0000;
    PWM2CONbits.EN = 1;
}

void PWM2_Stop(void)
{
    PWM2CONbits.EN = 0;
    PWM2S1P1 = 0x0000;

}

void PWM2_Restart(void)
{
    PWM2CONbits.EN = 1;
}

void PWM2_Pasue(void)
{
    PWM2CONbits.EN = 0;
}