#include <p24FJ512GU408.h>

#include "../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"

/*
Function:
 unsigned char assign_slave_address_and_card_detect()
	
Description:
 This Function will assign each slave(HVIO card) address and return the no. of cards detected
  
Parameters:


Return Values: It returns no. of cards and is type of unsigned char.

Remarks: Nil
 */

const static unsigned char minda_noida_month[]={'A','B','C','D','F','G','H','J','K','L','M','N'};

unsigned char assign_slave_address_and_card_detect() 
{
    unsigned char cards_count_lcl = 0, address_lcl = 0;

    /*
     *Algorithm
    1.set card select pin for 1st io card 'low' using 74HC138 
      make enable pin(Active Low 10E & 20E) 74HC244 high to low to enable (USE card_select(unsigned char); Function)
    2.send broadcast command to assign the address 01 
    3.send uni cast command "how are you"  to address 01
    4. slave will respond and check "i am fine" command from slave
    5. save card no. in variable

    6.set card select pin for 2nd io card 'low' using 74HC138 
    7.make strobe pin 74HC244 high to low to enable 
    8.send broadcast command to assign the address 02 
    9.send uni cast command how are u  to add 02
    10.slave will respond and check i am fine command from slave
    11.Increment variable the card no. & save in variable
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
     * 
     */
    for (address_lcl = 1; address_lcl <= MAX_CARD_NUMBER; address_lcl++) 
    {
        assign_slave_address_flag_glb = FALSE;
        card_select(address_lcl);
        Fill_Any_Char_Ram_With(modbus_array_glb, MAX_MODBUS_ARRAY_SIZE, 0x00);
        Fill_Small_Data_Ram(modbus_array_glb, MCMD_BRODCAST, MCMD_ASSIGN_ADDRESS, address_lcl, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x03);	
        timer1_counter=0;                               //Initialize timer count
        Timer1_50ms_Start();                            //Start timer1 of 50 ms
        while(timer1_counter <= 8);                     //time_out delay of 400ms 

        Fill_Any_Char_Ram_With(modbus_array_glb, MAX_MODBUS_ARRAY_SIZE, 0x00);
        Fill_Small_Data_Ram(modbus_array_glb, address_lcl, MCMD_HOW_ARE_YOU, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x02);
        modbus_data_array_index_glb= 0;
        RS485_data_receive_cnt = 0;
        
        timer1_counter=0;                               //Initialize timer count
        while(timer1_counter <= 10);                    //wait until slave responds to how r u command and master receives i am fine command (timeout delay 500ms required) 
      
            if(data_reception_complete_flag_glb == TRUE)
            {
                data_reception_complete_flag_glb = FALSE;
                if(Validate_CRC_Received_Data(modbus_array_glb,modbus_data_array_index_glb-1))
                {
                    check_slave_feedback(address_lcl);
                    if (ack_valid_flag_glb == TRUE )
                    {
                        ack_valid_flag_glb = FALSE;
                        modbus_data_array_index_glb = 0;
                        RS485_data_receive_cnt = 0;
                        cards_count_lcl++;
                    }
                    else
                    {
                        modbus_data_array_index_glb=0;
                        modbus_data_array_index_glb = 0;
                        RS485_data_receive_cnt = 0;
                        break;
                    }
                }
                else
                {
                    modbus_data_array_index_glb=0;
                    modbus_data_array_index_glb = 0;
                    RS485_data_receive_cnt = 0;
                    break;
                }
            }
            else
            break;
    }
    Timer1_50ms_Stop();                     // Stop Timer 1 of 50ms
    enable_74hc138_rd10_pin = 0;            // Disable 74HC138
    DelayMs(1);
    return (cards_count_lcl);               // Return Cards count
}

unsigned char Read_Date_Time_1(unsigned char* ram_ptr, unsigned char display_error_flag_lcl)
{
    unsigned char temp_local = 0;
    unsigned char year_lsb_local = 0, day_local = 0;
    unsigned char leap_count_local = 0;
    //-----------------------------------------------------//
    //this block will look for leap year change.
    day_local = ReadRtc(0x05);
    // shift day code as required in numbers. 
    day_local = (day_local & 0xC0) >> 6;
//    debug_temp_glb = day_local; //just for viewing during debug
    //if lleap count is erronus or eeprom is change prompt user to reinitialise RTC.
    //read leap count previously stored in eeprom during last year change or RTC initialize.    
    leap_count_local = E2prom_Byte_Read(e2prom_2_page_2_add, (LEAP_COUNT_ADDRESS & mask_higher_byte) >> 8, (LEAP_COUNT_ADDRESS & mask_lower_byte));
    if (leap_count_local > 0x03) //if not valid value i.e. 0 to 3 then give error and prompt.. 
    { //..user to initialise rtc and return from function.
        if (display_error_flag_lcl == TRUE) 
        {
            Display_String_20(space, 1, 20, 2); //display for user.
            Display_String_20(rtc_error, 1, 11, 2); //display for user.
            Display_String_20(set_date_time, 1, 17, 3);
            DelayS(2); //settling delay               
            Display_String_20(space, 1, 20, 2);
            Display_String_20(space, 1, 20, 3);
        }//end if(display_error_flag_lcl==TRUE)    
        return (FALSE); //return from function.
    }//end if(leap_count_loacl>0x03)    
    //leap count storage is shifted from bbram to eeprom and numbered value is stored instead of real value.
    //leap_count_loacl = ReadRtc(0x12);
    //leap_count_loacl = leap_count_loacl & 0xC0;
//    temp_char_glb = leap_count_local;
    //year_lsb_local = ReadRtc(0x11);
    year_lsb_local = E2prom_Byte_Read(e2prom_2_page_2_add, (YEAR_LSB_COUNT_ADDRESS & mask_higher_byte) >> 8, (YEAR_LSB_COUNT_ADDRESS & mask_lower_byte));
    //debug_temp_glb=year_lsb_local;
    //if(year_lsb_local) remaining...
    //leap_count_loacl=2;
    //day_local=1;
    if (leap_count_local != day_local)
    {
        if (leap_count_local == 0x03) 
        {
            if (day_local != 0x00) 
            {
                if (display_error_flag_lcl == TRUE) 
                {
                    Display_String_20(space, 1, 20, 2); //display for user.
                    Display_String_20(rtc_error, 1, 11, 2); //display for user.
                    Display_String_20(set_date_time, 1, 17, 3);
                    DelayS(2); //settling delay
                    Display_String_20(space, 1, 20, 2);
                    Display_String_20(space, 1, 20, 3);
                }//end if(display_error_flag_lcl==TRUE)    
                return (FALSE);
            }//END if(day_local!=0x00)    
        }//end if(leap_count_loacl==0x03)
        else
        {
            if (day_local == (leap_count_local + 1)) //ok condition.
                ;
            else
            {
                if (display_error_flag_lcl == TRUE) {
                    Display_String_20(space, 1, 20, 2); //display for user.
                    Display_String_20(rtc_error, 1, 11, 2); //display for user.
                    Display_String_20(set_date_time, 1, 17, 3);
                    DelayS(2); //settling delay               
                    Display_String_20(space, 1, 20, 2);
                    Display_String_20(space, 1, 20, 3);
                }//end if(display_error_flag_lcl==TRUE) 
                return (FALSE);
            }//end else    
        } //end else if(leap_count_loacl==0x03)   
        E2prom_Byte_Write(e2prom_2_page_2_add, ((LEAP_COUNT_ADDRESS) & mask_higher_byte) >> 8, (LEAP_COUNT_ADDRESS) & mask_lower_byte, day_local);
        //WriteRtc(0x12,day_local);
        year_lsb_local++;
        //debug_temp_glb=year_lsb_local;
        //--------10_07_19_2-----------start-------------//
        if (((year_lsb_local & 0x0F) > 0x09) && ((year_lsb_local & 0x0F) <= 0x0F)) 
        {
            //year_lsb_local=HexToBCD_1( year_lsb_local);
            year_lsb_local = (((year_lsb_local & 0xF0) >> 4)) + 1;
            year_lsb_local = (year_lsb_local) << 4;
            //    z=year_lsb_local;
        }//end if(year_lsb_local>0x09 && year_lsb_local<0x0F).
        //--------10_07_19_2-----------ends--------------//
        E2prom_Byte_Write(e2prom_2_page_2_add, ((YEAR_LSB_COUNT_ADDRESS) & mask_higher_byte) >> 8, (YEAR_LSB_COUNT_ADDRESS) & mask_lower_byte, year_lsb_local);
        //WriteRtc(0x10,0x20);
        //WriteRtc(0x11,year_lsb_local);

    }//end if(leap_count_loacl != day_local)
    //----------------------------------------------------//
    //---------capture the day----------------------------//	
    temp_local = ReadRtc(0x05) & 0x3F;
    ram_ptr[0] = (((temp_local & 0xF0) >> 4) + 0x30);
    ram_ptr[1] = (temp_local & 0x0F) + 0x30;
    ram_ptr[2] = '/';
    //---------capture the month-------------------------//	
    temp_local = ReadRtc(0x06)& 0x1F;
    ram_ptr[3] = ((temp_local & 0xF0) >> 4) + 0x30;
    ram_ptr[4] = (temp_local & 0x0F) + 0x30;
    ram_ptr[5] = '/';
    //---------capture the year lsb----------------------//
    //05_08_19_2 changed to eeprom 
    //temp_local  =  ReadRtc(0x11);
    temp_local = year_lsb_local;
    ram_ptr[8] = ((temp_local & 0xF0) >> 4) + 0x30;
    ram_ptr[9] = (temp_local & 0x0F) + 0x30;
    //---------capture the year msb----------------------//
    //05_08_19_2 hard coded forever.. 
    //temp_local  =  ReadRtc(0x10);                     
    temp_local = 0x20;
    ram_ptr[6] = ((temp_local & 0xF0) >> 4) + 0x30;
    ram_ptr[7] = (temp_local & 0x0F) + 0x30;
    //----------capture hour ---------------------------//
    temp_local = ReadRtc(0x04);
    ram_ptr[10] = ((temp_local & 0xF0) >> 4) + 0x30;
    ram_ptr[11] = (temp_local & 0x0F) + 0x30;
    ram_ptr[12] = ':';
    //---------capture minute--------------------------//
    temp_local = ReadRtc(0x03); //minute.
    ram_ptr[13] = ((temp_local & 0xF0) >> 4) + 0x30;
    ram_ptr[14] = (temp_local & 0x0F) + 0x30;
    ram_ptr[15] = ':';
    //--------capture second---------------------------//
    temp_local = ReadRtc(0x02);
    ram_ptr[16] = ((temp_local & 0xF0) >> 4) + 0x30;
    ram_ptr[17] = (temp_local & 0x0F) + 0x30;

    return (TRUE);

}//end void Read_Date_Time(unsigned char* ram_ptr).

unsigned char Validate_Date_Time(unsigned char* ram_ptr_lcl) 
{
    //unsigned char index_lcl=0;

    if (ram_ptr_lcl[0] < 0x30 || ram_ptr_lcl[0] > 0x33)//dd
        return (FALSE);
    if (ram_ptr_lcl[1] < 0x30 || ram_ptr_lcl[1] > 0x39)//dd
        return (FALSE);
    if (ram_ptr_lcl[3] < 0x30 || ram_ptr_lcl[3] > 0x31)//mm
        return (FALSE);
    if (ram_ptr_lcl[4] < 0x30 || ram_ptr_lcl[4] > 0x39)//mm
        return (FALSE);
    if (ram_ptr_lcl[6] != 0x32)//yyyy
        return (FALSE);
    if (ram_ptr_lcl[7] != 0x30)//yyyy
        return (FALSE);
    if (ram_ptr_lcl[8] < 0x30 || ram_ptr_lcl[8] > 0x39)//yyyy
        return (FALSE);
    if (ram_ptr_lcl[9] < 0x30 || ram_ptr_lcl[9] > 0x39)//yyyy
        return (FALSE);
    if (ram_ptr_lcl[10] < 0x30 || ram_ptr_lcl[10] > 0x32)//hh
        return (FALSE);
    if (ram_ptr_lcl[11] < 0x30 || ram_ptr_lcl[11] > 0x39)//hh
        return (FALSE);
    if (ram_ptr_lcl[13] < 0x30 || ram_ptr_lcl[13] > 0x36)//mm
        return (FALSE);
    if (ram_ptr_lcl[14] < 0x30 || ram_ptr_lcl[14] > 0x39)//mm
        return (FALSE);
    if (ram_ptr_lcl[16] < 0x30 || ram_ptr_lcl[16] > 0x36)//ss
        return (FALSE);
    if (ram_ptr_lcl[17] < 0x30 || ram_ptr_lcl[17] > 0x39)//ss
        return (FALSE);

    return (TRUE);

}

/*
Functon:
Input:None.
Output:None.
Calls:None.
Called By:
 */
void Fill_Any_Char_Ram_With(unsigned char* test_ptr, unsigned int size_local, unsigned char data_local) 
{
    unsigned int index_local;

    for (index_local = 0; index_local < size_local; index_local++) 
    {
        test_ptr[index_local] = data_local;
    }//end for(index_local=0;index_local<size_local;index_local++)
}//end void Fill_Any_Char_Ram_With(unsigned char* test_ptr,unsigned int size_local,unsigned char data_local)
/*
Functon:
Input:None.
Output:None.
Calls:None.
Called By:
 */
void Fill_Any_float_Ram_With(float* test_ptr, unsigned int size_local,float data_local) 
{
    unsigned int index_local;

    for (index_local = 0; index_local < size_local; index_local++) 
    {
        test_ptr[index_local] = data_local;
    }//end for(index_local=0;index_local<size_local;index_local++)
}//end void Fill_Any_Char_Ram_With(unsigned char* test_ptr,unsigned int size_local,unsigned char data_local)

unsigned char Convert(unsigned char* ram_ptr, unsigned char a, unsigned char b) 
{
    unsigned char num_local, temp_local;

    num_local = ram_ptr[a];
    num_local = num_local - 0x30;
    num_local = num_local << 4;
    temp_local = ram_ptr[b] - 0x30;
    num_local = num_local | temp_local;
    return (num_local);
}//end unsigned char Convert(unsigned char* ram_ptr,unsigned char a,unsigned char b)

/*
 Function:This function will retrive HVCT input data (cable number, last ckt no., hv voltage, insulation resistance , withstand time) from eeprom generally used cable specific settings.
          Settings which are having either SET or RESET values.
 Input:	  
 Output:  
 Calls:  E2prom_Byte_Write().,  E2prom_Byte_Read
 Called By:----
 */
void Retrive_Last_Selected_Program_And_Hvct_Data(void) 
{
    unsigned char last_ckt_number_lcl = 0;
    unsigned char hv_msb_lcl = 0, hv_lsb_lcl = 0;
    unsigned int hv_voltage_lcl = 0, insulation_resistance_lcl = 0, hv_time_lcl = 0;
    disp_start_ptr = start_main_menu;


    current_cable_number = E2prom_Byte_Read(e2prom_2_page_2_add, ((current_cable_number_address) & mask_higher_byte) >> 8, (current_cable_number_address) & mask_lower_byte);
    if (current_cable_number < 1 || current_cable_number > 128)
    {
        current_cable_number = 1;
        E2prom_Byte_Write(e2prom_2_page_2_add, ((current_cable_number_address) & mask_higher_byte) >> 8, (current_cable_number_address) & mask_lower_byte, current_cable_number);
    }

    last_ckt_number_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((last_ckt_number_base_address + (current_cable_number - 1)) & mask_higher_byte) >> 8, (last_ckt_number_base_address + (current_cable_number - 1)) & mask_lower_byte);
    ir_last_ckt_no_glb = last_ckt_number_lcl;


    hv_msb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((high_voltage_base_address + (current_cable_number * 2 - 2)) & mask_higher_byte) >> 8, (high_voltage_base_address + (current_cable_number * 2 - 2)) & mask_lower_byte);
    hv_lsb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((high_voltage_base_address + (current_cable_number * 2 - 1)) & mask_higher_byte) >> 8, (high_voltage_base_address + (current_cable_number * 2 - 1)) & mask_lower_byte);
    hv_voltage_lcl = hv_msb_lcl << 8;
    hv_voltage_glb = (hv_voltage_lcl | hv_lsb_lcl);

    hv_msb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((insulation_resistance_base_address + (current_cable_number * 2 - 2)) & mask_higher_byte) >> 8, (insulation_resistance_base_address + (current_cable_number * 2 - 2)) & mask_lower_byte);
    hv_lsb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((insulation_resistance_base_address + (current_cable_number * 2 - 1)) & mask_higher_byte) >> 8, (insulation_resistance_base_address + (current_cable_number * 2 - 1)) & mask_lower_byte);
    insulation_resistance_lcl = hv_msb_lcl << 8;
    insulation_resistance_glb = (insulation_resistance_lcl | hv_lsb_lcl);


    hv_msb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((withstand_time_base_address + (current_cable_number * 2 - 2)) & mask_higher_byte) >> 8, (withstand_time_base_address + (current_cable_number * 2 - 2)) & mask_lower_byte);
    hv_lsb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((withstand_time_base_address + (current_cable_number * 2 - 1)) & mask_higher_byte) >> 8, (withstand_time_base_address + (current_cable_number * 2 - 1)) & mask_lower_byte);
    hv_time_lcl = hv_msb_lcl << 8;
    hv_withstand_time_glb = (hv_time_lcl | hv_lsb_lcl);
    
    last_ckt_number_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((withstand_last_ckt_number_base_address + (current_cable_number - 1)) & mask_higher_byte) >> 8, (withstand_last_ckt_number_base_address + (current_cable_number - 1)) & mask_lower_byte);
    withstand_last_ckt_no_glb = last_ckt_number_lcl;


    hv_msb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((withstand_high_voltage_base_address + (current_cable_number * 2 - 2)) & mask_higher_byte) >> 8, (withstand_high_voltage_base_address + (current_cable_number * 2 - 2)) & mask_lower_byte);
    hv_lsb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((withstand_high_voltage_base_address + (current_cable_number * 2 - 1)) & mask_higher_byte) >> 8, (withstand_high_voltage_base_address + (current_cable_number * 2 - 1)) & mask_lower_byte);
    hv_voltage_lcl = hv_msb_lcl << 8;
    withstand_hv_voltage_glb = (hv_voltage_lcl | hv_lsb_lcl);

    hv_msb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((withstand_current_base_address + (current_cable_number * 2 - 2)) & mask_higher_byte) >> 8, (withstand_current_base_address + (current_cable_number * 2 - 2)) & mask_lower_byte);
    hv_lsb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add, ((withstand_current_base_address + (current_cable_number * 2 - 1)) & mask_higher_byte) >> 8, (withstand_current_base_address + (current_cable_number * 2 - 1)) & mask_lower_byte);
    insulation_resistance_lcl = hv_msb_lcl << 8;
    withstand_current_glb = (insulation_resistance_lcl | hv_lsb_lcl);


}

