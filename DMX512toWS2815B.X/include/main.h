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

// デバッグ等
#define DEBUG_PRINT

// クロック設定
#define OSC_CLK_32MHZ // 32MHz, (INTOSC 8MHz * x4 PLL)
//#define OSC_CLK_16MHZ // 16MHz, (INTOSC 16MHz)
//#define OSC_CLK_8MHZ // 8MHz, (INTOSC 8MHz)

// 内部インターバルタイマ
#define INTERVAL_TMR_100MSEC (uint8_t)10
#define INTERVAL_TMR_200MSEC (uint8_t)20
#define INTERVAL_TMR_500MSEC (uint8_t)50
#define INTERVAL_TMR_1SEC (uint8_t)100


// インターバルタイマ カウンタ
typedef union
{
    uint8_t A[6];
    struct
    {
        // LSB
        uint8_t counter5msec;
        uint8_t counter10msec;
        uint8_t counter100msec;
        uint8_t counter200msec;
        uint8_t counter500msec;
        uint8_t counter1sec;
        // MSB
    } fields;
} intervalTimerCounters;

// インターバルタイマ フラグ
typedef union
{
    uint8_t A;
    struct
    {
        // LSB
        uint8_t flag5msec:1;
        uint8_t flag10msec:1;
        uint8_t flag100msec:1;
        uint8_t flag200msec:1;
        uint8_t flag500msec:1;
        uint8_t flag1sec:1;
        // MSB
    } fields;
} intervalTimerFlags;

// リセット要因情報
typedef union {
    uint8_t byte;

    struct {
        uint8_t dmy : 1; //LSB
        uint8_t hot : 1;
        uint8_t so : 1;
        uint8_t su : 1;
        uint8_t rmclr : 1;
        uint8_t ri : 1;
        uint8_t bor : 1;
        uint8_t por : 1; //MSB
    } bits;
} reset_type;


extern intervalTimerFlags intvTmrFlg;
