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
#include "main.h"
#include "wdt.h"
#include "main.h"
#include "osc.h"
#include "pps.h"
#include "pin.h"
#include "ext_int.h"
#include "fvr.h"
#include "dac.h"
#include "fpmc.h"
#include "tmr0.h"
#include "tmr1.h"
#include "tmr2.h"
#include "tmr4.h"
#include "tmr6.h"
#include "pwm.h" // PWMはまとめて初期化
//#include "pwm3.h"
//#include "pwm4.h"
#include "ccp1.h"
#include "ccp2.h"
#include "opamp1.h"
#include "opamp2.h"
#include "eusart.h"
#include "ssp1.h"

// delayマクロ用
#ifdef OSC_CLK_32MHZ
#define _XTAL_FREQ 32000000
#elif defined OSC_CLK_16MHZ
#define _XTAL_FREQ 16000000
#elif defined OSC_CLK_8MHZ
#define _XTAL_FREQ 8000000
#endif

void SYSTEM_Initialize(void);