/*
 Function:This function will retrive data from eeprom generally used for general and cable specific settings.
          Settings which are having either SET or RESET values.
 Input:	  eeprom_number(unsigned char),internal address of eeprom(unsigned char),size of bytes to be read(unsigned int type),index for ram(int type), array for reading data(unsigned char* type),restore or not (unsigned char type).
 Output:  Array with read bytes.
 Calls:  E2prom_Byte_Write().
 Called By:----
 */
void Retrive_Specific_Data(unsigned char eeprom_addr_local, unsigned int address_local, unsigned int size_local, int index_local, unsigned char* ptr_local, unsigned char type_local)
{

    for (; index_local <= size_local; index_local++)// for number of bytes to be read.
    {
        ptr_local[index_local] = E2prom_Byte_Read(eeprom_addr_local, ((address_local) & mask_higher_byte) >> 8, (address_local) & mask_lower_byte);
        if (type_local == SET) 
        {
            //if read byte is having garbage vaule i.e. neither set or reset then initialise it.
            if (ptr_local[index_local] != SET && ptr_local[index_local] != RESET) 
            {
                E2prom_Byte_Write(eeprom_addr_local, ((address_local) & mask_higher_byte) >> 8, (address_local) & mask_lower_byte, RESET);
                ptr_local[index_local] = E2prom_Byte_Read(eeprom_addr_local, ((address_local) & mask_higher_byte) >> 8, (address_local) & mask_lower_byte);
            }//end if(ptr_local[index_local]!=SET && ptr_local[index_local]!=RESET)
        }//end if(type_local==SET)	
        address_local++;


    }//end for(;index_local<=size_local;index_local++)// for number of bytes to be read.

}//end void Retrive_Specific_Data(unsigned char eeprom_addr_local,unsigned int address_local,unsigned int size_local,int index_local,unsigned char* ptr_local,unsigned char type_local).

/*
 Function:This function will retrieve cable specific settings such as IR test setting , withstand test setting , pass count ,fail count etc from eeprom to ram
 Input:Current location of program number (unsigned char type), ram in which data has to be retrived(unsigned char type). 
 Output:Ram with all stored settings at there respective indices.
 Calls:Retrive_Specific_Data().
 Called By:
 */
void Retrive_Prg_Specific_Settings_and_data(unsigned char current_cable_local) 
{
    unsigned int address_local = 0; //address variable

    address_local = withstand_test_setting_base_address + current_cable_local; 
    //a version
    Retrive_Specific_Data(e2prom_2_page_2_add, address_local, 2, 2, prg_specific_setting_ram, SET); //11/10/11.

    address_local = ir_test_setting_base_address + current_cable_local; 
    Retrive_Specific_Data(e2prom_2_page_2_add, address_local, 1, 1, prg_specific_setting_ram, SET); //11/10/11.
    
    
//    //pass count..
//	Read_Bulk_Data_Eeprom(e2prom_2_page_2_add,pass_count_base_address,pass_fail_count_size,current_cable_local,digit_ram);//11/10/11.
//	Initialise_Numeric_Counts_If_Garbage_Value(digit_ram, pass_fail_count_size);
//	Read_Write_RTC(digit_ram,pass_count_bbram_base_address,3,WRITE);
//	//----------------------------------------------------------------//
//	//fail count..
//	Read_Bulk_Data_Eeprom(e2prom_2_page_2_add,fail_count_base_address,pass_fail_count_size,current_cable_local,digit_ram);//11/10/11.
//	Initialise_Numeric_Counts_If_Garbage_Value(digit_ram, pass_fail_count_size);
//	Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,WRITE);
//	//----------------------------------------------------------------//

}

/*
 Function:This function will read respective settings data from eeprom.
          According to current setting  display it accordingly.
 Input:   State of key parser (unsigned char), accessing index for ram(unsigned char),line (unsigned char).  
 Output:  none.
 Calls:  Retrive_Specific_Data(),Retrive_Cable_Specific_Settings(),Display_String_20().
 Called By:
 */
void Display_Setreset_Settings(unsigned char state_local, unsigned char conter_local, unsigned char line_local) 
{
    unsigned char * ram_ptr_lcl; 	
    unsigned char flag_local = 0;
    //---------------------------------------------------------------------//
    //switch according to current state.
    switch (state_local) 
    {
        case ST_SET_UP_GEN_SETTINGS://assign general settings ram to pointer,and read general settings.
            ram_ptr_lcl = gen_settings_ram;
            Retrive_Specific_Data(e2prom_2_page_2_add, gen_settings_base_address, 16, 1, ram_ptr_lcl, SET); //11/10/11.
//            ram_ptr_lcl = gen_settings_ram; 
            break;
        case ST_SET_UP_PRG_SETTINGS://assign cable settings ram to pointer,and read cable specific settingss.
            ram_ptr_lcl = prg_specific_setting_ram;
            Retrive_Prg_Specific_Settings_and_data(current_cable_number);
            break;
            
        case ST_SET_UP_TEST_SETTINGS://assign general settings ram to pointer,and read general settings.
            ram_ptr_lcl = test_settings_ram;
            Retrive_Specific_Data(e2prom_2_page_2_add, test_setting_base_address, 6, 1, ram_ptr_lcl, SET); //11/10/11.
            break;
           
        default: flag_local = TRUE; // for any other condition make flag true to restrict entry in next condition.
            break;
    }
    //--------------------------------------------------------------------//
    //for any other state than switch condition restrict the entry.
    if (flag_local != TRUE) 
    {
        if (ram_ptr_lcl[conter_local] == RESET) //if reset then display set?.
        {
            if (analyse_flag == TRUE) //for analyse menu dont display question mark.
                Display_String_20(off, 13, 5, line_local);
            else
                Display_String_20(on, 13, 6, line_local); //for other menu display question mark.

        } else if (ram_ptr_lcl[conter_local] == SET) // if set display reset? 
        {
            if (analyse_flag == TRUE) //for analyse menu dont display question mark.
                Display_String_20(on, 13, 5, line_local);
            else
                Display_String_20(off, 13, 6, line_local); //for other menu display question mark.
        }
    } else 
    {
        Display_String_20(clear, 13, 6, line_local); //display clear to clear counters of clear menu.
    }

}

void Caliberate_Serial_1(unsigned char type_lcl)
{
    unsigned char counter_local = 0;
    key_pressed_flag = 0x00;
    key_code = 0x00;
    byte_received_flag = FALSE;
    while (1)
    {
        if (key_pressed_flag == TRUE) 
        {

            key_pressed_flag = FALSE;
            key_data_glb = IC8574_ByteRead();
            key_code = (key_data_glb & 0X0F);
            if (key_code == ENTER_KEY) 
            {
                key_pressed_flag = 0x00;
                key_code = 0x00;
                byte_received_flag = FALSE;

                Clear_Display();

                if (type_lcl == 1)
                    Display_String_20(Rs232_1_tesT, 1, 12, 2);
                else
                    ;
                Display_String_20(arrow, 1, 4, 3);
                if (type_lcl == 1)
                    Transmit_Byte_Serial_1('H'); //11/101/11.
                else
                    ;
        
                timecount = 0;
            }
            else if(key_code == ESC_KEY)
            {
                break;
            }
        }

        if (byte_received_flag == TRUE && counter_local <= 3)
        {
            timecount = 0;
            byte_received_flag = FALSE;
            switch (counter_local) 
            {
                case 0:
                        if (ByteReceived == 'H')
                        {
                            counter_local++;

                            if (type_lcl == 1)
                                Transmit_Byte_Serial_1('V');
                            else
                                ;
                        }//end if(ByteReceived=='F')
                        else 
                        {
                            Display_String_20(fail, 5, 4, 3);
                            DelayMs(500);
                            DelayMs(500);
                            //								Ring_Buzzer_A(local_ram,FAIL_TONE,E2prom_Byte_Read((e2prom_2_page_2_add & mask_lower_byte),((tone_address)&mask_higher_byte) >> 8,(tone_address)& mask_lower_byte)+4,0x02);//11/10/11.
                            return;
                            ;
                        }//end else
                        break;
                case 1:
                        if (ByteReceived == 'V') 
                            {
                                counter_local++;

                                if (type_lcl == 1)
                                    Transmit_Byte_Serial_1('C');
                                else
                                    ;
                            }//end if(ByteReceived=='A')
                        else
                        {
                            Display_String_20(fail, 5, 4, 3);
                            DelayMs(500);
                            DelayMs(500);
                            return;
                        }//end else..
                        break;
                case 2:
                        if (ByteReceived == 'C') 
                        {
                            counter_local++;
                            if (type_lcl == 1)
                                Transmit_Byte_Serial_1('T'); //11/101/11.
                            else
                                ;
                        }//end if(ByteReceived=='C')
                        else
                        {
                            Display_String_20(fail, 5, 4, 3); // A VERSION
                            DelayMs(500);
                            DelayMs(500);
                            return;
                            ;
                        }//end else..
                        break;
                case 3:
                        if (ByteReceived == 'T')
                        {
                        DelayMs(500);
                        DelayMs(500);
                        DelayMs(500);

                        Display_String_20(pass, 5, 4, 3); // A VERSION
                        DelayMs(500);
                        DelayMs(500);
                        return;
                        }//end if(ByteReceived=='T')
                        else
                        {
                            Display_String_20(fail, 5, 4, 3); // A VERSION
                            DelayMs(500);
                            DelayMs(500);
                            return;
                            ;
                        }//end else..
                        break;
            }//end switch(counter_local).

        }//end if (byte_received_flag == TRUE && counter_local<=3).
    }//end while (loop_break_flag==FALSE).

}//end void Caliberate_Serial_1(unsigned char type_lcl).

/*
Function:   
Inputs:
Outouts:
Calls:
Called By:
 */
void E2prom_test()
{
    unsigned char read_local[3], index_local = 0, e2prom_address_local = 0;
    //	unsigned char local_ram[15];

    e2prom_address_local = 0xA4;
    read_local[0] = 0x00;
    read_local[1] = 0x00;
    Display_String_20("NA", 13, 2, 1);
    for (index_local = 1; index_local < 4; index_local++) 
    {
        E2prom_Byte_Write(e2prom_address_local, ((memory_chk_reserved_address) & mask_higher_byte) >> 8, (memory_chk_reserved_address) & mask_lower_byte, 0xFF);
        E2prom_Byte_Write(e2prom_address_local, ((memory_chk_reserved_address) & mask_higher_byte) >> 8, (memory_chk_reserved_address) & mask_lower_byte, 'F');
        E2prom_Byte_Write(e2prom_address_local + 2, ((memory_chk_reserved_address) & mask_higher_byte) >> 8, (memory_chk_reserved_address) & mask_lower_byte, 0xFF);
        E2prom_Byte_Write(e2prom_address_local + 2, ((memory_chk_reserved_address) & mask_higher_byte) >> 8, (memory_chk_reserved_address) & mask_lower_byte, 'F');
        read_local[0] = E2prom_Byte_Read(e2prom_address_local, ((memory_chk_reserved_address) & mask_higher_byte) >> 8, (memory_chk_reserved_address) & mask_lower_byte);
        read_local[1] = E2prom_Byte_Read(e2prom_address_local + 2, ((memory_chk_reserved_address) & mask_higher_byte) >> 8, (memory_chk_reserved_address) & mask_lower_byte);
        //10_07_19_1.Brackets added.
        if ((read_local[0] == 'F')&& (read_local[1] == 'F')) 
        {
            DelayMs(500);
            DelayMs(500);
            Display_String_20(pass, 13, 4, index_local + 1);
            Gen_Tone(PASS_TONE,4);
        } 
        else 
        {
            DelayMs(500);
            DelayMs(500);
            Display_String_20(fail, 13, 4, index_local + 1);
            Gen_Tone(FAIL_TONE,4);
        }
        e2prom_address_local = e2prom_address_local + 4;
        read_local[0] = 0x00;
        read_local[1] = 0x00;
    }//end for(index_local=1;index_local<4;index_local++)


}//end void E2prom_test()

/*
Function:   
Inputs:
Outouts:
Calls:
Called By:
 */
void Display_date_time() 
{
    unsigned char local_ram[24];

    key_pressed_flag = 0x00;
    key_code = 0x00;

    while (1) 
    {
        if (key_pressed_flag == TRUE)
        {
            //----------27_04_13_5------start-------//
            key_pressed_flag = FALSE;
            key_data_glb = IC8574_ByteRead();
            key_code = (key_data_glb & 0X0F);
            if (key_code != ESC_KEY) 
            {
                key_code = 0x00;
            }
            if (key_code == ESC_KEY)
            {
                key_code = 0x00;
                return;
            }
            //----------27_04_13_5------end---------//		
        }
        //05_08_19_1 use with error display.
        if (Read_Date_Time_1(local_ram, TRUE) != TRUE)
            return;
//        Display_Ram(local_ram, 1, 0, 10, 3);
//        Display_Ram(local_ram, 1, 10, 8, 4);
        Display_Ram(local_ram, 1, 0, 10, 2);
        Display_Ram(local_ram, 1, 10, 8, 4);
        DelayMs(250);
        DelayMs(250);
    }
}

void Send_Modbus_Data(unsigned char *array_ptr_lcl, unsigned int start_index_lcl, unsigned int size_lcl) 
{
    unsigned int index_lcl = 0, crc_lcl = 0;

    //----------B.3-----start---------//
    array_ptr_lcl[size_lcl] = 0xAA;
    array_ptr_lcl[size_lcl + 1] = 0xAA;
    array_ptr_lcl[size_lcl + 2] = 0xAA;
    array_ptr_lcl[size_lcl + 3] = 0xAA;
    size_lcl = size_lcl + 4;

    modbus_receive_enable_flag_glb = FALSE;
    crc_lcl = CRC_Calculate(array_ptr_lcl, size_lcl);
    array_ptr_lcl[size_lcl] = (unsigned char) (crc_lcl & 0x00FF);
    array_ptr_lcl[size_lcl + 1] = (unsigned char) ((crc_lcl & 0xFF00) >> 8);

    size_lcl = size_lcl + 2;
    modbus_array_index_glb = 0; //B.4
    rs485_control_ra10_pin = 1;
    for (index_lcl = start_index_lcl; index_lcl < size_lcl; index_lcl++)
    {
        Transmit_Byte_RS485(*(array_ptr_lcl + index_lcl));
    }//end for(index_lcl=start_index_lcl;index_lcl<size_lcl;index_lcl++).
    rs485_control_ra10_pin = 0;


}//end void Send_Modbus_Data(unsigned char *array_ptr_lcl,unsigned int start_index_lcl,unsigned int size_lcl,unsigned char port_lcl)

//  Function:
//  unsigned int CRC_Check(unsigned char *buf, unsigned int len)
//  Description:
//  This function calculates CRC (cyclic redundancy check).
//  Parameters:
//  A string (transmit_buffer[]) is passed with its length (len) to calculate CRC of 
//	the data to be transmitted to slave.
//  Return Values:
//  Function returns value of final CRC that is calculated and is of datatype unsigned int.
//  Remarks:
//   None

//---------------------------------------------------------------------

unsigned int CRC_Calculate(unsigned char * transmit_buffer_lcl, unsigned int len_lcl) 
{
    unsigned int crc_lcl = 0xFFFF; //initialise with 0xFFFF	
    unsigned int index_lcl_1, index_lcl_2;


    //for loop up to the total number of bytes present in array.
    for (index_lcl_1 = 0; index_lcl_1 < len_lcl; index_lcl_1++)
    {
        crc_lcl = crc_lcl ^ transmit_buffer_lcl[index_lcl_1]; //Xor with crc_lcl value.

        //loop for each bit of crc_lcl.
        for (index_lcl_2 = 0; index_lcl_2 < 8; index_lcl_2++) 
        {
            if ((crc_lcl & 0x0001) == 1) // check for LSB == 1
            {
                crc_lcl = (crc_lcl >> 1) & 0x7FFF; //shift data to right by 1
                crc_lcl = crc_lcl ^ 0xA001; //ex or crc data by preset value 0xa001
            }//end if((crc_lcl & 0x0001) == 1)				// check for LSB == 1
            else 
            {
                crc_lcl = (crc_lcl >> 1) & 0x7FFF; // for LSB == 0, shift data to 
                // right by 1
            }//end if((crc_lcl & 0x0001) == 1)				// check for LSB == 1. 
        }//end for(index_lcl_2 = 0;index_lcl_2 < 8; index_lcl_2 ++).
    }//end for(index_lcl_1 = 0; index_lcl_1 < len_lcl; index_lcl_1 ++).
    return crc_lcl; // calculated crc is returned
}//end function..



