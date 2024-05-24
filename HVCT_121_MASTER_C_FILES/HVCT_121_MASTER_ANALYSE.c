#include<p24fj512gu408.h>

#include "../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"


/*
Function:
 void Self_Test()
	
Description:This function is used for Self test 
  
Parameters: Nil
 

Return Values: It returns no. of cards and is type of unsigned char.

Remarks: Nil
*/

void Self_Test()
{
    cards=assign_slave_address_and_card_detect();       // assign each slave address
    
    Display_String_20(self_test,1,9,1);
    self_test_pass_flag = 0;
    DelayMs(500);
    DelayMs(500);
    // write functions to check no points are connected. return the self test flag 
    
    if(cards > 0)
    {
        self_test_pass_flag = TRUE;
        Display_String_20(pass,1,4,2);
        DelayMs(200);
        Display_String_20(pass,1,4,2);
        Gen_Tone(PASS_TONE,4);
    }
    else
    {
        self_test_pass_flag = FALSE;
        Display_String_20(fail,1,4,2);
		DelayMs(300);
        Display_String_20(fail,1,4,2);
        Gen_Tone(6,4);
        DelayMs(200);
    }

}

/*
Function:
 void Cards_Test()
	
Description:
 This function will will check cards connected and gives number of cards . This function is used in Diagnosis menu for Cards test
  
Parameters: Nil
 

Return Values: It returns no. of cards and is type of unsigned char.

Remarks: Nil
*/
void Cards_Test()
{
    Display_String_20(cards_test,1,9,1);
    unsigned char cards_count_lcl = 0, address_lcl = 0;

    /*
    3.send uni cast command how are u  to address 01
    4. slave will respond and check i am fine command from slave
    5. save card no. in variable

    send uni cast command how are u  to add 02
    slave will respond and check i am fine command from slave
    increment the card no. save in variable
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
     * 
     */
    for (address_lcl = 1; address_lcl <= MAX_CARD_NUMBER; address_lcl++) 
    {
        Timer1_50ms_Start();
        Fill_Any_Char_Ram_With(modbus_array_glb, MAX_MODBUS_ARRAY_SIZE, 0x00);
        Fill_Small_Data_Ram(modbus_array_glb, address_lcl, MCMD_HOW_ARE_YOU, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL, MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x02);
        modbus_data_array_index_glb= 0;
        RS485_data_receive_cnt = 0;
        
        timer1_counter=0;                               //start timer count
        while(timer1_counter <= 10);                     //wait until slave responds to how r u command and master receives i am fine (300ms required) (previously 6)
      
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
    }// end of for (address_lcl = 1; address_lcl <= MAX_CARD_NUMBER; address_lcl++) 
    Timer1_50ms_Stop();
    
    if(cards_count_lcl > 0)
    {
        Display_String_20(pass,1,4,2);
        DelayMs(200);
        Display_String_20(pass,1,4,2);
        Display_String_20(showcards,1,6,3);
        Display_Number_4digit(cards_count_lcl,7,2,3);
        DelayS(2);
    }
    else
    {
        Display_String_20(fail,1,4,2);
		DelayMs(300);
        Display_String_20(fail,1,4,2);
        Display_String_20(nocards,1,12,3);
        Gen_Tone(5,4);
        DelayMs(200);
    }

}

/*
  Function:This function will help in configuring serial number for tester as a production activity.
		   Data will be entered through serial port.
  Input:none.
  Output:none.
  Calls:Display_Ram(),Read_Bulk_Data_E2prom().
  Called By:
*/
void Read_Tester_Sr_No_Display()
{

    unsigned char index_local=0;
	unsigned int  address_local=0;
	address_local=tester_sr_no_eeprom_address;
    unsigned char local_ram [8] ;
//	//read tester serial number.
    for(index_local=0;index_local<7;index_local++)						//store and re retrive from eeprom in to ram.
		{
			local_ram[index_local]=E2prom_Byte_Read(e2prom_2_page_2_add,((address_local)& mask_higher_byte)>>8,(address_local)& mask_lower_byte);//11/10/11.
			address_local++;
		}//end for(index_local=0;index_local<7;index_local++).
//	Read_Bulk_Data_E2prom(e2prom_2_page_2_add,tester_sr_no_eeprom_address,7,1,local_ram);   // to be done later 03/11/2022 
	Display_Ram(local_ram,13,0,7,3);					   //display serial number..
}//end function..

/*
  Function:This function set volume level for the buzzer on power on and required times as amplifier doesent stored
		   it after power off.
  Input:volume level variable read from memory.
  Output:none.
  Calls:E2prom_Byte_Write(),Mute_Volume().
  Called By:
*/
void Configure_Stored_Volume_Setting(unsigned char volume_level_local)
{
	unsigned char index_local=0;

	switch(volume_level_local)						//switch and set value for iterations for volume level according to attribute.
	{
		case 1:volume_level_local=24;
			break;
		case 2:volume_level_local=30;
			break;
		case 3:volume_level_local=33;
			break;
		case 4:volume_level_local=35;
			break;
		case 5:volume_level_local=37;
			break;
		case 6:volume_level_local=39;
			break;
		case 7:volume_level_local=41;
			break;
		case 8:volume_level_local=43;
			break;
		case 9:volume_level_local=45;
			break;
		case 10:volume_level_local=64;
			break;
		default:volume_level_local=37;				//set default value of iteration.
				volume_level_global=5;
				E2prom_Byte_Write(e2prom_2_page_2_add,((volume_setting_address)&mask_higher_byte) >> 8,(volume_setting_address)&mask_lower_byte,volume_level_global);//11/10/11.
			break;
	}//end switch(volume_level_local).
	Mute_Volume();								   //mute previous volume level of amplifier to start from base.

	for(index_local=0;index_local<=volume_level_local;index_local++)
	{
		DelayMs(1);											//refer amplifier specifications for following explanation.
		tris_volume_control=1;						//configure port pin as input for open drain condition.
        DelayMs(1);	
		tris_volume_control=0;						//configure port pin as output to toggle required number of times.
        DelayMs(1);	
		volume_control=1;							//set port pin 1 to actually increase volume for every iteration.
        DelayMs(1);	
		tris_volume_control=1;						//configure port pin as input for open drain condition.
	}//end for(index_local=0;index_local<=volume_level_local;index_local++)
}//end function..
/*
  Function:This function will mute the volume of buzzer.
  Input:none.
  Output:none.
  Calls:none
  Called By:none.
*/
void Mute_Volume()
{
	unsigned char index_local=0;

	for(index_local=0;index_local<=64;index_local++)
	{
        DelayMs(1);	
		tris_volume_control=1;						//configure port pin as input first.
        DelayMs(1);	
		tris_volume_control=0;						//configure port pin as output..
        DelayMs(1);	
		volume_control=0;							//set volume control pin low to mute volume.
		tris_volume_control=1;						//again configure as input so that it can act as open drain for amplifier.
       
	}//end for(index_local=0;index_local<=64;index_local++).
}//end void Mute_Volume().


