/*
 * File:   pin.c
 * Author: Teruyuki
 *
 * Created on 2023/05/19, 13:46
 */


#include <xc.h>

void PIN_Initialize(void) {
    
    // PORTA
    PORTA = 0x00; // Initialize PORTA (RA0-RA5)
    LATA = 0x00; // Initialize Latch (RA0-RA5), bit3: Read as 0.
    ANSELA = 0b00000100; // 0:Digital 1:Analog, bit3: Read as 0.
    TRISA = 0b00001011; // 0:Output 1:Input(tri-stated), RA3 is input only.
    WPUA = 0b00111111; // 0: Pull-up disable, 1:Pullup-enable, bit7-6: Read as 0
    ODCONA = 0b00000000; // 0:Push-Pull 1:Open Drain, bit3,7-6: Read as 0
    SLRCONA = 0b00110111; // 0:slew at maximum rate 1: slew is limited, bit3,7-6: Read as 0
    INLVLA = 0x00; // 0:Input TTL level 1:Input ST level
    
    // PORTB: PIC16F1709 only.
    
    // PORTC
    PORTC = 0x00; // Initialize PORTC (RC7-RC0)
    LATC = 0x00; // Initialize Latch (RC7-RC0)
    ANSELC = 0b00000000; // 0:Digital 1:Analog, bit5-4: Read as 0
    TRISC = 0b00101000; //0:Output 1:Input(tri-state)
    WPUC = 0b11111111;  // 0: Pull-up disable, 1:Pullup-enable
    ODCONC = 0b00000000; // 0:Push-Pull 1:Open Drain, bit3-0,6-7: Read as 0
    SLRCONC = 0xFF; // 0:slew at maximum rate 1: slew is limited
    INLVLC = 0xFF; // 0:Input TTL level 1:Input ST level
    
/*
 * ピン配列
 * #1 VDD
 * #2 RA5 PWM4 31kHz 9bit RIN(逆) Output
 * #3 RA4 PWM3 31kHz 9bit FIN(正) Output
 * #4 RA3 nMCLR Input
 * #5 RC5 CCP1IN IR信号 Input
 * #6 RC4 CCP2PWM 15kHz 10bit LED_RED Output
 * #7 RC3 UART_RX Input
 * #8 RC2 UART_TX Output
 * #9 RC1 MSSP I2C SDA I/O
 * #10 RC0 MSSP I2C SCL I/O
 * #11 RA2 DAC1OUT2 Output Analog
 * #12 RA1 ISCPCLK Input
 * #13 RA0 ISCPDAT Input
 * #14 VSS
 */
}
