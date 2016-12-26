#include "uart.h"

void UART_Initialize(void)
{
    TRISCbits.TRISC7 = 1;           //RX1 -> RC7: input [1]
    
    TRISCbits.TRISC6 = 0;           //TX1 -> RC6: output [0]
    
    TXSTA1 = 0x24;                  //0b00100100
    RCSTA1 = 0x90;                  //0b10010000
    
    BAUDCON1bits.BRG16 = 1;         //16-bit baud configuration
    
    SPBRG1 = 0x8A;                  //115200                  
    SPBRGH1 = 0x00;
}

void UART_Send(uint8_t data)
{
    while(!TXSTA1bits.TRMT);
    TXREG1 = data;
}

uint8_t UART_Receive(void)
{
    uint8_t data;
    while(PIR1bits.RC1IF == 0);
    PIR1bits.RC1IF = 0;
    
    data = RCREG1;
    
    if(RCSTA1bits.FERR)
        data = RCREG1;

    if(RCSTA1bits.OERR)
    {
        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
    }
    //UART_Send(data);
    return (data);
}

/******************************************************************************/
/*                                END OF FILE                                 */
/******************************************************************************/
