





/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    system.h

  @Summary:
    This is the sysetm.h file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  PIC24FJ512GU408
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB             :  MPLAB X v5.50
*/

// Configuration bits: selected in the GUI

// FBOOT
#pragma config BTMODE = SINGLE    //Boot Mode Configuration bits->Device is in Single Boot (legacy) mode

// FSEC
#pragma config BWRP = OFF    //Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED    //Boot Segment Code-Protect Level bits->No Protection (other than BWRP)
#pragma config BSEN = OFF    //Boot Segment Control bit->No Boot Segment
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = DISABLED    //General Segment Code-Protect Level bits->No Protection (other than GWRP)
#pragma config CWRP = OFF    //Configuration Segment Write-Protect bit->Configuration Segment may be written
#pragma config CSS = DISABLED    //Configuration Segment Code-Protect Level bits->No Protection (other than CWRP)
#pragma config AIVTDIS = OFF    //Alternate Interrupt Vector Table bit->Disabled AIVT

// FBSLIM
#pragma config BSLIM = 8191    //Boot Segment Flash Page Address Limit bits->8191

// FOSCSEL
#pragma config FNOSC = PRIPLL   //Oscillator Source Selection->FRC
#pragma config PLLMODE = PLL96DIV3    //PLL Mode Selection->96 MHz PLL. Oscillator input is divided by 3 (12 MHz input)
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = HS    //Primary Oscillator Mode Select bits->HS Crystal Oscillator mode
#pragma config OSCIOFCN = ON    //OSC2 Pin Function bit->RC15 is general purpose digital I/O pin
#pragma config SOSCEN = OFF    //SOSC Power Selection Configuration bits->SOSC crystal driver is disabled (RC13/RC14 mode)
#pragma config PLLSS = PRI    //PLL Secondary Selection Configuration bit->PLL is fed by the Primary oscillator
#pragma config IOL1WAY = ON    //Peripheral pin select configuration bit->Allow only one reconfiguration
#pragma config FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled and Fail-safe Clock Monitor is disabled

// FWDT
#pragma config WDTPS = PS32768    //Watchdog Timer Postscaler bits->1:32768
#pragma config FWPSA = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config FWDTEN = OFF    //Watchdog Timer Enable bits->WDT and SWDTEN are disabled
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->WDT in non-windowed mode
#pragma config WDTWIN = WIN25    //Watchdog Timer Window Select bits->WDT window is 25% of WDT period
#pragma config WDTCMX = WDTCLK    //WDT MUX Source Select bits->WDT clock source is determined by the WDTCLK Configuration bits
#pragma config WDTCLK = LPRC    //WDT Clock Source Select bits->WDT uses LPRC

// FPOR
#pragma config BOREN = ON    //Brown Out Enable bit->Brown Out is enabled
#pragma config LPREGEN = OFF    //Low power regulator control->Low power regulator is disabled
#pragma config LPBOREN = ON    //Downside Voltage Protection Enable bit->Low power BOR is enabled when BOR is inactive

// FICD
#pragma config ICS = PGD2    //ICD Communication Channel Select bits->Communicate on PGEC2 and PGED2
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled
#pragma config BTSWP = OFF    //BOOTSWP Instruction Enable bit->BOOTSWP instruction is disabled

// FDMTIVTL
#pragma config DMTIVTL = 0    //DMT Window Interval Lower 16 bits->0

// FDMTIVTH
#pragma config DMTIVTH = 0    //DMT Window Interval Upper 16 bits->0

// FDMTCNTL
#pragma config DMTCNTL = 0    //DMT Instruction Count Time-out Value Lower 16 bits->0

// FDMTCNTH
#pragma config DMTCNTH = 0    //DMT Instruction Count Time-out Value Higher 16 bits->0

// FDMT
#pragma config DMTDIS = OFF    //DMT Disable bit->Dead Man Timer is disabled and can be enabled by software

// FDEVOPT1
#pragma config ALTCMPI = OFF    //Alternate Comparator Input Enable bit->C2INC and C3INC are on RG9 and RD11
#pragma config TMPRPIN = OFF    //Tamper Pin Enable bit->TMPRN pin function is disabled
#pragma config SOSCHP = ON    //SOSC High Power Enable bit (valid only when SOSCSEL = 1->SOSC high power mode (default)
#pragma config ALTI2C1 = OFF    //Alternate I2C pin Location->SDA1 and SCL1 on RA15 and RA14
#pragma config SMB3EN = ON    //SMBus 3 Enable bit->SMBus 3.0 input level


