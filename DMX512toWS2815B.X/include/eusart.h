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
#include "eusart.h"

#define EUSART1_RC_Enable() (RC1STAbits.CREN = 0x01)
#define EUSART1_RC_Disable() (RC1STAbits.CREN = 0x00)
#define EUSART1_TX_Enable() (TX1STAbits.TXEN = 0x01)
#define EUSART1_TX_Disable() (TX1STAbits.TXEN = 0x00)

#define EUSART1RCInterruptEnable() (PIE1bits.RCIE = 0x01)
#define EUSART1RCInterruptDisable() (PIE1bits.RCIE = 0x00)
#define EUSART1RCInterruptFlagClear() (PIR1bits.RCIF = 0x00)

#define EUSART1TXInterruptEnable() (PIE1bits.TXIE = 0x01)
#define EUSART1TXInterruptDisable() (PIE1bits.TXIE = 0x00)

#define EUSART_isOverrun() (RC1STAbits.OERR)
#define EUSART_isFrammingError() (RC1STAbits.FERR)

#define EUSART_ResetReceiver() do                    \
                               {                     \
                                    RC1STAbits.CREN = 0x00; \
                                    RC1STAbits.CREN = 0x01; \
                                } while (0)

#define USART_is_tx_ready() (PIR1bits.TXIF)

void EUSART_Initialize(void);
void EUSART_Write(uint8_t);