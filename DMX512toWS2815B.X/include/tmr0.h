/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#pragma once

#include <xc.h> // include processor files - each processor file is guarded.  
#include "main.h"

/*
 * Fosc: 32MHz → 31.25nsec
 * Fosc: 16MHz → 62.5nsec
 * Fosc: 8MHz → 125nsec
 *
 * TMR0 Period: 5msec
 * ((32MHz / 4)^-1 * PS(1:256)) = 32usec, 5msec / 32usec = 156.25, 256 - 156.25 = 99.75 → 0x64
 * ((16MHz / 4)^-1 * PS(1:128)) = 32usec, 5msec / 32usec = 156.25, 256 - 156.25 = 99.75 → 0x64
 * (( 8MHz / 4)^-1 * PS(1:64)) = 32usec, 5msec / 32usec = 156.25, 256 - 156.25 = 99.75 → 0x64
 */
#ifdef OSC_CLK_32MHZ
#define TMR0_RELOAD_VAL (uint8_t)0x64
#elif defined OSC_CLK_16MHZ
#define TMR0_RELOAD_VAL (uint8_t)0x64
#elif defined OSC_CLK_8MHZ
#define TMR0_RELOAD_VAL (uint8_t)0x64
#endif

#define TMR0InterruptEnable() (INTCONbits.T0IE = 0x01)
#define TMR0InterruptDisable() (INTCONbits.T0IE = 0x00)
#define TMR0InterruptFlagClear() (INTCONbits.T0IF = 0x00)

extern uint8_t tmr0_reload_val;

void TMR0_Initialize(void);
void TMR0_Realod(void);
void TMR0_WriteTimer(uint8_t);
uint8_t TMR0_ReadTimer();
