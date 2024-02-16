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

#define INTERRUPT_GlobalInterruptHighEnable() (INTCON0bits.GIE = 1)
#define INTERRUPT_GlobalInterruptHighDisable() (INTCON0bits.GIE = 0)
#define INTERRUPT_GlobalInterruptLowEnable() (INTCON0bits.GIEL = 1)
#define INTERRUPT_GlobalInterruptLowDisable() (INTCON0bits.GIEL = 0)

#define EXT_INT0_GetInterruptFlag() (PIR1bits.INT0IF)
#define EXT_INT0_InterruptFlagClear() (PIR1bits.INT0IF = 0)
#define EXT_INT0_InterruptEnable() (PIE1bits.INT0IE = 1)
#define EXT_INT0_InterruptDisable() (PIE1bits.INT0IE = 0)
#define EXT_INT0_risingEdgeSet() (INTCON0bits.INT0EDG = 1)
#define EXT_INT0_fallingEdgeSet() (INTCON0bits.INT0EDG = 0)

void Interrupt_Initialize(void);
void INT0_SetInterruptHandler(void (* InterruptHandler)(void));
void INT0_DefaultInterruptHandler(void);