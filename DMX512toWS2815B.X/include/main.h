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

// デバッグ等
#define DEBUG_PRINT

// クロック設定
#define OSC_CLK_32MHZ // 32MHz, (INTOSC 8MHz * x4 PLL)
//#define OSC_CLK_16MHZ // 16MHz, (INTOSC 16MHz)
//#define OSC_CLK_8MHZ // 8MHz, (INTOSC 8MHz)

// 動作制御スイッチ
#define ENABLE_REPEAT // リピートコマンドを認識する

// 内部インターバルタイマ
#define INTERVAL_TMR_10MSEC 2
#define INTERVAL_TMR_100MSEC 10
#define INTERVAL_TMR_200MSEC 20
#define INTERVAL_TMR_500MSEC 50
#define INTERVAL_TMR_1SEC 100

// モーターPWM制御
#define PWM_MOTOR_MAX (uint16_t)0x01FF // 最大出力
#define PWM_MOTOR_MIN (uint16_t)0x00A0 // 最低出力
#define PWM_MOTOR_NORMAL (uint16_t)0x0180 // 停車から発進時の初期値
#define PWM_MOTOR_INCREMENT_STEP (uint16_t)0x0002 // 発車時のPWMデューティ変化量*10msec(予定)
#define PWM_MOTOR_DECREMENT_STEP (uint16_t)0x0002 // 停車時のPWMデューティ変化量*10msec(予定)

// LED PWM制御
#define PWM_LED_MAX (uint16_t)0x03FF // 最大出力
#define PWM_LED_MIN (uint16_t)0x0000 // 最小出力
#define PWM_LED_DIM_STANDBY (uint16_t)0x003F // スタンバイ時のLED調光

// 赤外線信号
#define DATA_BUFFER_SIZE (uint8_t)32 // 受信データバッファサイズ
#define RECEIVE_TIMEOUT_10MSEC (uint8_t)15 // 受信タイムアウト時間 * 10msec
#define AEHA_TRAILER_TIME (uint8_t)1 // AEHAフォーマット トレイラー時間(8msec以上) * 10msec

#define RECEIVE_DATA_SIZE (uint8_t)4 // 受信データサイズ(コマンド判定用)

/*
 * リモコンコードとコマンドの関係
 * リモコンコード4バイト中の3バイト目を用いる
 */
#define RCCODE_PWR (uint8_t)0xD8 // スリープ
#define RCCODE_UP (uint8_t)0xA0 // アップ
#define RCCODE_DOWN (uint8_t)0x00 // ダウン
#define RCCODE_SOUND1 (uint8_t)0xF8 // 音 汽笛
#define RCCODE_SOUND2 (uint8_t)0x78 // 音
#define RCCODE_SOUND3 (uint8_t)0x58 // 音
#define RCCODE_STOP (uint8_t)0x20 // 停車

// インターバルタイマ カウンタ
typedef union
{
    uint8_t A[6];
    struct
    {
        // LSB
        uint8_t counter5msec;
        uint8_t counter10msec;
        uint8_t counter100msec;
        uint8_t counter200msec;
        uint8_t counter500msec;
        uint8_t counter1sec;
        // MSB
    } fields;
} intervalTimerCounters;

// インターバルタイマ フラグ
typedef union
{
    uint8_t A;
    struct
    {
        // LSB
        uint8_t flag5msec:1;
        uint8_t flag10msec:1;
        uint8_t flag100msec:1;
        uint8_t flag200msec:1;
        uint8_t flag500msec:1;
        uint8_t flag1sec:1;
        // MSB
    } fields;
} intervalTimerFlags;

// リセット要因情報
typedef union {
    uint8_t byte;

    struct {
        uint8_t dmy : 1; //LSB
        uint8_t hot : 1;
        uint8_t so : 1;
        uint8_t su : 1;
        uint8_t rmclr : 1;
        uint8_t ri : 1;
        uint8_t bor : 1;
        uint8_t por : 1; //MSB
    } bits;
} reset_type;

