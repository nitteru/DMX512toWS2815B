/* 
 * File:   clkref.c
 * Author: nishi
 *
 * Created on October 6, 2023, 4:47 PM
 */

#include "clkref.h"
#include "main.h"

void CLKREF_Initialize(void)
{
    /*
     * CLKREF
     * 
     * Duty: 50%
     * Clock Source: MFINTOSC 500kHz
     * Clock Divider: 1:16, 500kHz / 16 = 31.25kHz
     */
    
    CLKRCON = 0x14;
    CLKRCLK = 0x03;
}