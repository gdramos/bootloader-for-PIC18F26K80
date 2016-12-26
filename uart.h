/* 
 * File:                    flash
 * Author:                  Gerson Ramos
 * Date:                    Dec 2016
 */
 
#ifndef _UART_H
#define	_UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//<editor-fold defaultstate="collapsed" desc="Function Prototypes">
void    UART_Initialize(void);
void    UART_Send(uint8_t data);
uint8_t UART_Receive(void);
//</editor-fold>

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _UART_H */

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/
