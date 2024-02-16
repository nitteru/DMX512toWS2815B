/*
 * File:   pmd.c
 * Author: nishi
 *
 * Created on October 11, 2023, 1:56 PM
 */


#include <xc.h>
#include "pmd.h"
#include "main.h"

void PMD_Initialize(void) {
    // 使用しないペリフェラルを停止
    /* 
     * System Clock Network (Fosc) Enabled
     * Fixed Voltage Reference Enabled
     * High/Low-Voltage Detect Disabled
     * CRC Module Disabled
     * NVM Memory Scanner Enabled
     * Clock Reference Enabled
     * Interrupt-On-Change Disabled
     */
    PMD0 = 0x0;

    /*
     * SMT1 Module Disabled → Signal Measurement Timer(パルス幅計測などに使えそう｡Captureよりいいかも)
     * TMR6 Enabled
     * TMR5 Enabled
     * TMR4 Enabled
     * TMR3 Disabled
     * TMR2 Enabled
     * TMR1 Enabled
     * TMR0 Enabled
     */
    PMD1 = 0x00;

    /*
     * Active CLock Disabled
     * DAC Disabled
     * ADC Disabled
     * Comparator2 Enabled
     * COmparator1 Disabled
     * ZCD Enabled
     */
    PMD3 = 0x00;
    
    /*
     * CWG3 Disabled
     * CWG2 Disabled
     * CWG1 Disabled
     * DDS Disabled
     * NCO3 Disabled
     * NCO2 Disabled
     * NCO1 Disabled
     */
    PMD4 = 0x00;
    
    /*
     * Pulse-Width Modulator3 Disable
     * Pulse-Width Modulator2 Disable
     * Pulse-Width Modulator1 Disable
     * Capture Compare3 Disable
     * Capture Compare2 Disable
     * Capture Compare1 Disable
     */
    PMD5 = 0x00;

    /*
     * UART5 Disable
     * UART4 Disable
     * UART3 Disable
     * UART2 Disable
     * UART1 Disable
     * SPI2 Disable
     * SPI1 Disable
     */
    PMD6 = 0x00;
    
    /*
     * CLC8 Disable
     * CLC7 Disable
     * CLC6 Disable
     * CLC5 Disable
     * CLC4 Disable
     * CLC3 Disable
     * CLC2 Disable
     * CLC1 Disable
     */
    PMD7 = 0x00;

    /*
     * DMA6 Disable
     * DMA5 Disable
     * DMA4 Disable
     * DMA3 Disable
     * DMA2 Disable
     * DMA11 Disable
     */
    PMD8 = 0x00;
}
