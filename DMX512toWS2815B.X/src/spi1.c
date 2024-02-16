/*
 * File:   eusart1.c
 * Author: nishi
 *
 * Created on July 20, 2022, 3:46 PM
 */

#include <xc.h>
#include "spi1.h"
#include "main.h"

void SPI1_Initialize(void) {

    /*
     * EN: SPI Enable
     * LSBF: Data is exchanged MSb first
     * MST: Operate as the bus host
     * BMODE: SPIxTWIDTH setting applies to every byte
     *        BMODE = 0: 送信ビット数を指定してFIFOに転送
     *        BMODE = 1: 送信はFIFOに書き込んでから自動実行, 受信は受信カウンタで指定する
     */
    SPI1CON0 = 0b10000011;
    
    /*
     * SMP: SID input is sampled at the middle if data output time
     * CKE: Output data changes on transition from idle to Avtive clock state
     * CKP: Idle state for SCK is low level
     * FST: Delay to first SCK will be at least 1/2 baud period
     * SSP: SS is active-high
     * SDIP: SDI input is active-high
     * SDOP: SDO output is active-high
     */
    SPI1CON1 = 0b00000000;
    
    /*
     * BUSY: SPI Module Byst Status (Read only)
     * SSFLT: SS_in Fault Status (Read only)
     * SSET: SS_out is driven to the Active state while the transmit counter is not zero
     * TXR: Transmit Data-required Control
     * RXR: Receive FIFO Space-Required Control
     */
    SPI1CON2 = 0b00000010; // TXT = 1, RXR = 0: 送信専用モード (受信バッファがフルでも通信を続行)
    
    /*
     * CLKSEL: TMR2_Postscaler_OUT
     */
    SPI1CLK = 0b00000110;
    
    /*
     * SCK toggle frequency: FSCK = FBAUD = SPI Clock Frequency/(2*(n+1))
     * FBAUD 400kHz = 800kHz / (2 * (0 + 1))
     * クロックを2分周設定
     */
    SPI1BAUD = 0x00;
    
    /*
     * TCNTH[2:0]: Bits 10-8 of the transfer byte count (BMODE = 1)
     * TCNTL[7:0]: Bits 7-0 of the transfer byte count (BMODE = 1)
     */
    SPI1TCNT = 0x00;
    
    /*
     * TWIDTH[2:0]: Number of bits in each transfer byte count. Bits = n (when n > 0) or 8 (when n = 0) (BMODE = 1
     *              一回の送信で送信するビット数(0指定で8ビット)
     */
    SPI1TWIDTH = 0x00;
    
    /*
     * TXWE: No error has occurred (Clear)
     * CLB: Take no action (Set only)
     */
    SPI1STATUS = 0x00;
    
    /*
     * RXB: Receive Buffer, Contains the top-most byte of the RXFIFO. Read 0 means that the receive buffer is empty, 
     *      and set the RXRE Status bit.
     */
    
    /*
     * TXB: If the transmit buffer is full, Writing to this register does not affect the data in the TXFIFO. 
     *      The TXWE Status bit will be set.
     */
    
    /*
     * SRMTIE: Shift Register Empty Interrupt Enable
     * TCZIE: Transfer Counter is Zero Interrupt Enable
     * SOSIE: Start of Client Select Interrupt Enable
     * EOSIE: End if Client Select Interrupt Enable
     * RXOIE: Receiver Overflow Interrupt Enable
     * TXUIE: Transmitter Underflow Interrupt Enable
     */
    SPI1INTE = 0x00;
    
    /*
     * All Interrupt Flag Clear
     */
    SPI1INTF = 0x00;
}