/*
Function: unsigned int Validate_CRC_Received_Data(unsigned char* array_ptr_lcl,unsigned int index_array_ptr_lcl)
Description: This function compares received CRC bytes with calculated CRC		
Input: unsigned char* array_ptr_lcl,
Output:
Calls:
Called By:
*/
unsigned int Validate_CRC_Received_Data(unsigned char* array_ptr_lcl,unsigned int index_array_ptr_lcl)
{

	unsigned int crc_received_lcl=0,crc_calculated_lcl=0;
    
	crc_received_lcl = ((array_ptr_lcl[index_array_ptr_lcl]<<8) + array_ptr_lcl[index_array_ptr_lcl - 1]);

	crc_calculated_lcl = CRC_Calculate(array_ptr_lcl,index_array_ptr_lcl-1);

	if(crc_received_lcl == crc_calculated_lcl)
		return(TRUE);//return non zero value as indication of truness.
	else
		return(FALSE);
}
/*
Function: void card_select(unsigned char card_local) 
Description: 
Input:
Output:
Calls:
Called By:
*/
void check_slave_feedback(unsigned char slave_address)
{
    if (modbus_array_glb[0] == slave_address && modbus_array_glb[1] == MCMD_I_AM_FINE  )
        ack_valid_flag_glb = TRUE ;
    else
        ack_valid_flag_glb = FALSE;
}




/*
Function: void card_select(unsigned char card_local) 
Description: This Function makes pin LOW related to card selection given input card number card and makes  other pins HIGH using 74hc138 decoder hence
            * only one pin is low at a time . this function is used in assigning each slave(IO card) new address and card detection function             
Input: card number 1 to 8
Output: void
Calls: none
Called By:
*/
void card_select(unsigned char card_local) 
{
    enable_74hc138_rd10_pin = 0;
    switch (card_local) 
    {
        case 1:
            card_select_A2_rd9_pin = 0; // 74HC138 A2 select line
            asm("nop");
            card_select_A1_rd8_pin = 0; // 74HC138 A1 select line
            asm("nop");
            card_select_A0_rg7_pin = 0; // 74HC138 A0 select line
            asm("nop");
            enable_74hc138_rd10_pin = 1; //74HC138 E3 enable pin
            asm("nop");
            enable_74hc244_rd11_pin = 0; //74HC244 10E(Active Low) enable pin
            break;

        case 2:
            card_select_A2_rd9_pin = 0; // 74HC138 A2 select line
            asm("nop");
            card_select_A1_rd8_pin = 0; // 74HC138 A1 select line
            asm("nop");
            card_select_A0_rg7_pin = 1; // 74HC138 A0 select line
            asm("nop");
            enable_74hc138_rd10_pin = 1; //74HC138 E3 enable pin
            asm("nop");
            enable_74hc244_rd11_pin = 0; //74HC244 10E(Active Low) enable pin
            break;
            
        case 3:
            card_select_A2_rd9_pin = 0; // 74HC138 A2 select line
            asm("nop");
            card_select_A1_rd8_pin = 1; // 74HC138 A1 select line
            asm("nop");
            card_select_A0_rg7_pin = 0; // 74HC138 A0 select line
            asm("nop");
            enable_74hc138_rd10_pin = 1; //74HC138 E3 enable pin
            asm("nop");
            enable_74hc244_rd11_pin = 0; //74HC244 10E(Active Low) enable pin
            break;
            
        case 4:
            card_select_A2_rd9_pin = 0; // 74HC138 A2 select line
            asm("nop");
            card_select_A1_rd8_pin = 1; // 74HC138 A1 select line
            asm("nop");
            card_select_A0_rg7_pin = 1; // 74HC138 A0 select line
            asm("nop");
            enable_74hc138_rd10_pin = 1; //74HC138 E3 enable pin
            asm("nop");
            enable_74hc244_rd11_pin = 0; //74HC244 10E(Active Low) enable pin
            break;
            
        case 5:
            card_select_A2_rd9_pin = 1; // 74HC138 A2 select line
            asm("nop");
            card_select_A1_rd8_pin = 0; // 74HC138 A1 select line
            asm("nop");
            card_select_A0_rg7_pin = 0; // 74HC138 A0 select line
            asm("nop");
            enable_74hc138_rd10_pin = 1; //74HC138 E3 enable pin
            asm("nop");
            enable_74hc244_rd11_pin = 0; //74HC244 10E(Active Low) enable pin
            break;
            
        case 6:
            card_select_A2_rd9_pin = 1; // 74HC138 A2 select line
            asm("nop");
            card_select_A1_rd8_pin = 0; // 74HC138 A1 select line
            asm("nop");
            card_select_A0_rg7_pin = 1; // 74HC138 A0 select line
            asm("nop");
            enable_74hc138_rd10_pin = 1; //74HC138 E3 enable pin
            asm("nop");
            enable_74hc244_rd11_pin = 0; //74HC244 10E(Active Low) enable pin
            break;
            
        case 7:
            card_select_A2_rd9_pin = 1; // 74HC138 A2 select line
            asm("nop");
            card_select_A1_rd8_pin = 1; // 74HC138 A1 select line
            asm("nop");
            card_select_A0_rg7_pin = 0; // 74HC138 A0 select line
            asm("nop");
            enable_74hc138_rd10_pin = 1; //74HC138 E3 enable pin
            asm("nop");
            enable_74hc244_rd11_pin = 0; //74HC244 10E(Active Low) enable pin
            break;
            
        case 8:
            card_select_A2_rd9_pin = 1; // 74HC138 A2 select line
            asm("nop");
            card_select_A1_rd8_pin = 1; // 74HC138 A1 select line
            asm("nop");
            card_select_A0_rg7_pin = 1; // 74HC138 A0 select line
            asm("nop");
            enable_74hc138_rd10_pin = 1; //74HC138 E3 enable pin
            asm("nop");
            enable_74hc244_rd11_pin = 0; //74HC244 10E(Active Low) enable pin
            break;
            
        default:
            ;

    }

}

/*
Function: void Switch_To_LV(void) 
Description: This Functions  switches the relays on interface card to test LV(Continuity)            
Input: void
Output: void
Calls: none
Called By:
*/
void Switch_Interface_Relays_To_LV(void)
{
    RLYLV11_rb2_pin = RELAY_OFF;                    //WRITING 1 TO PORT pin will make relay OFF
    DelayMs(20); 
    RLYLV12_rb3_pin = RELAY_ON;                     //WRITING 0 TO PORT pin will make relay ON
    DelayMs(20); 
    RLYLV21_rb4_pin = RELAY_OFF;
    DelayMs(20); 
    RLYLV22_rb5_pin = RELAY_ON;
    DelayMs(20); 
    RLYHV11_re9_pin = RELAY_OFF;
    DelayMs(20); 
    RLYHV21_re8_pin = RELAY_OFF;
    DelayMs(20); 
    RLYHL1_rg9_pin = RELAY_OFF;
    DelayMs(20); 
    RLYHL2_rg8_pin = RELAY_OFF;
    DelayMs(20); 
    
}

/*
Function: void Switch_To_LV(void) 
Description: This Functions  switches the relays on interface card to test HV(IR test)            
Input: void
Output: void
Calls: none
Called By:
*/
void Switch_Interface_Relays_To_HV(void)
{
    RLYLV11_rb2_pin = RELAY_OFF;                    //WRITING 1 TO PORT pin will make relay OFF
    DelayMs(20); 
    RLYLV12_rb3_pin = RELAY_OFF;                     
    DelayMs(20); 
    RLYLV21_rb4_pin = RELAY_OFF;
    DelayMs(20); 
    RLYLV22_rb5_pin = RELAY_OFF;
    DelayMs(20); 
    RLYHV11_re9_pin = RELAY_ON;                     //WRITING 0 TO PORT pin will make relay ON
    DelayMs(20); 
    RLYHV21_re8_pin = RELAY_ON;
    DelayMs(20); 
    RLYHL1_rg9_pin = RELAY_ON;
    DelayMs(20); 
    RLYHL2_rg8_pin = RELAY_ON;
    DelayMs(20); 
    
}  





void Fill_Small_Data_Ram(unsigned char *array_ptr_lcl, unsigned char data_lcl_1, unsigned char data_lcl_2, unsigned char data_lcl_3, unsigned char data_lcl_4, unsigned char data_lcl_5, unsigned char data_lcl_6, unsigned char data_lcl_7, unsigned char data_lcl_8, unsigned char data_lcl_9, unsigned char data_lcl_10)
{
    array_ptr_lcl[0] = data_lcl_1;
    array_ptr_lcl[1] = data_lcl_2;
    array_ptr_lcl[2] = data_lcl_3;
    array_ptr_lcl[3] = data_lcl_4;
    array_ptr_lcl[4] = data_lcl_5;
    array_ptr_lcl[5] = data_lcl_6;
    array_ptr_lcl[6] = data_lcl_7;
    array_ptr_lcl[7] = data_lcl_8;
    array_ptr_lcl[8] = data_lcl_9;
    array_ptr_lcl[9] = data_lcl_10;
}


unsigned char Receive_Data_Char_Ram(unsigned char* ramptr, unsigned int size_local)
{
	unsigned char loop_break_flag=0,start_recep_flag=0;
	unsigned int index_local=0;

        //Now on wards data received in the function can be calculated by this variable and used by other functions.
        receive_data_index_glb=0;                                  //Make global reception index zero before starting reception.
		key_pressed_flag=0x00; 
		key_code=0x00;
		byte_received_flag =FALSE;
        Display_String_20(reciving,5,13,2);
		timer1_counter=0;
        Timer1_50ms_Start( );
        while (loop_break_flag==FALSE)
        {
            if(key_pressed_flag ==TRUE )
            {
                key_pressed_flag=FALSE;
                key_data_glb = IC8574_ByteRead();
                key_code =(key_data_glb & 0X0F);
                if(key_code==ESC_KEY)
                {
//                    key_pressed_flag = FALSE; 
                    return(FALSE);		
                } 
            }//end if(key_pressed_flag ==TRUE && key_code != ESC_KEY)

            if(timer1_counter > time_out_for_serial_reception && start_recep_flag==TRUE)
            {
                loop_break_flag=TRUE;
            }//end if(timecount > time_out_for_serial_reception && start_recep_flag==TRUE)
            if (byte_received_flag == TRUE && index_local< size_local)
            {

                ramptr[index_local]=ByteReceived;
                index_local++;
                //Increment the index so for byte count.
                receive_data_index_glb++;                                   
                byte_received_flag =FALSE;
                start_recep_flag=TRUE;
                timer1_counter=0;
            }//end if (byte_received_flag == TRUE && index_local<=size_local)
                
        }//end while (loop_break_flag==FALSE)
        Timer1_50ms_Stop( );
        Remove_Blinking_Cursor();

        if(loop_break_flag==TRUE)
            return(TRUE);		
        else
            return(FALSE);
			
		 
	return(FALSE);
}//end unsigned char Receive_Data_Char_Ram(unsigned char* ramptr, unsigned int size_local,unsigned char signecture_local)

/*
Function:   
Inputs:
Outouts:
Calls:
Called By:
*/
void Test_DigIO()
{
	unsigned char loop_break_flag=0,toggle_flag_lcl=0;

		key_pressed_flag=0x00; 
		key_code=0x00;
		
		while (loop_break_flag==FALSE)
		{
			if(key_pressed_flag ==TRUE)
			{
                key_pressed_flag=FALSE;
                key_data_glb = IC8574_ByteRead();
                key_code =(key_data_glb & 0X0F);
				if(key_code != ESC_KEY && key_code != ENTER_KEY)
				{
					key_pressed_flag = 0x00; 
					key_code = 0x00;
                    return;
				}
				if(key_code==ENTER_KEY)
				{
					key_pressed_flag=0x00; 
					key_code=0x00;
					Clear_Display();
					while(loop_break_flag==FALSE)
					{
						if(key_pressed_flag ==TRUE)
						{
                             key_pressed_flag=FALSE;
                            key_data_glb = IC8574_ByteRead();
                            key_code =(key_data_glb & 0X0F);
                            
							if(key_code != ESC_KEY)
							{
								key_pressed_flag = 0x00; 
								key_code = 0x00;
							}
							if(key_code==ESC_KEY)
							{
								key_pressed_flag = 0x00; 
								key_code = 0x00;
								return;	
							}
						}
						Display_String_20(start_analyse_menu+54+3,1,4,2);     // oth_a_n+317_05_18-2.
						if(toggle_flag_lcl==FALSE)
						{
							toggle_flag_lcl=TRUE;
							Display_String_20(rly_1_n,5,7,2);
							Relay_On_Operation_1(SET,RESET,SINGLE_RELAY);//29_12_15_2.
						}
						else
						{
							toggle_flag_lcl=FALSE;
							Display_String_20(rly_2_n,5,7,2);
							Relay_On_Operation_1(RESET,SET,SINGLE_RELAY);//29_12_15_2.
						}
						
					}//end while(loop_break_flag==FALSE)	
					return;
				}
				if(key_code==ESC_KEY)
					return;
					
			 }						
	  }

}

