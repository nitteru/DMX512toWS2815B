/*
 * File:   eusart.c
 * Author: Teruyuki
 *
 * Created on 2023/06/01, 16:22
 */


#include <xc.h>
#include "eusart.h"
#include "main.h"

void EUSART_Initialize(void) {
    /*
     * Baudrate Calc (Async)
     * 
     * BRG16:0, BRGH:0:
     *   Baudrate = Fosc / (64 * ([SPBRGH:SPBRGL] + 1))
     * BRG16:0, BRGH:1:
     *   Baudrate = Fosc / (16 * ([SPBRGH:SPBRGL] + 1))
     * BRG16:1, BRGH:0:
     *   Baudrate = Fosc / (16 * ([SPBRGH:SPBRGL] + 1))
     * BRG16:1, BRGH:1:
     *   Baudrate = Fosc / (4 * ([SPBRGH:SPBRGL] + 1))
     */
    
    PIE1bits.RCIE = 0x00;
    PIE1bits.TXIE = 0x00;
    PIR1bits.RCIF = 0x00;
    
    // 19200bps設定
    RC1STAbits.SPEN = 0x01; // Serial port enabled
    RC1STAbits.RX9 = 0x00; // Selects 8-bit reception (DMX512使用時は9bitとしてストップビット代わりにする)
    RC1STAbits.SREN = 0x00; // Asynchronous mode: Don't care
    RC1STAbits.CREN = 0x00; // Enables receiver
    RC1STAbits.ADDEN = 0x00; // Disables address detection, all bytes are received and ninth bit can be used as parity bit
    
    TX1STAbits.TXEN = 0x00;
    TX1STAbits.CSRC = 0x00; // Asynchronous mode: Don't care
    TX1STAbits.TX9 = 0x00; // Selects 8-bit transmission (DMX512使用時は9bitとしてストップビット代わりにする)
    TX1STAbits.SYNC = 0x00; // Asynchronous mode
    TX1STAbits.SENDB = 0; // Sync Break transmission completed (DMX512のスタートに使える?SENDBセットし､TX1REGに何かしら書き込むとスタート含めて12bit分送信｡TRMTで送信完了がわかる)
    TX1STAbits.BRGH = 0x01; // High speed
    TX1STAbits.TX9D = 0x00; 
    
    BAUD1CONbits.SCKP = 0x00; // Transmit non-inverted data to the TX/CK pin
    BAUD1CONbits.BRG16 = 0x01; // 16-bit Baud Rate Generator is used
    BAUD1CONbits.WUE = 0x00; // Receiver is operating normally (RCIDL==0の時､WUEを1にセットしてスリープに入るとBreak受信で復帰する)
    BAUD1CONbits.ABDEN = 0x00; // Auto-Baud Detect mode is disabled
    
#ifdef OSC_CLK_32MHZ
    // SPBRG = 416
    SP1BRGH = 0x01;
    SP1BRGL = 0xA0;
#elif defined OSC_CLK_16MHZ
    // SPBRG = 207, 19230.8bps
    SP1BRGH = 0x00;
    SP1BRGL = 0xCF;
    // SPBRG = 417, 9592.3bps
    /*
    SP1BRGH = 0x01;
    SP1BRGL = 0xA1;
    */ 
#elif defined OSC_CLK_8MHZ
    // SPBRG = 103
    SP1BRGH = 0x00;
    SP1BRGL = 0x67;
#endif
}

void EUSART_Write(uint8_t data)
{
    while (!PIR1bits.TXIF)
    //while (!TX1STAbits.TRMT)
    {
        // 送信バッファTXREGが空になる(TSRへのコピーが終わる)のを待つ
        // TSRから送信が完了するのを待つ場合はTRMTを見る
    }

    TX1REG = data;
}
