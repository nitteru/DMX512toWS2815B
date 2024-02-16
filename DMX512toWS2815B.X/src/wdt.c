/*
 * File:   wdt.c
 * Author: nishi
 *
 * Created on July 19, 2022, 4:54 PM
 */

#include <xc.h>
#include "main.h"

void WDT_Initialize(void)
{
    /*
     * Watch Dog Timer
     * 
     * Clock Source: LFINTOSC 31kHz
     * PreScaler: 1:8388608, 256sec
     * Window Delay: 100%オープン(窓未使用)
     * Note: CONFIG設定も併せて確認すること
     */
    //WDTCON0 = 0x17; // 2sec
    WDTCON0 = 0x25; // 256sec
    WDTCON1 = 0x07;
    
    PCON0bits.NOT_WDTWV = 1;
    PCON0bits.NOT_RWDT = 1;
}
