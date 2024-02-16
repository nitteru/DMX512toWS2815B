/*
 * File:   pin.c
 * Author: nishi
 *
 * Created on July 14, 2022, 3:07 PM
 */

#include <xc.h>
#include "pin.h"

void PIN_Initialize(void)
{
    // Output Latch
    LATA = 0b00000000;
    LATB = 0b00000000;
    LATC = 0b00000000;

    /*
     * Direction Control, 0:Output, 1:Input
     * Reset 1:Input
     *
     * 省電力のため未使用ピンはOutput Lowに設定すること
     */
    TRISA = 0b11011000; // RA7:EX CLK, RA6:GPIO, RA5:GPIO, RA4:GPIO RA3:GPIO, RA2:GPIO, RA1:GPIO, RA0:ADC 
    TRISB = 0b11000000; // RB7:ICSPDAT, RB6:ICSPCLK, RB5:NoUse, RB4:NoUse, RB3:NoUse, RB2:GPIO, RB1:GPIO, RB0:GPIO
    TRISC = 0b00100000; // RC7:GPIO, RC6:CLC4, RC5:UART RX, RC4:GPIO, RC3:GPIO, RC2:PWM3, RC1:PWM2 RC0:PWM1

    /*
     * Analog Control, 0:Diaital, 1:Analog
     * Reset 1:Analog
     */
    ANSELA = 0b00000001; // RA0:ADC
    ANSELB = 0b00000000;
    ANSELC = 0b00000000;

    /*
     * Weak Pull-Up Control, 0:Disable, 1:Enable
     * Reset 0:Disable
     */
    WPUA = 0x00;
    WPUB = 0x00;
    WPUC = 0x00;

    /*
     * Input Threshold Control, 0:TTL, 1:Schmitt trigger
     * Reset 1:Schmitt trigger
     */
    INLVLA = 0xFF;
    INLVLB = 0xFF;
    INLVLC = 0xFF;

    /*
     * Slew Rate Control, 0:Maximum Rate, 1:Limited Rate
     * Reset 1:Limited Rate
     */
    SLRCONA = 0xFF;
    SLRCONB = 0xFF;
    SLRCONC = 0xFF;

    /*
     * Open-Drain Control, 0:Push-Pull Drive, 1:Open-Drain Drive
     * Reset 0:Push-Pull Drive
     */
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x00;

    // Interrupt-On-Change, 0:Disable, 1:Enable
    // Negative Edge, Reset 0:Disable
    IOCAN = 0x00;
    IOCBN = 0x00;
    IOCCN = 0x00;
    // Positive Edge, Reset 0:Disable
    IOCAP = 0x00;
    IOCBP = 0x00;
    IOCCP = 0x00;
    // Flag, Reset 0
    IOCAF = 0x00;
    IOCBF = 0x00;
    IOCCF = 0x00;
}

/*
 * RA1, GPIO Output, PWELED
 */
static inline void PWELED_SetHigh(void)
{
    LATAbits.LATA1 = 1;
}

static inline void PWELED_SetLow(void)
{
    LATAbits.LATA1 = 0;
}

static inline void PWELED_Toggle(void)
{
    LATAbits.LATA1 = ~LATAbits.LATA1;
}

static inline uint8_t PWELED_GetValue(void)
{
    return PORTAbits.RA1; 
}

/*
 * RA2, GPIO Output, SIGSTS
 */
static inline void SIGSTS_SetHigh(void)
{
    LATAbits.LATA2 = 1;
}

static inline void SIGSTS_SetLow(void)
{
    LATAbits.LATA2 = 0;
}

static inline void SIGSTS_Toggle(void)
{
    LATAbits.LATA2 = ~LATAbits.LATA2;
}

static inline uint8_t SIGSTS_GetValue(void)
{
    return PORTAbits.RA2; 
}

/*
 * RA3, GPIO Output, ISOPSEN
 */
static inline void ISOPSEN_SetHigh(void)
{
    LATAbits.LATA3 = 1;
}

static inline void ISOPSEN_SetLow(void)
{
    LATAbits.LATA3 = 0;
}

