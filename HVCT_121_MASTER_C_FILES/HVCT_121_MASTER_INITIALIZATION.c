
#include <p24FJ512GU408.h>
#include <xc.h>
#include <stdio.h>
#include"../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"

void Initialize_IO_Pins(void)
{
    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
//    ANSA = 0x0000;
    ANSB = 0x0000;
    ANSD = 0x0000;
    ANSE = 0x0000;
    ANSG = 0x0000;
     /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPDC = 0x0000;
    CNPDD = 0x0000;
    CNPDE = 0x0000;
    CNPDF = 0x0000;
    CNPDG = 0x0000;
    CNPDH = 0x0000;
    CNPUA = 0x0000;
    CNPUB = 0x0000;
    CNPUC = 0x0000;
    CNPUD = 0x0000;
    CNPUE = 0x0000;
    CNPUF = 0x0000;
    CNPUG = 0x0000;
    CNPUH = 0x0000;
    IOCPDA = 0x0000;
    IOCPDB = 0x0000;
    IOCPDC = 0x0000;
    IOCPDD = 0x0000;
    IOCPDE = 0x0000;
    IOCPDF = 0x0000;
    IOCPDG = 0x0000;
    IOCPDH = 0x0000;
    IOCPUA = 0x0000;
    IOCPUB = 0x0000;
    IOCPUC = 0x0000;
    IOCPUD = 0x0000;
    IOCPUE = 0x0000;
    IOCPUF = 0x0000;
    IOCPUG = 0x0000;
    IOCPUH = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;
    ODCE = 0x0000;
    ODCF = 0x0000;
    ODCG = 0x0000;
    ODCH = 0x0000;
    
   
    /****************************************************************************
     *Interface Card Relay Port pins
     ***************************************************************************/
    TRIS_RLYLV11_rb2_pin = output_pin;
    TRIS_RLYLV12_rb3_pin = output_pin;
    TRIS_RLYLV21_rb4_pin = output_pin;
    TRIS_RLYLV22_rb5_pin = output_pin;
    TRIS_RLYHV11_re9_pin = output_pin;
    TRIS_RLYHV21_re8_pin = output_pin;
    TRIS_RLYHL1_rg9_pin = output_pin;
    TRIS_RLYHL2_rg8_pin = output_pin;
    
    
    /****************************************************************************
     *KBD Interrupt pin
     ***************************************************************************/
    TRIS_KBD_INTERRUPT_rd1_pin = input_pin;                   
    
    /****************************************************************************
     *Digital read pin 
     ***************************************************************************/
    TRIS_DIG_READ_rg6_pin = input_pin;             // DIG_READ Pin as input pin
    tris_board_check_ra9_pin = output_pin;         //For board check input pin
    
    /****************************************************************************
     *  SPI3 Port Pins
     ***************************************************************************/
    TRIS_SDO_SPI3_rd5_pin = output_pin;            // SD0 Pin as output pin
    TRIS_SDI_SPI3_rd4_pin = input_pin;             // SDI Pin as input pin
    TRIS_SCLK_SPI3_rd3_pin = output_pin;           // SCLK Pin as output pin
    TRIS_CS1_SPI3_rh0_pin = output_pin;            // CS1 Pin as output pin
    TRIS_CS2_SPI3_rf7_pin = output_pin;            // CS2 Pin as output pin
    TRIS_CS3_SPI3_rf8_pin = output_pin;            // CS3 Pin as output pin
    
    
    /****************************************************************************
     *  I2C Port Pins
     ***************************************************************************/
    TRIS_SCL_ra14_pin  = input_pin;                // SCL AS I/P                       
    TRIS_SDA_ra15_pin   = input_pin;               // SDA AS I/P
    tris_volume_control= output_pin;	
    tris_tone= output_pin;
    
    /****************************************************************************
     *  card select pins 74HC138
     ***************************************************************************/
    tris_card_select_A0_rg7_pin = output_pin;
    tris_card_select_A1_rd8_pin = output_pin;
    tris_card_select_A2_rd9_pin = output_pin;
    tris_enable_74hc138_rd10_pin = output_pin;
    tris_strobe_74hc244_rd11_pin = output_pin;
    
    /****************************************************************************
     *  rs485 contol pin
     ***************************************************************************/
    
    tris_rs485_control_ra10_pin = output_pin;
    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPINR18bits.U1RXR = 0x001E;    //RF2->UART1:U1RX
    RPOR8bits.RP16R = 0x0003;    //RF3->UART1:U1TX
    
    RPINR19bits.U2RXR = 0x0011;    //RF5->UART2:U2RX  (RS485)
    RPOR5bits.RP10R = 0x0005;    //RF4->UART2:U2TX    (RS485)

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
}