/*
Function:
 void continuity_test(unsigned char last_ckt_number_lcl)
	
Description:
 This function will test the 1 to 1  continuity between ckts
  
Parameters:
 *          last_ckt_number_lcl 

Return Values: none

Remarks: Nil
*/
void continuity_test(unsigned char last_ckt_number_lcl)
{
    unsigned char address_lcl,present_ckt_number_lcl,relay_number_lcl=0,ckt_lcl;
  
//    DelayMs(500);                                                          // Wait for 100ms
//    Switch_Interface_Relays_To_LV();                                                 // Switch relays on interface cards to LV side
    while((continuity_test_pass_flag_glb == FALSE) && continuity_test_fail_flag_glb == FALSE)
    {
        //send commands to make all relays OFF both card 1 and card 2;
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x02);
        DelayMs(200); 
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x02);
        DelayMs(200);
        Switch_Interface_Relays_To_LV(); 
        for(present_ckt_number_lcl=1;present_ckt_number_lcl <= last_ckt_number_lcl ; present_ckt_number_lcl++)
        {
            if(key_pressed_flag==TRUE)
            {
                key_pressed_flag=FALSE;
                key_data_glb = IC8574_ByteRead();
                key_code =(key_data_glb & 0X0F);
                if((key_code==ESC_KEY))
                {
                    continuity_test_fail_flag_glb = TRUE;
                    break;
                }
                key_data_glb = IC8574_ByteRead();
                key_code =(key_data_glb & 0X0F);
                if((key_code==ESC_KEY))
                {
                    continuity_test_fail_flag_glb = TRUE;
                    break;
                }
//                key_data_glb = IC8574_ByteRead();
//                key_code =(key_data_glb & 0X0F);
//                if((key_code==ESC_KEY))
//                {
//                    continuity_test_fail_flag_glb = TRUE;
//                    break;
//                }
//                key_data_glb = IC8574_ByteRead();
//                key_code =(key_data_glb & 0X0F);
//                if((key_code==ESC_KEY))
//                {
//                    continuity_test_fail_flag_glb = TRUE;
//                    break;
//                }
//                key_data_glb = IC8574_ByteRead();
//                key_code =(key_data_glb & 0X0F);
//                if((key_code==ESC_KEY))
//                {
//                    continuity_test_fail_flag_glb = TRUE;
//                    break;
//                }
            }
            if(present_ckt_number_lcl <=16)                                     // if ckt number is from 1 to 16 then select address 01 i.e for card 1
            {
                address_lcl = 0x01;                                             // for CKTs from 1 t0 16 select card with address "01" 
                relay_number_lcl =  present_ckt_number_lcl ;
            }
            if(present_ckt_number_lcl > 16)
            {
                address_lcl = 0x02;                                             // for CKTs from 17 t0 32 select card with address "02" i.e for card 2
                relay_number_lcl =  present_ckt_number_lcl-16;                  // CKT no. 17 to 32  are  point no. 1 to 16  of card 2 respectively hence conversion is done(if ckt no.  is 17 that means point no. 1 of card 2)       
            }
            //-----------------------------check ckt open -----Start------------------------------------//
            if(present_ckt_number_lcl==17)
            {
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                Send_Modbus_Data(modbus_array_glb,0x00,0x02);
                DelayMs(200); 
            }
            /*1. Make 1st relay of present ckt ON & read Port pin */
            Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
            Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_SINGLE_RLY_OFF_SINGLE_RLY_ON,relay_number_lcl+16,relay_number_lcl,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
            Send_Modbus_Data(modbus_array_glb,0x00,0x04);
            DelayMs(200); 
            if(DIG_READ_rg6_pin == HIGH )        // if '1' means not open , do nothing 
            {
                  ;
            }
            else if(DIG_READ_rg6_pin == LOW)    //if '0' means open
            {
                //Display present ckt open, break the loop & Start from beginning  (Stop at First Fault))
                if(DIG_READ_rg6_pin == LOW)
                {
                Display_String_20(open_ckt,1,9,3);
                Display_Number_4digit(present_ckt_number_lcl,10,2,3);
                 Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_SINGLE_RLY_OFF,relay_number_lcl,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                Send_Modbus_Data(modbus_array_glb,0x00,0x03);
                DelayMs(400); 
                break;
                }
            }
            
            /*2. Make 1st relay of present ckt OFF & 2nd relay of present ckt ON & read Port pin */
            Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
            Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_SINGLE_RLY_OFF_SINGLE_RLY_ON,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
            Send_Modbus_Data(modbus_array_glb,0x00,0x04);
            DelayMs(200);                                  // 200ms delay required after sending command
            if(DIG_READ_rg6_pin == HIGH )       
            {
                ;
            }
            else if(DIG_READ_rg6_pin == LOW)
            {
                if(DIG_READ_rg6_pin == LOW)
                {
                //Display present ckt Open
                Clear_Lcd_Line(3);
                Display_String_20(open_ckt,1,9,3);
                Display_Number_4digit(present_ckt_number_lcl,10,2,3);
                /* Make both relays associated with present ckt OFF */
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLY_PAIR_OFF,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                Send_Modbus_Data(modbus_array_glb,0x00,0x04);
                DelayMs(400); 
                break;
                }
            }
            //-----------------------------check ckt open -----End------------------------------------//
            
            
            //-----------------------------check ckt Short -----Start------------------------------------//
            Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLY_PAIR_OFF,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
            Send_Modbus_Data(modbus_array_glb,0x00,0x04);
            DelayMs(200);   
            Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLY_PAIR_ON,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
            Send_Modbus_Data(modbus_array_glb,0x00,0x04);
            DelayMs(200);                                  // 200ms delay required after sending command    
            if(DIG_READ_rg6_pin == LOW)       
            {
                Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLY_PAIR_OFF,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                Send_Modbus_Data(modbus_array_glb,0x00,0x04);
                DelayMs(200);
                Clear_Lcd_Line(3);
                Display_String_20(ckt_pass,1,18,3);                             // Display each ckt pass
                Display_Number_4digit(present_ckt_number_lcl,5,2,3);
                if(present_ckt_number_lcl == last_ckt_number_lcl )         
                {
                    continuity_test_pass_flag_glb = TRUE;
//                    DelayMs(400);
                }
            }
            else if(DIG_READ_rg6_pin == HIGH ) 
            {
                for(ckt_lcl=present_ckt_number_lcl+1; ckt_lcl<=last_ckt_number_lcl; ckt_lcl++ ) // to find out exact short between 2 circuits 
                {
                    if(key_pressed_flag==TRUE)
                    {
                        key_pressed_flag=FALSE;
                        key_data_glb = IC8574_ByteRead();
                        key_code =(key_data_glb & 0X0F);
                        if((key_code==ESC_KEY))
                        {
                            continuity_test_fail_flag_glb = TRUE;
                            break;
                        }
                    }
                    if(ckt_lcl <= 16)
                    {
                        address_lcl = 0x01;                                 // if ckt number is from 1 to 16 then select address 01 i.e for card 1
                        relay_number_lcl =  ckt_lcl;
                    }
                    if(ckt_lcl > 16)
                    {
                        address_lcl = 0x02;                                // for CKTs from 17 t0 32 select card with address "02" 
                        relay_number_lcl =  ckt_lcl-16;                     // CKT no. 17 to 32  are  point no. 1 to 16  of card 2 respectively hence conversion is done(if ckt no.  is 17 that means point no. 1 of card 2)       
                    }
                    
                    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                    Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLY_PAIR_ON,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                    Send_Modbus_Data(modbus_array_glb,0x00,0x04);
                    DelayMs(200);
                    if(DIG_READ_rg6_pin == HIGH )
                    {
                        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLY_PAIR_OFF,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
                        DelayMs(200);
                        asm("nop");
                    }
                    else if(DIG_READ_rg6_pin == LOW )
                    {
                        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLY_PAIR_OFF,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
                        DelayMs(200);
                       //display present ckt and ckt_lcl short
                        Clear_Lcd_Line(3);
                        Display_String_20(short_ckt,1,10,3);
                        Display_Number_4digit(present_ckt_number_lcl,11,2,3);
                        if(address_lcl == 0x01)
                        {
                        Display_Number_4digit(relay_number_lcl,15,2,3);
                        }
                        else if(address_lcl == 0x02)
                        {
                        Display_Number_4digit(relay_number_lcl+16,15,2,3);
                        }
                        DelayMs(200);
                        break;
                    }
                    if(ckt_lcl == last_ckt_number_lcl)
                    {
                        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLY_PAIR_OFF,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
                        DelayMs(200);
                        Display_String_20(short_fault,1,18,3);
                        Display_Number_4digit(present_ckt_number_lcl,5,2,3);
                        break;
                    }
                }
                break;
            }
            if(key_pressed_flag==TRUE)
            {
                key_pressed_flag=FALSE;
                key_data_glb = IC8574_ByteRead();
                key_code =(key_data_glb & 0X0F);
                if((key_code==ESC_KEY))
                {
                    continuity_test_fail_flag_glb = TRUE;
                    break;
                }
                key_data_glb = IC8574_ByteRead();
                key_code =(key_data_glb & 0X0F);
                if((key_code==ESC_KEY))
                {
                    continuity_test_fail_flag_glb = TRUE;
                    break;
                }
            }
        }// end for(present_ckt_number_lcl=1;present_ckt_number_lcl <= last_ckt_number_lcl ; present_ckt_number_lcl++)
    }
    if(continuity_test_pass_flag_glb == TRUE)
    {
    Clear_Lcd_Line(2);
    Clear_Lcd_Line(3);
    Display_String_20(continuity_pass,1,15,3);
    DelayMs(500);                                  //Pass tone to be added
    Clear_Lcd_Line(3);
    }
    else if(continuity_test_fail_flag_glb == TRUE)
    {
//        continuity_test_fail_flag_glb = FALSE;
        Clear_Lcd_Line(2);
        Clear_Lcd_Line(3);
        Display_String_20(continuity_fail,1,15,3);
        DelayMs(500);                                  //fail tone to be added
        Clear_Lcd_Line(3);
    }
//    DelayMs(200);
}





/*
Function:
 void HV_IR_test(unsigned char last_ckt_number_lcl)
	
Description:
 This function will carry out the IR test.
  
Parameters:
 *          last_ckt_number_lcl 

Return Values: none

Remarks: Nil
*/
void HV_IR_test(unsigned char last_ckt_number_lcl)
{
    unsigned char address_lcl,present_ckt_number_lcl,relay_number_lcl=0,index_lcl,ckt_pass_count_lcl=0;
    unsigned int read_data_lcl;
    float  measured_ir_lcl , leakage_current,monitor_voltage_lcl ,insulation_resistance_lcl=0;
    float voltage_lcl[10]; 
    insulation_resistance_lcl = (float)insulation_resistance_glb ;
//    DelayMs(1000);
    
//    Switch_Interface_Relays_To_HV();                    // Switch relays on interface cards to HV side
//    DelayMs(50);    

    // Initially all relays of both cards 1 & 2 are made ON hence all points are connected to HV-
    
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0x01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0x02,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 2 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Switch_Interface_Relays_To_HV();                    // Switch relays on interface cards to HV side
    for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
    {
        if(present_ckt_number_lcl <= 16)                                     // if ckt number is from 1 to 16 then select address 01 i.e for card 1
        {
            address_lcl = 0x01;                                             // for CKTs from 1 t0 16 select card with address "01" 
            relay_number_lcl =  present_ckt_number_lcl;
        }
        if(present_ckt_number_lcl > 16)
        {
            if(present_ckt_number_lcl == 17)
            {
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,0x01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
                Send_Modbus_Data(modbus_array_glb,0x00,0x02);
                DelayMs(200);
                
            }
            address_lcl = 0x02;                                             // for CKTs from 17 t0 32 select card with address "02" 
            relay_number_lcl =  present_ckt_number_lcl-16;                  // CKT no. 17 to 32  are  point no. 1 to 16  of card 2 respectively hence conversion is done(if ckt no.  is 17 that means point no. 1 of card 2)       
        }
        
        //Present Ckt(initially 1 and 17 ) is connected to HV+ by making relays OFF related to that points and others are connected to HV- by making relays ON
        
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLYS_ON_EXCEPT_PAIR,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
        DelayMs(200); 
        DelayMs(200); 
        
//        Clear_Display();
        Initialize_Lcd();
        Display_String_20(prgno,1,16,1);
        Display_String_20(pn,16,2,1);
        Display_Number_4digit(current_cable_number,18,3,1);
        
//        Clear_Lcd_Line(2);
        Display_String_20(testing,1,11,3);
        Set_HighVoltage(hv_voltage_glb);  //High voltage is applied only after relays are switched. 
        //------------------------------------------------------------------------------------------------------------------------//
        
        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        Timer2_100ms_Start();
       timer2_counter=0;
        while (timer2_counter <= 30)
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            monitor_voltage_glb = (read_voltage_glb * 1500)/5;
            voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
            monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
            voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
            Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
            Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
            Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
            Display_String_20(vdc,8,3,2);         
        }
      Timer2_100ms_Stop();
        
        //------------------------------------------------------------------------------------------------------------------------//
//        DelayS(3);                        // Delay of minimum 3 seconds required to settle the High voltage to the selected value

        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        for(index_lcl=0;index_lcl < 30;index_lcl++)       //  30 adc  readings are taken
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            Volts_glb[index_lcl]= read_voltage_glb ;   
            monitor_voltage_glb = Volts_glb[index_lcl] + monitor_voltage_glb ; 
            DelayMs(1);
        }
        monitor_voltage_glb = (monitor_voltage_glb - Volts_glb[0]) / 29 ;            // first reading is neglected. Average of 29 readings are taken
       
        monitor_voltage_glb = (monitor_voltage_glb * 1500)/5;                        // since monitor voltage(0V - 5V) is proportional to actual high voltage(0V - 1500V) hence conversion is done to calculated actual voltage observed .
       
        //--------------------To Display Float value on LCD Below Conversion is done-------------------------------------//
        voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
        monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
        voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
        Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
        Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
        Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
        Display_String_20(vdc,8,3,2);                                                // string VDC displayed 
        //---------------------------------------------------------------------------------------------------------------//
        Display_String_20(testing,1,11,3); 
        
        IR_Measure_Voltage_glb = 0 ;
        Fill_Any_float_Ram_With(Volts_glb,50,0.0);
        read_data_lcl = 0;
        read_voltage_glb =0;
        for(index_lcl=0;index_lcl<50;index_lcl++)
        {
               read_data_lcl=read_adc_IR_MEASURE();
               read_voltage_glb = read_data_lcl*0.001225;
               Volts_glb[index_lcl]= read_voltage_glb ;
               IR_Measure_Voltage_glb = Volts_glb[index_lcl] + IR_Measure_Voltage_glb ;
               DelayMs(1);
        }
        IR_Measure_Voltage_glb = IR_Measure_Voltage_glb / 50 ;                  // Average of 50 readings
        leakage_current =  (IR_Measure_Voltage_glb / 5100 )* 1000000;         // I = V/R (Leakage current = measured voltage across shunt / shunt resistance(5kohm))
        measured_ir_lcl =(monitor_voltage_glb / leakage_current);              //Insulation Resistance = voltage applied / leakage current ;
        Reset_HighVoltage();
        DelayMs(1000);

        if(measured_ir_lcl > insulation_resistance_lcl)
        {
            if(test_settings_ram[every_ckt_display_result_index] == SET)
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
                voltage_integer_part = (unsigned int)measured_ir_lcl;
                measured_ir_lcl = (measured_ir_lcl - voltage_integer_part) * 10000;
                voltage_fractional_part = (unsigned int)measured_ir_lcl;
                Clear_Lcd_Line(3);
                Display_Number_4digit(voltage_integer_part,1,4,3); 
                Display_String_20(dec_point,5,1,3);
                Display_Number_4digit(voltage_fractional_part,6,2,3);
                Display_String_20(mohm,8,4,3);
                Display_String_20(ckt_ir_pass,1,18,4);                             // Display each ckt pass
                Display_Number_4digit(present_ckt_number_lcl,5,2,4);
            } 
            else
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
            }
            
        }
        else if(measured_ir_lcl < insulation_resistance_lcl)
        {
            
            store_result_glb[present_ckt_number_lcl] = FAIL ;
            if(test_settings_ram[every_ckt_display_result_index ]== SET)
            {   
                voltage_integer_part = (unsigned int)measured_ir_lcl;
                measured_ir_lcl = (measured_ir_lcl - voltage_integer_part) * 10000;
                voltage_fractional_part = (unsigned int)measured_ir_lcl;
                Clear_Lcd_Line(3);
                Display_Number_4digit(voltage_integer_part,1,4,3); 
                Display_String_20(dec_point,5,1,3);
                Display_Number_4digit(voltage_fractional_part,6,2,3);
                Display_String_20(mohm,8,4,3);
                Display_String_20(ckt_ir_fail,1,18,4);                           // Display each ckt fail
                Display_Number_4digit(present_ckt_number_lcl,5,2,4); 
            }
            if(test_settings_ram[stop_at_first_fault_index]== SET)
            {
                ir_test_fail_flag_glb = TRUE ;
                break;
            }
        }// end of else if(measured_ir_lcl < insulation_resistance_lcl)
