#include <xc.h>
#include "eeprom.h"

void DATAEE_WriteByte(uint16_t bAdd, uint8_t bData)
{
    uint8_t GIEBitValue = INTCONbits.GIE;

    EEADRH = ((bAdd >> 8) & 0x03);
    EEADR = (bAdd & 0xFF);
    EEDATA = bData;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;     // Disable interrupts
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    // Wait for write to complete
    while (EECON1bits.WR)
    {}

    EECON1bits.WREN = 0;
    INTCONbits.GIE = GIEBitValue;   // restore interrupt enable
}

uint8_t DATAEE_ReadByte(uint16_t bAdd)
{
    EEADRH = ((bAdd >> 8) & 0x03);
    EEADR = (bAdd & 0xFF);
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    NOP();  // NOPs may be required for latency at high frequencies
    NOP();

    return (EEDATA);
}

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/