//--------------------------SPI Initialization-----------------------------------/
void Initialize_SPI3 (void) 
{
    
    // AUDEN disabled; FRMEN disabled; AUDMOD I2S; FRMSYPW One clock wide; AUDMONO stereo; FRMCNT 0; MSSEN disabled; FRMPOL disabled; IGNROV disabled; SPISGNEXT not sign-extended; FRMSYNC disabled; URDTEN disabled; IGNTUR disabled; 
    SPI3CON1H = 0x00;
    // WLENGTH 0; 
    SPI3CON2L = 0x00;
    // SPIROV disabled; FRMERR disabled; 
    SPI3STATL = 0x00;
    // SPI3BRGL 5; 
    SPI3BRGL = 0x95;        //95   SPI isolator circuit doesn't work for frequencies greater than 100KHz , so keep SCKL less than 100KHz
    // SPITBFEN disabled; SPITUREN disabled; FRMERREN disabled; SRMTEN disabled; SPIRBEN disabled; BUSYEN disabled; SPITBEN disabled; SPIROVEN disabled; SPIRBFEN disabled; 
    SPI3IMSKL = 0x00;
    // RXMSK 0; TXWIEN disabled; TXMSK 0; RXWIEN disabled; 
    SPI3IMSKH = 0x00;
    // SPI3URDTL 0; 
    SPI3URDTL = 0x00;
    // SPI3URDTH 0; 
    SPI3URDTH = 0x00;
    // SPIEN enabled; DISSDO disabled; MCLKEN FOSC/2; CKP Idle:Low, Active:High; SSEN disabled; MSTEN Master; MODE16 disabled; SMP Middle; DISSCK disabled; SPIFE Frame Sync pulse precedes; CKE Active to Idle; MODE32 disabled; SPISIDL disabled; ENHBUF enabled; DISSDI disabled; 
    SPI3CON1L = 0x8121;
    
}



//--------------------Serial UART 2 Initialization------------------------------//
void Initialize_Serial_2_RS485(void)
{
    IEC1bits.U2TXIE = 0;
    IEC1bits.U2RXIE = 0;

    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    // Data Bits = 8; Parity = None; Stop Bits = 1;
    U2MODE = (0x8008 & ~(1<<15));  // disabling UART ON bit
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; URXEN disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U2STA = 0x00;
    // BaudRate = 9600; Frequency = 16000000 Hz; U2BRG 416; 
    U2BRG = 0x1A0;
    
    IEC1bits.U2RXIE = 1;
    
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    U2MODEbits.UARTEN = 1;   // enabling UART ON bit
    U2STAbits.UTXEN = 1;

}
//--------------------Serial UART 1 Initialization------------------------------//
void Initialize_Serial_1_RS232(void)
{
    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 0;

    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    // Data Bits = 8; Parity = None; Stop Bits = 1;
    U1MODE = (0x8008 & ~(1<<15));  // disabling UART ON bit
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; URXEN disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U1STA = 0x00;
    // BaudRate = 9600; Frequency = 16000000 Hz; U1BRG 416; 
    U1BRG = 0x1A0;
    IEC0bits.U1RXIE = 1;
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    U1MODEbits.UARTEN = 1;   // enabling UART ON bit
    U1STAbits.UTXEN = 1;
}

void  Initialise_Serial_1_FACT_Editor(void)
{
    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 0;
    
     // STSEL 2; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    // Data Bits = 8; Parity = None; Stop Bits = 2;
    U1MODE = (0x8009 & ~(1<<15));  // disabling UART ON bit
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; URXEN disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U1STA = 0x00;
    // BaudRate = 38400; Frequency = 16000000 Hz; U1BRG 103; 
    U1BRG = 0x67;
    
     IEC0bits.U1RXIE = 1;
    //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
    U1MODEbits.UARTEN = 1;   // enabling UART ON bit
    U1STAbits.UTXEN = 1;
	
}//end void  Initialise_Serial_1(void).

//-----------------------------LCD Initialization-------------------------------//
void Initialize_Lcd(void)
{
	OpenXLCD_PIC30();
}