static inline void ISOPSEN_Toggle(void)
{
    LATAbits.LATA3 = ~LATAbits.LATA3;
}

static inline uint8_t ISOPSEN_GetValue(void)
{
    return PORTAbits.RA3; 
}

/*
 * RA4, GPIO input, RSTSW
 */
static inline void RSTSW_SetHigh(void)
{
    LATAbits.LATA4 = 1;
}

static inline void RSTSW_SetLow(void)
{
    LATAbits.LATA4 = 0;
}

static inline void RSTSW_Toggle(void)
{
    LATAbits.LATA4 = ~LATAbits.LATA4;
}

static inline uint8_t RSTSW_GetValue(void)
{
    return PORTAbits.RA4;
}

/*
 * RA5, GPIO output, DMXOUTEN
 */
static inline void DMXOUTEN_SetHigh(void)
{
    LATAbits.LATA5 = 1;
}

static inline void DMXOUTEN_SetLow(void)
{
    LATAbits.LATA5 = 0;
}

static inline void DMXOUTEN_Toggle(void)
{
    LATAbits.LATA5 = ~LATAbits.LATA5;
}

static inline uint8_t DMXOUTEN_GetValue(void)
{
    return PORTAbits.RA5;
}

/*
 * RA6, GPIO input, PSMODESW
 */
static inline void PSMODESW_SetHigh(void)
{
    LATAbits.LATA6 = 1;
}

static inline void PSMODESW_SetLow(void)
{
    LATAbits.LATA6 = 0;
}

static inline void PSMODESW_Toggle(void)
{
    LATAbits.LATA6 = ~LATAbits.LATA6;
}

static inline uint8_t PSMODESW_GetValue(void)
{
    return PORTAbits.RA6;
}


/*
 * RB0, GPIO Output, OSIGP
 */
static inline void OSIGP_SetHigh(void)
{
    LATBbits.LATB0 = 1;
}

static inline void OSIGP_SetLow(void)
{
    LATBbits.LATB0 = 0;
}

static inline void OSIGP_Toggle(void)
{
    LATBbits.LATB0 = ~LATBbits.LATB0;
}

static inline uint8_t OSIGP_GetValue(void)
{
    return PORTBbits.RB0;
}

/*
 * RB1, GPIO Output, OSIGN
 */
static inline void OSIGN_SetHigh(void)
{
    LATBbits.LATB1 = 1;
}

static inline void OSIGN_SetLow(void)
{
    LATBbits.LATB1 = 0;
}

static inline void OSIGN_Toggle(void)
{
    LATBbits.LATB1 = ~LATBbits.LATB1;
}

static inline uint8_t OSIGN_GetValue(void)
{
    return PORTBbits.RB1;
}

/*
 * RB2, GPIO Output, PSOUTEN
 */
static inline void PSOUTEN_SetHigh(void)
{
    LATBbits.LATB2 = 1;
}

static inline void PSOUTEN_SetLow(void)
{
    LATBbits.LATB2 = 0;
}

static inline void PSOUTEN_Toggle(void)
{
    LATBbits.LATB2 = ~LATBbits.LATB2;
}

static inline uint8_t PSOUTEN_GetValue(void)
{
    return PORTBbits.RB2;
}

/*
 * RB3, GPIO Output, TP1
 */
static inline void TP1_SetHigh(void)
{
    LATBbits.LATB3 = 1;
}

static inline void TP1_SetLow(void)
{
    LATBbits.LATB3 = 0;
}

static inline void TP1_Toggle(void)
{
    LATBbits.LATB3 = ~LATBbits.LATB3;
}

static inline uint8_t TP1_GetValue(void)
{
    return PORTBbits.RB3;
}

/*
 * RB4, GPIO Input, TP2
 */
static inline void TP2_SetHigh(void)
{
    LATBbits.LATB4 = 1;
}

static inline void TP2_SetLow(void)
{
    LATBbits.LATB4 = 0;
}

static inline void TP2_Toggle(void)
{
    LATBbits.LATB4 = ~LATBbits.LATB4;
}