/*
//////////////////////////Old ///////////////////////////////////////////////
//function drastically changed to incorporate two timing for two relays.
//---------------------29_12_15_2-----start---------------------//
void Relay_On_Operation_1(unsigned char relay_1_local,unsigned char relay_2_local,unsigned char type_lcl)
{
	volatile unsigned int time_local_1=0,time_local_2=0;//17_05_18_1_a.

	time_local_1=E2prom_Byte_Read(e2prom_2_page_2_add,(relay_1_on_time_address & mask_higher_byte) >> 8,(relay_1_on_time_address & mask_lower_byte));//11/10/11.
	time_local_2=E2prom_Byte_Read(e2prom_2_page_2_add,(relay_2_on_time_address & mask_higher_byte) >> 8,(relay_2_on_time_address & mask_lower_byte));
	
	if(time_local_1==0 || time_local_1> 9)
		time_local_1=2;
	if(time_local_2==0 || time_local_2> 9)
		time_local_2=2;

	time_local_1 = (time_local_1 * 1000) / 50;
	time_local_2 = (time_local_2 * 1000) / 50;
	timer1_counter=0;
	Timer1_50ms_Start();                            //Start timer1 of 50 ms
 	if(type_lcl==SINGLE_RELAY)
	{
		if(relay_1_local == SET) 
		{
			timer1_counter=0;
			while(timer1_counter <= time_local_1)				
			{
				if(relay_1_local==SET)
//					relay_op_1 = 0;			//switch on relay 1.
                   IC8574_2_ByteWrite(0b00100000); //switch on relay 1.
			}//end while(timecount <= time_local_1)
//			relay_op_1 = 1;					//switch off relay 1.
            IC8574_2_ByteWrite(0b00110000); //switch off relay 1.
		}//end if(relay_1_local == SET) 
		else if(relay_2_local == SET)
		{
			timer1_counter=0;
			while(timer1_counter <= time_local_2)				
			{
				if(relay_2_local==SET)		//switch on relay 2.
//					relay_op_2 = 0;	
                    IC8574_2_ByteWrite(0b00010000);//switch on relay 2.
			}//end while(timecount <= time_local_2)
			 IC8574_2_ByteWrite(0b00110000);	//switch off relay 2.
		}//end else if(relay_2_local == SET)
	}//end if(type_lcl==SINGLE_RELAY)
	else if(type_lcl==BOTH_RELAYS)
	{
		//time count for first relay is used in both relay mode.
		timer1_counter=0;
		while(timer1_counter <= time_local_1)				
		{
				IC8574_2_ByteWrite(0b00100000);			//switch on relay 1.
				IC8574_2_ByteWrite(0b00010000);			//switch on relay 2.
		}//end while(timecount <= time_local_1)
		IC8574_2_ByteWrite(0b00110000);					//switch off relay 2.
	}//end else if(type_lcl==SINGLE_RELAY)
	Timer1_50ms_Stop();	
}//end function..
//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/



void Relay_On_Operation_1(unsigned char relay_1_local,unsigned char relay_2_local,unsigned char type_lcl)
{
	volatile unsigned int time_local_1=0,time_local_2=0;//17_05_18_1_a.

	time_local_1=E2prom_Byte_Read(e2prom_2_page_2_add,(relay_1_on_time_address & mask_higher_byte) >> 8,(relay_1_on_time_address & mask_lower_byte));//11/10/11.
	time_local_2=E2prom_Byte_Read(e2prom_2_page_2_add,(relay_2_on_time_address & mask_higher_byte) >> 8,(relay_2_on_time_address & mask_lower_byte));
	
	if(time_local_1==0 || time_local_1> 9)
		time_local_1=2;
	if(time_local_2==0 || time_local_2> 9)
		time_local_2=2;

	time_local_1 = (time_local_1 * 1000) / 50;
	time_local_2 = (time_local_2 * 1000) / 50;
	timer1_counter=0;
	Timer1_50ms_Start();                            //Start timer1 of 50 ms
 	if(type_lcl==SINGLE_RELAY)
	{
		if(relay_1_local == SET) 
		{
			timer1_counter=0;
            IC8574_2_ByteWrite(0b00100000); //switch on relay 1.
   			while(timer1_counter <= time_local_1)				
			{
                 ;
			}//end while(timecount <= time_local_1)
            IC8574_2_ByteWrite(0b00110000); //switch off relay 1.
		}//end if(relay_1_local == SET) 
        if(relay_2_local == SET)
  
		{
			timer1_counter=0;
            IC8574_2_ByteWrite(0b00010000);//switch on relay 2.
			while(timer1_counter <= time_local_2)				
			{
                 ;
			}//end while(timecount <= time_local_2)
			 IC8574_2_ByteWrite(0b00110000);	//switch off relay 2.
		}//end else if(relay_2_local == SET)
	}//end if(type_lcl==SINGLE_RELAY)
	else if(type_lcl==BOTH_RELAYS)
	{
		//time count for first relay is used in both relay mode.
		timer1_counter=0;
        IC8574_2_ByteWrite(0b00000000);				//switch on relay 1 & 2
		while(timer1_counter <= time_local_1)				
		{
             ;
		}//end while(timecount <= time_local_1)
        IC8574_2_ByteWrite(0b00110000);					//switch off relay 1 & 2.
	}//end else if(type_lcl==SINGLE_RELAY)
	Timer1_50ms_Stop();	
}//end function..

////////////////////////////////////////printer//////////////////////////////////////////////////////



void Print_Lbl_Data ( )
//void Read_And_Print_Lbl_Data (unsigned char*ram_ptr_lcl,unsigned char demo_print_flg_lcl,unsigned char normal_print_flag_lcl,unsigned char demo_auth_print_flg_lcl,unsigned char prg_lcl)
{
	
	int print_index_lcl=0;//index reduced to  0 for tcht i.e no hash.
	unsigned char var_lcl=0,print_byte_lcl=0,var_lcl_2=0;//21_06_16_1
	unsigned char index_local_2=0;
	unsigned char rtc_ram_local[24];
	unsigned int size_lcl=0,p_address_lcl=0;
	unsigned char convert_index_lcl=0,srl_cnt_lcl=0;//10_04_18_6,03_09_14_2_a.
	unsigned long int decimal_index_lcl=0,val_lcl=0;//03_09_14_2_a.	
    unsigned char week=0,msd=0,lsd=0;//29_11_18_1.
    unsigned char address_lcl,allocate_address,present_ckt_number_lcl,number_lcl;
    unsigned int destination_address,index_lcl;
//    
//    operator_code_1[0]=0x41;//A
//    operator_code_1[1]=0x4C;//L
//    operator_code_1[2]=0x50;//P
//    operator_code_1[3]=0x48;//H
//    operator_code_1[4]=0x41;//A
//    operator_code_1[5]=0x23;//#
//    operator_code_1[6]=0x23;//#
//    operator_code_1[7]=0x23;//#
//    operator_code_2[0]=0x44;//D
//    operator_code_2[1]=0x59;//Y
//    operator_code_2[2]=0x41;//A
//    operator_code_2[3]=0x4E;//N
//    operator_code_2[4]=0x41;//A
//    operator_code_2[5]=0x4D;//M
//    operator_code_2[6]=0x4F;//O
//    operator_code_2[7]=0x23;//#
//    
    Clear_Display();
    Display_String_20(printing,1,12,2);
	// start printing
    present_ckt_number_lcl =  E2prom_Byte_Read(e2prom_2_page_2_add,((current_cable_number_address)& mask_higher_byte)>>8,(current_cable_number_address)& mask_lower_byte);
    allocate_address = Allocate_E2prom_Address_1(present_ckt_number_lcl);
    destination_address = check_addresses_of_E2prom(present_ckt_number_lcl);
    destination_address = (LABEL_DATA_SIZE*(destination_address-1)) + 1;
    Read_Bulk_Data_Eeprom(allocate_address,destination_address,LABEL_DATA_SIZE,present_ckt_number_lcl,digit_ram_1);
    //-----------------------21_06_19_5-----start---------------------//
    /*
      Serial number searching will only be done for first cycle of testing, after program selection manually or through bar code scanner.
      Similarly it will be done after serial data reception via demo print and after data saving through USB, even though program is not selected.
      This reduces overall cycle time wasted during printing for unnecessary search of serial number every time if serial number is not present in
      the label program
     */  
