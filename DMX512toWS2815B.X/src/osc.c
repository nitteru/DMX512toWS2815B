/*
 * File:   osc.c
 * Author: Teruyuki
 *
 * Created on 2023/05/19, 13:34
 */


#include <xc.h>
#include "osc.h"
#include "main.h"

void OSC_Initialize(void) {
#ifdef OSC_CLK_32MHZ
    /*
     * 32MHz設定
     * INTOSC 8Mhz * 4xPLL
     */
    OSCCONbits.SCS = 0x00; // defined in CONFIG1 and SCS set to "00" to use 4xPLL with internal oscillator
    OSCCONbits.IRCF = 0x0E; // 8MHz or 32MHz
    while(!OSCSTATbits.HFIOFR)
    {
        // HFINTOSC準備完了待ち
    }
    while(!OSCSTATbits.HFIOFS)
    {
        // HFINTOSC安定(0.5%)待ち
    }

    OSCCONbits.SPLLEN = 1; // 4x PLL enabled (PLLEN in CONFIG1 = 0)
    while(!OSCSTATbits.PLLR)
    {
        // PLL安定待ち
    }
#elif defined OSC_CLK_16MHZ
    /*
     * 16MHz設定
     * INTOSC 16MHz
     */
    OSCCONbits.SPLLEN = 0; // 4x PLL disabled (PLLEN in CONFIG1 = 0)
    OSCCONbits.SCS = 0x00; // defined in CONFIG1
    OSCCONbits.IRCF = 0x0F; // 16MHz
    while(!OSCSTATbits.HFIOFR)
    {
        // HFINTOSC準備完了待ち
    }
    while(!OSCSTATbits.HFIOFS)
    {
        // HFINTOSC安定(0.5%)待ち
    }
#elif defined OSC_CLK_8MHZ
    /*
     * 8MHz設定
     * INTOSC 8MHz
     */
    OSCCONbits.SPLLEN = 0; // 4x PLL disabled (PLLEN in CONFIG1 = 0)
    OSCCONbits.SCS = 0x00; // defined in CONFIG1
    OSCCONbits.IRCF = 0x0E; // 8MHz
    while(!OSCSTATbits.HFIOFR)
    {
        // HFINTOSC準備完了待ち
    }
    while(!OSCSTATbits.HFIOFS)
    {
        // HFINTOSC安定(0.5%)待ち
    }
#endif
}