//        DelayMs(1000);
    }// end of for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
     Clear_Lcd_Line(4);
//     Reset_HighVoltage();
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    // initially All relays of both card 1 & 2 are made ON hence all points are connected to HV-
    Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
   
    if(ckt_pass_count_lcl == last_ckt_number_lcl)
    {
        ir_test_pass_flag_glb = TRUE;
        Clear_Lcd_Line(2);
        Clear_Lcd_Line(3);
        Display_String_20(ir_pass,1,12,3);
        DelayMs(500); 
        Clear_Lcd_Line(3);
    }
    else
    {
        Clear_Lcd_Line(2);
        Clear_Lcd_Line(3);
        ir_test_fail_flag_glb = TRUE;
        Display_String_20(ir_fail,1,12,3);
        DelayMs(500); 
        Clear_Lcd_Line(3);
    }
}


/*
Function:
void HV_Withstand_test(unsigned char last_ckt_number_lcl)
	
Description:
 This function will carry out the Withstand test.
  
Parameters:
 *          last_ckt_number_lcl 

Return Values: none

Remarks: Nil
*/
void HV_Withstand_test(unsigned char last_ckt_number_lcl)
{
    unsigned char address_lcl,present_ckt_number_lcl,relay_number_lcl=0,index_lcl,ckt_pass_count_lcl=0;
    unsigned int read_data_lcl,i_lcl;
    float  measured_ir_lcl , leakage_current,monitor_voltage_lcl ,insulation_resistance_lcl=0;
    float voltage_lcl[10]; 
    insulation_resistance_lcl = (float)insulation_resistance_glb ;
//    DelayMs(1000);
    
    Switch_Interface_Relays_To_HV();                    // Switch relays on interface cards to HV side
//    DelayMs(50);    
    // Initially all relays of both cards 1 & 2 are made ON hence all points are connected to HV-
    
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 2 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    
    for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
    {
        if(present_ckt_number_lcl <= 16)                                     // if ckt number is from 1 to 16 then select address 01 i.e for card 1
        {
            address_lcl = 0x01;                                             // for CKTs from 1 t0 16 select card with address "01" 
            relay_number_lcl =  present_ckt_number_lcl;
        }
        if(present_ckt_number_lcl > 16)
        {
            if(present_ckt_number_lcl == 17)
            {
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,0x01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
                Send_Modbus_Data(modbus_array_glb,0x00,0x02);
                DelayMs(200);
                
            }
            address_lcl = 0x02;                                             // for CKTs from 17 t0 32 select card with address "02" 
            relay_number_lcl =  present_ckt_number_lcl-16;                  // CKT no. 17 to 32  are  point no. 1 to 16  of card 2 respectively hence conversion is done(if ckt no.  is 17 that means point no. 1 of card 2)       
        }
        
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        
        //Present Ckt(initially 1 nad 17 ) is connected to HV+ by making relays OFF related to that points and others are connected to HV- by making relays ON
        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLYS_ON_EXCEPT_PAIR,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
        DelayMs(200); 
//        DelayMs(200); 
      
//        Reset_HighVoltage();
        Set_HighVoltage(withstand_hv_voltage_glb);           //High voltage is applied only after relays are switched. 
      //---------------------------------------------------------------------------------------------------------//
        
        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        Timer2_100ms_Start();
       timer2_counter=0;
        while (timer2_counter <= 30)
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            monitor_voltage_glb = (read_voltage_glb * 1500)/5;
            voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
            monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
            voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
            Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
            Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
            Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
            Display_String_20(vdc,8,3,2);         
        }
      Timer2_100ms_Stop();
        
      //---------------------------------------------------------------------------------------------------------// 
        
        
//        DelayS(3);                      // Delay of min 3 seconds required to settle the High voltage to the selected value.
        Clear_Lcd_Line(2);
        
        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        for(index_lcl=0;index_lcl<30;index_lcl++)  //  30 adc  readings are taken
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            Volts_glb[index_lcl]= read_voltage_glb ;   
            monitor_voltage_glb = Volts_glb[index_lcl] + monitor_voltage_glb ; 
            DelayMs(1);
        }
        monitor_voltage_glb = (monitor_voltage_glb - Volts_glb[0]) /29 ;            // first reading is neglected. Average of 29 readings are taken
       
        monitor_voltage_glb = (monitor_voltage_glb * 1500)/5;                        // since monitor voltage(0V - 5V) is proportional to actual high voltage(0V - 1500V) hence conversion is done to calculated actual voltage observed .
       
        //--------------------To Display Float value on LCD Below Conversion is done-------------------------------------//
        voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
        monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
        voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
        Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
        Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
        Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
        Display_String_20(vdc,8,3,2);                                          // decimal point displayed
        //---------------------------------------------------------------------------------------------------------------//
        
        
        //----------------------------------------------------------------------------------------------------------------------//
        //Time required for withstand test is generated using Timer 2  of time period 100ms     
        timer2_counter = 0;
        sec_counter = 0;

        Timer2_100ms_Start();
        i_lcl =1;
        while(timer2_counter <= (hv_withstand_time_glb *0.01))
        {
    //        sec_counter = 0;
            Clear_Lcd_Line(4);
            Display_String_20(testing,1,11,3); 
            if(hv_withstand_time_glb == 500)
            {
                ;//Display_Number_4digit(hv_withstand_time_glb,18,3,3);
            } 
            else
            {
                Display_Number_4digit(i_lcl,19,2,3);
            }
            DelayMs(1000);
            i_lcl++;
        }
//        Display_Number_4digit(i_lcl,19,2,3);
        Timer2_100ms_Stop();   
 //----------------------------------------------------------------------------------------------------------------------//  
        
        IR_Measure_Voltage_glb = 0 ;
        Fill_Any_float_Ram_With(Volts_glb,50,0.0);
        read_data_lcl = 0;
        read_voltage_glb =0;
        for(index_lcl=0;index_lcl<50;index_lcl++)
        {
               read_data_lcl=read_adc_IR_MEASURE();
               read_voltage_glb = read_data_lcl*0.001225;
               Volts_glb[index_lcl]= read_voltage_glb ;
               IR_Measure_Voltage_glb = Volts_glb[index_lcl] + IR_Measure_Voltage_glb ;
               DelayMs(1);
        }
        IR_Measure_Voltage_glb = IR_Measure_Voltage_glb / 50 ;                  // Average of 50 readings
        leakage_current =  (IR_Measure_Voltage_glb / 5100 ) * 1000000;         // I = V/R (Leakage current = measured voltage across shunt / shunt resistance(5kohm))
        //measured_ir_lcl =(monitor_voltage_glb / leakage_current);              //Insulation Resistance = voltage applied / leakage current ;
        Reset_HighVoltage();

        if(leakage_current < withstand_current_glb)
        {
            if(test_settings_ram[every_ckt_display_result_index] == SET)
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
                Clear_Lcd_Line(3);
                Display_String_20(ckt_pass,1,11,4);                             // Display each ckt pass
                Display_Number_4digit(present_ckt_number_lcl,5,2,4);
            } 
            else
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
            }
            
        }
        else if(leakage_current > withstand_current_glb || (monitor_voltage_glb == 0.0))
        {
            
            store_result_glb[present_ckt_number_lcl] = FAIL ;
            if(test_settings_ram[every_ckt_display_result_index ]== SET)
            {   
                voltage_integer_part = (unsigned int)leakage_current;
                measured_ir_lcl = (leakage_current - voltage_integer_part) * 10000;
                voltage_fractional_part = (unsigned int)measured_ir_lcl;
                Clear_Lcd_Line(3);
                Display_Number_4digit(voltage_integer_part,1,4,3); 
                Display_String_20(dec_point,5,1,3);
                Display_Number_4digit(voltage_fractional_part,6,2,3);
                Display_String_20(ua,8,2,3);
                Display_String_20(ckt_fail,1,11,4);                           // Display each ckt fail
                Display_Number_4digit(present_ckt_number_lcl,5,2,4); 
            }
            if(test_settings_ram[stop_at_first_fault_index]== SET)
            {
                break;
            }
        }

//        DelayMs(200);

    } //end of for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
    
//    Reset_HighVoltage();
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    // initially All relays of both card 1 & 2 are made ON hence all points are connected to HV-
    Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    
    if(ckt_pass_count_lcl == last_ckt_number_lcl)
    {
        withstand_test_pass_flag_glb = TRUE;
    }
    else
    {
        withstand_test_fail_flag_glb = TRUE;
    }
}





/*
Function:
 void HV_IR_test(unsigned char last_ckt_number_lcl)
	
Description:
 This function will carry out the IR test.
  
Parameters:
 *          last_ckt_number_lcl 

Return Values: none

Remarks: Nil
*/
void Same_Voltage_Withstand_n_IR_Test(unsigned char last_ckt_number_lcl)
{
    unsigned char address_lcl,present_ckt_number_lcl,relay_number_lcl=0,index_lcl,ckt_pass_count_lcl=0;
    unsigned int read_data_lcl,i_lcl;
    float  measured_ir_lcl , leakage_current,monitor_voltage_lcl ,insulation_resistance_lcl=0;
    float voltage_lcl[10]; 
    insulation_resistance_lcl = (float)insulation_resistance_glb ;
//    DelayMs(1000);
    
    Switch_Interface_Relays_To_HV();                    // Switch relays on interface cards to HV side
//    DelayMs(50);    
    // Initially all relays of both cards 1 & 2 are made ON hence all points are connected to HV-
    
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 2 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    
    for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
    {
        if(present_ckt_number_lcl <= 16)                                     // if ckt number is from 1 to 16 then select address 01 i.e for card 1
        {
            address_lcl = 0x01;                                             // for CKTs from 1 t0 16 select card with address "01" 
            relay_number_lcl =  present_ckt_number_lcl;
        }
        if(present_ckt_number_lcl > 16)
        {
            if(present_ckt_number_lcl == 17)
            {
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,0x01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
                Send_Modbus_Data(modbus_array_glb,0x00,0x02);
                DelayMs(200);
                
            }
            address_lcl = 0x02;                                             // for CKTs from 17 t0 32 select card with address "02" 
            relay_number_lcl =  present_ckt_number_lcl-16;                  // CKT no. 17 to 32  are  point no. 1 to 16  of card 2 respectively hence conversion is done(if ckt no.  is 17 that means point no. 1 of card 2)       
        }
        
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        
        //Present Ckt(initially 1 nad 17 ) is connected to HV+ by making relays OFF related to that points and others are connected to HV- by making relays ON
        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLYS_ON_EXCEPT_PAIR,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
        DelayMs(200); 
//        DelayMs(200); 
      
//        Reset_HighVoltage();
        Set_HighVoltage(withstand_hv_voltage_glb);           //High voltage is applied only after relays are switched. 
      //---------------------------------------------------------------------------------------------------------//
        
        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        Timer2_100ms_Start();
       timer2_counter=0;
        while (timer2_counter <= 30)
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            monitor_voltage_glb = (read_voltage_glb * 1500)/5;
            voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
            monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
            voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
            Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
            Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
            Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
            Display_String_20(vdc,8,3,2);         
        }
      Timer2_100ms_Stop();
        
      //---------------------------------------------------------------------------------------------------------// 
        
        
//        DelayS(3);                      // Delay of min 3 seconds required to settle the High voltage to the selected value.
        Clear_Lcd_Line(2);
        
        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        for(index_lcl=0;index_lcl<30;index_lcl++)  //  30 adc  readings are taken
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            Volts_glb[index_lcl]= read_voltage_glb ;   
            monitor_voltage_glb = Volts_glb[index_lcl] + monitor_voltage_glb ; 
            DelayMs(1);
        }
        monitor_voltage_glb = (monitor_voltage_glb - Volts_glb[0]) /29 ;            // first reading is neglected. Average of 29 readings are taken
       
        monitor_voltage_glb = (monitor_voltage_glb * 1500)/5;                        // since monitor voltage(0V - 5V) is proportional to actual high voltage(0V - 1500V) hence conversion is done to calculated actual voltage observed .
       
        //--------------------To Display Float value on LCD Below Conversion is done-------------------------------------//
        voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
        monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
        voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
        Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
        Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
        Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
        Display_String_20(vdc,8,3,2);                                          // decimal point displayed
        //---------------------------------------------------------------------------------------------------------------//
        
        
        //----------------------------------------------------------------------------------------------------------------------//
        //Time required for withstand test is generated using Timer 2  of time period 100ms     
        timer2_counter = 0;
        sec_counter = 0;

        Timer2_100ms_Start();
        i_lcl =1;
        while(timer2_counter <= (hv_withstand_time_glb *0.01))
        {
    //        sec_counter = 0;
            Clear_Lcd_Line(4);
            Display_String_20(testing,1,11,3); 
            if(hv_withstand_time_glb == 500)
            {
                ;//Display_Number_4digit(hv_withstand_time_glb,18,3,3);
            } 
            else
            {
                Display_Number_4digit(i_lcl,19,2,3);
            }
            DelayMs(1000);
            i_lcl++;
        }