//    if(serial_number_searched_flag_glb==FALSE)
        {   
            Display_String_20("SEARCHNG SR NO",1,14,2);//21_06_19_5.
            srl_cnt_lcl=Search_Sr_No_1(allocate_address,digit_ram,destination_address,LABEL_DATA_SIZE);       
            serial_number_searched_flag_glb=TRUE; //21_06_19_5.
            Display_String_20(space,1,20,2);//21_06_19_5.
            //------------------------07_06_13_1------start----------------------//
            //serial number count and print delay issue solving....

            if((srl_cnt_lcl==0) || (srl_cnt_lcl>9))
            {
                srl_cnt_lcl=0;
            }//end if((serial_number_count==0) ||(serial_number_count>9))
            //10_04_18_6.
            //serial number is not incremented for demo print.
            //Increment_Ram_Data(digit_ram,serial_number_count);//21_02_14_1.
            Write_Bulk_Data_Eeprom(e2prom_2_page_2_add,serial_no_count_base_address,1,present_ckt_number_lcl,&srl_cnt_lcl);//11/10/11.
            if(current_cable_number == present_ckt_number_lcl/*prg_lcl*/)
            {
                serial_number_count=srl_cnt_lcl;
                Read_Write_RTC(&serial_number_count,serial_number_bbram_count_address,1,WRITE);
            }//end if(current_cable_number==prg_lcl)
            if((srl_cnt_lcl!=0) && (srl_cnt_lcl<=9))
            {
                //10_04_18_6.
                Write_Bulk_Data_Eeprom(e2prom_2_page_2_add,serial_number_base_address,serial_number_max_size,present_ckt_number_lcl,digit_ram);//11/10/11.			
                if(current_cable_number == present_ckt_number_lcl/*prg_lcl*/)
                    Read_Write_RTC(digit_ram,serial_number_bbram_base_address,srl_cnt_lcl,WRITE);
            }//end if((serial_number_count!=0)&& (serial_number_count<=9))
            //------------------------07_06_13_1------end------------------------//
        } //end if(serial_number_searched_flag_glb==FALSE)
        
	for(print_index_lcl = 0;print_index_lcl<LABEL_DATA_SIZE;print_index_lcl++)//index reduced to  0for tcht i.e no hash.
	{
	    print_byte_lcl=digit_ram_1[print_index_lcl];
        
		if(print_byte_lcl=='@')
		{
			print_index_lcl++;
			print_byte_lcl=digit_ram_1[print_index_lcl];
            
			switch(print_byte_lcl)
			{
		     	case '1': //print date.
                        Read_Date_Time_1(digit_ram, TRUE);
						for(index_local_2=0;index_local_2<=9;index_local_2++)
						{
							Print(digit_ram[index_local_2]);//Print(rtc_ram_local[index_local_2]);
							//TransmitCharacter(rtc_ram_local[index_local_2]);
						}
						break;

				case '2': //print time.
                        Read_Date_Time_1(digit_ram, TRUE);
                        for(index_local_2=10;index_local_2<=17;index_local_2++)
						{
							Print(digit_ram[index_local_2]);//Print(rtc_ram_local[index_local_2]);
							//TransmitCharacter(rtc_ram_local[index_local_2]);
						}
						break;
				case '3'://not used.
						break;
				case '4': //month
                        Read_Date_Time_1(digit_ram, TRUE);
                        Print(digit_ram[3]);//Print(rtc_ram_local[3]);
						Print(digit_ram[4]);//Print(rtc_ram_local[4]);

						//TransmitCharacter(rtc_ram_local[3]);
						//TransmitCharacter(rtc_ram_local[4]);
						break;
				case '5'://year
                        Read_Date_Time_1(digit_ram, TRUE);
                        Print(digit_ram[8]);//Print(rtc_ram_local[8]);
						Print(digit_ram[9]);//Print(rtc_ram_local[9]);

						//TransmitCharacter(rtc_ram_local[8]);
						//TransmitCharacter(rtc_ram_local[9]);
						break;
				case '6':
                         Read_Write_RTC(digit_ram,serial_number_bbram_base_address,9,READ);
	                     //---------------15/03/12-------start---------//
						 //21_02_14_X.
//						 if((demo_auth_print_flg_lcl == FALSE) && (demo_print_flg_lcl==FALSE))
//						 { 
						 Increment_Ram_Data(digit_ram,9/*serial_number_count*/);

	                     Read_Write_RTC(digit_ram,serial_number_bbram_base_address,9,WRITE);
//						 }//end if((demo_auth_print_flg_lcl == FALSE) && (demo_print_flg_lcl==FALSE)).
						 //---------------15/03/12--------ends----------//
                         //----------------24_12_18_1-----start-----------------//
                         //hex serial number printing logic,if setting is enabled. 
//                         if(extnd_settings_ram[EXTND_HEX_SRL_INDEX] == SET)
//                         {
//                            temp_char_glb=FALSE;                                //clear flag for next usage.
//                            //loop for checking whether serial number is cleared/overflown earlier by increment logic.
//                            for(index_local_2=0;index_local_2<9/*serial_number_count*/;index_local_2++)
//                            {
//                                if(digit_ram[index_local_2]!=0x30)// if at least one byte of serial number ram is non zero..
//                                {                                 //..means, number is not overflown.              
//                                    temp_char_glb=TRUE;           //flag true means no overflow happens.
//                                    break;
//                                }//end if(digit_ram[index_local_2]!=0x30).       
//                            }//end for(index_local_2=0;index_local_2<=serial_number_count;index_local_2++)
//                            //if jump setting is enabled then after overflow,increment ram to 01 instead of 00.
//                            if(/*(extnd_settings_ram[EXTND_HEX_JUMP_INDEX] == SET) && */(temp_char_glb==FALSE))
//                            {
//                                //msb to be made '1' instead of '0' i.e. after '000', 'A01' will come if setting is on.
//                                digit_ram[9/*serial_number_count*/-1]='1';     
//                                //store in bbram after data change.
//                                Read_Write_RTC(digit_ram,serial_number_bbram_base_address,9,WRITE); 
//                            }//end if((extnd_settings_ram[EXTND_HEX_JUMP_INDEX] == SET) && (temp_char_glb==FALSE))  
                            //read location specific counter from eeprom.values from 'A' to 'Z' are only valid.
//                            hex_data_counter_serial_number_glb=E2prom_Byte_Read(e2prom_1_page_1_add,((HEX_COUNTER_SRL_DATA_BASE_ADDRESS+(prg_lcl-1)) & mask_higher_byte) >> 8,((HEX_COUNTER_SRL_DATA_BASE_ADDRESS+(prg_lcl-1)) & mask_lower_byte));                             
//                            if( (hex_data_counter_serial_number_glb>='A')  && (hex_data_counter_serial_number_glb<='Z'))
//                            {    
//                               //if serial number is overflown earlier then increment msb to next character i.e...
//                               //...0X00 to 'A''B' ..etc. 
//                               if(temp_char_glb==FALSE)
//                               {    //increment to next byte.
//                                    hex_data_counter_serial_number_glb++;
//                                    E2prom_Byte_Write(e2prom_1_page_1_add,((HEX_COUNTER_SRL_DATA_BASE_ADDRESS+(current_cable_number-1))&mask_higher_byte) >> 8,(HEX_COUNTER_SRL_DATA_BASE_ADDRESS+(current_cable_number-1))&mask_lower_byte,hex_data_counter_serial_number_glb);				 
//                               }//if(temp_char_glb==FALSE).
//                               //update msb of serial number in ram byte with character if exist in valid range.
//                               digit_ram[0]=hex_data_counter_serial_number_glb;   
//                            }//end if( (hex_data_counter_serial_number_glb>='A')  && (hex_data_counter_serial_number_glb<='Z'))
//                            else
//                            {
//                                if(temp_char_glb==FALSE)
//                                {    
//                                    hex_data_counter_serial_number_glb='A';
//                                    digit_ram[0]=hex_data_counter_serial_number_glb; 
//                                    E2prom_Byte_Write(e2prom_1_page_1_add,((HEX_COUNTER_SRL_DATA_BASE_ADDRESS+(current_cable_number-1))&mask_higher_byte) >> 8,(HEX_COUNTER_SRL_DATA_BASE_ADDRESS+(current_cable_number-1))&mask_lower_byte,hex_data_counter_serial_number_glb);
//                                }//end if(temp_char_glb==FALSE) 
//                            }//end else if( (hex_data_counter_serial_number_glb>='A')  && (hex_data_counter_serial_number_glb<='Z'))  
//                         }//end if(extnd_settings_ram[EXTND_HEX_SRL_INDEX] == SET)
                         //----------------24_12_18_1-----ends------------------//
						 //--------------10_04_18_4,10_04_18_6-----start--------------//
						 //if(no_serial_number_flag == TRUE);
						 //else
//						 if((demo_auth_print_flg_lcl == TRUE)|| (demo_print_flg_lcl==TRUE)) 
//						 {
//							Fill_Any_Char_Ram_With(digit_ram,9,0x30);
//							if(demo_print_flg_lcl==TRUE)
//								Print_Ram(digit_ram,srl_cnt_lcl);
//							else  
//                                Print_Ram(digit_ram,serial_number_count);
//                          
//								
//						 }//end if((demo_auth_print_flg_lcl == TRUE)|| (demo_print_flg_lcl==TRUE)).
//						 else
//                         {    
                            Print_Ram(digit_ram,/*serial_number_count*/9);
//                         } 
						 
//						 //-----29/06/09----------------------------------//
//						 if(demo_print_flg_lcl==TRUE)
//						 	print_index_lcl = print_index_lcl + srl_cnt_lcl;
//						 else
//							print_index_lcl = print_index_lcl + serial_number_count;
//						//--------------10_04_18_4, 10_04_18_6-----ends---------------//
//						 if(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+print_index_lcl+1) & mask_higher_byte) >> 8,((p_address_lcl+print_index_lcl+1) & mask_lower_byte)) == '@'&& E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+print_index_lcl+2) & mask_higher_byte) >> 8,((p_address_lcl+print_index_lcl+2) & mask_lower_byte)) == '6')
//						 	print_index_lcl = print_index_lcl+2;
//						 else
//						 	print_index_lcl = print_index_lcl;
//						 //------------------------------------------------//
                         break;                       
                case '7': //--------------10_04_18_6-----start--------------//
//						  if((demo_auth_print_flg_lcl == TRUE) || (demo_print_flg_lcl==TRUE)) 
//						  {
//								Fill_Any_Char_Ram_With(digit_ram,9,0x30);
//								if(demo_print_flg_lcl==TRUE)
//									Print_Ram(digit_ram,srl_cnt_lcl);
//								else
//									Print_Ram(digit_ram,serial_number_count);
//						  }//end if((demo_auth_print_flg_lcl == TRUE)|| (demo_print_flg_lcl==TRUE)).
//						  else
//						  {
						  	Read_Write_RTC(digit_ram,serial_number_bbram_base_address,9,READ);// repeated serial numberonly retrieve here//printing
                            //------------------------24_12_18_1--------start------------------------//
                            //if hex serial setting is set and data is overflown to max digit limit then print 'A' or 'B'...,
                            //...depending on record.
//                            if((extnd_settings_ram[EXTND_HEX_SRL_INDEX] == SET) && (hex_data_counter_serial_number_glb>='A')  && (hex_data_counter_serial_number_glb<='Z'))
//                            {
//                                //counter represents number of iterations of data over flow i.e. 1='A',2='B' etc.
//                                digit_ram[0]=hex_data_counter_serial_number_glb; //update in the ram msb.
//                            }//end if((extnd_settings_ram[EXTND_HEX_SRL_INDEX] == SET) && (hex_data_counter_serial_number_glb>='A')  && (hex_data_counter_serial_number_glb<='Z'))
                            //------------------------24_12_18_1--------ends-------------------------//
                            Print_Ram(digit_ram,9/*serial_number_count*/);
//						  }//end if((demo_auth_print_flg_lcl == TRUE) || (demo_print_flg_lcl==TRUE)). 
						 //--------------10_04_18_6-----ends----------------//
	                      break;	
                case '8'://Date
                         Read_Date_Time_1(digit_ram, TRUE);
                         Print(digit_ram[0]);
						 Print(digit_ram[1]);
						 //TransmitCharacter(rtc_ram_local[0]);
						 //TransmitCharacter(rtc_ram_local[1]);

						break;
				case '9': //FAIL COUNT
                          Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,READ);
                          for(index_local_2=0;index_local_2<=2;index_local_2++)
						  {
							Print(digit_ram[index_local_2]);//Print(rtc_ram_local[index_local_2]);
							//TransmitCharacter(rtc_ram_local[index_local_2]);
						  }
                    // shift number 1 digit A/B/C (for KIML only)
						  break;
				case 'A': // lot number 3 digits (for KIML and GENTEK only)
						  break;
				case 'B': 
                         for(var_lcl=0;(operator_code_1[var_lcl] != ' ');var_lcl++)
						  {	
						  	  Print(operator_code_1[var_lcl]);
							  if(var_lcl == 7)
							  	break;
						  }	
						  break;				
				case 'C': 
                          for(var_lcl=0;(operator_code_2[var_lcl] != ' ');var_lcl++)        
						  {	
								Print(operator_code_2[var_lcl]);
								if(var_lcl == 7)	
									break;
						  }	
						  break; 
				//---------------------21_06_16_1------start---------------//
				case 'D':
					    Read_Convert_Shift_Data_1(digit_ram);
						//for hour..
                        Read_Date_Time_1(digit_ram, TRUE);
//                        Read_Convert_Shift_Data_1(digit_ram);
						var_lcl=((digit_ram[10]-0x30)<<4)+(digit_ram[11]-0x30);
						//for minute..
						var_lcl_2=((digit_ram[13]-0x30)<<4)+(digit_ram[14]-0x30);
						
						if((first_shift_hh_glb <= var_lcl) && (var_lcl <=second_shift_hh_glb))
						{
							if((first_shift_hh_glb == var_lcl) && (var_lcl_2<=first_shift_mm_glb))
								Print(third_shift_character_glb);
							else if((second_shift_hh_glb == var_lcl) && (var_lcl_2<=second_shift_mm_glb))
								Print(first_shift_character_glb);
							else if((second_shift_hh_glb == var_lcl) && (var_lcl_2 >=second_shift_mm_glb))
								Print(second_shift_character_glb);
							else
								Print(first_shift_character_glb);
						}//end 
						else if((second_shift_hh_glb <= var_lcl) && (var_lcl <=third_shift_hh_glb))
						{
							if((third_shift_hh_glb == var_lcl) && (var_lcl_2<=third_shift_mm_glb))
								Print(second_shift_character_glb);
							else if((third_shift_hh_glb == var_lcl) && (var_lcl_2 >=third_shift_mm_glb))
								Print(third_shift_character_glb);
							else
								Print(second_shift_character_glb);
						}//end 
						else
								Print(third_shift_character_glb); 
					    break;   
				//---------------------21_06_16_1------start---------------//
                //---------------------12_07_22_1_2-------start-----------------//
                case'E'://day in special format.
                        Read_Date_Time_1(digit_ram, TRUE);
                        //1='1',2='2',3=3'3,4='4',5='5',6='6',7='7',8='8',9='9',10='A',11='B',12='C',13='D',14='E',15='F',16='G',17='H',18='J',19='K',20='L',21='M',22='N',23='P',24='R',25='S',26='T',27='U',28='V',29='W',30='X',31='Y'.
						 val_lcl=(((digit_ram[0]-0x30)*10) + (digit_ram[1]-0x30));//val_lcl=(((rtc_ram_local[0]-0x30)*10) + (rtc_ram_local[1]-0x30));
						 if((val_lcl<=9) && (val_lcl>0))           // '1' to '9'.
							Print(val_lcl + 0x30);
						 else if((val_lcl>9) && (val_lcl<18))       //'A' to 'H'
							Print(val_lcl + 0x37);
                         else if((val_lcl>=18)&& (val_lcl<23))      //'J' to 'N'
							Print(val_lcl + 0x38);
                         else if((val_lcl>=23)&& (val_lcl<24))      //'P'
							Print(val_lcl + 0x39);                  
                         else if(val_lcl>=24)                       //'R' to 'Y'
							Print(val_lcl + 0x3A);
					   break;
                case'F'://day in special format.
                         Read_Date_Time_1(digit_ram, TRUE);
                        //1='1',2='2',3=3'3,4='4',5='5',6='6',7='7',8='8',9='9',10='A',11='B',12='C',13='D',14='E',15='F',16='G',17='H',18='J',19='K',20='L',21='M',22='N',23='P',24='R',25='S',26='T',27='V',28='W',29='X',30='Y',31='Z'.
                        val_lcl=(((digit_ram[0]-0x30)*10) + (digit_ram[1]-0x30));//val_lcl=(((rtc_ram_local[0]-0x30)*10) + (rtc_ram_local[1]-0x30));
						 if((val_lcl<=9) && (val_lcl>0))           // '1' to '9'.
							Print(val_lcl + 0x30);
						 else if((val_lcl>9) && (val_lcl<18))       //'A' to 'H'
							Print(val_lcl + 0x37);
                         else if((val_lcl>=18)&& (val_lcl<23))      //'J' to 'N'
							Print(val_lcl + 0x38);
                         else if((val_lcl>=23)&& (val_lcl<24))      //'P'
							Print(val_lcl + 0x39);                  
                         else if((val_lcl>=24) && (val_lcl<27))     //'R' to 'T'
							Print(val_lcl + 0x3A);
                         else if(val_lcl>=27)                       //'V' to 'Z'
                            Print(val_lcl + 0x3B);
					   break;       
                //---------------------12_07_22_1_2-------ends------------------//        
				//---------------------21_02_14_7------start----------//
				case'U':
//						Print((Convert_Ascii_To_Hex_Nibble(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+print_index_lcl+1) & mask_higher_byte) >> 8,((p_address_lcl+print_index_lcl+1) & mask_lower_byte)))<<4)+Convert_Ascii_To_Hex_Nibble(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+print_index_lcl+2) & mask_higher_byte) >> 8,((p_address_lcl+print_index_lcl+2) & mask_lower_byte))));
//						print_index_lcl=print_index_lcl+2;
						break;
				//---------------------21_02_14_7------end------------//     
				case 'V':   
//							switch(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+print_index_lcl+1) & mask_higher_byte) >> 8,((p_address_lcl+print_index_lcl+1) & mask_lower_byte)))
//						 	{
//								case '1': Print(rtc_ram_local[10]);
//									      Print(rtc_ram_local[11]);
//										//TransmitCharacter(rtc_ram_local[10]);
//										//TransmitCharacter(rtc_ram_local[11]);
//										break;
//								case '2': Print(rtc_ram_local[13]);
//								    	  Print(rtc_ram_local[14]);
//										//TransmitCharacter(rtc_ram_local[13]);
//										//TransmitCharacter(rtc_ram_local[14]);
//										break;
//								case '3': 
//										Print(rtc_ram_local[16]);
//										Print(rtc_ram_local[17]);
//										//TransmitCharacter(rtc_ram_local[16]);
//										//TransmitCharacter(rtc_ram_local[17]);
//										break;
//								default:
//										break;
//						 	}//switch(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+print_index_lcl+1) & mask_higher_byte) >> 8,((p_address_lcl+print_index_lcl+1) & mask_lower_byte)))
//						 	print_index_lcl++;
						 break;
				//----------------------28_05_13_2_a_b-----start--------------------//
				case 'X':
                        Print_Day_1(PRINT_DAY_CODE,PRINT_AND_RETURN);//21_02_14_1.
						break;
				case 'Y':
                        Print_Day_1(PRINT_DAY_YEAR,PRINT_AND_RETURN);//21_02_14_1.
						break;
				//----------------------28_05_13_2_a_b-----end----------------------//
                //------29_11_18_1---------------------start------------------------//
                case 'd':
					week=Week_By_Julian_Date();
					lsd = week & 0x0F;
					msd = (week & 0xF0) >> 4;
					Print(msd + 0x30);
					Print(lsd + 0x30);
					break;
                //------29_11_18_1---------ends---------------------------------------//        
				//----------------------03_09_14_2_a---------strat--------------------//
				case 'i'://print modulo 42 day...
                         Read_Date_Time_1(digit_ram, TRUE);
						 var_lcl=((digit_ram[0]-0x30)*10)+(digit_ram[1]-0x30);
						 Print(modulo_42[var_lcl]);
						 break;
				case 'j'://print modulo 42 month...
                         Read_Date_Time_1(digit_ram, TRUE);
						 var_lcl=((digit_ram[3]-0x30)*10)+(digit_ram[4]-0x30);
						 Print(modulo_42[var_lcl]);
						 break;
				case 'k'://print modulo year 42 year two digits...
                         Read_Date_Time_1(digit_ram, TRUE);
						 var_lcl=((digit_ram[8]-0x30)*10)+(digit_ram[9]-0x30);
						 Print('0');
						 Print(modulo_42[var_lcl]);
						 break;
				case 'l'://serial number in modulo style
//						 Read_Write_RTC(digit_ram,serial_number_bbram_base_address,9,READ);// repeated serial numberonly retrieve here//printing
//	                     Fill_Any_Char_Ram_With(digit_ram+16,9,0x00);
//						 for(convert_index_lcl=serial_number_count,decimal_index_lcl=1;convert_index_lcl>0;convert_index_lcl--)
//						 {
//							val_lcl=val_lcl+((digit_ram[convert_index_lcl-1]-0x30)*decimal_index_lcl);
//							decimal_index_lcl=decimal_index_lcl*10;
//						 }
//						 Base_To_BCD_Converter(val_lcl,43,digit_ram+16);
//						 Print(modulo_42[digit_ram[18]]);
//						 Print(modulo_42[digit_ram[17]]);
//						 Print(modulo_42[digit_ram[16]]);
						break;
			    //----------------------03_09_14_2_a---------end-----------------------//
				//----------------------09_02_18--start--------------------------------//
				 case'q'://day in pecial format.
                         Read_Date_Time_1(digit_ram, TRUE);
						 val_lcl=(((digit_ram[0]-0x30)*10) + (digit_ram[1]-0x30));//val_lcl=(((rtc_ram_local[0]-0x30)*10) + (rtc_ram_local[1]-0x30));
						 if((val_lcl<=9) && (val_lcl>0))
							Print(val_lcl + 0x30);
						 else if(val_lcl>9)
							Print(val_lcl + 0x37);
					   break;
				//04_11_17_2.
				 case'r'://month in special format
                        Read_Date_Time_1(digit_ram, TRUE);
						val_lcl=(((digit_ram[3]-0x30)*10) + (digit_ram[4]-0x30));//val_lcl=(((rtc_ram_local[3]-0x30)*10) + (rtc_ram_local[4]-0x30));
						 if((val_lcl<=9) && (val_lcl>0))
							Print(val_lcl + 0x30);
						 else if(val_lcl>9)
							Print(val_lcl + 0x37);
					   break;
				//04_11_17_3.
				 case's'://year in special format
//                         Read_Date_Time_1(digit_ram, TRUE);
//						 val_lcl=(((digit_ram[8]-0x30)*10) + (digit_ram[9]-0x30));//val_lcl=(((rtc_ram_local[8]-0x30)*10) + (rtc_ram_local[9]-0x30));
//						 switch(val_lcl)
//						 {
//							case 17:
//									val_lcl='J';
//									break;
//							case 18:val_lcl='K';
//									break;
//							case 19:val_lcl='M';
//									break;
//							case 20:val_lcl='N';		
//									break;			
//                         }//end switch(val_lcl)
//						 Print(val_lcl);
					   break;
				//----------------------09_02_18--ends---------------------------------//
				//----------------------10_04_18_1------start--------------------------//
				case't'://@t xxx @t ---> data between this is considered as barcode chunk to be printed on label.This barcode is logged in eeprom for comparison for correct printing.
//					    //one lbl can have multiple @t-@t pairs but addition of barcode bytes of all pairs should not exceed more than 64 bytes as maximum barcode capacity is 64 bytes.
//						//condition for first @t i.e initiator in @t--@t pair.
//						if((log_barcode_flag_glb==FALSE) && (log_barcode_index_glb<64) && (demo_print_flg_lcl==FALSE) && (demo_auth_print_flg_lcl==FALSE))
//						{
//							//for demo print and test print log barcode is not applicable.Similarly if log barcode index exceeds 64 do not come here.
//							log_barcode_flag_glb=TRUE;
//							if(log_barcode_index_glb==0)							//for first byte clear the array.
//								Fill_Any_Char_Ram_With(autho_glb_ram,65,0x00);		//clear array with 0x00, to find barcode end byte during logging in eeprom and comparison. 
//
//						}//end if(log_barcode_flag_glb==RESET)
//						else	//condition for second @t i.e. terminator in @t--@t pair.
//						{
//							log_barcode_flag_glb=FALSE;
//						}//end else if(log_barcode_flag_glb==RESET)
					   break;
				//----------------------10_04_18_1------ends---------------------------//
                //----------------------22_02_19_2------start--------------------------//
				case'u':
//                        Read_Write_RTC(digit_ram,START_TESTING_TIME_BBRAM_BASE_ADDRESS,RTC_STAMP_SIZE,READ);
//						switch(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+print_index_lcl+1) & mask_higher_byte) >> 8,((p_address_lcl+print_index_lcl+1) & mask_lower_byte)))
//                        {
//                            case '1': //print hour in HH format.
//                                      Print(digit_ram[10]);
//									  Print(digit_ram[11]);
//									  break;
//                            case '2': //print minute in MM format
//                                      Print(digit_ram[13]);
//                                      Print(digit_ram[14]);
//                                      break;
//                            case '3'://print second in SS format. 
//                                      Print(digit_ram[16]);
//                                      Print(digit_ram[17]);
//                                      break;
//                            case '5': //print year
//                                      Print(digit_ram[8]);
//                                      Print(digit_ram[9]);
//                                      break;
//                            case '4': //print month
//                                      Print(digit_ram[3]);
//                                      Print(digit_ram[4]);
//                                      break;
//                            case '8': 
//                                     //print day
//                                     Print(digit_ram[0]);
//                                     Print(digit_ram[1]);
//                                     break;
//                            default:
//                                    break;
//                        }//switch(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+print_index_lcl+1) & mask_higher_byte) >> 8,((p_address_lcl+print_index_lcl+1) & mask_lower_byte)))
//                        print_index_lcl++;
					   break;
				//----------------------22_02_19_2------ends---------------------------// 
                //------16_06_21_1---start--------------------------------//
                case 'x':
                            Read_Date_Time_1(digit_ram, TRUE);
                            val_lcl=(((digit_ram[3]-0x30)*10) + (digit_ram[4]-0x30));
                            Print(minda_noida_month[val_lcl-1]);		
                            break;
                //------16_06_21_1----------ends------------------------------// 
                //------12_07_22_3---start--------------------------------//
                case '!':
                           Read_Date_Time_1(digit_ram, TRUE);
                           val_lcl=(((digit_ram[3]-0x30)*10) + (digit_ram[4]-0x30));//val_lcl=(((rtc_ram_local[3]-0x30)*10) + (rtc_ram_local[4]-0x30));
						 if((val_lcl<=9) && (val_lcl>0))
							Print(val_lcl + 0x30);
						 else if(val_lcl>9)
							Print(val_lcl + 0x4E);
                            break;
                //------12_07_22_3----------ends------------------------------//             
				
			}//switch    
		}//end if(print_byte_lcl=='@')             
		else if(print_byte_lcl!=0x00)
		{
			Print(print_byte_lcl);	
			//TransmitCharacter(ram_ptr[print_index]);
		}//else
//		else
//			break;

	}//end for.
	Print(0X0D);
	Print(0X0A);
}//end void Read_And_Print_Lbl_Data (unsigned char*ram_ptr_lcl,unsigned char demo_print_flg_lcl,unsigned char normal_print_flag_lcl,unsigned char demo_auth_print_flg_lcl,unsigned char prg_lcl)



//

void Print_Ram(unsigned char* ram_ptr, unsigned char size_local)// printing
{
	unsigned char index_local;
	for(index_local=0;index_local<size_local;index_local++)
	{
		Print(ram_ptr[index_local]);	
	
	}//end for(index_local=0;index_local<size_local;index_local++)
}//end void Print_Ram(unsigned char* ram_ptr, unsigned char size_local)// printing



void Increment_Ram_Data(unsigned char * ram_ptr,unsigned char size_local)			
{
	char index_local;
	index_local = 0;
	ram_ptr[size_local-1]++;
	index_local=size_local-1;

	while((index_local>=0) && (ram_ptr[(unsigned char)index_local]>='0' && ram_ptr[(unsigned char)index_local]<='10')) //while(index_local>=0)
	{	
		
		if(ram_ptr[(unsigned char)index_local] > '9')
		{
			ram_ptr[(unsigned char)index_local] = 0x30;
			ram_ptr[(unsigned char)index_local-1]++;
			
		}//end if(ram_ptr[(unsigned char)index_local] > '9')
		index_local--;
	}//end while(index_local>=0).
}//end void Increment_Ram_Data(unsigned char * ram_ptr,unsigned char size_local).			



//
///*
// Function:
// Input: 
// Output:
// Calls:
// Called By:
//*/
void Print(unsigned char ch) 			  
{
	unsigned char time_out_flag_local=0;//29/06/09

    //-------------------------20_07_21_1------start-------------------------//
//    else if((extnd_settings_ram[EXTND_SERIAL_PRINTING_INDEX] == SET) && (extnd_settings_ram[EXTND_SWPRNHACG_INDEX] == SET))
//    {
        Transmit_Byte_Serial_1(ch);
//    }        
    //-------------------------20_07_21_1------ends--------------------------//
//	else
//	{
//		Transmit_Byte_Serial_2(ch);
//	}
	//----------------04_03_16_1----ends--------------------//
	//----------------ACTUAL PRINTING ROUTINE ENDS-----------------//
}//end void Print(unsigned char ch).




