#include "xc.h"
#include "flash.h"
#include "eeprom.h"
#include "uart.h"
#include "main.h"
#include "boot.h"

//<editor-fold defaultstate="collapsed" desc="Variables">
struct MessageHeader Header = {0, 0, 0, 0, BOOT_GetHeader};
uint8_t dataBuffer[64];
//</editor-fold>

void OSC_Initialize(void)
{
    OSCCON  =   0b01110000;
    
    OSCCON2 =   0b00001000;
    
    OSCTUNE =   0b11000000;
    
    REFOCON =   0b00000000;
}

void BOOT_GetHeader(void)
{
    Header.address = 0;
    Header.command = UART_Receive();
    for(int i = 0; i < 3; i++)
    {
        Header.address |= ((uint24_t)UART_Receive()) << (i*8);
    }
    Header.marker = UART_Receive();
    Header.length = UART_Receive();
}

void BOOT_Initialize(void)
{
 OSC_Initialize();
 UART_Initialize();
}

void BOOT_Read(void)
{
    uint24_t max_address,  min_address;
    uint8_t max_length, min_length;
    
    switch(Header.command)
    {
        case RD_FLASH:
            min_address = APPLICATION_MIN_ADDR;
            max_address = APPLICATION_MAX_ADDR;
            min_length = 0;
            max_length = 0x40;
            break;
        case RD_CONFIG:
            min_address = CONFIG_MIN_ADDR;
            max_address = CONFIG_MAX_ADDR;
            min_length = 0;
            max_length = 0x0E;
            break;
        case RD_EEDATA:
            min_address = EEPROM_MIN_ADDR;
            max_address = EEPROM_MAX_ADDR;
            min_length = 0;
            max_length = 0x40;
            break;
        default:
            return;
    }
    
    if(Header.address < min_address || Header.address > max_address)
    {
        /* Error Response */
        UART_Send(ADDRESS_ERROR);
        return;
    }
    
    /* Check for marker 0x00 */
    if(Header.marker != 0x00)
    {
        /* Error Response */
        UART_Send(MARKER_ERROR);
        return;
    }
    
    /* Receive number of bytes to read */
    if(Header.length < min_length || Header.length > max_length)
    {
        /* Error Response */
        UART_Send(COUNT_ERROR);
        return;
    }
    
    /* Receive the End of Frame Character */
    if(UART_Receive() != ETX)
    {
        /* Error Response */
        UART_Send(EOF_ERROR);
        return;
    }
    
    /* Response */
    UART_Send(STX);
    for(int i = 0; i < Header.length; i++)
    {
        switch(Header.command)
        {
            case RD_FLASH:
            case RD_CONFIG:
                UART_Send(FLASH_ReadByte(Header.address++));
                break;
            case RD_EEDATA:
                UART_Send(DATAEE_ReadByte(Header.address++));
                break;
            default:
                UART_Send(EXECUTION_ERROR);
                return;
        }
        
    }
    UART_Send(ETX);
}

void BOOT_Write(void)
{
    uint24_t max_address,  min_address;
    uint8_t max_length, min_length;
    uint16_t crc;
    
    switch(Header.command)
    {
        case WR_FLASH:
            min_address = APPLICATION_MIN_ADDR;
            max_address = 0x400000;
            min_length = 0;
            max_length = 0x40;
            break;
        case WR_EEDATA:
            min_address = EEPROM_MIN_ADDR;
            max_address = EEPROM_MAX_ADDR;
            min_length = 0;
            max_length = 0x40;
            break;
        default:
            return;
    }
    
    if(Header.address < min_address || Header.address > max_address)
    {
        /* Error Response */
        UART_Send(ADDRESS_ERROR);
        return;
    }
    
    /* Check for marker 0x00 */
    if(Header.marker != 0x00)
    {
        /* Error Response */
        UART_Send(MARKER_ERROR);
        return;
    }
    
    /* Receive number of bytes to write */
    if(Header.length < min_length || Header.length > max_length)
    {
        /* Error Response */
        UART_Send(COUNT_ERROR);
        return;
    }
    for(int i = 0; i < Header.length; i++)
    {
        dataBuffer[i] = UART_Receive();
    }
    
    /* Cyclical Redundancy Check */
    if(Header.length != 0)
    {
        crc = (uint16_t)UART_Receive();
        crc |= ((uint16_t)UART_Receive() << 8);

        if(crc != FLASH_crc16(dataBuffer, Header.length))
        {
            UART_Send(CRC_ERROR);
            return;
        }
    }
    
    /* Receive the End of Frame Character */
    if(UART_Receive() != ETX)
    {
        /* Error Response */
        UART_Send(EOF_ERROR);
        return;
    }
    
    /* Perform Write Operation */
    switch(Header.command)
    {
        case WR_FLASH:
            if(FLASH_AddToBlock((uint32_t)Header.address, dataBuffer, Header.length) == 0)
                UART_Send(SUCCESS);
            else 
                UART_Send(EXECUTION_ERROR);
            break;
        case WR_EEDATA:
            for(int i = 0; i < Header.length; i++)
            {
                DATAEE_WriteByte(Header.address++, dataBuffer[i]);
            }
            UART_Send(SUCCESS);
            break;
        default:
            UART_Send(EXECUTION_ERROR);
            break;
    }
}

void BOOT_ReadVersion(void)
{
    /* Receive the End of Frame Character */
    if(UART_Receive() != ETX)
    {
        /* Error Response */
        UART_Send(EOF_ERROR);
        return;
    }
    
    UART_Send(STX);
    
    /*Send Bootloader Version */
    UART_Send(BOOT_VER_MAJOR);
    UART_Send(BOOT_VER_MINOR);
    /*Send Device Identifier */
    UART_Send(FLASH_ReadByte(DEV_ID1_ADDR));
    UART_Send(FLASH_ReadByte(DEV_ID2_ADDR));
    //UART_Send(0x24);
    //UART_Send(0x61);
    
    UART_Send(ETX);
}

void BOOT_EraseUserFlash(void)
{
    /* Receive the End of Frame Character */
    if(UART_Receive() != ETX)
    {
        /* Error Response */
        UART_Send(EOF_ERROR);
        return;
    }
    
    DATAEE_WriteByte(BOOT_KEY, ~APP_MODE);
    
    uint32_t address = APPLICATION_MIN_ADDR;
    for(uint24_t i = 0; i < 1024; i++)
    {
        FLASH_EraseBlock(address);
        /* point to next block */
        address += 64;
    }
    
    UART_Send(SUCCESS);
    
}

uint8_t BOOT_Load(void)
{
    /* Wait for Start Character */
    while(UART_Receive() != STX);
    
    /* Get Header */
    Header.get();

    /* Process Command */
    switch(Header.command)
    {
        case RD_VERSION:
            BOOT_ReadVersion();
            break;
        case ER_FLASH:
            BOOT_EraseUserFlash();
            break;
        case WR_EEDATA:
        case WR_FLASH:
            BOOT_Write();
            break;
        case RD_FLASH:
        case RD_EEDATA:
        case RD_CONFIG:
            BOOT_Read();
            break;
        case WR_CONFIG:
            //WriteConfigurationBits();
            break;
        case RUN_APP:
            DATAEE_WriteByte(BOOT_KEY, APP_MODE);
            return 0;
        default:
            UART_Send(COMMAND_ERROR);
            break;
    }
    return 1;
}

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/
