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

#define TMR0_Disable() (T0CON0bits.EN = 0)

/*
 * TMR0 Period: 10msec
 * 10msec / (62.5nsec * 4) = 40000, 65536 - 40000 = 25536 → 0x63C0 // カウンタが足りないのでPSを1:4で対応
 */

#define TMR0_RELOAD_VAL (uint16_t)0x63C0

#define TMR0_Start()       \
    do                     \
    {                      \
        T0CON0bits.EN = 1; \
    } while (0)
#define TMR0_Stop()        \
    do                     \
    {                      \
        T0CON0bits.EN = 0; \
    } while (0)

#define TMR0_InterruptFlagClear() (PIR3bits.TMR0IF = 0)
#define TMR0_InterruptEnable() (PIE3bits.TMR0IE = 1)
#define TMR0_InterruptDisable() (PIE3bits.TMR0IE = 0)

void TMR0_Initialize(void);
void TMR0_Realod(void);
void TMR0_WriteTimer(uint16_t);
uint16_t TMR0_ReadTimer();
void Timer0_OverflowCallbackRegister(void (* CallbackHandler)(void));