//void InitializeByteArray(void)//Temparary data to send printing data
//{
//   unsigned char label_data[186];
//  
//    label_data[0]=0x0D;//
//    label_data[1]=0x0A;//
//    label_data[2]=0x02;//
//    label_data[3]=0x66;//f
//    label_data[4]=0x32;//2
//    label_data[5]=0x36;//6
//    label_data[6]=0x30;//0
//    label_data[7]=0x0D;//
//    label_data[8]=0x0A;//
//    label_data[9]=0x02;//
//    label_data[10]=0x30;//0
//    label_data[11]=0x32;//2
//    label_data[12]=0x32;//2
//    label_data[13]=0x30;//0
//    label_data[14]=0x0D;//
//    label_data[15]=0x0A;//
//    label_data[16]=0x02;//
//    label_data[17]=0x4C;//L
//    label_data[18]=0x0D;//
//    label_data[19]=0x0A;//
//    label_data[20]=0x44;//D
//    label_data[21]=0x31;//1
//    label_data[22]=0x31;//1
//    label_data[23]=0x0D;//
//    label_data[24]=0x0A;//
//    label_data[25]=0x48;//H
//    label_data[26]=0x31;//1
//    label_data[27]=0x30;//0
//    label_data[28]=0x0D;//
//    label_data[29]=0x0A;//
//    label_data[30]=0x43;//C
//    label_data[31]=0x30;//0
//    label_data[32]=0x30;//0
//    label_data[33]=0x30;//0
//    label_data[34]=0x30;//0
//    label_data[35]=0x0D;//
//    label_data[36]=0x0A;//
//    label_data[37]=0x52;//R
//    label_data[38]=0x30;//0
//    label_data[39]=0x30;//0
//    label_data[40]=0x30;//0
//    label_data[41]=0x30;//0
//    label_data[42]=0x0D;//
//    label_data[43]=0x0A;//
//    label_data[44]=0x34;//4
//    label_data[45]=0x39;//9
//    label_data[46]=0x31;//1
//    label_data[47]=0x31;//1
//    label_data[48]=0x30;//0
//    label_data[49]=0x30;//0
//    label_data[50]=0x32;//2
//    label_data[51]=0x30;//0
//    label_data[52]=0x30;//0
//    label_data[53]=0x30;//0
//    label_data[54]=0x30;//0
//    label_data[55]=0x30;//0
//    label_data[56]=0x31;//1
//    label_data[57]=0x30;//0
//    label_data[58]=0x30;//0
//    label_data[59]=0x20;//space
//    label_data[60]=0x41;//A
//    label_data[61]=0x49;//I
//    label_data[62]=0x33;//3
//    label_data[63]=0x20;//space
//    label_data[64]=0x53;//S
//    label_data[65]=0x55;//U
//    label_data[66]=0x56;//V
//    label_data[67]=0x0D;//
//    label_data[68]=0x0A;//
//    label_data[69]=0x31;//1
//    label_data[70]=0x39;//9
//    label_data[71]=0x31;//1
//    label_data[72]=0x31;//1
//    label_data[73]=0x30;//0
//    label_data[74]=0x30;//0
//    label_data[75]=0x32;//2
//    label_data[76]=0x30;//0
//    label_data[77]=0x30;//0
//    label_data[78]=0x31;//1
//    label_data[79]=0x30;//0
//    label_data[80]=0x30;//0
//    label_data[81]=0x30;//0
//    label_data[82]=0x39;//9
//    label_data[83]=0x38;//8
//    label_data[84]=0x20;//space
//    label_data[85]=0x32;//2
//    label_data[86]=0x33;//3
//    label_data[87]=0x40;//@
//    label_data[88]=0x34;//4
//    label_data[89]=0x40;//@
//    label_data[90]=0x38;//8
//    label_data[91]=0x0D;//
//    label_data[92]=0x0A;//
//    label_data[93]=0x31;//1
//    label_data[94]=0x39;//9
//    label_data[95]=0x31;//1
//    label_data[96]=0x31;//1
//    label_data[97]=0x30;//0
//    label_data[98]=0x30;//0
//    label_data[99]=0x32;//2
//    label_data[100]=0x30;//0
//    label_data[101]=0x30;//0
//    label_data[102]=0x32;//2
//    label_data[103]=0x39;//9
//    label_data[104]=0x30;//0
//    label_data[105]=0x30;//0
//    label_data[106]=0x39;//9
//    label_data[107]=0x38;//8
//    label_data[108]=0x20;//space
//    label_data[109]=0x40;//@
//    label_data[110]=0x4C;//L
//    label_data[111]=0x0D;//
//    label_data[112]=0x0A;//
//    label_data[113]=0x31;//1
//    label_data[114]=0x57;//W
//    label_data[115]=0x31;//1
//    label_data[116]=0x63;//c
//    label_data[117]=0x33;//3
//    label_data[118]=0x33;//3
//    label_data[119]=0x30;//0
//    label_data[120]=0x30;//0
//    label_data[121]=0x30;//0
//    label_data[122]=0x30;//0
//    label_data[123]=0x30;//0
//    label_data[124]=0x31;//1
//    label_data[125]=0x30;//0
//    label_data[126]=0x30;//0
//    label_data[127]=0x31;//1
//    label_data[128]=0x34;//4
//    label_data[129]=0x35;//5
//    label_data[130]=0x31;//1
//    label_data[131]=0x30;//0
//    label_data[132]=0x30;//0
//    label_data[133]=0x30;//0
//    label_data[134]=0x30;//0
//    label_data[135]=0x30;//0
//    label_data[136]=0x30;//0
//    label_data[137]=0x30;//0
//    label_data[138]=0x30;//0
//    label_data[139]=0x30;//0
//    label_data[140]=0x40;//@
//    label_data[141]=0x74;//t
//    label_data[142]=0x31;//1
//    label_data[143]=0x41;//A
//    label_data[144]=0x49;//I
//    label_data[145]=0x33;//3
//    label_data[146]=0x53;//S
//    label_data[147]=0x55;//U
//    label_data[148]=0x56;//V
//    label_data[149]=0x20;//space
//    label_data[150]=0x30;//0
//    label_data[151]=0x30;//0
//    label_data[152]=0x33;//3
//    label_data[153]=0x32;//2
//    label_data[154]=0x30;//0
//    label_data[155]=0x31;//1
//    label_data[156]=0x32;//2
//    label_data[157]=0x38;//8
//    label_data[158]=0x20;//space
//    label_data[159]=0x32;//2
//    label_data[160]=0x33;//3
//    label_data[161]=0x40;//@
//    label_data[162]=0x34;//4
//    label_data[163]=0x40;//@
//    label_data[164]=0x38;//8
//    label_data[165]=0x20;//space
//    label_data[166]=0x40;//@
//    label_data[167]=0x37;//7
//    label_data[168]=0x20;//space
//    label_data[169]=0x44;//D
//    label_data[170]=0x53;//S
//    label_data[171]=0x40;//@
//    label_data[172]=0x74;//t
//    label_data[173]=0x31;//1
//    label_data[174]=0x0D;//
//    label_data[175]=0x0A;//
//    label_data[176]=0x51;//Q
//    label_data[177]=0x30;//0
//    label_data[178]=0x30;//0
//    label_data[179]=0x30;//0
//    label_data[180]=0x31;//1
//    label_data[181]=0x0D;//
//    label_data[182]=0x0A;//
//    label_data[183]=0x45;//E
//    label_data[184]=0x20;//
//    label_data[185]=0x0D;//
//    label_data[186]=0x0A;//
//
//
////    
//}
/*
Function:This function will calculate week for both leap and non leap years.
		 Next week will start at next monday.
         At every monday week will be changed.
		 suppose jan 3 is saturday hence it it first week,and next monday i.e.
         jan 5 will be week second.
		 
*/
unsigned char Week_By_Julian_Date(void)
{
	unsigned int julain_day_lcl=0,monday_julian_day_lcl=0;
	unsigned char day_code_lcl=0,spl_monday_flag_lcl=0;


	//calculate julian day and day code for week calculation.
	julain_day_lcl=Print_Day_2(PRINT_JULIAN_DAY,RETURN_ONLY);
	day_code_lcl=(unsigned char)Print_Day_2(PRINT_DAY_CODE,RETURN_ONLY);


	//special case for every monday.
	if(day_code_lcl==1)
	{
		monday_julian_day_lcl=julain_day_lcl;
		if(julain_day_lcl==1)
		{
			spl_monday_flag_lcl=TRUE;
		}
	}
	else
	{
		monday_julian_day_lcl=julain_day_lcl-(day_code_lcl-1);
	}
	//case for dates of january from 1 to up coming monday...
	if(day_code_lcl>= julain_day_lcl)
	{
		//in this case it is first week only..
		return(HexToBCD_1(1));	
	}
	//regular case 
	
	//calculate week.
	if((monday_julian_day_lcl % 7) == 0)
	{
		return(HexToBCD_1((monday_julian_day_lcl/7)+1));
			
	}
	else
	{		
		if(spl_monday_flag_lcl==TRUE)
			return(HexToBCD_1(((monday_julian_day_lcl/7)+1)));
		else
			return(HexToBCD_1(((monday_julian_day_lcl/7)+2)));
	
	}
	
}


/*
Function:
Inputs:  
Outputs:
Calls:
Called By:
*/
unsigned int Print_Day_2(unsigned char type_lcl,unsigned char type_lcl_2)
{
	unsigned char rtc_day_lcl=0,rtc_month_lcl=0,rtc_year_lsb_lcl=0;
	unsigned char stored_day_lcl=0,stored_month_lcl=0,stored_year_lsb_lcl=0,stored_day_code_lcl=0,index_lcl=0;
	unsigned int data_lcl=0;
	unsigned char ramaining_days_stored_month_lcl=0,passed_days_rtc_month_lcl=0;
	unsigned int days_stored_year_lcl=0,days_next_year_lcl=0,days_completed_year_lcl=0;
	unsigned char msb_lcl=0,lsb_lcl=0;
	unsigned char rtc_read_day_lcl=0,rtc_read_month_lcl,rtc_read_year_lsb_lcl=0;
	//---------------------------------------------//
	// read current rtc data from 8583.
	rtc_day_lcl 				= ((ReadRtc(0x05)) & (0x3F));
	rtc_read_day_lcl			= rtc_day_lcl;
	msb_lcl=((rtc_day_lcl  & 0xF0) >> 4);
    lsb_lcl=(rtc_day_lcl& 0x0F);
	rtc_day_lcl= (msb_lcl*10)+ lsb_lcl;

	rtc_month_lcl 				= ((ReadRtc(0x06)) & (0x1F));
	rtc_read_month_lcl			= rtc_month_lcl;
	msb_lcl=((rtc_month_lcl  & 0xF0) >> 4);
    lsb_lcl=(rtc_month_lcl& 0x0F);
	rtc_month_lcl=(msb_lcl*10) + lsb_lcl;
    //--------------05_08_19_2-----start-------------------//
	//rtc_year_lsb_lcl    		=  ReadRtc(0x11);
    rtc_year_lsb_lcl=E2prom_Byte_Read(e2prom_2_page_2_add,(YEAR_LSB_COUNT_ADDRESS & mask_higher_byte) >> 8,(YEAR_LSB_COUNT_ADDRESS & mask_lower_byte));
    //--------------05_08_19_2-----ends-------------------//
	rtc_read_year_lsb_lcl		=  rtc_year_lsb_lcl;
	msb_lcl=((rtc_year_lsb_lcl  & 0xF0) >> 4);
    lsb_lcl=(rtc_year_lsb_lcl& 0x0F);
	rtc_year_lsb_lcl=(msb_lcl*10)+lsb_lcl;
	
	//---------------------------------------------//
	//---------------------------------------------//
	if(type_lcl== PRINT_DAY_CODE)
	{
		//read stored rtc data from bbram.
		stored_day_lcl 				= ReadRtc(stored_day_bbram_base_address);
	
		msb_lcl=((stored_day_lcl  & 0xF0) >> 4);
	    lsb_lcl=(stored_day_lcl& 0x0F);
		stored_day_lcl= (msb_lcl*10)+ lsb_lcl;
	
		stored_month_lcl 			= ReadRtc(stored_month_bbram_base_address);
		msb_lcl=((stored_month_lcl  & 0xF0) >> 4);
	    lsb_lcl=(stored_month_lcl& 0x0F);
		stored_month_lcl= (msb_lcl*10)+ lsb_lcl;
	
		stored_year_lsb_lcl 		= ReadRtc(stored_year_lsb_bbram_base_address);	
		msb_lcl=((stored_year_lsb_lcl  & 0xF0) >> 4);
	    lsb_lcl=(stored_year_lsb_lcl& 0x0F);
		stored_year_lsb_lcl=(msb_lcl*10)+lsb_lcl;
		//---------------------------------------------//
		//read stored day code from bbram.
		stored_day_code_lcl			= ReadRtc(day_code_bbram_base_address);
	
		//compare stored year at the tmie of last print and current rtc year.
		if(rtc_year_lsb_lcl == stored_year_lsb_lcl)
		{
			//if year is same then compare stored month at the tmie of last print and rtc month.
			if(rtc_month_lcl == stored_month_lcl)
			{
				//if month is same then compare stored day stored at time of last print and rtc day.
				if(rtc_day_lcl == stored_day_lcl)
				{
					//do not do nothing.
				}//end 
				else
				{	//if year and month are same then this condition is active.
					if(rtc_day_lcl > stored_day_lcl)
					{
						data_lcl = (rtc_day_lcl - stored_day_lcl);	//retrive day diffrence.
						data_lcl = (data_lcl%7);
						if(data_lcl==0)//if mod is zero assign day code as 7.
							data_lcl=7;					
						stored_day_code_lcl=(stored_day_code_lcl+data_lcl)%7;
						if(stored_day_code_lcl==0)//if mod is zero assign day code as 7.				
							stored_day_code_lcl=7;
	
						WriteRtc(stored_day_bbram_base_address,rtc_read_day_lcl); //write current day to update.
						WriteRtc(day_code_bbram_base_address,stored_day_code_lcl);//write current day code to update.
					}//end if(rtc_day_lcl > stored_day_lcl)
					
				}//end if(rtc_day_lcl == stored_day_lcl)
	
			}//end
			else
			{
				//if year is matched but month is not matched then this condition is active.
				data_lcl=0;
				if(rtc_month_lcl > stored_month_lcl)
				{
					index_lcl=stored_month_lcl+1;
					//this loop will evaluate days from remaning month except current and rtc month.
					for(;index_lcl<rtc_month_lcl;index_lcl++)
					{
						data_lcl=data_lcl+Month_Data(index_lcl,stored_year_lsb_lcl);
					}
					//calculate remaining days from stored month.
					ramaining_days_stored_month_lcl=Month_Data(stored_month_lcl,stored_year_lsb_lcl)-stored_day_lcl;
					//calculate passed days from rtc month.
					passed_days_rtc_month_lcl=rtc_day_lcl;
					//add all to calculate modulus.
					data_lcl=data_lcl+passed_days_rtc_month_lcl+ramaining_days_stored_month_lcl;
					data_lcl = (data_lcl%7);
					if(data_lcl==0)//if mod is zero assign day code as 7.
						data_lcl=7;		
					stored_day_code_lcl=(stored_day_code_lcl+data_lcl)%7;
					if(stored_day_code_lcl==0)//if mod is zero assign day code as 7.
						stored_day_code_lcl=7;
					WriteRtc(stored_day_bbram_base_address,rtc_read_day_lcl);//write current day to update.
					WriteRtc(stored_month_bbram_base_address,rtc_read_month_lcl);//write current month to update.
					WriteRtc(day_code_bbram_base_address,stored_day_code_lcl);//write current day code to update.
				}//end if(rtc_month_lcl > stored_month_lcl)
			}//end else if(rtc_month_lcl == stored_month_lcl)
		}//end if(rtc_year_lsb_lcl == stored_year_lsb_lcl)
		else
		{
			if(rtc_year_lsb_lcl > stored_year_lsb_lcl)
			{
				index_lcl=stored_year_lsb_lcl+1;
				//this loop will calculate days from all years between stored and current year, except stored and current year.
				for(;index_lcl<rtc_year_lsb_lcl;index_lcl++)
				{
					days_completed_year_lcl= days_completed_year_lcl +(( (index_lcl%4) == 0 )? 366 :365);
				}//end for(;index_lcl<rtc_year_lsb_lcl;index_lcl++).
				index_lcl=stored_month_lcl+1;
				//this loop will calculate reamening days from stored year.
				for(;index_lcl<=12;index_lcl++)
				{
					days_stored_year_lcl=days_stored_year_lcl+Month_Data(index_lcl,stored_year_lsb_lcl);
				}//end for(;index_lcl<=12;index_lcl++)
				//this loop will calculate reamening days from current year.
				index_lcl=1;
				for(;index_lcl<=rtc_month_lcl-1;index_lcl++)
				{
					days_next_year_lcl=days_next_year_lcl+Month_Data(index_lcl,stored_year_lsb_lcl+1);
				}//end for(;index_lcl<=rtc_month_lcl-1;index_lcl++)
				//calculate remaining days from stored month.
				ramaining_days_stored_month_lcl=Month_Data(stored_month_lcl,stored_year_lsb_lcl)-stored_day_lcl;
				//calculate passed days from rtc month.
				passed_days_rtc_month_lcl=rtc_day_lcl;
				//add all to calculate modulus.
				data_lcl=days_completed_year_lcl+days_stored_year_lcl+days_next_year_lcl+ramaining_days_stored_month_lcl+passed_days_rtc_month_lcl;	
				data_lcl=(data_lcl%7);
				if(data_lcl==0)//if mod is zero assign day code as 7.
					data_lcl=7;		
				stored_day_code_lcl=(stored_day_code_lcl+data_lcl)%7;
				if(stored_day_code_lcl==0)
					stored_day_code_lcl=7;//if mod is zero assign day code as 7.
	
				WriteRtc(stored_day_bbram_base_address,rtc_read_day_lcl);//write current day to update.
				WriteRtc(stored_month_bbram_base_address,rtc_read_month_lcl);//write current month to update.
				WriteRtc(stored_year_lsb_bbram_base_address,rtc_read_year_lsb_lcl);//write current year to update.
				WriteRtc(day_code_bbram_base_address,stored_day_code_lcl);//write current day code to update.
				
			}//end if(rtc_year_lsb_lcl > stored_year_lsb_lcl)
			else;
	
		}//end else..
		
		//-----19_03_14_2-----start----------//
		if(type_lcl_2==PRINT_AND_RETURN)
			Print(stored_day_code_lcl+0x30);
		//(type_lcl_2==RETURN_ONLY)
		return(stored_day_code_lcl);
		//-----19_03_14_2-----end------------//
	}//if(type_lcl== PRINT_DAY_CODE)
	else
	{
		data_lcl=0;
		for(index_lcl=1;index_lcl<rtc_month_lcl;index_lcl++)
		{
			data_lcl=data_lcl+Month_Data(index_lcl,rtc_year_lsb_lcl);
		}//end for(index_lcl=1;index_lcl<rtc_month_lcl;index_lcl++)
		//-----19_03_14_2-----start----------//
		if(type_lcl_2==PRINT_AND_RETURN)
		{
			Print(((DAA(HexToBCD_1(data_lcl+rtc_day_lcl))& 0x0F00)>>8) + 0x30);
			Print(((DAA(HexToBCD_1(data_lcl+rtc_day_lcl))& 0x00F0)>>4) + 0x30);
			Print((DAA(HexToBCD_1(data_lcl+rtc_day_lcl))& 0x000F)+0x30);
		}//end if(type_lcl_2==PRINT_AND_RETURN)
		return((data_lcl+rtc_day_lcl));
		//-----19_03_14_2-----end------------//
	}//end else if(type_lcl== PRINT_DAY_CODE)

}//end unsigned int Print_Day_2(unsigned char type_lcl,unsigned char type_lcl_2).



