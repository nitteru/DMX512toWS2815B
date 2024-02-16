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

/*
 * PWM Interrupt (PWMxIF): SaP1IF, SaP2IFのOR, PWMxIFは手動ではクリアできない｡各スライスのフラグをクリアすること｡
 * PWM Period Interrupt (PWMxPIF): PWM周期毎(PWMx_timerとPRが一致した時), ポストスケーラ有
 * SaPxIF: 各スライスの割り込み｡タイミングは各モードによる｡
 */

#define PWM1_InterruptEnable() (PIE4bits.PWM1IE = 1)
#define PWM1_InterruptDisable() (PIE4bits.PWM1IE = 0)
#define PWM1PERIOD_InterruptEnable() (PIE4bits.PWM1PIE = 1)
#define PWM1PERIOD_InterruptDisable() (PIE4bits.PWM1PIE = 0)
#define PWM1PERIOD_InterruptFlagClear() (PIR4bits.PWM1PIF = 0)
#define PWM1SaP1_InterruptEnable() (PWM1GIEbits.S1P1IE = 1)
#define PWM1SaP1_InterruptDisable() (PWM1GIEbits.S1P1IE = 0)
#define PWM1SaP2_InterruptEnable() (PWM1GIEbits.S1P2IE = 1)
#define PWM1SaP2_InterruptDisable() (PWM1GIEbits.S1P2IE = 0)
#define PWM1SaP1_InterruptFlagClear() (PWM1GIRbits.S1P1IF = 0)
#define PWM1SaP2_InterruptFlagClear() (PWM1GIRbits.S1P2IF = 0)

#define PWM1_LoadBufferRegisters() (PWMLOADbits.MPWM1LD = 1)

void PWM1_Initialize(void);
void PWM1_SetDuty(uint16_t);
uint16_t PWM1_GetDuty(void);
void PWM1_Start(void);
void PWM1_Stop(void);
void PWM1_Restart(void);
void PWM1_Pasue(void);
