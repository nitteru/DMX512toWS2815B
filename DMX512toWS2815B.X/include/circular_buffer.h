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

#define CIRCULAR_BUFFER_SIZE 128               // 循環バッファサイズ (最大256バイト, 2^nサイズで指定すること)
#define cbuffer_next(n) (((n) + 1) & mod_mask) // 循環バッファの次の挿入位置

struct circular_buffer_data
{
    uint8_t queue[CIRCULAR_BUFFER_SIZE];
    uint8_t head; // キューの先頭位置
    uint8_t tail; // キューの末尾位置
};
typedef struct circular_buffer_data cbuffer_t;

extern uint8_t mod_mask; // 剰余計算用マスク

void cBuffer_Init(cbuffer_t *);                    // 循環バッファの初期化
int8_t cBuffer_isEmpty(cbuffer_t *);               // 循環バッファが空であるか
int8_t cBuffer_isFull(cbuffer_t *);                // 循環バッファが満杯であるか
int8_t cBuffer_Set(cbuffer_t *, uint8_t, uint8_t); // 指定したインデックスに要素をセット
int8_t cBuffer_Push(cbuffer_t *, uint8_t);         // 末尾に要素を追加
int8_t cBuffer_Pop(cbuffer_t *, uint8_t *);        // 先頭の要素を取り出す
int8_t cBuffer_UseInRange(cbuffer_t *, uint8_t);   // 指定したインデックスが使用中であるか
