/*
 * File:   eusart1.c
 * Author: nishi
 *
 * Created on July 20, 2022, 3:46 PM
 */

#include <xc.h>
#include "eusart1.h"
#include "main.h"

void UART_Initialize(void) {
    /*
     * Baudrate Calc (Async)
     * BRGS: 0, Normal Rate
     *   Baudrate = Fosc / (16 * ([UxBRG] + 1))
     *   UxBRG = Fosc / (16 * Baudrate) - 1
     * 
     * BRGS: 1, High Rate
     *   Baudrate = Fosc / (4 * ([UxBRG] + 1))
     *   UxBRG = Fosc / (4 * Baudrate) - 1
     */
    
    UART1RCInterruptDisable();
    UART1TXInterruptDisable();
    UART1RCInterruptFlagClear();
        
    U1RXB = 0x00;
    U1RXCHK = 0x00;
    U1TXB = 0x00;
    U1TXCHK = 0x00;
    
    /*
     * 8bit Asyncモードではパラメータは未使用
     */
    U1P1H = 0x00;
    U1P1L = 0x00;
    U1P2H = 0x00;
    U1P2L = 0x00;
    U1P3H = 0x00;
    U1P3L = 0x00;    

    /*
     * Asynchronous 8bit UART mode
     * Auto-Baud Detect disable
     * Assert UxRXIF in middle of first Stop bit
     */
    U1CON0 = 0x30;
    
    /*
     * Receiver operates normally
     * Set RXBKIF on rising RX input after RX in has been low for the minimum Break time
     * TX output is driven by transmit shift register
     * Break transmission completed or disabled
     */
    U1CON1 = 0x00;

    /*
     * RX input shifter stops all activity on receiver Overflow condition
     * RX polarity is not inverted, Idle state is high
     * Transmit 1 Stop bit, receiver verifies first Stop bit
     * Checksum is disabled
     * Output data is not inverted, TX output is high in Idle state
     * Flow control is off
     */
    U1CON2 = 0x00;

    /*
     * Transmit Write Error Status: Clear or No error
     * Assert UxRXIF in middle of first Stop bit
     * Transmit buffer is empty. Setting this bit will clear the transmit buffer and output shift register.
     * Transmit buffer is not full
     * Receive buffer is empty. Setting this bit will clear the RX buffer
     */
    U2FIFO = 0x2E;

    // 19200bps設定
    U1BRGL = 0x33; // dec 51, Error: 0.1%
    U1BRGH = 0x0;
    
    U1ERRIR = 0x80;
    U1UIR = 0x00;
    U1ERRIE = 0x00; // 必要に応じて割り込みを有効化してコールバック関数を設定すること
    
    U1CON1bits.ON = 0x01; // Serial port enabled
}

void UART_Write(uint8_t data)
{
    while (!UART_is_tx_ready())
    //while (!TX1STAbits.TRMT)
    {
        // 送信バッファTXREGが空になる(TSRへのコピーが終わる)のを待つ
        // TSRから送信が完了するのを待つ場合はTRMTを見る
    }

    U1TXB = data;
}

uint8_t UART_Read(void)
{
    return U1RXB;
}
