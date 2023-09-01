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

uint8_t sleepFlag = 0; // スリープに入るフラグ
uint8_t sleepExitFlag = 0; // 1:スリープから復帰する条件を満たしているか確認中

intervalTimerCounters intvTmrCnt;
intervalTimerFlags intvTmrFlg;

uint8_t rcvData = 0x00; // EUSART 1byte受信

enum nodeFrame nFrame = NODE_WAIT;

uint16_t edgeCaptureValue = 0; // キャプチャした値
uint8_t captureTimerOverflow = 0; // キャプチャタイマーOF回数
uint8_t isCaptured = 0; // キャプチャ完了フラグ

uint8_t rcvByteFlag = 0; // 1バイト受信完了フラグ
uint8_t rcvByteBuffer = 0; // 1バイト受信バッファ
uint8_t rcvShiftCounter = 0; // 1バイト受信バッファシフトカウンタ
uint8_t dataFrameBuffer[DATA_BUFFER_SIZE]; // データバッファ AEHAを踏まえて最大32バイトとする?
uint8_t dataFrameCounter = 0; // データバッファの格納位置
uint8_t repeatFlag = 0; // リピート信号受信

uint8_t rcvTimeOutEnable = 0; // 受信タイムアウト管理
uint8_t rcvTimeOutFlag = 0; // 受信タイムアウトフラグ
uint8_t rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC; // 受信タイムアウトカウンタ * 10msec

uint8_t aehaTrailerEnable = 0; // AEHAトレーラー受信管理
uint8_t aehaTrailerFlag = 0; // AEHAトレーラー受信フラグ
uint8_t aehaTrailerCounter = AEHA_TRAILER_TIME; // AEHAトレーラー用カウンタ


enum IRCmd irCmd = IRCMD_STOP; // コマンド
uint8_t executeFlag = 0; // リモコン信号受信完了→実行処理へ

commandInfo cmdInfo; // 実行コマンド情報

/*
 * 走行状態: 0:停車, 1:前進, 2:後退
 * 出力: PWM_MOTER_MAX～PWM_MOTER_MIN
 * スリープ復帰はCCP1入力をPPSでINTに切り替えて待機?
 */
uint8_t status = 0x00; // 0:停止, 1:前進, 2:後退
uint16_t pwmDutyTarget = 0x0000; // PWM ONデューティ目標値
uint16_t pwmDuty = 0x0000; // PWM ONデューティ
uint8_t pwmDutyTblIndex = 0; // スピード設定テーブルのインデックス


uint8_t foo = 0x00; // デバッグ UART出力
uint8_t dbgUARTcnt = 0x00;

/*
 * High→Lowで区切られる区間の既定値(タイマーカウント値)､誤差範囲(概ね±30%)のセット
 * [0][*]: NECフォーマットリーダー 24T
 * [1][*]: AEHAフォーマットリーダー 12T
 * [2][*]: NECフォーマット 0 2T
 * [3][*]: NECフォーマット 1 4T
 * [4][*]: AEHAフォーマット 0 2T
 * [5][*]: AEHAフォーマット 1 4T
 * [6][*]: NECフォーマットリピート 20T
 * [7][*]: AEHAフォーマットリピート 16T
 * 
 * TMR1の分解能: 250nsec * 8 = 2usec
 * 各区間を2usecで割った値で定義していく
 */
const uint16_t tbl[8][2] = {
    {6744, 674}, // NEC リーダー
    {2550, 255}, // AEHA リーダー
    {562, 56}, // NEC 0
    {1124, 112}, // NEC 1
    {425, 42}, // AEHA 0
    {850, 85}, // AEHA 1
    {5620, 562}, // NEC リピート
    {3400, 340} // AEHA リピート
};

/* 
 * リモコンの各ボタンに割り当てられているデータ
 * 
 * 前半2バイト 0x10, 0xEF: XORで0xFF, ANDで0x00
 * 後半2バイト  XORで0xFF, ANDで0x00
 */ 