//        Display_Number_4digit(i_lcl,19,2,3);
        Timer2_100ms_Stop();   
 //----------------------------------------------------------------------------------------------------------------------//  
        
        IR_Measure_Voltage_glb = 0 ;
        Fill_Any_float_Ram_With(Volts_glb,50,0.0);
        read_data_lcl = 0;
        read_voltage_glb =0;
        for(index_lcl=0;index_lcl<50;index_lcl++)
        {
               read_data_lcl=read_adc_IR_MEASURE();
               read_voltage_glb = read_data_lcl*0.001225;
               Volts_glb[index_lcl]= read_voltage_glb ;
               IR_Measure_Voltage_glb = Volts_glb[index_lcl] + IR_Measure_Voltage_glb ;
               DelayMs(1);
        }
        IR_Measure_Voltage_glb = IR_Measure_Voltage_glb / 50 ;                  // Average of 50 readings
        leakage_current =  (IR_Measure_Voltage_glb / 5100 ) * 1000000;         // I = V/R (Leakage current = measured voltage across shunt / shunt resistance(5kohm))
        measured_ir_lcl =(monitor_voltage_glb / leakage_current);              //Insulation Resistance = voltage applied / leakage current ;
        Reset_HighVoltage();

        if((leakage_current < withstand_current_glb)&&(measured_ir_lcl > insulation_resistance_lcl))
        {
            if(test_settings_ram[every_ckt_display_result_index] == SET)
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
                Clear_Lcd_Line(3);
                Display_String_20(ckt_pass,1,11,4);                             // Display each ckt pass
                Display_Number_4digit(present_ckt_number_lcl,5,2,4);
            } 
            else
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
            }
            
        }
        else if(((leakage_current > withstand_current_glb) && (measured_ir_lcl < insulation_resistance_lcl)) || (monitor_voltage_glb == 0.0))
        {
            
            store_result_glb[present_ckt_number_lcl] = FAIL ;
            if(test_settings_ram[every_ckt_display_result_index ]== SET)
            {   
                voltage_integer_part = (unsigned int)leakage_current;
                measured_ir_lcl = (leakage_current - voltage_integer_part) * 10000;
                voltage_fractional_part = (unsigned int)measured_ir_lcl;
                Clear_Lcd_Line(3);
                Display_Number_4digit(voltage_integer_part,1,4,3); 
                Display_String_20(dec_point,5,1,3);
                Display_Number_4digit(voltage_fractional_part,6,2,3);
                Display_String_20(ua,8,2,3);
                Display_String_20(ckt_fail,1,11,4);                           // Display each ckt fail
                Display_Number_4digit(present_ckt_number_lcl,5,2,4); 
            }
            if(test_settings_ram[stop_at_first_fault_index]== SET)
            {
                break;
            }
        }

//        DelayMs(200);

    } //end of for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)

//    Reset_HighVoltage();
    
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    // initially All relays of both card 1 & 2 are made ON hence all points are connected to HV-
    Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    
    if(ckt_pass_count_lcl == last_ckt_number_lcl)
    {
        withstand_test_pass_flag_glb = TRUE;
        ir_test_pass_flag_glb = TRUE;
    }
    else
    {
        withstand_test_fail_flag_glb = TRUE;
        ir_test_fail_flag_glb = TRUE;
    }

}







/*
  Function:This function will help in configuring serial number for trester as a production activity.
		   Data will be entered through serial port.
  Input:none.
  Output:none.
  Calls:Receive_Data_Char_Ram(),Dsiplay_Ram(),Display_String_20(),E2prom_Byte_Write().
  Called By:
*/
void Configure_Tester_Sr_No_Eeprom()
{
	unsigned char local_ram[12];
	unsigned char index_local=0;
	unsigned int  address_local=0;
	unsigned char flag_local=0;

	address_local=tester_sr_no_eeprom_address;							//assign address.
	flag_local=Receive_Data_Char_Ram(local_ram,11);					   //receive the data in ram 8 characters allowed.
	if(flag_local == TRUE)											   //store data on valid reception.
	{
        if((local_ram[0] == 0xF9)||(local_ram[1] == 0x3F)||(local_ram[2] == 0x4D)||(local_ram[3] == 0x6C))    // Check if first 4 bytes are FAPL>> F=0xF9 ,A=0x3F ,P=4D ,L=6C) then only assign SRL number
        {
            for(index_local=4;index_local<11;index_local++)						//store and re retrive from eeprom in to ram.
            {
                E2prom_Byte_Write(e2prom_2_page_2_add,((address_local)&mask_higher_byte) >> 8,(address_local)&mask_lower_byte,local_ram[index_local]);
                local_ram[index_local]= 0;
                local_ram[index_local]=E2prom_Byte_Read(e2prom_2_page_2_add,((address_local)& mask_higher_byte)>>8,(address_local)& mask_lower_byte);
                address_local++;
            }//end for(index_local=0;index_local<7;index_local++).
            Display_Ram(local_ram,1,4,7,3);									//display for verification.
            Display_String_20(space,8,4,3);									//space required to shade previous display.
            DelayMs(700);
        }
        else
        {
             Display_String_20(error,1,9,3);
             Display_String_20(space,8,4,3);	
              DelayMs(700);
        }
	}//end if(flag_local == TRUE).

}//end function..





void Harness_Test()
{
   // Store_UUT_Variables_To_Eeprom(current_cable_number);
   // Retrive_Prg_Specific_Settings_and_data(current_cable_number);
    Clear_Lcd_Line(4);
    Check_circuit_number( );
    continuity_test(last_ckt_no_glb);
    if(continuity_test_pass_flag_glb == TRUE)
    {
        continuity_test_pass_flag_glb = FALSE;
        if((prg_specific_setting_ram[withstand_test_index] == SET)&&(prg_specific_setting_ram[ir_test_index] == SET))
        {
            Display_String_20(ir_test,1,7,2);
            Display_String_20(withstand_test,1,14,3);
            DelayS(1);
            Clear_Lcd_Line(2);
            Clear_Lcd_Line(3);
            combine_test_flag_glb = TRUE ;
            if(withstand_hv_voltage_glb == hv_voltage_glb)
            {
                Same_Voltage_Withstand_n_IR_Test(last_ckt_no_glb); 
            }
            else
            {
                Different_Voltage_Withstand_n_IR_Test(last_ckt_no_glb); 
            }
        }
        if((prg_specific_setting_ram[ir_test_index] == SET)&&(combine_test_flag_glb != TRUE))
        {
            Clear_Lcd_Line(2);
            Display_String_20(ir_test,1,7,2);
            DelayS(1);
            
            HV_IR_test(ir_last_ckt_no_glb);
        }// end of if(prg_specific_setting_ram[ir_test_index] == SET)
        if((combine_test_flag_glb != TRUE)&&(((prg_specific_setting_ram[withstand_test_index] == SET)&&(prg_specific_setting_ram[ir_test_index] == RESET)) || ((prg_specific_setting_ram[withstand_test_index] == SET)&&(ir_test_pass_flag_glb == TRUE))))
        {
            Clear_Lcd_Line(2);
            Display_String_20(withstand_test,1,14,2);
            DelayS(1);
//              Clear_Lcd_Line(2);
            HV_Withstand_test(withstand_last_ckt_no_glb); 
        }

        if(((ir_test_pass_flag_glb == TRUE) && (prg_specific_setting_ram[withstand_test_index] == RESET)) || (withstand_test_pass_flag_glb == TRUE))
        {
            Clear_Lcd_Line(2);
            Clear_Lcd_Line(4);
            ir_test_pass_flag_glb = FALSE; 
            withstand_test_pass_flag_glb= FALSE;
            Clear_Lcd_Line(3);
            Initialize_Lcd();
            Display_String_20(pass,1,5,3);	
            // display pass count
            Read_Write_RTC(digit_ram,pass_count_bbram_base_address,3,READ);
            Increment_Numeric_Array (digit_ram,3);
            Display_Ram(digit_ram,7,0,3,3);
            Read_Write_RTC(digit_ram,pass_count_bbram_base_address,3,WRITE);
            Gen_Tone(PASS_TONE,4);
            Relay_On_Operation_1(SET,RESET,SINGLE_RELAY);//29_12_15_2.
            combine_test_flag_glb = FALSE;
            if(gen_settings_ram[glb_prn_lbl_index]== SET)
            {
                Print_Lbl_Data();
            }
            return;
        }
        
        else if((ir_test_fail_flag_glb == TRUE) || (withstand_test_fail_flag_glb == TRUE))
        {
            Clear_Lcd_Line(2);
            Clear_Lcd_Line(4);
            ir_test_fail_flag_glb = FALSE;
            withstand_test_fail_flag_glb = FALSE;
             // display FAIL count
            Clear_Lcd_Line(3);
            Initialize_Lcd();
            Display_String_20(fail,1,5,3);	
            Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,READ);
            Increment_Numeric_Array (digit_ram,3);
            Display_Ram(digit_ram,7,0,3,3);
            Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,WRITE);
            Gen_Tone(5,4);   // Fail tone
//            Relay_On_Operation_1(SET,RESET,SINGLE_RELAY);//29_12_15_2.
            Relay_On_Operation_1(RESET,SET,SINGLE_RELAY);//29_12_15_2.
            combine_test_flag_glb = FALSE;
            if(test_settings_ram[fail_bin_index]== SET)
            {
                WriteRtc(fail_bin_bbram_base_address,SET);
                fail_bin_signal();
            }
            return;
        }
//                                                                                Display_String_20(press_ent_start,1,18,4);
//                                                                          state_code = ST_IDLE_TEST;
    }// end of if(continuity_test_pass_flag_glb == TRUE)
    else if(continuity_test_fail_flag_glb == TRUE)
    {
        Clear_Lcd_Line(2);
        continuity_test_fail_flag_glb = FALSE;
         Clear_Lcd_Line(3);
         Initialize_Lcd();
        Display_String_20(fail,1,5,3);	
        // display pass count
        Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,READ);
        Increment_Numeric_Array (digit_ram,3);
        Display_Ram(digit_ram,7,0,3,3);
        Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,WRITE);
        Gen_Tone(5,4);      //Fail Tone
        combine_test_flag_glb = FALSE;
        return;
    }
}


