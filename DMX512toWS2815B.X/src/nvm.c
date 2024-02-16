/*
 * File:   nvm.c
 * Author: nishi
 *
 * Created on July 22, 2022, 11:06 AM
 */

#include <xc.h>
#include "nvm.h"
#include "main.h"

void NVM_Initialize(void)
{
    NVMCON1 = 0x00;
    
    NVMADRU = 0x00;
    NVMADRH = 0x00;
    NVMADRL = 0x00;
    
    NVMDATH = 0x00;
    NVMDATL = 0x00;
    
    TBLPTRU = 0x00; // Table Pointerの使い方を調べること
    TBLPTRH = 0x00;
    TBLPTRL = 0x00;
    //TBLPTRUbits.ACSS = 0; // Access Program Flash Memory space
    
    TABLAT = 0x00; // Table Latchの使い方を調べること
}

uint8_t DFM_Read(uint8_t address)
{
    uint8_t readData = 0x00;
    //NVMCON1bits.NVMREGS = 1; // Point to DFM
    NVMADRU = 0x00; // EEPROM Address High → アドレスはEEPROM領域､プログラムフラッシュ領域両対応に持っていきたい
    NVMADRH = 0x70; 
    NVMADRL = address;
    //NVMCON1bits.RD = 1; // Initiate read cycle
    /*
    while (NVMCON1bits.RD == 1)
        ;
     */
    readData = NVMDATL;

    return readData;
}

void DFM_Write(uint8_t address, uint8_t data)
{
    //NVMCON1bits.NVMREGS = 1;
    NVMADRU = 0x00; // EEPROM Address High → アドレスはEEPROM領域､プログラムフラッシュ領域両対応に持っていきたい
    NVMADRH = 0x70;
    NVMADRL = address;
    NVMDATH = 0x00; // EEPROMは8ビットだがフラッシュは14(16)ビット
    NVMDATL = data;
    //NVMCON1bits.WREN = 1; // Enable write/erase
    INTCON0bits.GIE = 0; // Disable interrupts
    INTCON0bits.GIEL = 0;
    // The next three steps are the required unlock sequence
    NVMLOCK = 0x55; // First unlock code
    NVMLOCK = 0xAA; // Second unlock code
    //NVMCON1bits.WR = 1;   // Initiate write/erase cycle
    INTCON0bits.GIE = 1; // enable interrupts
    INTCON0bits.GIEL = 1;
    /*
    while(NVMCON1bits.WR);     // Wait to write
    NVMCON1bits.WREN = 0; // Disable further write/erase cycles
     */
/*    
    // Verify byte
    if (NVMCON1bits.WRERR)
    {
        // ERASE_FAULT_RECOVERY();
    }
*/
}

void DFM_Erase(uint8_t address)
{
    //NVMCON1bits.NVMREGS = 1; // Point to DFM
    NVMADRU = 0x00; // 元のコードも動作しない実装だったので再度確認すること
    NVMADRH = 0x00; // DFM_ADDR; // NVMADRH = 0x70, NVMADRL
    NVMADRL = 0x00; 
    INTCON0bits.GIE = 0; // Disable interrupts
    INTCON0bits.GIEL = 0;
    // Perform required unlock sequence
    NVMLOCK = 0x55; // First unlock code
    NVMLOCK = 0xAA; // Second unlock code
    //NVMCON1bits.WR = 1;   // Begin program/erase cycle
    INTCON0bits.GIE = 1; // enable interrupts
    INTCON0bits.GIEL = 1;
    //NVMCON1bits.WREN = 0; // Disable program/erase

    // Verify byte erase operation success and call the recovery function if needed
    if (NVMCON1bits.WRERR)
    {
        // ERASE_FAULT_RECOVERY();
    }
}

uint8_t DFM_Verify(uint8_t address, uint8_t data)
{
    uint8_t readData = 0x00;
    readData = DFM_Read(address);

    if (data == readData)
    {
        return 1;
    }

    return 0;
}