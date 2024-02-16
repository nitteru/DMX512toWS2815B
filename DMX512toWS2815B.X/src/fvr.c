/*
 * File:   fvr.c
 * Author: nishi
 *
 * Created on July 19, 2022, 4:45 PM
 */

#include <xc.h>
#include "fvr.h"
#include "main.h"

void FVR_Initialize(void)
{
    /*
     * Temperature Indicator is disabled
     * VOUT = 2VT (Low Range)
     * FVR Buffer 2 Gain is 2x, (2.048V)
     * FVR Buffer 1 Gain is 2x, (2.048V)
     */
    FVRCON = 0x8A;
    while (FVRCONbits.RDY == 0)
    {
        // FVR安定待ち
    }
}

void FVR_Enable(void)
{
    FVRCONbits.EN = 1;
    while (FVRCONbits.RDY == 0)
    {
        // FVR安定待ち
    }
}