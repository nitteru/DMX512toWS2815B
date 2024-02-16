/*
 * File:   device_config.c
 * Author: Teruyuki
 *
 * Created on 2023/10/03
 */


#include <xc.h>
#include "device_config.h"

/*
 * Configの内容はXC8コンパイラのドキュメントを参照すること
 * 
 * file:///C:/Program%20Files/Microchip/xc8/v2.45/docs/chips/18f27q43.html
 */
#pragma config FEXTOSC = HS
#pragma config RSTOSC = EXTOSC_4PLL
#pragma config CLKOUTEN = OFF
#pragma config PR1WAY = ON
#pragma config CSWEN =  ON
#pragma config FCMEN = ON
#pragma config MCLRE = EXTMCLR
#pragma config PWRTS = PWRT_OFF
#pragma config MVECEN = ON
#pragma config IVT1WAY = ON
#pragma config LPBOREN = OFF
#pragma config BOREN = ON
#pragma config BORV = VBOR_1P9
#pragma config ZCD = OFF
#pragma config PPS1WAY = ON
#pragma config STVREN = ON
#pragma config LVP = ON
#pragma config XINST = OFF
#pragma config WDTCPS = WDTCPS_31
#pragma config WDTE = SWDTEN
#pragma config WDTCWS = WDTCWS_7
#pragma config WDTCCS = LFINTOSC
#pragma config BBSIZE = BBSIZE_512
#pragma config BBEN = OFF
#pragma config SAFEN = OFF
#pragma config DEBUG = OFF
#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD	= OFF
#pragma config WRTSAF = OFF
#pragma config WRTAPP = OFF
#pragma config CP = OFF

void SYSTEM_Initialize(void)
{
    OSC_Initialize();
    PMD_Initialize();
    WDT_Initialize();
    PPS_Initialize(); // PPS初期化
    PIN_Initialize();
    Interrupt_Initialize();
    
    TMR0_Initialize(); // インターバルタイマ用
    
    CLC_Initialize();

    TMR1_Initialize(); // 磁気信号パルス幅測定用
    CCP1_Initialize(); // 磁気信号パルス幅測定用
    
    SPI1_Initialize();
    
    PWM1_Initialize(); // LED R調光用
    PWM2_Initialize(); // LED G調光用
    PWM3_Initialize(); // LED B調光用

    CLKREF_Initialize(); // 負電源PWM用

    FVR_Initialize(); // ソーラーパネル出力･バッテリー電圧監視 ADC用基準電圧源
    ADC_Initialize(); // ソーラーパネル出力･バッテリー電圧監視
    
    NVM_Initialize(); // EEPROMアクセス用

    DAC_Initialize(); // 音声出力用
    TMR4_Initialize(); // DACバッファ更新用インターバルタイマ
}