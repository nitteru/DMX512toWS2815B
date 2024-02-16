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

// UART受信バッファ
uint8_t rcvData = 0x00;

void main(void) {

    INTERRUPT_GlobalInterruptHighDisable();
    INTERRUPT_GlobalInterruptLowDisable();

	// リセット要因の検出
    BORCONbits.SBOREN = 1; // BOR enable
    do
    {
        // BOR回路立ち上がり待ち
    } while(!BORCONbits.BORRDY);
    
	resetType.byte = 0x00;
    if (PCON0bits.nBOR == 0 && PCON0bits.nPOR == 0)
    {
        // 初回の起動(コールドスタート)
        PCON0bits.nPOR = 1;   // PORをクリア
        PCON0bits.nBOR = 1;   // BORをクリア
        PCON0bits.nRMCLR = 1; // MCLRクリア
    }
    else
    {
        /*
         * 何らかの要因でリセット(ホットスタート)
         * 各種リセットごとに必要な処理を追記
         */
        if (PCON0bits.nBOR == 0)
        {
            // BOR
			resetType.bits.bor = 1; // ブラウンアウトリセットでリセット
            PCON0bits.nBOR = 1;
        }
        if (PCON0bits.nPOR == 0)
        {
            // POR
			resetType.bits.por = 1;
            PCON0bits.nPOR = 1;
        }
        if (PCON0bits.nRI == 0)
        {
            // リセットコマンド
			resetType.bits.ri = 1;
            PCON0bits.nRI = 1;
        }
        if (PCON0bits.nRMCLR == 0)
        {
            // MCLR
			resetType.bits.rmclr = 1;
            PCON0bits.nRMCLR = 1;
        }
        if (PCON0bits.STKOVF == 1)
        {
            // スタックオーバー
			resetType.bits.so = 1;
            PCON0bits.STKOVF = 0;
        }
        if (PCON0bits.STKUNF == 1)
        {
            // スタックアンダーフロー
			resetType.bits.su = 1;
            PCON0bits.STKUNF = 0;
        }
    }

    /*
     * システム初期設定
     */
    SYSTEM_Initialize();

    /*
     * 各種電源出力など設定
     * とりあえずバッテリーモードはあとから実装
     */
    PWELED_SetHigh(); // Power LED
    ISOPSEN_SetHigh(); // 二次側用電源
    DMXOUTEN_SetHigh(); // DMX512用電源
    PSOUTEN_SetHigh(); // 5V電源
    SIGOUTnEN_SetHigh(); // 信号ドライバ
    /*
     * RS485トランシーバを送信モードで固定
     * 両方Lowでスリープ
     */
    RS485nDE_SetHigh(); 
    RS485nRE_SetHigh();
            
    // 内部インターバルタイマスタート
    TMR0_InterruptFlagClear();
    TMR0_InterruptEnable();

    // 基本的にデバッグ用
    UART1_RC_Enable();
    //UART1_TX_Enable();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    

    while (1) {
        CLRWDT();

        if (UART_isOverrun()) {
            // オーバーランエラーがあったらUART初期化
            UART_ResetReceiver();
        }

        if (PIR4bits.U1RXIF && UART_isFrammingError()) {
            PIR4bits.U1RXIF = 0;
            rcvData = UART_Read(); // 捨てる
        }

        // インターバルタイマー
#if 0
        if (intvTmrFlg.fields.flag5msec) {
            // 5msec
            intvTmrFlg.fields.flag5msec = 0;
            calIntervalTimer();
        }
#endif
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

        /*
         * 1. DMX512を受信 1パケット513バイトをバッファ
         * 2. SPIに向けて送信
         */
        
        
        return;
    }
}

void calIntervalTimer(void) {
    // 対象単位時間ごとに実行する
    // 10msec用フラグはTMR0割り込みで立てる
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