/*
Function:
Inputs:  
Outputs:
Calls:
Called By:
*/
unsigned char Month_Data(unsigned char mnth_lcl,unsigned char year_lcl)
{
	unsigned char var_lcl=0;
	switch(mnth_lcl)
	{
		case 1: //jan.
				var_lcl=31;
				break;
		case 2: //feb.
				if((year_lcl%4)==0)
					var_lcl=29;
				else
					var_lcl=28;
				break;
		case 3: //mar.
				var_lcl=31;
				break;
		case 4: //april.
				var_lcl=30;
				break;
		case 5: //may.
				var_lcl=31;
				break;
		case 6: //june.
				var_lcl=30;
				break;
		case 7: //july.
				var_lcl=31;
				break;
		case 8: //aug.
				var_lcl=31;
				break;
		case 9: //sept.
				var_lcl=30;
				break;
		case 10://oct.
				var_lcl=31;
				break;
		case 11://nov
				var_lcl=30;
				break;
		case 12://dec
				var_lcl=31;
				break;
		default:var_lcl=31;
				break;
	}//end switch(mnth_lcl)
	return(var_lcl);
}//end unsigned char Month_Data(unsigned char mnth_lcl,unsigned char year_lcl)




/*
Function:
Inputs:  
Outputs:
Calls:
Called By:
*/
//21_02_14_1
unsigned int Print_Day_1(unsigned char type_lcl_1,unsigned char type_lcl_2)
{
	unsigned char rtc_day_lcl=0,rtc_month_lcl=0,rtc_year_lsb_lcl=0;
	unsigned char stored_day_lcl=0,stored_month_lcl=0,stored_year_lsb_lcl=0,stored_day_code_lcl=0,index_lcl=0;
	unsigned int data_lcl=0;
	unsigned char ramaining_days_stored_month_lcl=0,passed_days_rtc_month_lcl=0;
	unsigned int days_stored_year_lcl=0,days_next_year_lcl=0,days_completed_year_lcl=0;
	unsigned char msb_lcl=0,lsb_lcl=0;
	unsigned char rtc_read_day_lcl=0,rtc_read_month_lcl,rtc_read_year_lsb_lcl=0;
	//---------------------------------------------//
	// read current rtc data from 8583.
	rtc_day_lcl 				= ((ReadRtc(0x05)) & (0x3F));
	rtc_read_day_lcl			= rtc_day_lcl;
	msb_lcl=((rtc_day_lcl  & 0xF0) >> 4);
    lsb_lcl=(rtc_day_lcl& 0x0F);
	rtc_day_lcl= (msb_lcl*10)+ lsb_lcl;

	rtc_month_lcl 				= ((ReadRtc(0x06)) & (0x1F));
	rtc_read_month_lcl			= rtc_month_lcl;
	msb_lcl=((rtc_month_lcl  & 0xF0) >> 4);
    lsb_lcl=(rtc_month_lcl& 0x0F);
	rtc_month_lcl=(msb_lcl*10) + lsb_lcl;
    //-------------------05_08_19_2----start-----------------//
	//rtc_year_lsb_lcl    		=  ReadRtc(0x11);
    rtc_year_lsb_lcl=E2prom_Byte_Read(e2prom_2_page_2_add,(YEAR_LSB_COUNT_ADDRESS & mask_higher_byte) >> 8,(YEAR_LSB_COUNT_ADDRESS & mask_lower_byte));
    //-------------------05_08_19_2----ends------------------//
	rtc_read_year_lsb_lcl		=  rtc_year_lsb_lcl;
	msb_lcl=((rtc_year_lsb_lcl  & 0xF0) >> 4);
    lsb_lcl=(rtc_year_lsb_lcl& 0x0F);
	rtc_year_lsb_lcl=(msb_lcl*10)+lsb_lcl;
	
	//---------------------------------------------//
	//---------------------------------------------//
	if(type_lcl_1== PRINT_DAY_CODE)//21_02_14_1.
	{
		//read stored rtc data from bbram.
		stored_day_lcl 				= ReadRtc(stored_day_bbram_base_address);
	
		msb_lcl=((stored_day_lcl  & 0xF0) >> 4);
	    lsb_lcl=(stored_day_lcl& 0x0F);
		stored_day_lcl= (msb_lcl*10)+ lsb_lcl;
	
		stored_month_lcl 			= ReadRtc(stored_month_bbram_base_address);
		msb_lcl=((stored_month_lcl  & 0xF0) >> 4);
	    lsb_lcl=(stored_month_lcl& 0x0F);
		stored_month_lcl= (msb_lcl*10)+ lsb_lcl;
	
		stored_year_lsb_lcl 		= ReadRtc(stored_year_lsb_bbram_base_address);	
		msb_lcl=((stored_year_lsb_lcl  & 0xF0) >> 4);
	    lsb_lcl=(stored_year_lsb_lcl& 0x0F);
		stored_year_lsb_lcl=(msb_lcl*10)+lsb_lcl;
		//---------------------------------------------//
		//read stored day code from bbram.
		stored_day_code_lcl			= ReadRtc(day_code_bbram_base_address);

	
		//compare stored year at the tmie of last print and current rtc year.
		if(rtc_year_lsb_lcl == stored_year_lsb_lcl)
		{
			//if year is same then compare stored month at the tmie of last print and rtc month.
			if(rtc_month_lcl == stored_month_lcl)
			{
				//if month is same then compare stored day stored at time of last print and rtc day.
				if(rtc_day_lcl == stored_day_lcl)
				{
					//do not do nothing.
				}//end 
				else
				{	//if year and month are same then this condition is active.
					if(rtc_day_lcl > stored_day_lcl)
					{
						data_lcl = (rtc_day_lcl - stored_day_lcl);	//retrive day diffrence.
						data_lcl = (data_lcl%7);
						if(data_lcl==0)//if mod is zero assign day code as 7.
							data_lcl=7;					
						stored_day_code_lcl=(stored_day_code_lcl+data_lcl)%7;
						if(stored_day_code_lcl==0)//if mod is zero assign day code as 7.				
							stored_day_code_lcl=7;
	
						WriteRtc(stored_day_bbram_base_address,rtc_read_day_lcl); //write current day to update.
						WriteRtc(day_code_bbram_base_address,stored_day_code_lcl);//write current day code to update.
					}//end if(rtc_day_lcl > stored_day_lcl)
					
				}//end if(rtc_day_lcl == stored_day_lcl)
	
			}//end
			else
			{
				//if year is matched but month is not matched then this condition is active.
				data_lcl=0;
				if(rtc_month_lcl > stored_month_lcl)
				{
					index_lcl=stored_month_lcl+1;
					//this loop will evaluate days from remaning month except current and rtc month.
					for(;index_lcl<rtc_month_lcl;index_lcl++)
					{
						data_lcl=data_lcl+Month_Data(index_lcl,stored_year_lsb_lcl);
					}//end for(;index_lcl<rtc_month_lcl;index_lcl++)
					//calculate remaining days from stored month.
					ramaining_days_stored_month_lcl=Month_Data(stored_month_lcl,stored_year_lsb_lcl)-stored_day_lcl;
					//calculate passed days from rtc month.
					passed_days_rtc_month_lcl=rtc_day_lcl;
					//add all to calculate modulus.
					data_lcl=data_lcl+passed_days_rtc_month_lcl+ramaining_days_stored_month_lcl;
					data_lcl = (data_lcl%7);
					if(data_lcl==0)//if mod is zero assign day code as 7.
						data_lcl=7;		
					stored_day_code_lcl=(stored_day_code_lcl+data_lcl)%7;
					if(stored_day_code_lcl==0)//if mod is zero assign day code as 7.
						stored_day_code_lcl=7;
					WriteRtc(stored_day_bbram_base_address,rtc_read_day_lcl);//write current day to update.
					WriteRtc(stored_month_bbram_base_address,rtc_read_month_lcl);//write current month to update.
					WriteRtc(day_code_bbram_base_address,stored_day_code_lcl);//write current day code to update.
				}//end if(rtc_month_lcl > stored_month_lcl)
			}//end else if(rtc_month_lcl == stored_month_lcl)
		}//end if(rtc_year_lsb_lcl == stored_year_lsb_lcl)
		else
		{
			if(rtc_year_lsb_lcl > stored_year_lsb_lcl)
			{
				index_lcl=stored_year_lsb_lcl+1;
				//this loop will calculate days from all years between stored and current year, except stored and current year.
				for(;index_lcl<rtc_year_lsb_lcl;index_lcl++)
				{
					days_completed_year_lcl= days_completed_year_lcl +(( (index_lcl%4) == 0 )? 366 :365);
				}
				index_lcl=stored_month_lcl+1;
				//this loop will calculate reamening days from stored year.
				for(;index_lcl<=12;index_lcl++)
				{
					days_stored_year_lcl=days_stored_year_lcl+Month_Data(index_lcl,stored_year_lsb_lcl);
				}
				//this loop will calculate reamening days from current year.
				index_lcl=1;
				for(;index_lcl<=rtc_month_lcl-1;index_lcl++)
				{
					days_next_year_lcl=days_next_year_lcl+Month_Data(index_lcl,stored_year_lsb_lcl+1);
				}
				//calculate remaining days from stored month.
				ramaining_days_stored_month_lcl=Month_Data(stored_month_lcl,stored_year_lsb_lcl)-stored_day_lcl;
				//calculate passed days from rtc month.
				passed_days_rtc_month_lcl=rtc_day_lcl;
				//add all to calculate modulus.
				data_lcl=days_completed_year_lcl+days_stored_year_lcl+days_next_year_lcl+ramaining_days_stored_month_lcl+passed_days_rtc_month_lcl;	
				data_lcl=(data_lcl%7);
				if(data_lcl==0)//if mod is zero assign day code as 7.
					data_lcl=7;		
				stored_day_code_lcl=(stored_day_code_lcl+data_lcl)%7;
				if(stored_day_code_lcl==0)
					stored_day_code_lcl=7;//if mod is zero assign day code as 7.
	
				WriteRtc(stored_day_bbram_base_address,rtc_read_day_lcl);//write current day to update.
				WriteRtc(stored_month_bbram_base_address,rtc_read_month_lcl);//write current month to update.
				WriteRtc(stored_year_lsb_bbram_base_address,rtc_read_year_lsb_lcl);//write current year to update.
				WriteRtc(day_code_bbram_base_address,stored_day_code_lcl);//write current day code to update.
				
			}
			else;
	
		}
		//-----21_02_14_1-----start----------//
		if(type_lcl_2==PRINT_AND_RETURN)
			Print(stored_day_code_lcl+0x30);
		//(type_lcl_2==RETURN_ONLY)
		return(stored_day_code_lcl);
		//-----21_02_14_1-----end------------//
	}
	else
	{
		data_lcl=0;
		for(index_lcl=1;index_lcl<rtc_month_lcl;index_lcl++)
		{
			data_lcl=data_lcl+Month_Data(index_lcl,rtc_year_lsb_lcl);
		}

		//-----21_02_14_1-----start----------//
		if(type_lcl_2==PRINT_AND_RETURN)
		{
			Print(((DAA(HexToBCD_1(data_lcl+rtc_day_lcl))& 0x0F00)>>8) + 0x30);
			Print(((DAA(HexToBCD_1(data_lcl+rtc_day_lcl))& 0x00F0)>>4) + 0x30);
			Print((DAA(HexToBCD_1(data_lcl+rtc_day_lcl))& 0x000F)+0x30);
		}//end if(type_lcl_2==PRINT_AND_RETURN)
		return((data_lcl+rtc_day_lcl));
		//-----21_02_14_1-----end------------//
	}//end else

}


void Read_Convert_Shift_Data_1(unsigned char* shift_details_array_lcl)
{
	unsigned char /*shift_details_array_lcl[32],*/index_lcl=0,validate_data_flag_lcl=0;

	for(index_lcl=0;index_lcl<=SHIFT_DETAILS_SIZE;index_lcl++)
		*(shift_details_array_lcl+index_lcl)=E2prom_Byte_Read(e2prom_2_page_2_add,((SHIFT_DETAILS_ADDRESS+index_lcl)& mask_higher_byte)>>8,(SHIFT_DETAILS_ADDRESS+index_lcl)& mask_lower_byte);
		//shift_details_array_lcl[index_lcl] = E2promRead(e2prom_2_address,((SHIFT_DETAILS_ADDRESS+index_lcl)& mask_higher_byte)>>8,(SHIFT_DETAILS_ADDRESS+index_lcl)& mask_lower_byte);
	if((*(shift_details_array_lcl+2)==':') && (*(shift_details_array_lcl+5)=='=')&& (*(shift_details_array_lcl+15)==','))
		validate_data_flag_lcl=TRUE;
	else
	{
        //if no shift data present then initialize variables with default value.
        //-----------17_05_18_3----start--------------//
        //default first shift timing is 06:00, character='A'.
		first_shift_hh_glb=0x06;                //shift HH
		first_shift_mm_glb=0x00;                //shift MM
		first_shift_character_glb='A';          //shift character i.e. 'A'.
        //Fill the similar data in the ram for processing in state diagram.
        *(shift_details_array_lcl)  ='0';       
        *(shift_details_array_lcl+1)='6';
        *(shift_details_array_lcl+2)=':';
        *(shift_details_array_lcl+3)='0';
        *(shift_details_array_lcl+4)='0';
        *(shift_details_array_lcl+5)='=';
        *(shift_details_array_lcl+6)='A';
        *(shift_details_array_lcl+7)=',';
        
        //default first shift timing is 14:00, character='B'.
		second_shift_hh_glb= 0x14; 
		second_shift_mm_glb= 0x00; 
		second_shift_character_glb='B';
        //Fill the similar data in the ram for processing in state diagram.
        *(shift_details_array_lcl+8) ='1';
        *(shift_details_array_lcl+9) ='4';
        *(shift_details_array_lcl+10)=':';
        *(shift_details_array_lcl+11)='0';
        *(shift_details_array_lcl+12)='0';
        *(shift_details_array_lcl+13)='=';
        *(shift_details_array_lcl+14)='B';
        *(shift_details_array_lcl+15)=',';
        //default first shift timing is 22:00, character='C'.
		third_shift_hh_glb=0x22;  
		third_shift_mm_glb=0x00;  
		third_shift_character_glb   ='C';
        //Fill the similar data in the ram for processing in state diagram.
        *(shift_details_array_lcl+16)='2';
        *(shift_details_array_lcl+17)='2';
        *(shift_details_array_lcl+18)=':';
        *(shift_details_array_lcl+19)='0';
        *(shift_details_array_lcl+20)='0';
        *(shift_details_array_lcl+21)='=';
        *(shift_details_array_lcl+22)='C';
        //-----------17_05_18_3----ends---------------//
		return;
	}//end else if..
	if(validate_data_flag_lcl==TRUE)
	{
		first_shift_hh_glb=  (((*(shift_details_array_lcl)-0x30)*16)+(*(shift_details_array_lcl+1)-0x30));
		first_shift_mm_glb=  (((*(shift_details_array_lcl+3)-0x30)*16)+(*(shift_details_array_lcl+4)-0x30));
		first_shift_character_glb= *(shift_details_array_lcl+6);

		second_shift_hh_glb=  (((*(shift_details_array_lcl+8)-0x30)*16)+(*(shift_details_array_lcl+9)-0x30));
		second_shift_mm_glb=  (((shift_details_array_lcl[11]-0x30)*16)+(*(shift_details_array_lcl+12)-0x30));
		second_shift_character_glb= *(shift_details_array_lcl+14);

		third_shift_hh_glb=  (((*(shift_details_array_lcl+16)-0x30)*16)+(*(shift_details_array_lcl+17)-0x30));
		third_shift_mm_glb=  (((*(shift_details_array_lcl+19)-0x30)*16)+(*(shift_details_array_lcl+20)-0x30));
		third_shift_character_glb= *(shift_details_array_lcl+22);

	}//end if(validate_data_flag_lcl==TRUE)
	
}//end unsigned char Read_Convert_Shift_Data()