/*
Function:
 void fail_bin_signal()
	
Description:
 This function will used to for fail bin input signal check. 
  
Parameters:
 *          Nil

Return Values: none

Remarks: Nil
*/
void fail_bin_signal()
{
    while(key_data_glb != 0xEF)
    {
        key_data_glb = IC8574_ByteRead();
        Clear_Display();
        Display_String_20(wait_for_fail_bin,2,19,2);
        Display_String_20(input_signal,4,14,3);
        DelayMs(1500);
    }
    WriteRtc(fail_bin_bbram_base_address,RESET);
}





/*
Function:
 void HV_IR_Value(unsigned char last_ckt_number_lcl)
	
Description:
 This function will used to shows the Voltage, Current and Insulation Resistance Value. 
  
Parameters:
 *          last_ckt_number_lcl 

Return Values: none

Remarks: Nil
*/
void HV_IR_Value(unsigned char last_ckt_number_lcl)
{

    unsigned char address_lcl,present_ckt_number_lcl,relay_number_lcl=0,index_lcl,ckt_pass_count_lcl=0;
    unsigned int read_data_lcl;
    unsigned int measured_ir_integer_part,measured_ir_lcl,measured_ir_fractional_part;
    unsigned int current_integer_part,monitor_current_lcl,current_fractional_part ;
    float  measured_ir , leakage_current,monitor_voltage_lcl ,insulation_resistance_lcl=0;
    float voltage_lcl[10]; 
    insulation_resistance_lcl = (float)insulation_resistance_glb ;
    DelayMs(1000);
    
 // Switch_Interface_Relays_To_HV();                    // Switch relays on interface cards to HV side
    DelayMs(50);    

    // Initially all relays of both cards 1 & 2 are made ON hence all points are connected to HV-
    
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0x01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0x02,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 2 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Switch_Interface_Relays_To_HV();                    // Switch relays on interface cards to HV side
    for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
    {
        if(present_ckt_number_lcl <= 16)                                     // if ckt number is from 1 to 16 then select address 01 i.e for card 1
        {
            address_lcl = 0x01;                                             // for CKTs from 1 t0 16 select card with address "01" 
            relay_number_lcl =  present_ckt_number_lcl;
        }
        if(present_ckt_number_lcl > 16)
        {
            if(present_ckt_number_lcl == 17)
            {
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,0x01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
                Send_Modbus_Data(modbus_array_glb,0x00,0x02);
                DelayMs(200);
                
            }
            address_lcl = 0x02;                                             // for CKTs from 17 t0 32 select card with address "02" 
            relay_number_lcl =  present_ckt_number_lcl-16;                  // CKT no. 17 to 32  are  point no. 1 to 16  of card 2 respectively hence conversion is done(if ckt no.  is 17 that means point no. 1 of card 2)       
        }
        
        //Present Ckt(initially 1 and 17 ) is connected to HV+ by making relays OFF related to that points and others are connected to HV- by making relays ON
        Clear_Display();
        Display_String_20(display,1,7,1); 
        Display_String_20(circuit,9,7,1); 
        Display_String_20(no,17,3,1); 
        Display_Number_4digit(present_ckt_number_lcl,1,3,2); 
        Display_String_20(values,5,6,2); 
//        const static unsigned char display[]=		    {"DISPLAY"};
//const static unsigned char circuit[]=		    {"CIRCUIT"};
//const static unsigned char no[]=		    {"NO."};
//const static unsigned char values[]=		    {"VALUES"};
        
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLYS_ON_EXCEPT_PAIR,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
        DelayMs(200); 
        DelayMs(200); 
        //Clear_Lcd_Line(2);
     //   Display_String_20(testing,1,11,3);
        Set_HighVoltage(hv_voltage_glb);  //High voltage is applied only after relays are switched. 
        //------------------------------------------------------------------------------------------------------------------------//
        
        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        Timer2_100ms_Start();
       timer2_counter=0;
        while (timer2_counter <= 30)
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            monitor_voltage_glb = (read_voltage_glb * 1500)/5;
            voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
            monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
            voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
//            Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
//            Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
//            Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
//            Display_String_20(vdc,8,3,2);         
        }
      Timer2_100ms_Stop();
        
        //------------------------------------------------------------------------------------------------------------------------//
//        DelayS(3);                        // Delay of minimum 3 seconds required to settle the High voltage to the selected value

        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        for(index_lcl=0;index_lcl < 30;index_lcl++)       //  30 adc  readings are taken
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            Volts_glb[index_lcl]= read_voltage_glb ;   
            monitor_voltage_glb = Volts_glb[index_lcl] + monitor_voltage_glb ; 
            DelayMs(1);
        }
        monitor_voltage_glb = (monitor_voltage_glb - Volts_glb[0]) / 29 ;            // first reading is neglected. Average of 29 readings are taken
       
        monitor_voltage_glb = (monitor_voltage_glb * 1500)/5;                        // since monitor voltage(0V - 5V) is proportional to actual high voltage(0V - 1500V) hence conversion is done to calculated actual voltage observed .
       
        //--------------------To Display Float value on LCD Below Conversion is done-------------------------------------//
        voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
        monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
        voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
        //Clear_Lcd_Line(2);
//        Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
//        Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
//        Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
//        Display_String_20(vdc,8,3,2);                                                // string VDC displayed 
//        //---------------------------------------------------------------------------------------------------------------//
//        Display_String_20(testing,1,11,3); 
        
        IR_Measure_Voltage_glb = 0 ;
        Fill_Any_float_Ram_With(Volts_glb,50,0.0);
        read_data_lcl = 0;
        read_voltage_glb =0;
        for(index_lcl=0;index_lcl<50;index_lcl++)
        {
               read_data_lcl=read_adc_IR_MEASURE();
               read_voltage_glb = read_data_lcl*0.001225;
               Volts_glb[index_lcl]= read_voltage_glb ;
               IR_Measure_Voltage_glb = Volts_glb[index_lcl] + IR_Measure_Voltage_glb ;
               DelayMs(1);
        }
        IR_Measure_Voltage_glb = IR_Measure_Voltage_glb / 50 ;                  // Average of 50 readings
        leakage_current =  (IR_Measure_Voltage_glb / 5100 ) * 1000000;         // I = V/R (Leakage current = measured voltage across shunt / shunt resistance(5kohm))

        measured_ir =(monitor_voltage_glb / leakage_current);              //Insulation Resistance = voltage applied / leakage current ;

        current_integer_part = (unsigned int)leakage_current;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
        monitor_current_lcl = (leakage_current - current_integer_part) * 10000 ; // fractional part is separated out 
        current_fractional_part = (unsigned int)monitor_current_lcl; 

        measured_ir_integer_part = (unsigned int)measured_ir;
        measured_ir_lcl = (measured_ir - measured_ir_integer_part) * 10000;
        measured_ir_fractional_part = (unsigned int)measured_ir_lcl;
        
//        Display_String_20(current,12,7,1); 
//        Display_Number_4digit(current_integer_part,12,3,2);                           // decimal part displayed
//        Display_String_20(dec_point,15,1,2);                                          // decimal point displayed
//        Display_Number_4digit(current_fractional_part,16,2,2);                        // fractional part displayed
//        Display_String_20(ua,19,2,2);

        Reset_HighVoltage();
//        Clear_Display();
 //Clear_Lcd_Line(2);
//        Voltage
//        Display_String_20(voltage,1,7,1);
//        Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
//        Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
//        Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
//        Display_String_20(vdc,8,3,2);                                                // string VDC disp
//        /////////////OR//////////////////
        Display_String_20(voltage,1,7,3);
        Display_Number_4digit(voltage_integer_part,1,4,4);                           // decimal part displayed
        Display_String_20(dec_point,5,1,4);                                          // decimal point displayed
        Display_Number_4digit(voltage_fractional_part,6,2,4);                        // fractional part displayed
        Display_String_20(vdc,8,3,4);                                                // string VDC disp
        DelayMs(3000);
        Clear_Lcd_Line(3);
        Clear_Lcd_Line(4);
//        Current
//        Display_String_20(current,12,7,1); 
//        Display_Number_4digit(current_integer_part,12,3,2);                           // decimal part displayed
//        Display_String_20(dec_point,15,1,2);                                          // decimal point displayed
//        Display_Number_4digit(current_fractional_part,16,2,2);                        // fractional part displayed
//        Display_String_20(ua,19,2,2);
//        /////////////OR//////////////////
        Display_String_20(current,1,7,3); 
        Display_Number_4digit(current_integer_part,1,3,4);                           // decimal part displayed
        Display_String_20(dec_point,4,1,4);                                          // decimal point displayed
        Display_Number_4digit(current_fractional_part,5,2,4);                        // fractional part displayed
        Display_String_20(ua,8,2,4);
        DelayMs(3000);
        Clear_Lcd_Line(3);
        Clear_Lcd_Line(4);
//        Insulation Resistance
        Display_String_20(ir_value,1,2,3); 
        Display_Number_4digit(measured_ir_integer_part,1,4,4); 
        Display_String_20(dec_point,5,1,4);
        Display_Number_4digit(measured_ir_fractional_part,6,2,4);
        Display_String_20(mohm,8,4,4);
        DelayMs(3000);

        
//        if(measured_ir_lcl > insulation_resistance_lcl)
//        {
//            if(test_settings_ram[every_ckt_display_result_index] == SET)
//            {
//                ckt_pass_count_lcl++;
//                store_result_glb[present_ckt_number_lcl] = PASS ;
//                Clear_Lcd_Line(3);
//                Display_String_20(ckt_ir_pass,1,18,4);                             // Display each ckt pass
//                Display_Number_4digit(present_ckt_number_lcl,5,2,4);
//            } 
//            else
//            {
//                ckt_pass_count_lcl++;
//                store_result_glb[present_ckt_number_lcl] = PASS ;
//            }
//            
//        }
//        else if(measured_ir_lcl < insulation_resistance_lcl)
//        {
//            
//            store_result_glb[present_ckt_number_lcl] = FAIL ;
//            if(test_settings_ram[every_ckt_display_result_index ]== SET)
////            {   
//                voltage_integer_part = (unsigned int)measured_ir_lcl;
//                measured_ir_lcl = (measured_ir_lcl - voltage_integer_part) * 10000;
//                voltage_fractional_part = (unsigned int)measured_ir_lcl;
//                Clear_Lcd_Line(4);
//                Display_Number_4digit(voltage_integer_part,1,4,4); 
//                Display_String_20(dec_point,5,1,4);
//                Display_Number_4digit(voltage_fractional_part,6,2,4);
//                Display_String_20(mohm,8,4,4);
//                DelayMs(1000);
////                Display_String_20(ckt_ir_fail,1,18,4);                           // Display each ckt fail
//                Display_Number_4digit(present_ckt_number_lcl,5,2,4); 
//            }
//            if(test_settings_ram[stop_at_first_fault_index]== SET)
//            {
//                ir_test_fail_flag_glb = TRUE ;
//                break;
//            }
//        }// end of else if(measured_ir_lcl < insulation_resistance_lcl)
        DelayMs(1000);
    }// end of for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
     //Clear_Lcd_Line();
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    // initially All relays of both card 1 & 2 are made ON hence all points are connected to HV-
    Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
}



