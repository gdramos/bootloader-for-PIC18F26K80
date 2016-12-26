#include <xc.h>
#include <stdint.h>
#include "eeprom.h"
#include "main.h"
#include "uart.h"
#include "boot.h"

//<editor-fold defaultstate="collapsed" desc="Interrupt Vector Remapping">
#asm
PSECT intcode
        GOTO    APPLICATION_HP_INT_VECTOR
PSECT intcodelo
        GOTO    APPLICATION_LP_INT_VECTOR
#endasm
//</editor-fold>
        
//<editor-fold defaultstate="collapsed" desc="Variables">
uint8_t exit_boot;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Main Function">
void main(void)
{
    exit_boot = 1;
    
#ifdef BACKDOOR_IO
    /* IO Pin Backdoor check */
    TRISBbits.TRISB0 = 1;
    ANCON1bits.ANSEL10 = 0;
    
    if(PORTBbits.RB0 == 0)
    {
        exit_boot = 0;
    }
#endif
    
#ifdef BOOT_INDICATOR
    /* Bootloader State Indicator */
    TRISCbits.TRISC3 = 0;
    PORTCbits.RC3 = 0;
#endif
    
    /* Check Eeprom Boot Mode byte */
    if(exit_boot == 1 && DATAEE_ReadByte(BOOT_KEY) != APP_MODE)
    {
        exit_boot = 0;
    }
    
    if(!exit_boot)
    {
        /* Initialize necessary peripherals */
        BOOT_Initialize();
        
        /* Perform Bootloader Task*/
        while(BOOT_Load());
    }
    
    /* Program is ready to Jump to Application */
    asm("GOTO " ___mkstr(APPLICATION_RESET_VECTOR));
}
//</editor-fold>

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/