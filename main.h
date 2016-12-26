/* 
 * File:                    main
 * Author:                  Gerson Ramos
 * Date:                    Dec 2016
 */
 
#include <xc.h>
#include <stdint.h>

#ifndef _MAIN_H
#define	_MAIN_H

// <editor-fold defaultstate="collapsed" desc="Configuration Bits">
// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG    // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF      // Extended Instruction Set (Disabled)

// CONFIG1H
#pragma config FOSC = INTIO2
#pragma config PLLCFG = ON      // PLL x4 Enable bit (Enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = ON      // Power Up Timer (Enabled)
#pragma config BOREN = ON       // Brown Out Detect (Controlled with SBOREN bit)
#pragma config BORV = 2         // Brown-out Reset Voltage bits (2.0V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)

// CONFIG3H
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-01FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 02000-03FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 04000-05FFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 06000-07FFF (Disabled)

// CONFIG5H
#pragma config CPB = ON         // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-01FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 02000-03FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 04000-05FFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 06000-07FFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = ON        // Table Write Protect Boot (Enabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-01FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 02000-03FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 04000-05FFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 06000-07FFF (Disabled)

// CONFIG7H
#pragma config EBTRB = ON       // Table Read Protect Boot (Disabled)
//</editor-fold>
 
//<editor-fold defaultstate="collapsed" desc="Definitions">
#define     BOOTLOADER_MAX_ADDR             0x0FFF      //4K    =   0x1000      <- Always Code Protected
#define     FLASH_MAX_ADDR                  0xFFFF      //64K   =   0x10000

#define     APPLICATION_MIN_ADDR            0x1000
#define     APPLICATION_MAX_ADDR            FLASH_MAX_ADDR
#define     APPLICATION_FLASH_SIZE          APPLICATION_MAX_ADDR - APPLICATION_MIN_ADDR

#define     EEPROM_MAX_ADDR                 0x400       //1K
#define     EEPROM_MIN_ADDR                 0x000

#define     CONFIG_MAX_ADDR                 0x30000D       
#define     CONFIG_MIN_ADDR                 0x300000

#define     APPLICATION_RESET_VECTOR        APPLICATION_MIN_ADDR
#define     APPLICATION_HP_INT_VECTOR       APPLICATION_MIN_ADDR + 0x0008
#define     APPLICATION_LP_INT_VECTOR       APPLICATION_MIN_ADDR + 0x0018

#define     AWAITING_COMMAND                0xBB
#define     EXITING_BACKDOOR                0xCC
#define     BACKDOOR_TIMEOUT                10000

#define     BACKDOOR_IO
//#define     BACKDOOR_TIMED
#define     BOOT_INDICATOR

//</editor-fold>

// <editor-fold defaultstate="collapsed" desc="Type Definitions and Structures">
struct MessageHeader
{
    uint8_t command;
    uint32_t address;
    uint8_t marker;
    uint8_t length;
    void (*get)(void);
};
//</editor-fold>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#endif	/* _MAIN_H */

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/
    