/*
Function:
void Calibration( )
	
Description:
 This function will be used for to calculate error between resistance enter value and measure value should be calculate.
  
Parameters:
 *          none 

Return Values: none

Remarks: Nil
*/
void Calibration( )
{
    float calculated_slope_value_m, calculated_offset_value_c;
    unsigned int calculated_slope_value_m_integer, calculated_slope_value_m_lcl, calculated_slope_value_m_fractional ;
    unsigned int calculated_offset_value_c_integer, calculated_offset_value_c_lcl, calculated_offset_value_c_fractional ;

//    calculated_slope_value_m = 1.32 ;//((100 - 6.95)/(98 - 3.5));//calculated_slope_value(m) 
    calculated_slope_value_m = ((input_resistance_value_y2 - input_resistance_value_y1)/(measured_resistance_x2 - measured_resistance_x1));//calculated_slope_value(m) 

    calculated_slope_value_m_integer = (unsigned int)calculated_slope_value_m;                    // (calculated_slope_value_m) contains Float value so integer value is separated out and stored in int variable (calculated_slope_value_m_integer)
    calculated_slope_value_m_lcl = (calculated_slope_value_m - calculated_slope_value_m_integer) * 100 ; // fractional part is separated out 
    calculated_slope_value_m_fractional = (unsigned int)calculated_slope_value_m_lcl;      
    
    E2prom_Byte_Write(e2prom_2_page_1_add,((calculated_slope_value_m_base_address+(1*2-2))&mask_higher_byte) >> 8,(calculated_slope_value_m_base_address+(1*2-2))&mask_lower_byte,calculated_slope_value_m_integer);
    E2prom_Byte_Write(e2prom_2_page_1_add,((calculated_slope_value_m_base_address+(1*2-1))&mask_higher_byte) >> 8,(calculated_slope_value_m_base_address+(1*2-1))&mask_lower_byte,calculated_slope_value_m_fractional);
    
    hv_msb_glb = E2prom_Byte_Read(e2prom_2_page_1_add,((calculated_slope_value_m_base_address+(1*2-2))&mask_higher_byte) >> 8,(calculated_slope_value_m_base_address+(1*2-2))&mask_lower_byte);
    hv_lsb_glb = E2prom_Byte_Read(e2prom_2_page_1_add,((calculated_slope_value_m_base_address+(1*2-1))&mask_higher_byte) >> 8,(calculated_slope_value_m_base_address+(1*2-1))&mask_lower_byte);

//    calculated_offset_value_c = (10.95 - (calculated_slope_value_m * 8.5));  // calculated_offset_value_c
    calculated_offset_value_c = (input_resistance_value_y1 - (calculated_slope_value_m * measured_resistance_x1));  // calculated_offset_value_c

    calculated_offset_value_c_integer = (unsigned int)calculated_offset_value_c;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
    calculated_offset_value_c_lcl = (calculated_offset_value_c - calculated_offset_value_c_integer) * 100 ; // fractional part is separated out 
    calculated_offset_value_c_fractional = (unsigned int)calculated_offset_value_c_lcl;      

    E2prom_Byte_Write(e2prom_2_page_1_add,((calculated_offset_value_c_base_address+(1*2-2))&mask_higher_byte) >> 8,(calculated_offset_value_c_base_address+(1*2-2))&mask_lower_byte,calculated_offset_value_c_integer);
    E2prom_Byte_Write(e2prom_2_page_1_add,((calculated_offset_value_c_base_address+(1*2-1))&mask_higher_byte) >> 8,(calculated_offset_value_c_base_address+(1*2-1))&mask_lower_byte,calculated_offset_value_c_fractional);
        
    hv_msb_glb = E2prom_Byte_Read(e2prom_2_page_1_add,((calculated_offset_value_c_base_address+(1*2-2))&mask_higher_byte) >> 8,(calculated_offset_value_c_base_address+(1*2-2))&mask_lower_byte);
    hv_lsb_glb = E2prom_Byte_Read(e2prom_2_page_1_add,((calculated_offset_value_c_base_address+(1*2-1))&mask_higher_byte) >> 8,(calculated_offset_value_c_base_address+(1*2-1))&mask_lower_byte);
       DelayMs(200); 
    
/* unsigned int calculated_slope_value_m, calculated_offset_value_c;

     calculated_slope_value_m = ((input_resistance_value_y2 - input_resistance_value_y1)/(measured_resistance_x2 - measured_resistance_x1));//calculated_slope_value(m) 
    
    hv_msb_glb = ((0xFF00 & calculated_slope_value_m) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
    hv_lsb_glb = (0x00FF & calculated_slope_value_m);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

    E2prom_Byte_Write(e2prom_2_page_1_add,((calculated_slope_value_m_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(calculated_slope_value_m_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
    E2prom_Byte_Write(e2prom_2_page_1_add,((calculated_slope_value_m_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(calculated_slope_value_m_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);
    
    hv_msb_glb = E2prom_Byte_Read(e2prom_2_page_1_add,((calculated_slope_value_m_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(calculated_slope_value_m_base_address+(current_cable_number*2-2))&mask_lower_byte);
    hv_lsb_glb = E2prom_Byte_Read(e2prom_2_page_1_add,((calculated_slope_value_m_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(calculated_slope_value_m_base_address+(current_cable_number*2-2))&mask_lower_byte);

    calculated_offset_value_c = (input_resistance_value_y1 - (calculated_slope_value_m * measured_resistance_x1));  // calculated_offset_value_c

    hv_msb_glb = ((0xFF00 & calculated_offset_value_c) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
    hv_lsb_glb = (0x00FF & calculated_offset_value_c);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

    E2prom_Byte_Write(e2prom_2_page_1_add,((calculated_offset_value_c_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(calculated_offset_value_c_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
    E2prom_Byte_Write(e2prom_2_page_1_add,((calculated_offset_value_c_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(calculated_offset_value_c_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);
    
    
    hv_msb_glb = E2prom_Byte_Read(e2prom_2_page_1_add,((calculated_offset_value_c_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(calculated_offset_value_c_base_address+(current_cable_number*2-2))&mask_lower_byte);
    hv_lsb_glb = E2prom_Byte_Read(e2prom_2_page_1_add,((calculated_offset_value_c_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(calculated_offset_value_c_base_address+(current_cable_number*2-1))&mask_lower_byte);
    
*/
    
}


/*
Function:
void HV_IR_test_1(unsigned char last_ckt_number_lcl)
	
Description:
 This function will be used for to measure the resistance value for calibrate function.
  
Parameters:
 *          (last_ckt_number_lcl)

Return Values: none

Remarks: Nil
*/


