/*
 * File:   interrupts.c
 * Author: Teruyuki
 *
 * Created on 2023/05/19, 15:04
 */


#include <xc.h>
#include "interrupts.h"
#include "main.h"
#include "tmr0.h"

void __interrupt() ISR(void) {

    /*
     * 1回の割り込みで処理できるのは1つの割り込み要因のみ
     * 同時多発的に割り込みが発生し､コンテキストスイッチの影響が
     * 無視できない場合はelse ifを止めてifで連結すること
     */

    if (INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1) {
        // TMR0
        INTCONbits.TMR0IF = 0;
        
        TMR0 = tmr0_reload_val;
        
        //内部インターバルタイマー用
        intvTmrFlg.fields.flag5msec = 1;
        
    } else if (INTCONbits.INTE == 1 && INTCONbits.INTF == 1) {
        // External Intrrupt
        INTCONbits.INTF = 0;
        
        // 未使用
        
    } else if (INTCONbits.IOCIE == 1 && INTCONbits.IOCIF == 1) {
        // Interrupt On Change
        
        // INTCONのIOCIFは個別のフラグをすべてクリアが必要
        IOCAFbits.IOCAF0 = 0;
        //IOCBFbits.IOCBF0 = 0;
        //IOCCFbits.IOCCF0 = 0;
        
        // 未使用
        
    } else if (INTCONbits.PEIE == 1) {
        // Peripheral Interrupts
        if (PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF == 1) {
            // TMR1
            PIR1bits.TMR1IF = 0;
            
            T1CONbits.TMR1ON = 0;
            TMR1H = 0x00;
            TMR1L = 0x00;
            T1CONbits.TMR1ON = 1;
            
             // オーバーフローフラグ
            captureTimerOverflow = 1;
            
        } else if (PIE1bits.TMR2IE == 1 && PIR1bits.TMR2IF == 1) {
            // TMR2
            PIR1bits.TMR2IF = 0;
            
            // モーター PWM用
            
        } else if (PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1) {
            // UART Receive
            PIR1bits.RCIF = 0;
            
            // HW受信バッファからSW受信バッファにコピー
            if(RCSTAbits.OERR)
            {
                // オーバーランエラー
                RCSTAbits.CREN = 0;
                RCSTAbits.CREN = 1;
            }
            
            if(RCSTAbits.FERR)
            {
                // フレーミングエラー
                uint8_t ret = RCREG;
            }
            
            // リングバッファにでも放り込む?
            if((RCSTAbits.FERR | RCSTAbits.OERR) == 0)
            {
                
            }
            
        } else if (PIE1bits.ADIE == 1 && PIR1bits.ADIF == 1) {
            // ADC
            PIR1bits.ADIF = 0;
            
            // 未使用
            
        } else if (PIE1bits.SSP1IE == 1 && PIR1bits.SSP1IF == 1) {
            // SSP1
            PIR1bits.SSP1IF = 0;
            
            // 未使用
            
        } else if (PIE1bits.CCP1IE == 1 && PIR1bits.CCP1IF == 1) {
            // CCP1
            PIR1bits.CCP1IF = 0;
            
            // キャプチャしたタイマーの値を取得
            edgeCaptureValue = (uint16_t)(((uint16_t)CCPR1H << 8) | (uint16_t)CCPR1L);
            isCaptured = 1;
            
            T1CONbits.TMR1ON = 0;
            TMR1H = 0x00;
            TMR1L = 0x00;
            T1CONbits.TMR1ON = 1;  
            
        } else if (PIE2bits.BCL1IE == 1 && PIR2bits.BCL1IF == 1) {
            // MSSP bus collision
            PIR2bits.BCL1IF = 0;
            
            // 今のところ未使用
            
        } else if (PIE2bits.TMR4IE == 1 && PIR2bits.TMR4IF == 1) {
            // TMR4
            PIR2bits.TMR4IF = 0;
            
            // LED PWM用
            
        } else if (PIE2bits.TMR6IE == 1 && PIR2bits.TMR6IF == 1) {
            // TMR6
            PIR2bits.TMR6IF = 0;
            
            // バッファからDACにデータを転送する
            
        } else if (PIE2bits.CCP2IE == 1 && PIR2bits.CCP2IF == 1) {
            // CCP2
            PIR2bits.CCP2IF = 0;
            
            // PWMモード
        }
    }
}
