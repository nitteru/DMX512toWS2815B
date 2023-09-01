/*
 * File:   main.c
 * Author: Teruyuki
 *
 * Created on 2023/05/19, 12:35
 */


#include <xc.h>
#include "device_config.h"
#include "main.h"
#include "interrupts.h"

reset_type resetType; // リセット要因格納

// インターバルタイマ
intervalTimerCounters intvTmrCnt;
intervalTimerFlags intvTmrFlg;
uint8_t rcvData = 0x00; // EUSART 1byte受信

void main(void) {

    GlobalInterruptDisable();
    PeripheralInterruptDisable();

    // リセット要因の検出
    resetType.byte = 0x00;
    if (PCONbits.nBOR == 0 && PCONbits.nPOR == 0) {
        // コールドスタート
        // リセット要因検出のための準備
        PCONbits.nPOR = 1; // パワーオンリセットをセット(発生すると0)
        PCONbits.nBOR = 1; // ブラウンアウトリセットをセット (発生すると0)
        PCONbits.nRMCLR = 1; // MCLRセット (発生すると0)
    } else { //ホットスタート
        if (PCONbits.nBOR == 0) {
            resetType.bits.bor = 1; // ブラウンアウトリセットでリセット
            PCONbits.nBOR = 1; // ブラウンアウトリセットをセット (発生すると0)
        }
        if (PCONbits.nPOR == 0) {
            resetType.bits.por = 1;
            PCONbits.nPOR = 1; // パワーオンリセットをクリア (発生すると0)
        }
        if (PCONbits.nRI == 0) {
            resetType.bits.ri = 1;
            PCONbits.nRI = 1;
        }
        if (PCONbits.nRMCLR == 0) {
            resetType.bits.rmclr = 1;
            PCONbits.nRMCLR = 1;
        }
        if (PCONbits.STKOVF == 1) { // スタックオーバー
            resetType.bits.so = 1;
            PCONbits.STKOVF = 0;
        }
        if (PCONbits.STKUNF == 1) { // スタックアンダーフロー
            resetType.bits.su = 1;
            PCONbits.STKUNF = 0;
        }
    }

    SYSTEM_Initialize();

    // 内部インターバルタイマスタート
    TMR0InterruptFlagClear();
    TMR0InterruptEnable();

    // 基本的にデバッグ用
    // EUSART1_RC_Enable();
    EUSART1_TX_Enable();

    PeripheralInterruptEnable();
    GlobalInterruptEnable();

    while (1) {
        CLRWDT();

        if (EUSART_isOverrun()) {
            // オーバーランエラーがあったらUART初期化
            EUSART_ResetReceiver();
        }

        if (PIR1bits.RCIF && EUSART_isFrammingError()) {
            PIR1bits.RCIF = 0;
            //rcvData = RC1REG; // 捨てる
        }

        // インターバルタイマー
        if (intvTmrFlg.fields.flag5msec) {
            // 5msec
            intvTmrFlg.fields.flag5msec = 0;
            calIntervalTimer();
        }

        if (intvTmrFlg.fields.flag10msec) {
            // 10msec
            intvTmrFlg.fields.flag10msec = 0;
        }

        if (intvTmrFlg.fields.flag100msec) {
            // 100msec
            intvTmrFlg.fields.flag100msec = 0;
        }

        if (intvTmrFlg.fields.flag200msec) {
            // 200msec
            intvTmrFlg.fields.flag200msec = 0;
        }

        if (intvTmrFlg.fields.flag500msec) {
            // 500msec
            intvTmrFlg.fields.flag500msec = 0;
        }

        if (intvTmrFlg.fields.flag1sec) {
            // 1sec
            intvTmrFlg.fields.flag1sec = 0;
        }

        // DMX512を受信
        
        
        return;
    }
}

void calIntervalTimer(void) {
    // 対象単位時間ごとに実行する
    // 5msec用フラグはTMR0割り込みで立てる
    if (++intvTmrCnt.fields.counter10msec == INTERVAL_TMR_10MSEC) {
        intvTmrCnt.fields.counter10msec = 0;
        intvTmrFlg.fields.flag10msec = 1;
    }

    if (++intvTmrCnt.fields.counter100msec == INTERVAL_TMR_100MSEC) {
        intvTmrCnt.fields.counter100msec = 0;
        intvTmrFlg.fields.flag100msec = 1;
    }

    if (++intvTmrCnt.fields.counter200msec == INTERVAL_TMR_200MSEC) {
        intvTmrCnt.fields.counter200msec = 0;
        intvTmrFlg.fields.flag200msec = 1;
    }

    if (++intvTmrCnt.fields.counter500msec == INTERVAL_TMR_500MSEC) {
        intvTmrCnt.fields.counter500msec = 0;
        intvTmrFlg.fields.flag500msec = 1;
    }

    if (++intvTmrCnt.fields.counter1sec == INTERVAL_TMR_1SEC) {
        intvTmrCnt.fields.counter1sec = 0;
        intvTmrFlg.fields.flag1sec = 1;
    }

}
