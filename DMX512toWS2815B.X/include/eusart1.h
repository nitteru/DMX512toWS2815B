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

#define UART1_RC_Enable() (U1CON0bits.RXEN = 0x01)
#define UART1_RC_Disable() (U1CON0bits.RXEN = 0x00)
#define UART1_TX_Enable() (U1CON0bits.TXEN = 0x01)
#define UART1_TX_Disable() (U1CON0bits.TXEN = 0x00)

#define UART1RCInterruptEnable() (PIE4bits.U1RXIE = 0x01)
#define UART1RCInterruptDisable() (PIE4bits.U1RXIE = 0x00)
#define UART1RCInterruptFlagClear() (PIR4bits.U1RXIF = 0x00)

#define UART1TXInterruptEnable() (PIE4bits.U1TXIE = 0x01)
#define UART1TXInterruptDisable() (PIE4bits.U1TXIE = 0x00)
#define UART1TXInterruptFlagClear() (PIR4bits.U1TXIF = 0x00)

#define UART1OverrunInterruptFlagClear() (U1ERRIRbits.RXFOIF = 0x00)

#define UART_isOverrun() (U1ERRIRbits.RXFOIF)
#define UART_isFrammingError() (U1ERRIRbits.FERIF)

#define UART_ResetReceiver() do                    \
                               {                     \
                                    U1CON0bits.RXEN = 0x00; \
                                    U1CON0bits.RXEN = 0x01; \
                                } while (0)

#define UART_is_tx_ready() (uint8_t)(U1FIFObits.TXBE && U1CON0bits.TXEN)
#define UART_is_rx_ready() (uint8_t)(!U1FIFObits.RXBE)

void UART_Initialize(void);
void UART_Write(uint8_t);
uint8_t UART_Read(void);
uint8_t UART1_IsRxReady(void);