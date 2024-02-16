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

// レジスタはTimer1,3共通
#define TMR1_RELOAD_VAL (uint16_t)0x0000

#define TMR1_Start()      \
    do                    \
    {                     \
        T1CONbits.ON = 1; \
    } while (0)
#define TMR1_Stop()       \
    do                    \
    {                     \
        T1CONbits.ON = 0; \
    } while (0)

#define TMR1_InterruptEnable() (PIE3bits.TMR1IE = 0x01)
#define TMR1_InterruptDisable() (PIE3bits.TMR1IE = 0x00)
#define TMR1_InterruptFlagClear() (PIR3bits.TMR1IF = 0x00)
#define Timer1_HasOverflowOccured() (PIR3bits.TMR1IF)

extern uint8_t tmr1OverflowFlag;

void TMR1_Initialize(void);
void TMR1_Realod(void);
void TMR1_WriteTimer(uint16_t);
uint16_t TMR1_ReadTimer();
void Timer1_OverflowCallbackRegister(void (* CallbackHandler)(void));