const uint8_t receiveIRData[13][4] = {
    {0x10, 0xEF, 0xD8, 0x27}, // #01 電源
    {0x10, 0xEF, 0xF8, 0x07}, // #02 A
    {0x10, 0xEF, 0x78, 0x87}, // #03 B
    {0x10, 0xEF, 0x58, 0xA7}, // #04 C
    {0x10, 0xEF, 0xB1, 0x4E}, // #05 左上矢印
    {0x10, 0xEF, 0xA0, 0x5F}, // #06 上矢印
    {0x10, 0xEF, 0x21, 0xDE}, // #07 右上矢印
    {0x10, 0xEF, 0x10, 0xEF}, // #08 左矢印
    {0x10, 0xEF, 0x20, 0xDF}, // #09 中央
    {0x10, 0xEF, 0x80, 0x7F}, // #10 右矢印
    {0x10, 0xEF, 0x11, 0xEE}, // #11 左下矢印
    {0x10, 0xEF, 0x00, 0xFF}, // #12 下矢印
    {0x10, 0xEF, 0x81, 0x7E} // #13 右下矢印
};

/*
 * スピード設定
 * index 0:遅い, 2:中速, 4:速い
 */
const uint16_t pwmDutyTbl[5] = {0x00FF, 0x0140, 0x0180, 0x01C0, 0x01FF};

/*
 * LED調光設定
 * [step][count]
 */
