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

// レジスタはTimer2,4,6共通

#define TMR2_RELOAD_VAL (uint8_t)0x00

#define TMR2_Start()      \
    do                    \
    {                     \
        T2CONbits.ON = 1; \
    } while (0)
#define TMR2_Stop()       \
    do                    \
    {                     \
        T2CONbits.ON = 0; \
    } while (0)

#define TMR2_InterruptEnable() (PIE3bits.TMR2IE = 0x01)
#define TMR2_InterruptDisable() (PIE3bits.TMR2IE = 0x00)
#define TMR2_InterruptFlagClear() (PIR3bits.TMR2IF = 0x00)

void TMR2_Initialize(void);
void TMR2_Realod(void);
void TMR2_WriteTimer(uint8_t);
uint8_t TMR2_ReadTimer();
void Timer2_OverflowCallbackRegister(void (* InterruptHandler)(void));