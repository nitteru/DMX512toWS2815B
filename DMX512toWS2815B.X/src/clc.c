/*
 * File:   clc.c
 * Author: nitte
 *
 * Created on December 16, 2023, 12:47 PM
 */


#include <xc.h>
#include "clc.h"
#include "main.h"

void CLC_Initialize(void)
{
    /*
     * CLC2
     */
    // SLCT 0x1; 
    CLCSELECT = 0x1;
    // LCG1POL not_inverted; LCG2POL inverted; LCG3POL inverted; LCG4POL inverted; LCPOL not_inverted; 
    CLCnPOL = 0xE;
    // LCD1S CLCIN2 (CLCIN2PPS); 
    CLCnSEL0 = 0x2;
    // LCD2S FOSC; 
    CLCnSEL1 = 0x8;
    // LCD3S CCP1 (PWM); 
    CLCnSEL2 = 0x1F;
    // LCD4S LFINTOSC; 
    CLCnSEL3 = 0xA;
    // LCG1D1N disabled; LCG1D1T disabled; LCG1D2N disabled; LCG1D2T disabled; LCG1D3N disabled; LCG1D3T enabled; LCG1D4N disabled; LCG1D4T disabled; 
    CLCnGLS0 = 0x20;
    // LCG2D1N disabled; LCG2D1T disabled; LCG2D2N disabled; LCG2D2T disabled; LCG2D3N disabled; LCG2D3T disabled; LCG2D4N disabled; LCG2D4T disabled; 
    CLCnGLS1 = 0x0;
    // LCG3D1N disabled; LCG3D1T disabled; LCG3D2N disabled; LCG3D2T disabled; LCG3D3N disabled; LCG3D3T disabled; LCG3D4N disabled; LCG3D4T disabled; 
    CLCnGLS2 = 0x0;
    // LCG4D1N disabled; LCG4D1T disabled; LCG4D2N disabled; LCG4D2T disabled; LCG4D3N disabled; LCG4D3T disabled; LCG4D4N disabled; LCG4D4T disabled; 
    CLCnGLS3 = 0x0;
    // LCOUT 0x00; 
    CLCDATA = 0x0;
    // LCMODE 4-input AND; LCINTN disabled; LCINTP disabled; LCEN enabled; 
    CLCnCON = 0x82;

    /*
     * CLC4
     */
    // SLCT 0x3; 
    CLCSELECT = 0x3;
    // LCG1POL inverted; LCG2POL not_inverted; LCG3POL not_inverted; LCG4POL not_inverted; LCPOL not_inverted; 
    CLCnPOL = 0x1;
    // LCD1S CLCIN4 (CLCIN4PPS); 
    CLCnSEL0 = 0x4;
    // LCD2S CLC2_OUT; 
    CLCnSEL1 = 0x34;
    // LCD3S SPI1 SDO OUT; 
    CLCnSEL2 = 0x40;
    // LCD4S SPI1 SCK OUT; 
    CLCnSEL3 = 0x41;
    // LCG1D1N disabled; LCG1D1T disabled; LCG1D2N enabled; LCG1D2T disabled; LCG1D3N disabled; LCG1D3T disabled; LCG1D4N enabled; LCG1D4T disabled; 
    CLCnGLS0 = 0x44;
    // LCG2D1N disabled; LCG2D1T disabled; LCG2D2N disabled; LCG2D2T disabled; LCG2D3N enabled; LCG2D3T disabled; LCG2D4N disabled; LCG2D4T disabled; 
    CLCnGLS1 = 0x10;
    // LCG3D1N disabled; LCG3D1T disabled; LCG3D2N disabled; LCG3D2T disabled; LCG3D3N disabled; LCG3D3T disabled; LCG3D4N disabled; LCG3D4T enabled; 
    CLCnGLS2 = 0x80;
    // LCG4D1N disabled; LCG4D1T disabled; LCG4D2N disabled; LCG4D2T disabled; LCG4D3N disabled; LCG4D3T enabled; LCG4D4N disabled; LCG4D4T disabled; 
    CLCnGLS3 = 0x20;
    // LCOUT 0x00; 
    CLCDATA = 0x0;
    // LCMODE AND-OR; LCINTN disabled; LCINTP disabled; LCEN enabled; 
    CLCnCON = 0x80;
    
#if 0
    /*
     * Initialize PIC16(L)F1509 CLC2, CLC4, Timer2, and MSSP
     * for WS2811 signal transmission protocol
     * Loading SSP1BUF register sends that data byte out the RC4/CLC4 pin
     * 
     * Clock (Fosc): 16MHz
     * 低速ビット時間: 2.5usec
     */
    
    /*
     * PWM1 routed straight through CLC2
     * 
     * Gate 1 Data 3に非反転(True)でPWM1OUTを接続｡ほかは適当に割当て｡
     * 残りのGateは割り当てない｡
     * Gate 2-4の出力は反転する(未割り当ては0なので1にして4-input ANDに入力するため)
     */
    CLC2GLS0 = 0x00100000; // LC2G1D3T: lxcd3T is gated into lcxg1
    CLC2GLS1 = 0x00000000; // No data selection output is gated into lxcg2
    CLC2GLS2 = 0x00000000; // No data selection output is gated into lxcg3
    CLC2GLS3 = 0b00000000; // No data selection output is gated into lxcg4
    CLC2SEL0 = 0b00000000; // Input Data2: LC2_in[4] (Fosc) is selected for lcxd2, Input Data1: LC2_in[0] (CLC2IN0) is selectd for lcxd1
    CLC2SEL1 = 0b00000110; // Input Data4: LC2_in[12] (LFINTOSC) is selected for lcxd4, Input Data1: LC2_in[14] (PWM1OUT) is selectd for lcxd3
    CLC2POL = 0b00001110; // Output is not inverted, lcxg2-4 outputs of the gate is inverted
    CLC2CON = 0b10000010; // CLC Enable, Port Pin Output Disable, Rising edgie Interrupt Disable, Falling edge Interrupt Didable, Cell is 4-input AND
    
    /*
     * (SPI SDO & SPI CLK) || (nSPI SDO & SPI CLK & PWM1) 
     * PWM1 comes through CLC2
     * 
     * Gate1: SCKの反転とPWM1OUTの反転をOR, 出力を反転してANDと同等
     * Gate2: SDOの反転
     * Gate3: SCKの非反転
     * Gate4: SDOの非反転
     * 
     * Gate1､2をAND : (nSPI SDO & SPI CLK & PWM1) 
     * Gate3､4をAND : (SPI SDO & SPI CLK)
     * AND-ORロジックで最終結果が得られる
     */
    CLC4GLS0 = 0b01000100; // LC4G1D4N: lcxd4N is gated into lcxg1, LC4G1D2N: lcxd2N is gated into lcxg1
    CLC4GLS1 = 0b00010000; // LC4G2D3N: lcxd3N is gated into lcxg2
    CLC4GLS2 = 0b10000000; // LC4G3D4T: lcxd4T is gated into lcxg3
    CLC4GLS3 = 0b00100000; // LC4G4D3T: lcxd3T is gated into lcxg4
    CLC4SEL0 = 0b01010000; // Input Data 2: LC4_in[9] (CLC2_OUT, PWM1OUT) is selected for lcxd2, Input Data 1: CLC4_in[0] (CLC4IN0) is selected for lcxd1
    CLC4SEL1 = 0b00000101; // Input Data 4: LC4_in[12] (SCK_OUT) is selected for lcxd4, Input Data 3: LC4_in[13] (SDO_OUT) is selected for lcxd3
    CLC4POL = 0b00000001; // The output of gate 1 is inverted, Gate 2-4 Polarity is not inverted
    CLC4CON = 0b11000000; // CLC Enable, Port Pin Output Enable, Interrupt Disable, Cell is AND-OR

    /*
     * Adjust Timer2 period for desired baud rate
     * One bit period is two Timer2 periods
     * 
     * Timer2 clock source is Fosc/4
     */
    T2CON = 0b00000100; // 1:1 Output Postscaler, Timer 2 is on, Clock Prescaler is 1
    PR2 = 5; // (2.5usec / 2) * (16MHz/4)

#endif
}

void CLC2_Enable(void) 
{
    // SLCT 0x1; 
    CLCSELECT = 0x1;
    CLCnCONbits.EN = 1;
}

void CLC2_Disable(void) 
{
    // SLCT 0x1; 
    CLCSELECT = 0x1;
    CLCnCONbits.EN = 0;
}


uint8_t CLC2_OutputStatusGet(void) 
{
    // SLCT 0x1; 
    CLCSELECT = 0x1;
    return(CLCDATAbits.CLC2OUT);
}

void CLC4_Enable(void) 
{
    // SLCT 0x3; 
    CLCSELECT = 0x3;
    CLCnCONbits.EN = 1;
}

void CLC4_Disable(void) 
{
    // SLCT 0x3; 
    CLCSELECT = 0x3;
    CLCnCONbits.EN = 0;
}

uint8_t CLC4_OutputStatusGet(void) 
{
    // SLCT 0x3; 
    CLCSELECT = 0x3;
    return(CLCDATAbits.CLC4OUT);
}