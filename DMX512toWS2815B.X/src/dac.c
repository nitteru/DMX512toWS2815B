/*
 * File:   dac.c
 * Author: nishi
 *
 * Created on 2023/05/11, 16:10
 */


#include <xc.h>
#include "dac.h"
#include "main.h"

void DAC_Initialize(void)
{
    /*
     * DAC Negative Reference Selection: Vss
     * DAC Positive Reference Selection: FVR2
     * DAC1OUT1 is enabled on pin RA0 only
     */
    DAC1CON = 0x18;    
    DAC1DATL = 0x00;
}