/*
 * File:   device_config.c
 * Author: Teruyuki
 *
 * Created on 2023/05/19, 12:37
 */


#include <xc.h>
#include "device_config.h"

// CONFIG1
#pragma config FOSC = INTOSC // INTOSC oscillator: I/O function on CLKIN pin
#ifdef __DEBUG
#pragma config WDTE = OFF // WDT enabled
#pragma config PWRTE = OFF // PWRT enabled
#else
#pragma config WDTE = ON // WDT enabled
#pragma config PWRTE = ON // PWRT enabled
#endif
#pragma config MCLRE = ON // MCLR/VPP pin function is MCLR
#pragma config CP = OFF // Program memory code protection is disabled
#pragma config BOREN = ON // Brown-out Reset enabled
#pragma config CLKOUTEN = OFF // CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
#pragma config IESO = OFF // Internal/External Switchover mode is enabled
#pragma config FCMEN = OFF // Fail-Safe Clock Monitor is disabled

// CONFIG2
#pragma config WRT = OFF // Write protection off
#pragma config PPS1WAY = OFF // The PPSLOCK bit can be set and cleared repeatedly by software
#pragma config ZCDDIS = ON // 	Zero-cross detect circuit is enabled at POR
#ifdef OSC_CLK_32MHZ
#pragma config PLLEN = ON // 4x PLL enabled
#else
#pragma config PLLEN = OFF // 4x PLL disabled
#endif

#pragma config STVREN = ON // 	Stack Overflow or Underflow will cause a Reset
#pragma config BORV = LO // Brown-out Reset Voltage (Vbor), low trip point selected.
#pragma config LPBOR = ON // Low-Power BOR is enabled
#pragma config LVP = ON // Low-voltage programming enabled

void SYSTEM_Initialize(void)
{
    OSC_Initialize();
    WDT_Initialze();
    PPS_Initialize();
    PIN_Initialize();
    //EXTINT_Initialize(); // 未使用
    TMR0_Initialize(); // 内部インターバルタイマー
    //TMR2_Initialize(); // モーター用PWM
    //PWM_Initialize(); // PWM3,4の初期化 (TMRの初期化も同時に行う)
    //FVR_Initialize(); // DAC用基準電圧
    TMR6_Initialize(); // DAC更新用
    //DAC_Initialize(); // 音声再生用
    //TMR1_Initialize(); // Capture用
    //CCP1_Initialize(); // CCP1 IR信号Capture 初期化 (モードはccp1.hで設定)
    //TMR4_Initialize(); // LED用PWM
    //CCP2_Initialize(); // CCP2 PWM 初期化 (モードはccp2.hで設定)
    //OPAMP1_Initialize(); // OPAMP1 初期化
    //OPAMP2_Initialize(); // OPAMP2 初期化
    EUSART_Initialize(); // EUSART(UART) 初期化
    //SSP1_Initialize(); // MSSP1 初期化 (モードはssp1.hで設定)
}