// コマンド実行情報
typedef union {
    uint16_t word;
    
    struct {
        uint8_t excuteFlag :1; // 実行フラグ LSB
        uint8_t mode :1; // 実行状態 0:実行完了/待ち, 1:実行中
        uint8_t reserve :2; // 予約 
        uint8_t excuteCmd :4; // コマンド種別 0-7
        uint8_t arg:8; // コマンドパラメータ
    } bits;
} commandInfo;

enum IRCmd
{
    IRCMD_STOP, // 停車
    IRCMD_UP, // 増速
    IRCMD_DOWN, // 減速
    IRCMD_WHISTLE // 汽笛再生
};

enum nodeFrame
{
    NODE_WAIT, // 受信待機
    // 以下､リーダーを見てフォーマットを判断
    NODE_NEC_CUSTOMERCODE_L, // カスタマーコード下位 (NEC)
    NODE_NEC_CUSTOMERCODE_H, // カスタマーコード上位 (NEC)
    NODE_NEC_DATA, // データ (NEC)
    NODE_NEC_nDATA, // 反転データ (NEC)
    NODE_NEC_REPEAT, // リピートコード (NEC)
    NODE_AEHA_CUSTOMERCODE_L, // カスタマーコード下位 (AEHA)
    NODE_AEHA_CUSTOMERCODE_H, // カスタマーコード上位 (AEHA)
    NODE_AEHA_PARITY_SYSTEM, // パリティ4ビット+システムコード4ビット (AEHA)
    NODE_AEHA_DATA_N, // データ 送信バイト数は任意
    NODE_TIMEOUT, // 受信タイムアウト
    NODE_RECEIVE_COMPLETE_NEC, // NECフォーマット受信完了
    NODE_RECEIVE_COMPLETE_AEHA, // AEHAフォーマット受信完了
            // AEHAはトレーラ(1T, 8ms以上の空白)をもって終端とする
            // NECは規定バイト数受信したら完了とする
    NODE_WAIT_FOR_REPEAT // リピート待ち → タイムアウトで終了
};

// 受信したデータ
enum receiveData
{
    NEC_LEADER, // NECフォーマットリーダー
    AEHA_LEADER, // AEHAフォーマットリーダー
};


extern intervalTimerFlags intvTmrFlg; // インターバルタイマフラグ
extern uint16_t edgeCaptureValue; // キャプチャした値
extern uint8_t isCaptured;
extern uint8_t captureTimerOverflow; // キャプチャタイマーOFフラグ

void calIntervalTimer(void);


/*
 * 受信解析全般
 * 
 * 変調単位と各フィールドの誤差を含めた判定の方法
 * 16bitタイマー TMR1を使用
 * 各フィールド､ビットのtyp幅と誤差範囲を2次元配列に入れて判定
 * → フィールド固定のNECフォーマットには使えそう｡AEHAは可変長フレームなので最後の見つけ方を工夫する?
 * LSBから送信される
 */

/*
 * NECフォーマット解析用
 * 変調
 *   950nm, サブキャリア 38kHz typ 1/3duty
 * Frame
 *   Leader, Customer Code (8bit), Customer Code INV (8bit), Data (8bit), Data INV (8bit), Stop bit (1T)
 * Repeat
 *   16T On, 4T Off, Stop bit (1T)
 * Transmission
 *   FrameとRepeat間は108msec
 * 
 * 変調単位の±30%を読み取る 0.392～0.728msec (±168usec) → TMR1分解能 2usec前後?, 16bitタイマーで125msec前後まで測定可能
 */

/*
 * 家製協(AEHA)フォーマット解析用
 * 変調
 *   950nm, サブキャリア 38kHz typ 1/3duty 
 * Frame
 *   Leader, Customer Code (16bit), パリティ カスタマーコードを4bitずつXOR (4bit), Data0 (8bit), Data N (8bit), Stop bit (1T), Trailer
 *   N: 6 (48bit) typ
 *   Trailer 8msec以上の無送信区間?
 * Repeat
 *   8T On, 8T Off, Stop bit (1T)
 * Transmission
 *   FrameとRepeat間は130msec typ (規定はされていない)
 * 
 * 0.425msec±18%(±75usec)を範囲とする?
 */