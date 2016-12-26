/* 
 * File:                    Flash
 * Author:                  Gerson Ramos
 * Date:                    Dec 2016
 */
 
#ifndef _EEPROM_H
#define	_EEPROM_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//<editor-fold defaultstate="collapsed" desc="Definitions">
#define     BOOT_KEY        0x3FF
#define     APP_MODE        0xAA
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Function Prototypes">
void DATAEE_WriteByte(uint16_t bAdd, uint8_t bData);
uint8_t DATAEE_ReadByte(uint16_t bAdd);
//</editor-fold>

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _EEPROM_H */

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/