#include"xc.h"
#include"../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"
#include"../HVCT_121_MASTER_H_FILES/hvct_121_master_global_var.h"
//unsigned char temp_arr[255];
//unsigned char index_lcl;
int main(void)
{
//    unsigned char index_local=0;//unused variables
//    unsigned int temp = 255 ,present_ckt_number_lcl,address_lcl,relay_number_lcl,last_ckt_number_lcl=19 ,i_lcl;//unused variables
    Initialize_IO_Pins();
    Initialize_Lcd();
    InitialiseI2c();
    Initialise_CN_Interrupt();                          //.. interrupt for key detection.
    Initialize_SPI3();
    Initialize_Timer1_50ms ();                               // initialize timer 1 with time period of 50 ms
    Initialize_Timer2_100ms ();                               // initialize timer 1 with time period of 100 ms
    Initialize_Serial_1_RS232();
    Initialize_Serial_2_RS485();
//    InitializeByteArray();                               //Temporary function to send printing Data
   Initilize_interface_card ();
   Reset_HighVoltage();                                 // Initially set voltage to zero before testing 
  
    CS1_4151_SPI3_rh0_pin = 1 ;
////    //-------------------Testing Block Start-------------------------------// 
   
//   Retrive_Specific_Data(e2prom_2_page_2_add,test_setting_base_address,6,1,test_settings_ram,SET);
//    cards=assign_slave_address_and_card_detect();       // assign each slave address
//    hv_voltage_glb= 50;
//    insulation_resistance_glb= 25;
//    ir_last_ckt_no_glb =10;
//    HV_IR_test(ir_last_ckt_no_glb);
//    while(1);
//    {
//    Reset_HighVoltage();
//    
//    DelayS(3); 
//    Set_HighVoltage(500);           //High voltage is applied only after relays are switched. 
//    DelayS(3); 
//    
//    Reset_HighVoltage();
//    DelayS(1); 
//    insulation_resistance_glb = 25;
//    HV_Withstand_test(3);
////    HV_IR_test(3);
//    while(1);
//////
//    unsigned char print_byte_lcl=label_data[1];
 //   	Read_Convert_Shift_Data_1(digit_ram);
 //    Print_Lbl_Data();
//    DelayMs(1);
//    asm("nop");
//     Print_Lbl_Data();
     
    //-------------------Testing Block End -------------------------------//
    
    Remove_Blinking_Cursor();
    Display_String_20(welcome,1,10,1);                   //display "!WELCOME!".
    Display_String_20(futura_1,1,6,2);                  //display "FUTURA".
    Display_String_20(hvct,1,8,3);                     //display "HIGH VOLTAGE TESTER".
    Display_String_20(harness_tester,1,14,4);                     //display "HIGH VOLTAGE TESTER".
    DelayS(1);
    
    volume_level_global=E2prom_Byte_Read(e2prom_2_page_2_add,((volume_setting_address)& mask_higher_byte)>>8,(volume_setting_address)& mask_lower_byte);	
	Configure_Stored_Volume_Setting(volume_level_global);
    cards=assign_slave_address_and_card_detect();       // assign each slave address
   
//    Self_Test(); 
    Clear_Display();
    Display_Power_On_A(cards);                          //Displays SW Version & tester Serial No.         
    Retrive_Last_Selected_Program_And_Hvct_Data();
    Retrive_Specific_Data(e2prom_2_page_2_add,gen_settings_base_address,16,1,gen_settings_ram,SET);
    Retrive_Specific_Data(e2prom_2_page_2_add,test_setting_base_address,6,1,test_settings_ram,SET);
    volume_level_global=E2prom_Byte_Read(e2prom_2_page_2_add,((volume_setting_address)& mask_higher_byte)>>8,(volume_setting_address)& mask_lower_byte);	
	Configure_Stored_Volume_Setting(volume_level_global);
    Store_UUT_Variables_To_Eeprom(current_cable_number);
    Retrive_Prg_Specific_Settings_and_data(current_cable_number);
    DelayS(1);
    Clear_Display();
    Remove_Blinking_Cursor();
    fail_bin_data = ReadRtc(fail_bin_bbram_base_address);
    if(fail_bin_data == SET)
    {
        fail_bin_signal();
    }
    Idle_Display();
    while(TRUE)    // Super Loop start
    {
        if(key_pressed_flag==TRUE)
        {
            key_pressed_flag=FALSE;
            key_data_glb = IC8574_ByteRead();
            key_code =(key_data_glb & 0X0F);

            if((key_code==ESC_KEY)||(key_code==ENTER_KEY)||(key_code==UP_KEY)||(key_code==DOWN_KEY))			
            {
               DelayMs(40);         // calibrated delay for key operations
               key_parser();
            }
            else
            {
                key_pressed_flag=FALSE;
                key_code=0x00;
            }
        }//end if(key_pressed_flag==TRUE)
    }//End of while(TRUE) Super Loop End
      return 0;
}

/**
 End of File
*/


/*
 Function:This function will store all cable specific bbram related variables to their respective eeprom location
	 	  so that on power on as well as change cable number variables  can be retrieved back.
 Input:None.
 Output:None.
 Calls:Read_Write_RTC(),Write_Bulk_Data_Eeprom().
 Called By:...
*/
void Store_UUT_Variables_To_Eeprom(unsigned char cable_local)
{

//	//save serial number.
//	serial_number_count= ReadRtc(serial_number_bbram_count_address);					   //read serial number count.
//	if(serial_number_count>9)															   //if garbage value then initialise it.
//		serial_number_count=0;
//	if((serial_number_count!=0) && (serial_number_count<=9))
//	{
//		Read_Write_RTC(digit_ram,serial_number_bbram_base_address,serial_number_count,READ);  //read from bbram. 
//		Write_Bulk_Data_Eeprom(e2prom_2_page_2_add,serial_number_base_address,serial_number_max_size,cable_local,digit_ram);//write in eeprom.
//	}//end if((serial_number_count!=0) && (serial_number_count<=9))
	
	//---------------------------------------------------------------------//
	// save pass count.
	Read_Write_RTC(digit_ram,pass_count_bbram_base_address,pass_fail_count_size,READ);//read from bbram. 
	Write_Bulk_Data_Eeprom(e2prom_2_page_2_add,pass_count_base_address,pass_fail_count_size,cable_local,digit_ram);//write in eeprom.
	//---------------------------------------------------------------------//	
	//---------------------------------------------------------------------//
	//save fail count.
	Read_Write_RTC(digit_ram,fail_count_bbram_base_address,pass_fail_count_size,READ);//read from bbram.
	Write_Bulk_Data_Eeprom(e2prom_2_page_2_add,fail_count_base_address,pass_fail_count_size,cable_local,digit_ram);//write in eeprom.
//	//---------------------------------------------------------------------//	
}//end void Store_UUT_Variables_To_Eeprom(unsigned char cable_local).
