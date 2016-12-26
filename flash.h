/* 
 * File:                    Flash
 * Author:                  Gerson Ramos
 * Date:                    Dec 2016
 */
 
#ifndef _FLASH_H
#define	_FLASH_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//<editor-fold defaultstate="collapsed" desc="Definitions">
#define     WRITE_FLASH_BLOCKSIZE    64
#define     ERASE_FLASH_BLOCKSIZE    64
#define     END_FLASH                65536

/* To guarantee 64 byte block only */
#define     BLOCK_MASK              0xFFFC0
#define     BLOCK_SIZE              64
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Type Definitions and Structures">
struct Block
{
    uint32_t startAddress;
    uint32_t endAddress;
    uint8_t data[64];
    uint8_t* dataPtr;
};
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Function Prototypes">
uint8_t     FLASH_ReadByte(uint32_t flashAddr);
uint16_t    FLASH_ReadWord(uint32_t flashAddr);
void        FLASH_WriteByte(uint32_t flashAddr, uint8_t *flashRdBufPtr, uint8_t byte);
int8_t      FLASH_WriteBlock(uint32_t writeAddr, uint8_t *flashWrBufPtr);
void        FLASH_EraseBlock(uint32_t baseAddr);
uint8_t     FLASH_AddToBlock(uint32_t writeAddr, uint8_t *flashWrBufPtr, uint8_t count);
uint16_t    FLASH_crc16(uint8_t* data_p, uint8_t length);
//</editor-fold>

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _FLASH_H */

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/
