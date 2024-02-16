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

#define ADC_Disable() (ADCON0bits.ON = 0)

#define ADC_CH_RA4_ANA4 (uint8_t)0x04
#define ADC_CH_RA4_ANA5 (uint8_t)0x05
#define ADC_CH_FVR_BUF1 (uint8_t)0x3E
#define ADC_CH_FVR_BUF2 (uint8_t)0x3F
#define ADC_CH_VSS (uint8_t)0x3A

void ADC_Initialize(void);
/**
 * @ingroup adcc
 * @brief This API starts A/D conversion on selected channel.
 * @param channel - Analog channel number on which A/D conversion has to be applied.
 *                  Refer adcc_channel_t for available list of channels
 * @return none
 */
void ADCC_StartConversion(uint8_t channel);

/**
 * @ingroup adcc
 * @brief This API checks if ongoing A/D conversion is complete.
 * @pre ADCC_StartConversion function should be called before calling this function.
 * @param none
 * @retval true - if conversion is complete
 * @retval false - if conversion is ongoing
 */
uint8_t ADCC_IsConversionDone(void);

/**
 * @ingroup adcc
 * @brief This API is used to retrieve the result of latest A/D conversion.
 * @pre ADCC_StartConversion function should be called before calling this function and completion status should be checked using ADCC_IsConversionDone function.
 * @param none
 * @return The result of A/D conversion. Refer adc_result_t
 */
uint16_t ADCC_GetConversionResult(void);

/**
 * @ingroup adcc
 * @brief This API is used to retrieve the result of single A/D conversion on given channel in single-ended mode.
 * @pre ADCC_DisableContinuousConversion function should have been called before calling this function
 * @param channel - Analog channel number on which A/D conversion has to be applied.
 *                  Refer adcc_channel_t  for available channels
 * @return The result of A/D conversion. Refer adc_result_t
 */
uint16_t ADCC_GetSingleConversion(uint8_t channel);

/**
 * @ingroup adcc
 * @brief This API is used to stop ongoing A/D conversion.
 * @pre ADCC_StartConversion function should have been called before calling this function.
 * @param none
 * @return none
 */
void ADCC_StopConversion(void);