void HV_IR_test_1(unsigned char last_ckt_number_lcl)
{
    unsigned char address_lcl,present_ckt_number_lcl,relay_number_lcl=0,index_lcl,ckt_pass_count_lcl=0;
    unsigned int read_data_lcl;
    float  measured_ir_lcl , leakage_current,monitor_voltage_lcl ,insulation_resistance_lcl=0;
    float voltage_lcl[10]; 
    insulation_resistance_lcl = (float)insulation_resistance_glb ;
    DelayMs(1000);
    
//    Switch_Interface_Relays_To_HV();                    // Switch relays on interface cards to HV side
    DelayMs(50);    

    // Initially all relays of both cards 1 & 2 are made ON hence all points are connected to HV-
    
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0x01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0x02,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 2 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Switch_Interface_Relays_To_HV();                    // Switch relays on interface cards to HV side
    for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
    {
        if(present_ckt_number_lcl <= 16)                                     // if ckt number is from 1 to 16 then select address 01 i.e for card 1
        {
            address_lcl = 0x01;                                             // for CKTs from 1 t0 16 select card with address "01" 
            relay_number_lcl =  present_ckt_number_lcl;
        }
        if(present_ckt_number_lcl > 16)
        {
            if(present_ckt_number_lcl == 17)
            {
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,0x01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
                Send_Modbus_Data(modbus_array_glb,0x00,0x02);
                DelayMs(200);
                
            }
            address_lcl = 0x02;                                             // for CKTs from 17 t0 32 select card with address "02" 
            relay_number_lcl =  present_ckt_number_lcl-16;                  // CKT no. 17 to 32  are  point no. 1 to 16  of card 2 respectively hence conversion is done(if ckt no.  is 17 that means point no. 1 of card 2)       
        }
        
        //Present Ckt(initially 1 and 17 ) is connected to HV+ by making relays OFF related to that points and others are connected to HV- by making relays ON
        
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLYS_ON_EXCEPT_PAIR,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
        DelayMs(200); 
        DelayMs(200); 
        Clear_Lcd_Line(2);
//        Display_String_20(testing,1,11,3);
        Set_HighVoltage(hv_voltage_glb);  //High voltage is applied only after relays are switched. 
        //------------------------------------------------------------------------------------------------------------------------//
        
        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        Timer2_100ms_Start();
       timer2_counter=0;
        while (timer2_counter <= 30)
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            monitor_voltage_glb = (read_voltage_glb * 1500)/5;
            voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
            monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
            voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
//            Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
//            Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
//            Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
//            Display_String_20(vdc,8,3,2);         
        }
      Timer2_100ms_Stop();
        
        //------------------------------------------------------------------------------------------------------------------------//
//        DelayS(3);                        // Delay of minimum 3 seconds required to settle the High voltage to the selected value

        monitor_voltage_glb = 0;
        Fill_Any_float_Ram_With(Volts_glb,30,0.0);
        for(index_lcl=0;index_lcl < 30;index_lcl++)       //  30 adc  readings are taken
        {
            read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
            read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
            Volts_glb[index_lcl]= read_voltage_glb ;   
            monitor_voltage_glb = Volts_glb[index_lcl] + monitor_voltage_glb ; 
            DelayMs(1);
        }
        monitor_voltage_glb = (monitor_voltage_glb - Volts_glb[0]) / 29 ;            // first reading is neglected. Average of 29 readings are taken
       
        monitor_voltage_glb = (monitor_voltage_glb * 1500)/5;                        // since monitor voltage(0V - 5V) is proportional to actual high voltage(0V - 1500V) hence conversion is done to calculated actual voltage observed .
       
        //--------------------To Display Float value on LCD Below Conversion is done-------------------------------------//
        voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
        monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
        voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
//        Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
//        Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
//        Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
//        Display_String_20(vdc,8,3,2);                                                // string VDC displayed 
//        ---------------------------------------------------------------------------------------------------------------//
//        Display_String_20(testing,1,11,3); 
        
        IR_Measure_Voltage_glb = 0 ;
        Fill_Any_float_Ram_With(Volts_glb,50,0.0);
        read_data_lcl = 0;
        read_voltage_glb =0;
        for(index_lcl=0;index_lcl<50;index_lcl++)
        {
               read_data_lcl=read_adc_IR_MEASURE();
               read_voltage_glb = read_data_lcl*0.001225;
               Volts_glb[index_lcl]= read_voltage_glb ;
               IR_Measure_Voltage_glb = Volts_glb[index_lcl] + IR_Measure_Voltage_glb ;
               DelayMs(1);
        }
        IR_Measure_Voltage_glb = IR_Measure_Voltage_glb / 50 ;                  // Average of 50 readings
        leakage_current =  (IR_Measure_Voltage_glb / 5100 ) * 1000000;         // I = V/R (Leakage current = measured voltage across shunt / shunt resistance(5kohm))
        measured_ir_lcl =(monitor_voltage_glb / leakage_current);              //Insulation Resistance = voltage applied / leakage current ;
        Reset_HighVoltage();

         if( calibrate_flag_glb == FALSE )
        {
            measured_resistance_x1 = measured_ir_lcl ;
        
            hv_msb_glb = ((0xFF00 & measured_resistance_x1) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
            hv_lsb_glb = (0x00FF & measured_resistance_x1);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

            E2prom_Byte_Write(e2prom_2_page_1_add,((measured_resistance_x1_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(measured_resistance_x1_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
            E2prom_Byte_Write(e2prom_2_page_1_add,((measured_resistance_x1_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(measured_resistance_x1_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);
        }
        if( calibrate_flag_glb == TRUE )
        {
            measured_resistance_x2 = measured_ir_lcl ;
        
            hv_msb_glb = ((0xFF00 & measured_resistance_x2) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
            hv_lsb_glb = (0x00FF & measured_resistance_x2);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

            E2prom_Byte_Write(e2prom_2_page_1_add,((measured_resistance_x2_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(measured_resistance_x2_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
            E2prom_Byte_Write(e2prom_2_page_1_add,((measured_resistance_x2_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(measured_resistance_x2_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);
        }

    /*    if(measured_ir_lcl > insulation_resistance_lcl)
        {
            if(test_settings_ram[every_ckt_display_result_index] == SET)
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
                Clear_Lcd_Line(3);
                Display_String_20(ckt_ir_pass,1,18,4);                             // Display each ckt pass
                Display_Number_4digit(present_ckt_number_lcl,5,2,4);
            } 
            else
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
            }
            
        }
        else if(measured_ir_lcl < insulation_resistance_lcl)
        {
            
            store_result_glb[present_ckt_number_lcl] = FAIL ;
            if(test_settings_ram[every_ckt_display_result_index ]== SET)
            {   
                voltage_integer_part = (unsigned int)measured_ir_lcl;
                measured_ir_lcl = (measured_ir_lcl - voltage_integer_part) * 10000;
                voltage_fractional_part = (unsigned int)measured_ir_lcl;
                Clear_Lcd_Line(3);
                Display_Number_4digit(voltage_integer_part,1,4,3); 
                Display_String_20(dec_point,5,1,3);
                Display_Number_4digit(voltage_fractional_part,6,2,3);
                Display_String_20(mohm,8,4,3);
                Display_String_20(ckt_ir_fail,1,18,4);                           // Display each ckt fail
                Display_Number_4digit(present_ckt_number_lcl,5,2,4); 
            }
            if(test_settings_ram[stop_at_first_fault_index]== SET)
            {
                ir_test_fail_flag_glb = TRUE ;
                break;
            }
        }// end of else if(measured_ir_lcl < insulation_resistance_lcl)*/
        DelayMs(1000);
    }// end of for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
     Clear_Lcd_Line(4);
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    // initially All relays of both card 1 & 2 are made ON hence all points are connected to HV-
    Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
   
  /*  if(ckt_pass_count_lcl == last_ckt_number_lcl)
    {
        ir_test_pass_flag_glb = TRUE;
        Clear_Lcd_Line(2);
        Clear_Lcd_Line(3);
        Display_String_20(ir_pass,1,12,3);
        DelayMs(500); 
        Clear_Lcd_Line(3);
    }
    else
    {
        Clear_Lcd_Line(2);
        Clear_Lcd_Line(3);
        ir_test_fail_flag_glb = TRUE;
        Display_String_20(ir_fail,1,12,3);
        DelayMs(500); 
        Clear_Lcd_Line(3);
    }*/
}




/*
Function:
void Self_IO_Test(void)
	
Description:
 This function will be used for to check IO points are shorted or open.
  
Parameters:
none

Return Values: none

Remarks: Nil
*/


void Self_IO_Test(void)//----------------3
{
   unsigned char address_lcl,present_ckt_number_lcl,present_ckt_number_io,relay_number_lcl=0,ckt_lcl;
   unsigned int total_circuit_lcl ;
//    unsigned char address_lcl,present_ckt_number_lcl,relay_number_lcl=0,ckt_lcl;
  
    DelayMs(500);                                                          // Wait for 100ms
//    Switch_Interface_Relays_To_LV();                                                 // Switch relays on interface cards to LV side

        //send commands to make all relays OFF both card 1 and card 2;
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x02);
        DelayMs(200); 
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x02);
        DelayMs(200);
        address_lcl = 0x01;
        total_circuit_lcl = 32 ;//cards*32;
        Switch_Interface_Relays_To_LV(); 
        while(address_lcl <= cards )//0x03)
        {
            for(present_ckt_number_io=1;present_ckt_number_io < total_circuit_lcl ; present_ckt_number_io++)
            {
                for(present_ckt_number_lcl=present_ckt_number_io;present_ckt_number_lcl < total_circuit_lcl ; present_ckt_number_lcl++)
                {

    //                address_lcl = 0x01;                                             // for CKTs from 1 t0 16 select card with address "01" 
                    relay_number_lcl =  present_ckt_number_lcl ;
                    if(key_pressed_flag==TRUE)// For Escape function
                    {
                        key_pressed_flag=FALSE;
                        key_data_glb = IC8574_ByteRead();
                        key_code =(key_data_glb & 0X0F);
                        if((key_code==ESC_KEY))
                        {
                            break;
                        }
                    }
                    /*1. Make 1st relay of present ckt ON & read Port pin */
                    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                    Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_SINGLE_RLY_OFF_SINGLE_RLY_ON,relay_number_lcl+1,present_ckt_number_io,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                    Send_Modbus_Data(modbus_array_glb,0x00,0x04);
                    DelayMs(200); 
                    if(DIG_READ_rg6_pin == HIGH )        // if '1' means not open , do nothing 
                    {
                        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLY_PAIR_OFF,relay_number_lcl+1,present_ckt_number_io,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
                        DelayMs(200); 
                        self_test_flag = TRUE ;
                    }
                    else if(DIG_READ_rg6_pin == LOW)    //if '0' means open
                    {
                       ;// DelayMs(400);       
                    }
                    if(key_pressed_flag==TRUE)// For Escape function
                    {
                        key_pressed_flag=FALSE;
                        key_data_glb = IC8574_ByteRead();
                        key_code =(key_data_glb & 0X0F);
                        if((key_code==ESC_KEY))
                        {
                            break;
                        }
                    }
                    if(self_test_flag == TRUE)
                    {
                        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_SINGLE_RLY_OFF_SINGLE_RLY_ON,present_ckt_number_io,relay_number_lcl+1,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
                        DelayMs(200); 
                        if(DIG_READ_rg6_pin == HIGH )        // if '1' means not open , do nothing 
                        {
                            self_test_flag = FALSE ;
                            break;
                        }
                        else if(DIG_READ_rg6_pin == LOW)    //if '0' means open
                        {
                            ;//DelayMs(400); 
                        }
                    }
                } 
                if(self_test_flag == FALSE && DIG_READ_rg6_pin == HIGH)            
                {
                    break;
                } 
            } 
            if(self_test_flag == FALSE && DIG_READ_rg6_pin == HIGH)            
            {
                break;
            }
            Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
            Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
            Send_Modbus_Data(modbus_array_glb,0x00,0x02);
            DelayMs(200);
            address_lcl++;    
        }
        if(self_test_flag == FALSE && DIG_READ_rg6_pin == HIGH) 
        {
            Clear_Display();
            Display_String_20(self_test_fail,1,14,2);
            DelayMs(2000); 
            Clear_Display();
            present_ckt_number_lcl = present_ckt_number_lcl +1;
            Display_String_20(please_check_points,1,19,1);
            Display_String_20(card_no,1,9,2);
            Display_Number_4digit(address_lcl,10,2,2);
            Display_String_20(short_ckt,1,10,3);
            Display_Number_4digit(present_ckt_number_io,11,2,3);  
            Display_String_20(and,14,3,3);
            Display_Number_4digit(present_ckt_number_lcl,18,2,3);  
            DelayMs(2000); 
        }
        else
        {
            Display_String_20(self_test_pass,1,14,2);
            DelayMs(1000); 
        }
}    



/*
Function:
void Check_Board()
	
Description:
 This function will be used with help of probe to IO Points numbers are same or not
  
Parameters:
none

Return Values: none

Remarks: Nil
*/


void Check_Board()
{
    unsigned char address_lcl,present_ckt_number_lcl,relay_number_lcl=0,ckt_lcl;

	key_pressed_flag=FALSE;											 //to escape from board check menu clear flag and register.
	key_code =0x00;
	if(cards == 0)													 //display and return if cards do sent found.
	{
        Display_String_20(nocards,1,12,1);
        DelayMs(1000);
        return;
	}//end if(cards == 0)
    Display_String_20(board_diagno,1,19,1);
	Display_String_20(connect_probe,1,15,2);

    board_check_ra9_pin = 1;                                                //Enable soft board check pin.     
    DelayMs(10);
    Switch_Interface_Relays_To_LV(); 
	while(key_code != ESC_KEY)
	{
        address_lcl = 0x01 ;
        while(address_lcl <= cards)
        {
            for(present_ckt_number_lcl=1;present_ckt_number_lcl <= 32 ; present_ckt_number_lcl++)
            {
                if(key_pressed_flag==TRUE)
                {
                    key_pressed_flag=FALSE;
                    key_data_glb = IC8574_ByteRead();
                    key_code =(key_data_glb & 0X0F);
                    if((key_code==ESC_KEY))
                    {
                        break;
                    }
                }
                if(key_pressed_flag==TRUE)
                {
                    key_pressed_flag=FALSE;
                    key_data_glb = IC8574_ByteRead();
                    key_code =(key_data_glb & 0X0F);
                    if((key_code==ESC_KEY))
                    {
                        break;
                    }
                }
                
                relay_number_lcl = present_ckt_number_lcl ;
                
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_SINGLE_RLY_ON,relay_number_lcl,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                Send_Modbus_Data(modbus_array_glb,0x00,0x03);
                DelayMs(400);//DelayMs(500);                            // 200ms delay required after sending command    
                if(DIG_READ_rg6_pin == HIGH )        
                {
                    Clear_Lcd_Line(3);
                    Clear_Lcd_Line(4);
                    Display_String_20(card_no,1,9,3);                             // Display each ckt pass
                    Display_Number_4digit(address_lcl,11,2,3);
                    Display_String_20(io_no,1,7,4);                             // Display each ckt pass
                    Display_Number_4digit(relay_number_lcl,9,2,4);
                    DelayMs(600);
                    Display_Number_4digit(00,11,2,3);
                    Display_Number_4digit(00,9,2,4);
                }
                if(key_pressed_flag==TRUE)
                {
                    key_pressed_flag=FALSE;
                    key_data_glb = IC8574_ByteRead();
                    key_code =(key_data_glb & 0X0F);
                    if((key_code==ESC_KEY))
                    {
                        break;
                    }
                }
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_SINGLE_RLY_OFF,relay_number_lcl,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
                Send_Modbus_Data(modbus_array_glb,0x00,0x03);
                DelayMs(400);//DelayMs(500); 
                if(key_pressed_flag==TRUE)
                {
                    key_pressed_flag=FALSE;
                    key_data_glb = IC8574_ByteRead();
                    key_code =(key_data_glb & 0X0F);
                    if((key_code==ESC_KEY))
                    {
                        break;
                    }
                }
            }//end circuit no's for loop   
            address_lcl++ ;
        }//end cards while loop
        
    }//end ESC while loop
    board_check_ra9_pin = 0;                                                  //disable soft board check pin.     
    DelayMs(1000);
}//end function.











/*
Function:
void Different_Voltage_Withstand_n_IR_Test(unsigned char last_ckt_number_lcl)
	
Description:
 This function will carry out if IR and Withstand setting is ON & Set Different Voltages.
  
Parameters:
 *          last_ckt_number_lcl 

Return Values: none

Remarks: Nil
*/
void Different_Voltage_Withstand_n_IR_Test(unsigned char last_ckt_number_lcl)
{
    unsigned char address_lcl,present_ckt_number_lcl,relay_number_lcl=0,index_lcl,ckt_pass_count_lcl=0;
    unsigned int read_data_lcl,i_lcl;
    float  measured_ir_lcl , leakage_current, leakage_current_lcl ,monitor_voltage_lcl ,insulation_resistance_lcl=0;
    float voltage_lcl[10]; 
    insulation_resistance_lcl = (float)insulation_resistance_glb ;
//    DelayMs(1000);
    
    Switch_Interface_Relays_To_HV();                    // Switch relays on interface cards to HV side
//    DelayMs(50);    
    // Initially all relays of both cards 1 & 2 are made ON hence all points are connected to HV-
    
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 2 ON 
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    
    for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)
    {
        if(present_ckt_number_lcl <= 16)                                     // if ckt number is from 1 to 16 then select address 01 i.e for card 1
        {
            address_lcl = 0x01;                                             // for CKTs from 1 t0 16 select card with address "01" 
            relay_number_lcl =  present_ckt_number_lcl;
        }
        if(present_ckt_number_lcl > 16)
        {
            if(present_ckt_number_lcl == 17)
            {
                Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
                Fill_Small_Data_Ram(modbus_array_glb,0x01,MCMD_ALL_RLY_ON,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL); /// all relays of card 1 ON 
                Send_Modbus_Data(modbus_array_glb,0x00,0x02);
                DelayMs(200);
                
            }
            address_lcl = 0x02;                                             // for CKTs from 17 t0 32 select card with address "02" 
            relay_number_lcl =  present_ckt_number_lcl-16;                  // CKT no. 17 to 32  are  point no. 1 to 16  of card 2 respectively hence conversion is done(if ckt no.  is 17 that means point no. 1 of card 2)       
        }
        
        Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
        
        //Present Ckt(initially 1 nad 17 ) is connected to HV+ by making relays OFF related to that points and others are connected to HV- by making relays ON
        Fill_Small_Data_Ram(modbus_array_glb,address_lcl,MCMD_MAKE_RLYS_ON_EXCEPT_PAIR,relay_number_lcl,relay_number_lcl+16,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
        Send_Modbus_Data(modbus_array_glb,0x00,0x04);
        DelayMs(200); 
//        DelayMs(200); 
      
        if( present_ckt_number_lcl <= ir_last_ckt_no_glb )
        {
            Set_HighVoltage(hv_voltage_glb);           //High voltage is applied only after relays are switched. 
          //---------------------------------------------------------------------------------------------------------//

            monitor_voltage_glb = 0;
            Fill_Any_float_Ram_With(Volts_glb,30,0.0);
            Timer2_100ms_Start();
            timer2_counter=0;
            while (timer2_counter <= 30)
            {
                read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
                read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
                monitor_voltage_glb = (read_voltage_glb * 1500)/5;
                voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
                monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
                voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
                Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
                Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
                Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
                Display_String_20(vdc,8,3,2);         
            }
            Timer2_100ms_Stop();

          //---------------------------------------------------------------------------------------------------------// 
    //        DelayS(3);                      // Delay of min 3 seconds required to settle the High voltage to the selected value.
            Clear_Lcd_Line(2);

            monitor_voltage_glb = 0;
            Fill_Any_float_Ram_With(Volts_glb,30,0.0);
            for(index_lcl=0;index_lcl<30;index_lcl++)  //  30 adc  readings are taken
            {
                read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
                read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
                Volts_glb[index_lcl]= read_voltage_glb ;   
                monitor_voltage_glb = Volts_glb[index_lcl] + monitor_voltage_glb ; 
                DelayMs(1);
            }
            monitor_voltage_glb = (monitor_voltage_glb - Volts_glb[0]) /29 ;            // first reading is neglected. Average of 29 readings are taken

            monitor_voltage_glb = (monitor_voltage_glb * 1500)/5;                        // since monitor voltage(0V - 5V) is proportional to actual high voltage(0V - 1500V) hence conversion is done to calculated actual voltage observed .

            //--------------------To Display Float value on LCD Below Conversion is done-------------------------------------//
            voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
            monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
            voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
            Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
            Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
            Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
            Display_String_20(vdc,8,3,2);                                          // decimal point displayed
            //---------------------------------------------------------------------------------------------------------------//

            IR_Measure_Voltage_glb = 0 ;
            Fill_Any_float_Ram_With(Volts_glb,50,0.0);
            read_data_lcl = 0;
            read_voltage_glb =0;
            for(index_lcl=0;index_lcl<50;index_lcl++)
            {
                   read_data_lcl=read_adc_IR_MEASURE();
                   read_voltage_glb = read_data_lcl*0.001225;
                   Volts_glb[index_lcl]= read_voltage_glb ;
                   IR_Measure_Voltage_glb = Volts_glb[index_lcl] + IR_Measure_Voltage_glb ;
                   DelayMs(1);
            }
            IR_Measure_Voltage_glb = IR_Measure_Voltage_glb / 50 ;                  // Average of 50 readings
            leakage_current_lcl =  (IR_Measure_Voltage_glb / 5100 ) * 1000000;         // I = V/R (Leakage current = measured voltage across shunt / shunt resistance(5kohm))
            measured_ir_lcl =(monitor_voltage_glb / leakage_current);              //Insulation Resistance = voltage applied / leakage current ;
        }
      
        
        if( present_ckt_number_lcl <= withstand_last_ckt_no_glb )
        {
            Set_HighVoltage(withstand_hv_voltage_glb);           //High voltage is applied only after relays are switched. 
          //---------------------------------------------------------------------------------------------------------//

            monitor_voltage_glb = 0;
            Fill_Any_float_Ram_With(Volts_glb,30,0.0);
            Timer2_100ms_Start();
            timer2_counter=0;
            while (timer2_counter <= 30)
            {
                read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
                read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
                monitor_voltage_glb = (read_voltage_glb * 1500)/5;
                voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
                monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
                voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
                Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
                Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
                Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
                Display_String_20(vdc,8,3,2);         
            }
            Timer2_100ms_Stop();

          //---------------------------------------------------------------------------------------------------------// 

    //        DelayS(3);                      // Delay of min 3 seconds required to settle the High voltage to the selected value.
            Clear_Lcd_Line(2);

            monitor_voltage_glb = 0;
            Fill_Any_float_Ram_With(Volts_glb,30,0.0);
            for(index_lcl=0;index_lcl<30;index_lcl++)  //  30 adc  readings are taken
            {
                read_data_lcl=read_adc_HV_MONITOR();       // read adc value of monitor voltage and store in int variable
                read_voltage_glb = read_data_lcl*0.001220703125; // store actual value in float variable using step value multiplication.(adc is 12 bit and Vref = 5V, hence 5/4096 = 0.001220703125  )
                Volts_glb[index_lcl]= read_voltage_glb ;   
                monitor_voltage_glb = Volts_glb[index_lcl] + monitor_voltage_glb ; 
                DelayMs(1);
            }
            monitor_voltage_glb = (monitor_voltage_glb - Volts_glb[0]) /29 ;            // first reading is neglected. Average of 29 readings are taken

            monitor_voltage_glb = (monitor_voltage_glb * 1500)/5;                        // since monitor voltage(0V - 5V) is proportional to actual high voltage(0V - 1500V) hence conversion is done to calculated actual voltage observed .

            //--------------------To Display Float value on LCD Below Conversion is done-------------------------------------//
            voltage_integer_part = (unsigned int)monitor_voltage_glb;                    // (monitor_voltage_glb) contains Float value so integer value is separated out and stored in int variable (voltage_int_part)
            monitor_voltage_lcl = (monitor_voltage_glb - voltage_integer_part) * 10000 ; // fractional part is separated out 
            voltage_fractional_part = (unsigned int)monitor_voltage_lcl;                 // fractional part is typecast into integer and stored in int var (voltage_fractional_part)
            Display_Number_4digit(voltage_integer_part,1,4,2);                           // decimal part displayed
            Display_String_20(dec_point,5,1,2);                                          // decimal point displayed
            Display_Number_4digit(voltage_fractional_part,6,2,2);                        // fractional part displayed
            Display_String_20(vdc,8,3,2);                                          // decimal point displayed
            //---------------------------------------------------------------------------------------------------------------//

            //----------------------------------------------------------------------------------------------------------------------//
            //Time required for withstand test is generated using Timer 2  of time period 100ms     
            timer2_counter = 0;
            sec_counter = 0;

            Timer2_100ms_Start();
            i_lcl =1;
            while(timer2_counter <= (hv_withstand_time_glb *0.01))
            {
        //        sec_counter = 0;
                Clear_Lcd_Line(4);
                Display_String_20(testing,1,11,3); 
                if(hv_withstand_time_glb == 500)
                {
                    ;//Display_Number_4digit(hv_withstand_time_glb,18,3,3);
                } 
                else
                {
                    Display_Number_4digit(i_lcl,19,2,3);
                }
                DelayMs(1000);
                i_lcl++;
            }
    //        Display_Number_4digit(i_lcl,19,2,3);
            Timer2_100ms_Stop();   
     //----------------------------------------------------------------------------------------------------------------------//  

            IR_Measure_Voltage_glb = 0 ;
            Fill_Any_float_Ram_With(Volts_glb,50,0.0);
            read_data_lcl = 0;
            read_voltage_glb =0;
            for(index_lcl=0;index_lcl<50;index_lcl++)
            {
                   read_data_lcl=read_adc_IR_MEASURE();
                   read_voltage_glb = read_data_lcl*0.001225;
                   Volts_glb[index_lcl]= read_voltage_glb ;
                   IR_Measure_Voltage_glb = Volts_glb[index_lcl] + IR_Measure_Voltage_glb ;
                   DelayMs(1);
            }
            IR_Measure_Voltage_glb = IR_Measure_Voltage_glb / 50 ;                  // Average of 50 readings
            leakage_current_lcl =  (IR_Measure_Voltage_glb / 5100 ) * 1000000;         // I = V/R (Leakage current = measured voltage across shunt / shunt resistance(5kohm))
    //        measured_ir_lcl =(monitor_voltage_glb / leakage_current);              //Insulation Resistance = voltage applied / leakage current ;
        }

        Reset_HighVoltage();

        if((leakage_current_lcl < withstand_current_glb)&&(measured_ir_lcl > insulation_resistance_lcl))
        {
            if(test_settings_ram[every_ckt_display_result_index] == SET)
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
                Clear_Lcd_Line(3);
                Display_String_20(ckt_pass,1,11,4);                             // Display each ckt pass
                Display_Number_4digit(present_ckt_number_lcl,5,2,4);
            } 
            else
            {
                ckt_pass_count_lcl++;
                store_result_glb[present_ckt_number_lcl] = PASS ;
            }
            
        }
        else if(((leakage_current_lcl > withstand_current_glb) && (measured_ir_lcl < insulation_resistance_lcl)) || (monitor_voltage_glb == 0.0))
        {
            
            store_result_glb[present_ckt_number_lcl] = FAIL ;
            if(test_settings_ram[every_ckt_display_result_index ]== SET)
            {   
                voltage_integer_part = (unsigned int)leakage_current;
                measured_ir_lcl = (leakage_current - voltage_integer_part) * 10000;
                voltage_fractional_part = (unsigned int)measured_ir_lcl;
                Clear_Lcd_Line(3);
                Display_Number_4digit(voltage_integer_part,1,4,3); 
                Display_String_20(dec_point,5,1,3);
                Display_Number_4digit(voltage_fractional_part,6,2,3);
                Display_String_20(ua,8,2,3);
                Display_String_20(ckt_fail,1,11,4);                           // Display each ckt fail
                Display_Number_4digit(present_ckt_number_lcl,5,2,4); 
            }
            if(test_settings_ram[stop_at_first_fault_index]== SET)
            {
                break;
            }
        }

//        DelayMs(200);

    } //end of for(present_ckt_number_lcl=1; present_ckt_number_lcl <= last_ckt_number_lcl; present_ckt_number_lcl++)

//    Reset_HighVoltage();
    
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    // initially All relays of both card 1 & 2 are made ON hence all points are connected to HV-
    Fill_Small_Data_Ram(modbus_array_glb,0X01,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    Fill_Any_Char_Ram_With(modbus_array_glb,MAX_MODBUS_ARRAY_SIZE,0x00);
    Fill_Small_Data_Ram(modbus_array_glb,0X02,MCMD_ALL_RLY_OFF,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL,MCMD_NULL);
    Send_Modbus_Data(modbus_array_glb,0x00,0x02);
    DelayMs(200); 
    
    if(ckt_pass_count_lcl == last_ckt_number_lcl)
    {
        withstand_test_pass_flag_glb = TRUE;
        ir_test_pass_flag_glb = TRUE;
    }
    else
    {
        withstand_test_fail_flag_glb = TRUE;
        ir_test_fail_flag_glb = TRUE;
    }

}