const uint16_t dimmerLEDTbl[5][2] =
{
    {20, 50}, // 20/1024 step, 50 count
    {29, 35}, // 29/1024 step, 35 count
    {40, 25}, // 40/1024 step, 25 count
    {67, 15}, // 67/1024 step, 15 count
    {200, 5}, // 40/1024 step, 25 count
};
uint8_t dimmerLEDFlag = 0x00; // 0:スタンバイ時の明るさ, 1:調光動作
uint8_t dimmerLEDcount = 0x00;
uint8_t dimmerLEDdirFlag = 0x00; // 0:増える, 1:減る
uint16_t dimmerLEDPWMDuty = PWM_LED_DIM_STANDBY; // PWMデューティ

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

    /*
        // Backup Reset Registerを使ったリセット要因の判定
        if(__powerdown == 1 && __timeout == 1 &&  __resetbits == 0x00)
        {
        }
     */

    SYSTEM_Initialize();
    
    // 内部インターバルタイマスタート
    TMR0InterruptFlagClear();
    TMR0InterruptEnable();
    
    // DAC用FVR Enable
    FVR_Enable();
    
    // モーターをブレーキで待機
    //PWM3_UpdateDuty(0x0000);
    //PWM4_UpdateDuty(0x0000);
    PWM3_UpdateDuty(0x01FF); // HHでブレーキ､LLでスタンバイ(空転)
    PWM4_UpdateDuty(0x01FF);

    // LED点灯試験
    CCP2_PWM_UpdateDuty(PWM_LED_DIM_STANDBY);
    
    // Capture割り込み有効化
    CCP1InterruptFlagClear();
    CCP1InterruptEnable();

    // 基本的にデバッグ用
    // EUSART1_RC_Enable();
    EUSART1_TX_Enable();
    
    PeripheralInterruptEnable();
    GlobalInterruptEnable();
    
    while (1) {
        CLRWDT();

        if (EUSART_isOverrun())
        {
            // オーバーランエラーがあったらUART初期化
            EUSART_ResetReceiver();
        }

        if (PIR1bits.RCIF && EUSART_isFrammingError())
        {
            PIR1bits.RCIF = 0;
            rcvData = RC1REG; // 捨てる
        }
        
        // インターバルタイマー
        if(intvTmrFlg.fields.flag5msec)
        {
            // 5msec
            intvTmrFlg.fields.flag5msec = 0;
            calIntervalTimer();
        }

        if(intvTmrFlg.fields.flag10msec)
        {
            // 10msec
            intvTmrFlg.fields.flag10msec = 0;
            
            // コマンド実行
            if(cmdInfo.bits.excuteFlag == 1)
            {
                // 実行
                switch (cmdInfo.bits.excuteCmd)
                {
                    case IRCMD_STOP:
                        // 条件を満たしたら実行フラグを下ろす
                        if(status == 1)
                        {
                            if((pwmDuty -= PWM_MOTOR_DECREMENT_STEP) < PWM_MOTOR_MIN)
                            {
                                PWM3_UpdateDuty(0x01FF); // ブレーキ
                                PWM4_UpdateDuty(0x01FF);
                                
                                status = 0;
                                cmdInfo.bits.excuteFlag = 0;
                            }
                            else
                            {
                                PWM3_UpdateDuty(0x0000); // どっちがどっちかは後ほど (前進時と逆)
                                PWM4_UpdateDuty(pwmDuty);
                            }
                        }
                        else if(status == 2)
                        {
                            if((pwmDuty -= PWM_MOTOR_DECREMENT_STEP) < PWM_MOTOR_MIN)
                            {
                                PWM3_UpdateDuty(0x01FF); // ブレーキ
                                PWM4_UpdateDuty(0x01FF);
                                
                                status = 0;
                                cmdInfo.bits.excuteFlag = 0;
                            }
                            else
                            {
                                PWM4_UpdateDuty(0x0000); // どっちがどっちかは後ほど (前進時と逆)
                                PWM3_UpdateDuty(pwmDuty);                            
                            }
                        }
                        break;
                    case IRCMD_UP:
                        if(status == 1)
                        {
                            if((pwmDuty += PWM_MOTOR_INCREMENT_STEP) > pwmDutyTarget)
                            {
                                pwmDuty = pwmDutyTarget;
                                cmdInfo.bits.excuteFlag = 0;
                            }
                            else
                            {
                                PWM3_UpdateDuty(0x0000); // どっちがどっちかは後ほど
                                PWM4_UpdateDuty(pwmDuty);
                            }
                        }
                        else if(status == 2)
                        {
                            if((pwmDuty -= PWM_MOTOR_DECREMENT_STEP) < pwmDutyTarget)
                            {
                                pwmDuty = pwmDutyTarget;
                                cmdInfo.bits.excuteFlag = 0;
                            }
                            else
                            {
                                PWM4_UpdateDuty(0x0000); // どっちがどっちかは後ほど (前進時と逆)
                                PWM3_UpdateDuty(pwmDuty);
                            }
                        }
                        break;
                    case IRCMD_DOWN:
                        if(status == 1)
                        {
                            if((pwmDuty -= PWM_MOTOR_DECREMENT_STEP) < pwmDutyTarget)
                            {
                                pwmDuty = pwmDutyTarget;
                                cmdInfo.bits.excuteFlag = 0;
                            }
                            else
                            {
                                PWM3_UpdateDuty(0x0000); // どっちがどっちかは後ほど
                                PWM4_UpdateDuty(pwmDuty);
                            }
                        }
                        else if(status == 2)
                        {
                            if((pwmDuty += PWM_MOTOR_INCREMENT_STEP) > pwmDutyTarget)
                            {
                                pwmDuty = pwmDutyTarget;
                                cmdInfo.bits.excuteFlag = 0;
                            }
                            else
                            {
                                PWM4_UpdateDuty(0x0000); // どっちがどっちかは後ほど (前進時と逆)
                                PWM3_UpdateDuty(pwmDuty);
                            }
                        }
                        break;
                    case IRCMD_WHISTLE:
                        cmdInfo.bits.excuteFlag = 0;
                        break;
                    default:
                        break;
                }
            }
            
            if(dimmerLEDFlag == 0)
            {
                CCP2_PWM_UpdateDuty(dimmerLEDPWMDuty);
            }
            else if(dimmerLEDFlag == 1)
            {
                if(dimmerLEDdirFlag == 0)
                {
                    dimmerLEDPWMDuty += dimmerLEDTbl[pwmDutyTblIndex][0];
                }
                else if(dimmerLEDdirFlag == 1)
                {
                    dimmerLEDPWMDuty -= dimmerLEDTbl[pwmDutyTblIndex][0];
                }
                
                if(++dimmerLEDcount >= dimmerLEDTbl[pwmDutyTblIndex][1])
                {
                    dimmerLEDdirFlag = !dimmerLEDdirFlag;
                    dimmerLEDcount = 0;
                }
                
                CCP2_PWM_UpdateDuty(dimmerLEDPWMDuty);
            }
        }

        if(intvTmrFlg.fields.flag100msec)
        {
            // 100msec
            intvTmrFlg.fields.flag100msec = 0;
        }

        if(intvTmrFlg.fields.flag200msec)
        {
            // 200msec
            intvTmrFlg.fields.flag200msec = 0;
        }

        if(intvTmrFlg.fields.flag500msec)
        {
            // 500msec
            intvTmrFlg.fields.flag500msec = 0;
        }
        
        if(intvTmrFlg.fields.flag1sec)
        {
            // 1sec
            intvTmrFlg.fields.flag1sec = 0;
        }

        if (isCaptured) {
            // CCP Captureでパルス幅を検出した
            isCaptured = 0;

            // AEHAフォーマットのトレーラー検出フラグを立てる
            // 10msec以上経過したら受信完了
            // 通常のタイムアウト処理はいらないかも → リーダーを受信したら初期状態から開始になるため

            /*
             * パルス幅範囲のデータ構造
             * 各部位のLowパルス幅(赤外線出力期間)の既定値､誤差範囲を持つ
             * 
             */
            if (captureTimerOverflow) {
                nFrame = NODE_WAIT;
            } else {
                // どの区間に該当するか
                if (((tbl[0][0] - tbl[0][1]) < edgeCaptureValue) && (edgeCaptureValue < (tbl[0][0] + tbl[0][1]))) {
                    // NECフォーマットのリーダー
                    rcvTimeOutFlag = 0;
                    rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;
                    rcvTimeOutEnable = 1;
                    
                    repeatFlag = 0;
                    rcvByteBuffer = 0;
                    rcvShiftCounter = 0;
                    rcvByteFlag = 0;
                    dataFrameCounter = 0;
                    executeFlag = 0;

                    for (uint8_t i = 0; i < DATA_BUFFER_SIZE; i++) {
                        dataFrameBuffer[i] = 0x00;
                    }

                    nFrame = NODE_NEC_CUSTOMERCODE_L;
                } else if (((tbl[1][0] - tbl[1][1]) < edgeCaptureValue) && (edgeCaptureValue < (tbl[1][0] + tbl[1][1]))) {
                    // AEHAフォーマットのリーダー
                    rcvTimeOutFlag = 0;
                    rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;
                    rcvTimeOutEnable = 1;

                    repeatFlag = 0;
                    rcvByteBuffer = 0;
                    rcvShiftCounter = 0;
                    rcvByteFlag = 0;
                    dataFrameCounter = 0;
                    executeFlag = 0;

                    for (uint8_t i = 0; i < DATA_BUFFER_SIZE; i++) {
                        dataFrameBuffer[i] = 0x00;
                    }

                    nFrame = NODE_AEHA_CUSTOMERCODE_L;
                } else if (((tbl[2][0] - tbl[2][1]) < edgeCaptureValue) && (edgeCaptureValue < (tbl[2][0] + tbl[2][1]))) {
                    // NEC,AEHAフォーマットの0
                    rcvByteBuffer = (rcvByteBuffer >> 1);
                    //rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;
                    
                    if (++rcvShiftCounter == 8) {
                        // 1Byte受信
                        rcvShiftCounter = 0;
                        rcvByteFlag = 1;
                    }
                } else if (((tbl[3][0] - tbl[3][1]) < edgeCaptureValue) && (edgeCaptureValue < (tbl[3][0] + tbl[3][1]))) {
                    // NEC,AEHAフォーマットの1
                    rcvByteBuffer = (rcvByteBuffer >> 1) | 0x80;
                    //rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;
                    
                    if (++rcvShiftCounter == 8) {
                        // 1Byte受信
                        rcvShiftCounter = 0;
                        rcvByteFlag = 1;
                    }
                } else if (((tbl[6][0] - tbl[6][1]) < edgeCaptureValue) && (edgeCaptureValue < (tbl[6][0] + tbl[6][1]))) {
                    // NECフォーマットのリピート
                    //rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;
                    repeatFlag = 1;
                } else if (((tbl[7][0] - tbl[7][1]) < edgeCaptureValue) && (edgeCaptureValue < (tbl[7][0] + tbl[7][1]))) {
                    // AEHAフォーマットのリピート
                    //rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;
                    repeatFlag = 1;
                }
            }
        }

        switch (nFrame) {
            case NODE_WAIT:
                // 受信待機
                break;
            case NODE_NEC_CUSTOMERCODE_L:
                if (rcvByteFlag) {
                    // 1バイト受信した
                    rcvByteFlag = 0;
                    dataFrameBuffer[dataFrameCounter] = rcvByteBuffer;

                    if (++dataFrameCounter == DATA_BUFFER_SIZE) {
                        dataFrameCounter = 0;
                        nFrame = NODE_WAIT;
                    }

                    nFrame = NODE_NEC_CUSTOMERCODE_H;
                }
                break;
            case NODE_NEC_CUSTOMERCODE_H:
                if (rcvByteFlag) {
                    // 1バイト受信した
                    rcvByteFlag = 0;
                    dataFrameBuffer[dataFrameCounter] = rcvByteBuffer;

                    if (++dataFrameCounter == DATA_BUFFER_SIZE) {
                        dataFrameCounter = 0;
                        nFrame = NODE_WAIT;
                    }

                    nFrame = NODE_NEC_DATA;
                }
                break;
            case NODE_NEC_DATA:
                if (rcvByteFlag) {
                    // 1バイト受信した
                    rcvByteFlag = 0;
                    dataFrameBuffer[dataFrameCounter] = rcvByteBuffer;

                    if (++dataFrameCounter == DATA_BUFFER_SIZE) {
                        dataFrameCounter = 0;
                        nFrame = NODE_WAIT;
                    }

                    nFrame = NODE_NEC_nDATA;
                }
                break;
            case NODE_NEC_nDATA:
                if (rcvByteFlag) {
                    // 1バイト受信した
                    rcvByteFlag = 0;
                    dataFrameBuffer[dataFrameCounter] = rcvByteBuffer;

                    if (++dataFrameCounter == DATA_BUFFER_SIZE) {
                        dataFrameCounter = 0;
                        nFrame = NODE_WAIT;
                    }

                    nFrame = NODE_RECEIVE_COMPLETE_NEC;
                }
                break;
            case NODE_AEHA_CUSTOMERCODE_L:
                if (rcvByteFlag) {
                    // 1バイト受信した
                    rcvByteFlag = 0;
                    dataFrameBuffer[dataFrameCounter] = rcvByteBuffer;

                    if (++dataFrameCounter == DATA_BUFFER_SIZE) {
                        dataFrameCounter = 0;
                        nFrame = NODE_WAIT;
                    }

                    nFrame = NODE_AEHA_CUSTOMERCODE_H;
                }
                break;
            case NODE_AEHA_CUSTOMERCODE_H:
                if (rcvByteFlag) {
                    // 1バイト受信した
                    rcvByteFlag = 0;
                    dataFrameBuffer[dataFrameCounter] = rcvByteBuffer;

                    if (++dataFrameCounter == DATA_BUFFER_SIZE) {
                        dataFrameCounter = 0;
                        nFrame = NODE_WAIT;
                    }

                    nFrame = NODE_AEHA_PARITY_SYSTEM;
                }
                break;
            case NODE_AEHA_PARITY_SYSTEM:
                if (rcvByteFlag) {
                    // 1バイト受信した
                    rcvByteFlag = 0;
                    dataFrameBuffer[dataFrameCounter] = rcvByteBuffer;

                    if (++dataFrameCounter == DATA_BUFFER_SIZE) {
                        dataFrameCounter = 0;
                        nFrame = NODE_WAIT;
                    }

                    aehaTrailerEnable = 1;
                    nFrame = NODE_AEHA_DATA_N;
                }
                break;
            case NODE_AEHA_DATA_N:
                if (rcvByteFlag) {
                    // 1バイト受信した
                    rcvByteFlag = 0;
                    
                    if (dataFrameCounter > 3)
                    {
                        // データを1Byte以上受信したのでタイムアウトを無効化
                        // 一定時間受信しないのはトレイラーとみなすため
                        rcvTimeOutEnable = 0;
                    }
                    
                    dataFrameBuffer[dataFrameCounter] = rcvByteBuffer;

                    if (++dataFrameCounter == DATA_BUFFER_SIZE) {
                        //dataFrameCounter = 0;
                        nFrame = NODE_RECEIVE_COMPLETE_AEHA; // AEHAの場合は受信完了扱い
                    }
                    else {
                        nFrame = NODE_AEHA_DATA_N;
                        aehaTrailerCounter = AEHA_TRAILER_TIME;
                    }
                }
                break;
            case NODE_RECEIVE_COMPLETE_NEC:
                /*
                rcvTimeOutEnable = 1;
                rcvTimeOutFlag = 0;
                rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;
                */
                // ここでコマンドに応じた処理を行うか､処理部を外に出す
                // 受信バイト数: dataFrameCounter - 1
                // 受信内容: dataFrameBuffer[]              
                executeFlag = 1;
                
#if defined ENABLE_REPEAT                                
                nFrame = NODE_WAIT_FOR_REPEAT;
#else
                rcvTimeOutEnable = 0;
                rcvTimeOutFlag = 0;
                rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;                
                nFrame = NODE_WAIT;
#endif               
                break;
            case NODE_RECEIVE_COMPLETE_AEHA:
                /*
                rcvTimeOutEnable = 1;
                rcvTimeOutFlag = 0;
                rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;
                */ 
                // ここでコマンドに応じた処理を行うか､処理部を外に出す
                // 受信バイト数: dataFrameCounter - 1
                // 受信内容: dataFrameBuffer[]
                
                executeFlag  = 1;
                
                nFrame = NODE_WAIT_FOR_REPEAT;
                break;
            case NODE_TIMEOUT:
                // 受信タイムアウト
                // 受信した内容やカウンタをクリア → 内容はリピート信号で再利用するため
                repeatFlag = 0;
                rcvByteBuffer = 0;
                rcvShiftCounter = 0;
                rcvByteFlag = 0;
                dataFrameCounter = 0;

                for (uint8_t i = 0; i < DATA_BUFFER_SIZE; i++) {
                    dataFrameBuffer[i] = 0x00;
                }
                
                nFrame = NODE_WAIT;
                break;
            case NODE_WAIT_FOR_REPEAT:
                // リピート信号待ち
                // 受信タイムアウトで終了
                if(repeatFlag)
                {
                    repeatFlag = 0;
                    // リピート時の対応
                    rcvTimeOutCounter = RECEIVE_TIMEOUT_10MSEC;
                    rcvTimeOutEnable = 1;
                    rcvTimeOutFlag = 0;
                    
                    // 受信バイト数: dataFrameCounter - 1
                    // 受信内容: dataFrameBuffer[]
                    executeFlag = 1;
                }
                break;
            default:
                break;
        }
        
        // 受信内容に応じた処理を実行
        if(executeFlag)
        {
            executeFlag = 0;
            
            /*
             * # 方針
             * 
             * ## モーター制御
             * 
             * + 前進､後退のコマンドを受け取る
             * + 出力目標値(PWM Duty)をセット
             * + 前進､後退実行フラグをON
             * + 内部インターバルタイマ実行処理を行う
             *   + PWM Dutyをインクリメントまたはデクリメント → Duty更新
             * + パラメーター(目標値)に達したら実行フラグOFF
             * 
             * ### モーター制御中の出力変更
             * + 目標値を更新するだけ
             * 
             * ### 正転変更
             * + これも目標値を更新するだけ
             * + PWM Dutyは予定では9bit｡電源電圧上昇につき､PWM Duty上限は50%とする｡
             * + 実行フラグと正転パラメーター､目標値で管理する
             * + パラメーターはブレーキも含む?
             * 
             * ## 音声再生
             * + EEPROMから音声データを読み込む
             * + 音声データはあるサイズのバッファとする
             * + バッファの容量管理パラメータを付与し､データがあればTMR割り込みでDACのバッファに送信する
             */

            // とりあえずUARTに出力
#ifdef DEBUG_PRINT
            // デバッグ用出力
            for(dbgUARTcnt = 0; dbgUARTcnt < dataFrameCounter; dbgUARTcnt++)
            {
                foo = dataFrameBuffer[dbgUARTcnt];
                EUSART_Write(foo);
                /*
                if (USART_is_tx_ready())
                {
                    EUSART_Write(foo);
                }
                */
            }
#endif       
            // 前回受信したコマンド実行中ではない
            if(cmdInfo.bits.excuteFlag == 0)
            {
                // コマンド判定
                // 受信バイト数が一致
                if(dataFrameCounter == RECEIVE_DATA_SIZE)
                {
                    // 前半2バイトを正常に受信
                    if((dataFrameBuffer[0] ^ dataFrameBuffer[1]) == 0xFF)
                    {
                        // 後半2バイトを正常に受信
                        if((dataFrameBuffer[2] ^ dataFrameBuffer[3]) == 0xFF)
                        {
                            // 押されたボタンに対応するコマンドを設定
                            switch(dataFrameBuffer[2])
                            {
                                case RCCODE_PWR:
                                    /*
                                     * スリープ
                                     * ･ モータードライバ: アイドル↔スタンバイ
                                     * ･ LED: 消灯↔点灯
                                     * ･ 音声再生: 停止
                                     * ･ マイコン: スリープする
                                     */
                                    break;
                                case RCCODE_UP:
                                    if(status == 0)
                                    {
                                        // 停車中なら前進中速で開始
                                        pwmDutyTblIndex = 2;
                                        pwmDutyTarget = pwmDutyTbl[pwmDutyTblIndex];
                                        pwmDuty = PWM_MOTOR_MIN;
                                        PWM3_UpdateDuty(0x0000); // どっちがどっちかは後ほど
                                        PWM4_UpdateDuty(pwmDuty);
                                        status = 1; // 前進

                                        dimmerLEDFlag = 1;
                                        dimmerLEDcount = 0;
                                        dimmerLEDdirFlag = 0;
                                        dimmerLEDPWMDuty = 0;

                                        cmdInfo.bits.excuteCmd = IRCMD_UP; // アップボタン押した
                                        cmdInfo.bits.excuteFlag = 1;
                                    }
                                    else if (status == 1)
                                    {
                                        // 増速
                                        if(pwmDutyTblIndex < 4)
                                        {
                                            pwmDutyTblIndex++;
                                            pwmDutyTarget = pwmDutyTbl[pwmDutyTblIndex]; // 目標値設定

                                            dimmerLEDcount = 0;
                                            dimmerLEDdirFlag = 0;
                                            dimmerLEDPWMDuty = 0;

                                            cmdInfo.bits.excuteCmd = IRCMD_UP; // アップボタン押した
                                            cmdInfo.bits.excuteFlag = 1;
                                        }
                                    }
                                    else if (status == 2)
                                    {
                                        // 減速
                                        if(pwmDutyTblIndex > 0)
                                        {
                                            pwmDutyTblIndex--;
                                            pwmDutyTarget = pwmDutyTbl[pwmDutyTblIndex]; // 目標値設定

                                            dimmerLEDcount = 0;
                                            dimmerLEDdirFlag = 0;
                                            dimmerLEDPWMDuty = 0;

                                            cmdInfo.bits.excuteCmd = IRCMD_UP; // アップボタン押した
                                            cmdInfo.bits.excuteFlag = 1;
                                        }
                                    }
                                    break;
                                case RCCODE_DOWN:
                                    if(status == 0)
                                    {
                                        // 停車中なら後進中速で開始
                                        pwmDutyTblIndex = 2;
                                        pwmDutyTarget = pwmDutyTbl[pwmDutyTblIndex];
                                        pwmDuty = PWM_MOTOR_MIN;
                                        PWM4_UpdateDuty(0x0000); // どっちがどっちかは後ほど
                                        PWM3_UpdateDuty(pwmDuty);
                                        status = 2; // 後進

                                        dimmerLEDFlag = 1;
                                        dimmerLEDcount = 0;
                                        dimmerLEDdirFlag = 0;
                                        dimmerLEDPWMDuty = 0;

                                        cmdInfo.bits.excuteCmd = IRCMD_DOWN; // ダウンボタン押した
                                        cmdInfo.bits.excuteFlag = 1;
                                    }
                                    else if (status == 1)
                                    {
                                        // 減速
                                        if(pwmDutyTblIndex > 0)
                                        {
                                            pwmDutyTblIndex--;
                                            pwmDutyTarget = pwmDutyTbl[pwmDutyTblIndex]; // 目標値設定

                                            dimmerLEDcount = 0;
                                            dimmerLEDdirFlag = 0;
                                            dimmerLEDPWMDuty = 0;

                                            cmdInfo.bits.excuteCmd = IRCMD_DOWN; // ダウンボタン押した
                                            cmdInfo.bits.excuteFlag = 1;
                                        }
                                    }
                                    else if (status == 2)
                                    {
                                        // 増速
                                        if(pwmDutyTblIndex < 4)
                                        {
                                            pwmDutyTblIndex++;
                                            pwmDutyTarget = pwmDutyTbl[pwmDutyTblIndex]; // 目標値設定

                                            dimmerLEDcount = 0;
                                            dimmerLEDdirFlag = 0;
                                            dimmerLEDPWMDuty = 0;

                                            cmdInfo.bits.excuteCmd = IRCMD_DOWN; // アップボタン押した
                                            cmdInfo.bits.excuteFlag = 1;
                                        }
                                    }
                                    break;
                                case RCCODE_SOUND1:
                                    /*
                                     * 発進後のLED点灯パターンを1に
                                     */
                                    break;
                                case RCCODE_SOUND2:
                                    /*
                                     * 発進後のLED点灯パターンを2に
                                     */
                                    break;
                                case RCCODE_SOUND3:
                                    /*
                                     * 発進後のLED点灯パターンを3に
                                     */
                                    break;
                                case RCCODE_STOP:
                                    if(status != 0)
                                    {
                                        cmdInfo.bits.excuteCmd = IRCMD_STOP;
                                        cmdInfo.bits.excuteFlag = 1;
                                        dimmerLEDFlag = 0; // LEDをスタンバイ時に
                                        dimmerLEDcount = 0x00;
                                        dimmerLEDdirFlag = 0x00; // 0:増える
                                        dimmerLEDPWMDuty = PWM_LED_DIM_STANDBY; // PWMデューティ
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }    
                    }
                }
            }
                
            // 実行完了したので受信内容を初期化
            dataFrameCounter = 0;
            for (uint8_t clearCnt = 0; clearCnt < DATA_BUFFER_SIZE; clearCnt++) {
                dataFrameBuffer[clearCnt] = 0x00;
            }

            nFrame = NODE_WAIT;
            
        }
        
        if(sleepFlag)
        {
            // スリープ制御
            /*
             * 割り込みでスリープ解除後に割り込みルーチンを実行しない場合は
             * グローバル割り込みをOFFにする｡
             * スリープ解除に用いるペリフェラル割り込みはON､用いないペリフェラル
             * 割り込みはOFFにすること｡
             */
            // スリープ復帰にEXT INTをつかい､赤外線信号入力を割り当てる
            sleepExitFlag = 1;
            ExtIntInterruptEnable(); // 割り込み有効化
            GlobalInterruptDisable(); // スリープ復帰時に割り込みルーチンへ飛ばない
            CLRWDT(); // 事前にWDTカウンタをクリア
            SLEEP(); // スリープ実行
            NOP(); // PC+1のためのNOP

            if (STATUSbits.nPD) {
                // スリープ直前に割り込みが発生し､スリープが実行されなかった
                ExtIntInterruptDisable(); // 割り込み無効化
                sleepExitFlag = 0;
            }
        }


    }

    return;
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