unsigned long int Base_To_BCD_Converter(unsigned long int val_lcl,unsigned char base_lcl,unsigned char* result_array_lcl)
{
	unsigned long int result_lcl=0;
	unsigned char index_lcl=0;
	
	while(1)
	{
		//division..
		//example:122=5288/43 iteration 1
		//example:2=122/43  iteration 2
		result_lcl=(val_lcl/base_lcl);
		//example:is(122<43) iteration 1
		//example:is(2<43)   iteration 2
		if(result_lcl<base_lcl)
		{
			//example:36=(122%43)
			result_array_lcl[index_lcl]=(val_lcl%base_lcl);
			index_lcl++;
			//example:2.
			result_array_lcl[index_lcl]=result_lcl;
			return(0);//28_02_15_5.
		}//end if(result_lcl<base_lcl).
		//example:42=(5288%43)
		result_array_lcl[index_lcl]=(val_lcl%base_lcl);
		index_lcl++;
		//example:var_lcl=122.
		val_lcl=result_lcl;
	}//end while(1).
	//result_array_lcl[0]=42
	//result_array_lcl[1]=36
	//result_array_lcl[2]=02
	return(0);
}//end unsigned long int Base_To_BCD_Converter(unsigned long int val_lcl,unsigned char base_lcl,unsigned char* result_array_lcl)






/*
Function:allocated eeprom addres to write label data in eeprom (Modify with old function TSR )		 
Inputs: program number
Outputs:
Calls:
Called By:
*/
//25/11/11.
unsigned char Allocate_E2prom_Address_1(unsigned char max_prg_local)
{
	    if(max_prg_local<=32)
				return(e2prom_3_page_1_add);
		else if((max_prg_local>=33) && (max_prg_local<=64))
				return(e2prom_3_page_2_add);
	    else if((max_prg_local>=65) && (max_prg_local<=96))
				return(e2prom_4_page_1_add);
		else if((max_prg_local>=97)&& (max_prg_local<=128))
				return(e2prom_4_page_2_add);
	
	return(FALSE);
}//end unsigned char Allocate_E2prom_Address(unsigned char cable_local,unsigned char max_prg_local).




/*
Function:allocated eeprom addres to write label data in eeprom (Modify with old function TSR )		 
Inputs: program number
Outputs:
Calls:
Called By:
*/

unsigned char check_addresses_of_E2prom(unsigned char program_number)
{
    unsigned int address_assign;
    
    if(program_number<=32)
    {
        if(program_number==1)
        {
            address_assign = 1; 
        }
        else if(program_number==2)
        {
            address_assign = 2; 
        }
        else if(program_number==3)
        {
            address_assign = 3; 
        }
        else if(program_number==4)
        {
            address_assign = 4; 
        }
        else if(program_number==5)
        {
            address_assign = 5; 
        }
        else if(program_number==6)
        {
            address_assign = 6; 
        }
        else if(program_number==7)
        {
            address_assign = 7; 
        }
        else if(program_number==8)
        {
            address_assign = 8; 
        }
        else if(program_number==9)
        {
            address_assign = 9; 
        }
        else if(program_number==10)
        {
            address_assign = 10; 
        }
        else if(program_number==11)
        {
            address_assign = 11; 
        }
        else if(program_number==12)
        {
            address_assign = 12; 
        }
        else if(program_number==13)
        {
            address_assign = 13; 
        }
        else if(program_number==14)
        {
            address_assign = 14; 
        }
        else if(program_number==15)
        {
            address_assign = 15; 
        }
        else if(program_number==16)
        {
            address_assign = 16; 
        }
        else if(program_number==17)
        {
            address_assign = 17; 
        }
        else if(program_number==18)
        {
            address_assign = 18; 
        }
        else if(program_number==19)
        {
            address_assign = 19; 
        }
        else if(program_number==20)
        {
            address_assign = 20; 
        }
        else if(program_number==21)
        {
            address_assign = 21; 
        }
        else if(program_number==22)
        {
            address_assign = 22; 
        }
        else if(program_number==23)
        {
            address_assign = 23; 
        }
        else if(program_number==24)
        {
            address_assign = 24; 
        }
        else if(program_number==25)
        {
            address_assign = 25; 
        }
        else if(program_number==26)
        {
            address_assign = 26; 
        }
        else if(program_number==27)
        {
            address_assign = 27; 
        }
        else if(program_number==28)
        {
            address_assign = 28; 
        }
        else if(program_number==29)
        {
            address_assign = 29; 
        }
        else if(program_number==30)
        {
            address_assign = 30; 
        }
        else if(program_number==31)
        {
            address_assign = 31; 
        }
        else if(program_number==32)
        {
            address_assign = 32; 
        }
    }
    else if((program_number>=33)&&(program_number<=64))
    {
        if(program_number==33)
        {
            address_assign = 1; 
        }
        else if(program_number==34)
        {
            address_assign = 2; 
        }
        else if(program_number==35)
        {
            address_assign = 3; 
        }
        else if(program_number==36)
        {
            address_assign = 4; 
        }
        else if(program_number==37)
        {
            address_assign = 5; 
        }
        else if(program_number==38)
        {
            address_assign = 6; 
        }
        else if(program_number==39)
        {
            address_assign = 7; 
        }
        else if(program_number==40)
        {
            address_assign = 8; 
        }
        else if(program_number==41)
        {
            address_assign = 9; 
        }
        else if(program_number==42)
        {
            address_assign = 10; 
        }
        else if(program_number==43)
        {
            address_assign = 11; 
        }
        else if(program_number==44)
        {
            address_assign = 12; 
        }
        else if(program_number==45)
        {
            address_assign = 13; 
        }
        else if(program_number==46)
        {
            address_assign = 14; 
        }
        else if(program_number==47)
        {
            address_assign = 15; 
        }
        else if(program_number==48)
        {
            address_assign = 16; 
        }
        else if(program_number==49)
        {
            address_assign = 17; 
        }
        else if(program_number==50)
        {
            address_assign = 18; 
        }
        else if(program_number==51)
        {
            address_assign = 19; 
        }
        else if(program_number==52)
        {
            address_assign = 20; 
        }
        else if(program_number==53)
        {
            address_assign = 21; 
        }
        else if(program_number==54)
        {
            address_assign = 22; 
        }
        else if(program_number==55)
        {
            address_assign = 23; 
        }
        else if(program_number==56)
        {
            address_assign = 24; 
        }
        else if(program_number==57)
        {
            address_assign = 25; 
        }
        else if(program_number==58)
        {
            address_assign = 26; 
        }
        else if(program_number==59)
        {
            address_assign = 27; 
        }
        else if(program_number==60)
        {
            address_assign = 28; 
        }
        else if(program_number==61)
        {
            address_assign = 29; 
        }
        else if(program_number==62)
        {
            address_assign = 30; 
        }
        else if(program_number==63)
        {
            address_assign = 31; 
        }
        else if(program_number==64)
        {
            address_assign = 32; 
        }
    }
    else if((program_number>=65)&&(program_number<=96))
    {
        if(program_number==65)
        {
            address_assign = 1; 
        }
        else if(program_number==66)
        {
            address_assign = 2; 
        }
        else if(program_number==67)
        {
            address_assign = 3; 
        }
        else if(program_number==68)
        {
            address_assign = 4; 
        }
        else if(program_number==69)
        {
            address_assign = 5; 
        }
        else if(program_number==70)
        {
            address_assign = 6; 
        }
        else if(program_number==71)
        {
            address_assign = 7; 
        }
        else if(program_number==72)
        {
            address_assign = 8; 
        }
        else if(program_number==73)
        {
            address_assign = 9; 
        }
        else if(program_number==74)
        {
            address_assign = 10; 
        }
        else if(program_number==75)
        {
            address_assign = 11; 
        }
        else if(program_number==76)
        {
            address_assign = 12; 
        }
        else if(program_number==77)
        {
            address_assign = 13; 
        }
        else if(program_number==78)
        {
            address_assign = 14; 
        }
        else if(program_number==79)
        {
            address_assign = 15; 
        }
        else if(program_number==80)
        {
            address_assign = 16; 
        }
        else if(program_number==81)
        {
            address_assign = 17; 
        }
        else if(program_number==82)
        {
            address_assign = 18; 
        }
        else if(program_number==83)
        {
            address_assign = 19; 
        }
        else if(program_number==84)
        {
            address_assign = 20; 
        }
        else if(program_number==85)
        {
            address_assign = 21; 
        }
        else if(program_number==86)
        {
            address_assign = 22; 
        }
        else if(program_number==87)
        {
            address_assign = 23; 
        }
        else if(program_number==88)
        {
            address_assign = 24; 
        }
        else if(program_number==89)
        {
            address_assign = 25; 
        }
        else if(program_number==90)
        {
            address_assign = 26; 
        }
        else if(program_number==91)
        {
            address_assign = 27; 
        }
        else if(program_number==92)
        {
            address_assign = 28; 
        }
        else if(program_number==93)
        {
            address_assign = 29; 
        }
        else if(program_number==94)
        {
            address_assign = 30; 
        }
        else if(program_number==95)
        {
            address_assign = 31; 
        }
        else if(program_number==96)
        {
            address_assign = 32; 
        }
    }
    else if((program_number>=97)&&(program_number<=128))
    {
        if(program_number==97)
        {
            address_assign = 1; 
        }
        else if(program_number==98)
        {
            address_assign = 2; 
        }
        else if(program_number==99)
        {
            address_assign = 3; 
        }
        else if(program_number==100)
        {
            address_assign = 4; 
        }
        else if(program_number==101)
        {
            address_assign = 5; 
        }
        else if(program_number==102)
        {
            address_assign = 6; 
        }
        else if(program_number==103)
        {
            address_assign = 7; 
        }
        else if(program_number==104)
        {
            address_assign = 8; 
        }
        else if(program_number==105)
        {
            address_assign = 9; 
        }
        else if(program_number==106)
        {
            address_assign = 10; 
        }
        else if(program_number==107)
        {
            address_assign = 11; 
        }
        else if(program_number==108)
        {
            address_assign = 12; 
        }
        else if(program_number==109)
        {
            address_assign = 13; 
        }
        else if(program_number==110)
        {
            address_assign = 14; 
        }
        else if(program_number==111)
        {
            address_assign = 15; 
        }
        else if(program_number==112)
        {
            address_assign = 16; 
        }
        else if(program_number==113)
        {
            address_assign = 17; 
        }
        else if(program_number==114)
        {
            address_assign = 18; 
        }
        else if(program_number==115)
        {
            address_assign = 19; 
        }
        else if(program_number==116)
        {
            address_assign = 20; 
        }
        else if(program_number==117)
        {
            address_assign = 21; 
        }
        else if(program_number==118)
        {
            address_assign = 22; 
        }
        else if(program_number==119)
        {
            address_assign = 23; 
        }
        else if(program_number==120)
        {
            address_assign = 24; 
        }
        else if(program_number==121)
        {
            address_assign = 25; 
        }
        else if(program_number==122)
        {
            address_assign = 26; 
        }
        else if(program_number==123)
        {
            address_assign = 27; 
        }
        else if(program_number==124)
        {
            address_assign = 28; 
        }
        else if(program_number==125)
        {
            address_assign = 29; 
        }
        else if(program_number==126)
        {
            address_assign = 30; 
        }
        else if(program_number==127)
        {
            address_assign = 31; 
        }
        else if(program_number==128)
        {
            address_assign = 32; 
        }
    }
    return(address_assign);
}



/*
Function:This function used for to store label LBL file in EEPROM	 
Inputs: 
Outputs:
Calls:
Called By:
*/
void Store_Label_Data_In_E2prom( )
{
    unsigned char address_lcl,allocate_address,present_ckt_number_lcl,number_lcl;
    unsigned int destination_address,index_lcl;
    
    present_ckt_number_lcl =  E2prom_Byte_Read(e2prom_2_page_2_add,((current_cable_number_address)& mask_higher_byte)>>8,(current_cable_number_address)& mask_lower_byte);
    allocate_address = Allocate_E2prom_Address_1(present_ckt_number_lcl);
    destination_address = check_addresses_of_E2prom(present_ckt_number_lcl);
    destination_address = (LABEL_DATA_SIZE*(destination_address-1)) + 1;
    Write_Bulk_Data_Eeprom(allocate_address,destination_address,LABEL_DATA_SIZE,present_ckt_number_lcl,digit_ram_1);
//        Fill_Any_Char_Ram_With(digit_ram_1, LABEL_DATA_SIZE, 0);
//        Read_Bulk_Data_Eeprom(allocate_address,destination_address,LABEL_DATA_SIZE,present_ckt_number_lcl,digit_ram_1);
//        DelayS(1);
}//end ofStore_Label_Data_In_E2prom( )

//
//
//unsigned char Search_Sr_No(unsigned char prg_lcl, unsigned char* op_ram,unsigned int p_address_lcl,unsigned int size_lcl)
//{
//	unsigned int index_local_1=0,index_local_2=0;
//	unsigned char counter_local=0;
//	
//	while(index_local_1<=size_lcl)
//	{
//		if(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1)& mask_lower_byte)) == '@' && E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1+1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1+1)& mask_lower_byte)) == '6')
//		{	
//			index_local_1=index_local_1+2;
//			for(;index_local_2<=9;index_local_2++)
//			{
//				op_ram[index_local_2]=E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,(p_address_lcl+index_local_1) & mask_lower_byte);
//				counter_local++;
//				if(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1)& mask_lower_byte)) == '@' && E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1+1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1+1)& mask_lower_byte)) == '6')
//				{
//					counter_local--;
//					return(counter_local);
//				}//end if(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1)& mask_lower_byte)) == '@' && E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1+1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1+1)& mask_lower_byte)) == '6')
//				index_local_1++;
//			}//end for(;index_local_2<=9;index_local_2++).
//
//		}//end if(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1)& mask_lower_byte)) == '@' && E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1+1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1+1)& mask_lower_byte)) == '6')
//		index_local_1++;
//	}//end while(index_local_1<=size_lcl)
//	return(0);
//}//end unsigned char Search_Sr_No(unsigned char prg_lcl, unsigned char* op_ram,unsigned int p_address_lcl,unsigned int size_lcl)




unsigned char Search_Sr_No_1(unsigned char prg_lcl, unsigned char* op_ram,unsigned int p_address_lcl,unsigned int size_lcl)
{
	unsigned int index_local_1=0,index_local_2=0;
	unsigned char counter_local=0;
	
	while(index_local_1<=size_lcl)
	{
		if(E2prom_Byte_Read(prg_lcl,((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1)& mask_lower_byte)) == '@' && E2prom_Byte_Read(prg_lcl,((p_address_lcl+index_local_1+1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1+1)& mask_lower_byte)) == '6')
		{	
			index_local_1=index_local_1+2;
			for(;index_local_2<=9;index_local_2++)
			{
				op_ram[index_local_2]=E2prom_Byte_Read(prg_lcl,((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,(p_address_lcl+index_local_1) & mask_lower_byte);
				counter_local++;
				if(E2prom_Byte_Read(prg_lcl,((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1)& mask_lower_byte)) == '@' && E2prom_Byte_Read(prg_lcl,((p_address_lcl+index_local_1+1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1+1)& mask_lower_byte)) == '6')
				{
					counter_local--;
					return(counter_local);
				}//end if(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1)& mask_lower_byte)) == '@' && E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1+1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1+1)& mask_lower_byte)) == '6')
				index_local_1++;
			}//end for(;index_local_2<=9;index_local_2++).

		}//end if(E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1)& mask_lower_byte)) == '@' && E2prom_Byte_Read(Allocate_E2prom_Address(prg_lcl,max_number_programs),((p_address_lcl+index_local_1+1) & mask_higher_byte) >> 8,((p_address_lcl+index_local_1+1)& mask_lower_byte)) == '6')
		index_local_1++;
	}//end while(index_local_1<=size_lcl)
	return(0);
}//end unsigned char Search_Sr_No(unsigned char prg_lcl, unsigned char* op_ram,unsigned int p_address_lcl,unsigned int size_lcl)






void Check_circuit_number( )
{
    if((prg_specific_setting_ram[withstand_test_index] == SET)&&(prg_specific_setting_ram[ir_test_index] == SET))
    {
        (ir_last_ckt_no_glb > withstand_last_ckt_no_glb ? (last_ckt_no_glb = ir_last_ckt_no_glb ): (last_ckt_no_glb = withstand_last_ckt_no_glb));
    }
    else if((prg_specific_setting_ram[withstand_test_index] == RESET)&&(prg_specific_setting_ram[ir_test_index] == SET))
    {
        last_ckt_no_glb = ir_last_ckt_no_glb ;
    }
    else if((prg_specific_setting_ram[withstand_test_index] == SET)&&(prg_specific_setting_ram[ir_test_index] == RESET))
    {
        last_ckt_no_glb = withstand_last_ckt_no_glb ;
    }
    return(last_ckt_no_glb);
}