static inline uint8_t TP2_GetValue(void)
{
    return PORTBbits.RB4;
}

/*
 * RB5, GPIO Output, TP3
 */
static inline void TP3_SetHigh(void)
{
    LATBbits.LATB5 = 1;
}

static inline void TP3_SetLow(void)
{
    LATBbits.LATB5 = 0;
}

static inline void TP3_Toggle(void)
{
    LATBbits.LATB5 = ~LATBbits.LATB5;
}

static inline uint8_t TP3_GetValue(void)
{
    return PORTBbits.RB5;
}


/*
 * RC0, PWM, PWMR
 */
static inline void PWMR_SetHigh(void)
{
    LATCbits.LATC0 = 1;
}

static inline void PWMR_SetLow(void)
{
    LATCbits.LATC0 = 0;
}

static inline void PWMR_Toggle(void)
{
    LATCbits.LATC0 = ~LATCbits.LATC0;
}

static inline uint8_t PWMR_GetValue(void)
{
    return PORTCbits.RC0;
}

/*
 * RC1, PWM, PWMG
 */
static inline void PWMG_SetHigh(void)
{
    LATCbits.LATC1 = 1;
}

static inline void PWMG_SetLow(void)
{
    LATCbits.LATC1 = 0;
}

static inline void PWMG_Toggle(void)
{
    LATCbits.LATC1 = ~LATCbits.LATC1;
}

static inline uint8_t PWMG_GetValue(void)
{
    return PORTCbits.RC1;
}

/*
 * RC2, PWM, PWMB
 */
static inline void PWMB_SetHigh(void)
{
    LATCbits.LATC2 = 1;
}

static inline void PWMB_SetLow(void)
{
    LATCbits.LATC2 = 0;
}

static inline void PWMB_Toggle(void)
{
    LATCbits.LATC2 = ~LATCbits.LATC2;
}

static inline uint8_t PWMB_GetValue(void)
{
    return PORTCbits.RC2;
}

/*
 * RC3, GPIO Output, RS485nRE
 */
static inline void RS485nRE_SetHigh(void)
{
    LATCbits.LATC3 = 1;
}

static inline void RS485nRE_SetLow(void)
{
    LATCbits.LATC3 = 0;
}

static inline void RS485nRE_Toggle(void)
{
    LATCbits.LATC3 = ~LATCbits.LATC3;
}

static inline uint8_t RS485nRE_GetValue(void)
{
    return PORTCbits.RC3;
}

/*
 * RC4, GPIO Output, RS485nDE
 */
static inline void RS485nDE_SetHigh(void)
{
    LATCbits.LATC4 = 1;
}

static inline void RS485nDE_SetLow(void)
{
    LATCbits.LATC4 = 0;
}

static inline void RS485nDE_Toggle(void)
{
    LATCbits.LATC4 = ~LATCbits.LATC4;
}

static inline uint8_t RS485nDE_GetValue(void)
{
    return PORTCbits.RC4;
}

/*
 * RC5, UART RX
 */

/*
 * RC6, CLC Output, SIGOUT
 */
static inline void SIGOUT_SetHigh(void)
{
    LATCbits.LATC6 = 1;
}

static inline void SIGOUT_SetLow(void)
{
    LATCbits.LATC6 = 0;
}

static inline void SIGOUT_Toggle(void)
{
    LATCbits.LATC6 = ~LATCbits.LATC6;
}

static inline uint8_t SIGOUT_GetValue(void)
{
    return PORTCbits.RC6;
}


/*
 * RC7, GPIO Output, SIGOUTnEN
 */
static inline void SIGOUTnEN_SetHigh(void)
{
    LATCbits.LATC7 = 1;
}

static inline void SIGOUTnEN_SetLow(void)
{
    LATCbits.LATC7 = 0;
}

static inline void SIGOUTnEN_Toggle(void)
{
    LATCbits.LATC7 = ~LATCbits.LATC7;
}

static inline uint8_t SIGOUTnEN_GetValue(void)
{
    return PORTCbits.RC7;
}