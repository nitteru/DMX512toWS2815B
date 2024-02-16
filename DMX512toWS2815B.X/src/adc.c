/*
 * File:   adc.c
 * Author: nishi
 *
 * Created on July 19, 2022, 5:16 PM
 */


#include <xc.h>
#include "adc.h"
#include "main.h"

void ADC_Initialize(void)
{
#if 0
    ADLTHL = 0x0; // ADC Lower Threshold Register
    ADLTHH = 0x0;

    ADUTHL = 0x0; //ADC Upper Threshold Register
    ADUTHH = 0x0;

    ADSTPTL = 0x0; // ADC Threshold Setpoint
    ADSTPTH = 0x0;

    ADACCU = 0x00; // ADC Accumulator Register 18bit
    ADACCH = 0x00;
    ADACCL = 0x00;

    ADRPT = 0x04; // ADC Repeat Setting, 4回平均

    ADPCH = 0x04; // ADC Positive Channel: RA4
    
    ADACQL = 0x0; // ADC Acquisition Time Control
    ADACQH = 0x0;
    
    ADCAP = 0x0; // ADC Additional Sample Capacitor Selection, disable

    ADPREL = 0x0; // ADC Precharge Time Control
    ADPREH = 0x0;

    ADCG1A = 0x0; // ADC Channel Group Selection Port A, All disable
    ADCG1B = 0x0; // ADC Channel Group Selection Port B, All disable
    ADCG1C = 0x0; // ADC Channel Group Selection Port C, All disable
    
    ADCON1bits.PPOL = 0; // External analog I/O pin is connected to VSS
    ADCON1bits.IPEN = 0; // Bit has no effect (DSEN=0)
    ADCON1bits.GPOL = 0; // ADC guard Ring outputs start as digital low during Precharge stage
    ADCON1bits.PCSC = 0; // Precharge applies to both the internal sampling capacitor and the external I/O pin
    ADCON1bits.DSEN = 0; // Double-Sample disable, Selected computation is performed after every conversion
    
    ADCON2bits.PSIS = 0; // ADRES is transferred to ADPREV at the start of conversion
    ADCON2bits.CRS = 0x03; // Burst Average mode, The accumulated value is right-shifted by CRS (divided by 2^CRS)
    ADCON2bits.ACLR = 0; // Clearing action is complete (or not started)
    ADCON2bits.MD = 0x02; // Average mode
    
    ADCON3bits.CALC = 0x00; // ADC Error Calculation Mode Select
    ADCON3bits.SOI = 0; // This bit is not used (CONT=0)
    ADCON3bits.TMD = 0x00; // Threshold Interrupt Mode Select, Never interrupt

    ADSTAT = 0x0; // ADC Status Register

    ADREF = 0x3; // ADC Positive Voltage Reference, FVR_buf1

    ADACT = 0x0; // External Trigger Disabled

#ifdef CLOCK_FOSC_32MHz
    ADCLK = 0x1F; // ADC Clock frequency = FOSC/(2*(n+1)) (CS=0) = 2usec, ここではCS=1のため無効
#elif defined CLOCK_FOSC_16MHz
    ADCLK = 0x0F; // ADC Clock frequency = FOSC/(2*(n+1)) (CS=0) = 2usec, ここではCS=1のため無効
#elif defined CLOCK_FOSC_8MHz
    ADCLK = 0x07; // ADC Clock frequency = FOSC/(2*(n+1)) (CS=0) = 2usec, ここではCS=1のため無効
#endif
    
    ADCON0bits.ON = 1;
    ADCON0bits.FM = 1; // ADRES and ADPREV data are right justified
    ADCON0bits.CONT = 0; // Continuous Operation disable
    ADCON0bits.CS = 1; // Clock supplied from ADCRC dedicated oscillator
    ADCON0bits.GO = 0; // ADC conversion completed/not in progress
    
    // Clear the ADC interrupt flag
    PIR4bits.ADIF = 0;

    // Clear the ADC Threshold interrupt flag
    PIR4bits.ADTIF = 0;
#endif
}

void ADCC_StartConversion(uint8_t channel)
{
    // select the A/D channel
    ADPCH = channel;
  
    // Turn on the ADC module
    ADCON0bits.ADON = 1;

    // Start the conversion
    ADCON0bits.ADGO = 1;
}

uint8_t ADCC_IsConversionDone(void)
{
    // Start the conversion
    return ((uint8_t)(!ADCON0bits.ADGO));
}

uint16_t ADCC_GetConversionResult(void)
{
    // Return the result
    return ((uint16_t)((ADRESH << 8) + ADRESL));
}

uint16_t ADCC_GetSingleConversion(uint8_t channel)
{
    // select the A/D channel
    ADPCH = channel;  

    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    
    //Disable the continuous mode.
    ADCON0bits.ADCONT = 0;

    // Start the conversion
    ADCON0bits.ADGO = 1;


    // Wait for the conversion to finish
    while (ADCON0bits.ADGO)
    {
    }
    
    // Conversion finished, return the result
    return ((uint16_t)((ADRESH << 8) + ADRESL));
}

inline void ADCC_StopConversion(void)
{
    //Reset the ADGO bit.
    ADCON0bits.ADGO = 0;
}

