/*
 * File:   osc.c
 * Author: nishi
 *
 * Created on August 9, 2022, 1:31 PM
 */

#include <xc.h>

void OSC_Initialize(void)
{
    /*
     * Oscillator Module
     * 
     * Clock Source: EXTOSC 16MHz, PLL x4(CONFIG1で設定)
     * Clock Switching: なし
     * Oscillator Enable: 明示的なONはしない
     * Fail-Sage Clock Monitor: 未使用
     */
    OSCCON3 = 0x40;
    OSCTUNE = 0x00;
    OSCFRQ = 0x00;
    OSCEN = 0x00;
    
    do
    {
        // 周波数切替え待機
    } while(OSCSTATbits.EXTOR);
    
    /*
     * Active Clock Tuning is disabled
     */
    ACTCON = 0x00;
    
#if 0
    // ACT使用時
    do
    {
        // ACTロック待機
    } while(ACTCONbits.ACTLOCK);
#endif
}
