/*
 * File:   circular_buffer.c
 * Author: nishi
 *
 * Created on February 8, 2022, 3:00 PM
 */

#include <xc.h>
#include "circular_buffer.h"

uint8_t mod_mask = 0;

void cBuffer_Init(cbuffer_t *cbuf)
{
    cbuf->head = 0;
    cbuf->tail = 0;
    mod_mask = CIRCULAR_BUFFER_SIZE - 1;
}

/* 循環バッファが空であるか
 * 0: 空でない
 * 1: 空
 */
int8_t cBuffer_isEmpty(cbuffer_t *cbuf)
{
    return (cbuf->head == cbuf->tail);
}

/* 循環バッファが満杯であるか
 * 0: フルでない
 * 1: フル
 */
int8_t cBuffer_isFull(cbuffer_t *cbuf)
{
    return (cbuffer_next(cbuf->tail) == cbuf->head);
}

// 指定したインデックスに要素をセット
int8_t cBuffer_Set(cbuffer_t *cbuf, uint8_t index, uint8_t data)
{
    if (index >= CIRCULAR_BUFFER_SIZE)
    {
        return -1;
    }

#if 0
    // スタックを消費するので呼び出し元で判断してもらう
    if(cbuffer_use_in_range(cbuf, index))
    {
        return -1;
    }
#endif

    cbuf->queue[index] = data;

    return 0;
}

// 末尾に要素を追加
int8_t cBuffer_Push(cbuffer_t *cbuf, uint8_t data)
{
// スタックを消費するので関数を使わない
#if 1
    if (cbuffer_next(cbuf->tail) == cbuf->head)
    {
        return -1;
    }
#else
    if (cbuffer_isfull(cbuf))
    {
        return -1;
    }
#endif

    cbuf->queue[cbuf->tail] = data;
    cbuf->tail = cbuffer_next(cbuf->tail);

    return 0;
}

// 先頭の要素を取り出す
int8_t cBuffer_Pop(cbuffer_t *cbuf, uint8_t *data)
{
// スタックを消費するので関数を使わない
#if 1
    if (cbuf->head == cbuf->tail)
    {
        return -1;
    }
#else
    if (cbuffer_isempty(cbuf))
    {
        return -1;
    }
#endif

    *data = cbuf->queue[cbuf->head];
    cbuf->head = cbuffer_next(cbuf->head);

    return 0;
}

// 指定したインデックスが使用中であるか
int8_t cBuffer_UseInRange(cbuffer_t *cbuf, uint8_t index)
{
    if ((cbuf->head <= index) && (index < cbuf->tail))
    {
        return 1;
    }

    if ((cbuf->head > cbuf->tail) && (cbuf->head >= index) && (index > cbuf->tail))
    {
        return 1;
    }

    if ((cbuf->head > cbuf->tail) && (cbuf->head <= index) && (index < cbuf->tail))
    {
        return 1;
    }

    return 0;
}
