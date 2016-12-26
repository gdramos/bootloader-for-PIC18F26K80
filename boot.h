/* 
 * File:                    boot
 * Author:                  Gerson Ramos
 * Date:                    Dec 2016
 */

#ifndef _BOOT_H
#define	_BOOT_H

#include <xc.h> // include processor files - each processor file is guarded.  

//<editor-fold defaultstate="collapsed" desc="Definitions">
#define     RD_VERSION          0x00
#define     RD_FLASH            0x01
#define     WR_FLASH            0x02
#define     ER_FLASH            0x03
#define     RD_EEDATA           0x04
#define     WR_EEDATA           0x05
#define     RD_CONFIG           0x06
#define     WR_CONFIG           0x07
#define     RUN_APP             0x08
    
#define     STX                 0x0F
#define     ETX                 0x04
#define     DLE                 0x05

#define     SUCCESS             0xFF
#define     COMMAND_ERROR       0xFE
#define     MARKER_ERROR        0xFD
#define     ADDRESS_ERROR       0xFC
#define     COUNT_ERROR         0xFB
#define     EOF_ERROR           0xFA
#define     EXECUTION_ERROR     0xF9
#define     CRC_ERROR           0xF8

#define     BOOT_VER_MAJOR      0x01
#define     BOOT_VER_MINOR      0x00

#define     DEV_ID1_ADDR        0x3FFFFE
#define     DEV_ID2_ADDR        0x3FFFFF

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Function Prototypes">
void    OSC_Initialize(void);
void    BOOT_GetHeader(void);
void    BOOT_Initialize(void);
void    BOOT_Read(void);
void    BOOT_Write(void);
uint8_t BOOT_Load(void);
void    BOOT_ReadVersion(void);
void    BOOT_EraseUserFlash(void);
//</editor-fold>

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _BOOT_H */

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/
