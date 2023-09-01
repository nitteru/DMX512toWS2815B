/*
 * File:   wdt.c
 * Author: Teruyuki
 *
 * Created on 2023/05/19, 14:53
 */


#include <xc.h>

void WDT_Initialze(void) {
    WDTCONbits.SWDTEN = 0x01; // WDT is turned on, WDTEN<1:0> = 01
    WDTCONbits.WDTPS = 0x0B; // 1:65536 (2sec)
}
