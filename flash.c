#include <xc.h>
#include <stdint.h>
#include "flash.h"

struct Block currentBlock;

uint8_t FLASH_ReadByte(uint32_t flashAddr)
{
    TBLPTRU = (uint8_t)((flashAddr & 0x00FF0000) >> 16);
    TBLPTRH = (uint8_t)((flashAddr & 0x0000FF00)>> 8);
    TBLPTRL = (uint8_t)(flashAddr & 0x000000FF);

    asm("TBLRD*+");

    return (TABLAT);
}

/*uint16_t FLASH_ReadWord(uint32_t flashAddr)
{
    return ((((uint16_t)FLASH_ReadByte(flashAddr+1))<<8)|(FLASH_ReadByte(flashAddr)));
}

void FLASH_WriteByte(uint32_t flashAddr, uint8_t *flashRdBufPtr, uint8_t byte)
{
    uint32_t blockStartAddr = (uint32_t)(flashAddr & ((END_FLASH-1) ^ (ERASE_FLASH_BLOCKSIZE-1)));
    uint8_t offset = (uint8_t)(flashAddr & (ERASE_FLASH_BLOCKSIZE-1));
    uint8_t i;

    // Entire row will be erased, read and save the existing data
    for (i=0; i<ERASE_FLASH_BLOCKSIZE; i++)
    {
        flashRdBufPtr[i] = FLASH_ReadByte((blockStartAddr+i));
    }

    // Load byte at offset
    flashRdBufPtr[offset] = byte;

    // Writes buffer contents to current block
    FLASH_WriteBlock(blockStartAddr, flashRdBufPtr);
}*/

int8_t FLASH_WriteBlock(uint32_t writeAddr, uint8_t *flashWrBufPtr)
{
    uint32_t blockStartAddr  = (uint32_t )(writeAddr & ((END_FLASH-1) ^ (ERASE_FLASH_BLOCKSIZE-1)));
    uint8_t GIEBitValue = INTCONbits.GIE;     // Save interrupt enable
    uint8_t i;

    // Flash write must start at the beginning of a row
    if( writeAddr != blockStartAddr )
    {
        return -1;
    }

    // Block erase sequence
    FLASH_EraseBlock(writeAddr);

    // Block write sequence
    TBLPTRU = (uint8_t)((writeAddr & 0x00FF0000) >> 16);    // Load Table point register
    TBLPTRH = (uint8_t)((writeAddr & 0x0000FF00)>> 8);
    TBLPTRL = (uint8_t)(writeAddr & 0x000000FF);

    // Write block of data
    for (i=0; i<WRITE_FLASH_BLOCKSIZE; i++)
    {
        TABLAT = flashWrBufPtr[i];  // Load data byte

        if (i == (WRITE_FLASH_BLOCKSIZE-1))
        {
            asm("TBLWT*");
        }
        else
        {
            asm("TBLWT*+");
        }
    }

    EECON1bits.EEPGD = 1;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0; // Disable interrupts
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;  // Start program

    EECON1bits.WREN = 0;    // Disable writes to memory
    INTCONbits.GIE = GIEBitValue;   // Restore interrupt enable

    return 0;
}

void FLASH_EraseBlock(uint32_t baseAddr)
{
    uint8_t GIEBitValue = INTCONbits.GIE;   // Save interrupt enable

    TBLPTRU = (uint8_t)((baseAddr & 0x00FF0000) >> 16);
    TBLPTRH = (uint8_t)((baseAddr & 0x0000FF00)>> 8);
    TBLPTRL = (uint8_t)(baseAddr & 0x000000FF);

    EECON1bits.EEPGD = 1;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    EECON1bits.FREE = 1;
    INTCONbits.GIE = 0; // Disable interrupts
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = GIEBitValue;   // Restore interrupt enable
}

void FLASH_ClearBlockData(void)
{
    for(int i = 0; i < 64; i++)
    {
        currentBlock.data[i] = 0xFF;
    }
}

uint8_t FLASH_AddToBlock(uint32_t writeAddr, uint8_t *flashWrBufPtr, uint8_t count)
{   
    uint8_t result = 0;
    
    if((writeAddr < currentBlock.startAddress) || 
       (writeAddr > currentBlock.endAddress) || count == 0)
    {
        if(currentBlock.dataPtr != currentBlock.data)
        {
            /* Save previous block */
            result += FLASH_WriteBlock(currentBlock.startAddress, currentBlock.data);
            currentBlock.dataPtr = currentBlock.data;
            FLASH_ClearBlockData();
        }
        
        /* Reset addresses */
        currentBlock.startAddress = writeAddr & BLOCK_MASK;
        currentBlock.endAddress = currentBlock.startAddress + BLOCK_SIZE - 1;
        
        /* Return if dummy call */
        if(count == 0)
        {
            return result;
        }
    }
    
    currentBlock.dataPtr = &currentBlock.data[writeAddr - currentBlock.startAddress];
    
    for(int i = 0; i < count; i++)
    {
        /* Do we need to switch to the next block? */
        if((writeAddr + i) > currentBlock.endAddress)
        {
             /* Save previous block */
            result += FLASH_WriteBlock(currentBlock.startAddress, currentBlock.data);
            /* Reset addresses and pointer */
            currentBlock.startAddress = writeAddr + i;
            currentBlock.endAddress = currentBlock.startAddress + BLOCK_SIZE - 1;
            currentBlock.dataPtr = currentBlock.data;
            FLASH_ClearBlockData();
        }
        
        *currentBlock.dataPtr++ = *flashWrBufPtr++;
    }
    
    return result;
}

uint16_t FLASH_crc16(uint8_t* data_p, uint8_t length)
{
    uint8_t x;
    /* Initial Polynomial */
    uint16_t crc = 0xFFFF;

    while (length--)
    {
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }
    return crc;
}

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/
