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
 * RA0, ADC, BATVOL
 */

/*
 * RA1, GPIO Output, PWELED
 */
static inline void PWELED_SetHigh(void);
static inline void PWELED_SetLow(void);
static inline void PWELED_Toggle(void);
static inline uint8_t PWELED_GetValue(void);

/*
 * RA2, GPIO Output, SIGSTS
 */
static inline void SIGSTS_SetHigh(void);
static inline void SIGSTS_SetLow(void);
static inline void SIGSTS_Toggle(void);
static inline uint8_t SIGSTS_GetValue(void);

/*
 * RA3, GPIO Output, ISOPSEN
 */
static inline void ISOPSEN_SetHigh(void);
static inline void ISOPSEN_SetLow(void);
static inline void ISOPSEN_Toggle(void);
static inline uint8_t SIGSTS_GetValue(void);

/*
 * RA4, GPIO input, RSTSW
 */
static inline void RSTSW_SetHigh(void);
static inline void RSTSW_SetLow(void);
static inline void RSTSW_Toggle(void);
static inline uint8_t RSTSW_GetValue(void);

/*
 * RA5, GPIO output, DMXOUTEN
 */
static inline void DMXOUTEN_SetHigh(void);
static inline void DMXOUTEN_SetLow(void);
static inline void DMXOUTEN_Toggle(void);
static inline uint8_t DMXOUTEN_GetValue(void);

/*
 * RA6, GPIO input, PSMODESW
 */
static inline void PSMODESW_SetHigh(void);
static inline void PSMODESW_SetLow(void);
static inline void PSMODESW_Toggle(void);
static inline uint8_t PSMODESW_GetValue(void);

/*
 * RA7, OSC input, External Xtal
 */


/*
 * RB0, GPIO Output, OSIGP
 */
static inline void OSIGP_SetHigh(void);
static inline void OSIGP_SetLow(void);
static inline void OSIGP_Toggle(void);
static inline uint8_t OSIGP_GetValue(void);

/*
 * RB1, GPIO Output, OSIGN
 */
static inline void OSIGN_SetHigh(void);
static inline void OSIGN_SetLow(void);
static inline void OSIGN_Toggle(void);
static inline uint8_t OSIGN_GetValue(void);

/*
 * RB2, GPIO Output, PSOUTEN
 */
static inline void PSOUTEN_SetHigh(void);
static inline void PSOUTEN_SetLow(void);
static inline void PSOUTEN_Toggle(void);
static inline uint8_t PSOUTEN_GetValue(void);

/*
 * RB3, GPIO Output, TP1
 */
static inline void TP1_SetHigh(void);
static inline void TP1_SetLow(void);
static inline void TP1_Toggle(void);
static inline uint8_t TP1_GetValue(void);

/*
 * RB4, GPIO Input, TP2
 */
static inline void TP2_SetHigh(void);
static inline void TP2_SetLow(void);
static inline void TP2_Toggle(void);
static inline uint8_t TP2_GetValue(void);

/*
 * RB5, GPIO Output, TP3
 */
static inline void TP3_SetHigh(void);
static inline void TP3_SetLow(void);
static inline void TP3_Toggle(void);
static inline uint8_t TP3_GetValue(void);

/*
 * RB6, ICSPCLK
 */

/*
 * RB7, ICSPDAT
 */


/*
 * RC0, PWM1, PWMR
 */
static inline void PWMR_SetHigh(void);
static inline void PWMR_SetLow(void);
static inline void PWMR_Toggle(void);
static inline uint8_t PWMR_GetValue(void);

/*
 * RC1, PWM2, PWMG
 */
static inline void PWMG_SetHigh(void);
static inline void PWMG_SetLow(void);
static inline void PWMG_Toggle(void);
static inline uint8_t PWMG_GetValue(void);

/*
 * RC2, PWM3, PWMB
 */
static inline void PWMB_SetHigh(void);
static inline void PWMB_SetLow(void);
static inline void PWMB_Toggle(void);
static inline uint8_t PWMB_GetValue(void);

/*
 * RC3, GPIO Output, RS485nRE
 */
static inline void RS485nRE_SetHigh(void);
static inline void RS485nRE_SetLow(void);
static inline void RS485nRE_Toggle(void);
static inline uint8_t RS485nRE_GetValue(void);

/*
 * RC4, GPIO Output, RS485nDE
 */
static inline void RS485nDE_SetHigh(void);
static inline void RS485nDE_SetLow(void);
static inline void RS485nDE_Toggle(void);
static inline uint8_t RS485nDE_GetValue(void);

/*
 * RC5, UART RX
 */

/*
 * RC6, CLC Output, SIGOUT
 */
static inline void SIGOUT_SetHigh(void);
static inline void SIGOUT_SetLow(void);
static inline void SIGOUT_Toggle(void);
static inline uint8_t SIGOUT_GetValue(void);

/*
 * RC7, GPIO Output, SIGOUTnEN
 */
static inline void SIGOUTnEN_SetHigh(void);
static inline void SIGOUTnEN_SetLow(void);
static inline void SIGOUTnEN_Toggle(void);
static inline uint8_t SIGOUTnEN_GetValue(void);

void PIN_Initialize(void);