//---------------------------I2C Initialization---------------------------------//
void InitialiseI2c(void)
{
    // ACKEN disabled; STRICT disabled; STREN disabled; GCEN disabled; SMEN disabled; DISSLW enabled; I2CSIDL disabled; ACKDT Sends ACK; SCLREL Holds; RSEN disabled; A10M 7 Bit; PEN disabled; RCEN disabled; SEN disabled; I2CEN enabled; 
    I2C1CONL = 0x8000;
    // BCL disabled; D_nA disabled; R_nW disabled; P disabled; S disabled; I2COV disabled; IWCOL disabled; 
    I2C1STAT = 0x00;
    // Baud Rate Generator Value: I2CBRG 78;   
    I2C1BRG = 0x4E;  // 100KHz frequency
}//end void InitialiseI2c(void)


//-------------------------TIMER1 Initialization--------------------------------//
void Initialize_Timer1_50ms (void)
{
    //TMR1 0; 
    TMR1 = 0x00;
    //Period = 0.05 s; Frequency = 16000000 Hz; PR1 12499; 
    PR1 = 0x30D3;
    //TCKPS 1:64; TON enabled; TSIDL disabled; TCS FOSC/2; TECS SOSC; TSYNC disabled; TGATE disabled; 
    T1CON = 0x8020;
    
    T1CONbits.TON = 0;  // Initially keep timer off
   

    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
	IPC0bits.T1IP = 4;
   
}

void Timer1_50ms_Start( void )
{
    /* Reset the status information */

    /*Enable the interrupt*/
    IEC0bits.T1IE = 1;

    /* Start the Timer */
    T1CONbits.TON = 1;
}

void Timer1_50ms_Stop( void )
{
    /* Stop the Timer */
    T1CONbits.TON = 0;

    /*Disable the interrupt*/
    IEC0bits.T1IE = 0;
}

//-------------------------TIMER2 Initialization--------------------------------//

void Initialize_Timer2_100ms (void)
{
    //TMR2 0; 
    TMR2 = 0x00;
    //Period = 0.1 s; Frequency = 16000000 Hz; PR2 24999; 
    PR2 = 0x61A7;
    //TCKPS 1:64; T32 16 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TECS SOSC; TGATE disabled; 
    T2CON = 0x8020;

    T2CONbits.TON = 0; // Initially keep timer off

    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    IPC1bits.T2IP = 4;
}

void Timer2_100ms_Start( void )
{
    /* Reset the status information */

    /*Enable the interrupt*/
    IEC0bits.T2IE = 1;

    /* Start the Timer */
    T2CONbits.TON = 1;
}

void Timer2_100ms_Stop( void )
{
    /* Stop the Timer */
    T2CONbits.TON = 0;

    /*Disable the interrupt*/
    IEC0bits.T2IE = 0;
}


//-------------------------CN Interrupt Initialization--------------------------------//
/*
 Function:This function will initialise CN interrupt used for key board.	  
 Input:
 Output:
 Calls:
 Called By:
*/
void Initialise_CN_Interrupt()
{
    IEC1bits.IOCIE = 0; //Disable IOCIE Interrupt-on-Change Interrupt Enable bit
    
    IEC1bits.IOCIE = 1; //Enable IOCIE Interrupt-on-Change Interrupt Enable bit

    IOCPUDbits.CNPUD1=0;                    //disabled weak pullup.
    
    IPC4bits.IOCIP = 2;                     //    IOCI: IOC -  Interrupt On Change Interrupt
    /****************************************************************************
     * Interrupt On Change: negative
     ***************************************************************************/
    IOCNDbits.IOCND1 = 1;    //Pin : RD1
//    IOCPDbits.IOCPD1 = 1;
    IOCPDbits.IOCPD1 = 0;    //Pin : RD1
    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    IOCFDbits.IOCFD1 = 0;    //Pin : RD1
    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    PADCONbits.IOCON = 1;    //Config for PORTD
    
}//end void Initialise_CN_Interrupt().



void Initilize_interface_card (void)
{
    RLYLV11_rb2_pin = RELAY_OFF;                  
    DelayMs(1); 
    RLYLV12_rb3_pin = RELAY_OFF;                     
    DelayMs(1); 
    RLYLV21_rb4_pin = RELAY_OFF;
    DelayMs(1); 
    RLYLV22_rb5_pin = RELAY_OFF;
    DelayMs(1); 
    RLYHV11_re9_pin = RELAY_OFF;
    DelayMs(1); 
    RLYHV21_re8_pin = RELAY_OFF;
    DelayMs(1); 
    RLYHL1_rg9_pin = RELAY_OFF;
    DelayMs(1); 
    RLYHL2_rg8_pin = RELAY_OFF;
    DelayMs(1); 
}
