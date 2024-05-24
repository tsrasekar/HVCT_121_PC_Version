#include <p24FJ512GU408.h>

#include "../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"

void key_parser()
{
    unsigned char local_ram[1024];
	switch(state_code)												 
	{																 
		case ST_IDLE:  

            switch(key_code)
            {
                case UP_KEY    : 
                                   menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_MAIN_MENU);
                                   menu_counter=Display_Navigation(menu_counter,UP_KEY,MAX_LIMIT_MAIN_MENU);
                                   state_code = ST_IDLE;                    		    
                                   break;	
                case DOWN_KEY  : 
                                   menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_MAIN_MENU);
                                   Display_Navigation(menu_counter,DOWN_KEY,MAX_LIMIT_MAIN_MENU);
                                   state_code = ST_IDLE;            
                                   break;
                case ENTER_KEY : 
                                   switch(menu_counter)
                                   {
                                        case ST_IDLE_TEST       : 
                                                                    state_code = menu_counter;
                                                                    
                                                                    Clear_Display();
                                                                    if(cards == 0)
                                                                    {
                                                                        Display_String_20(nocards,4,12,2); 
                                                                        DelayS(1);
                                                                        state_code = ST_IDLE;
                                                                        Clear_Display();
                                                                        Idle_Display();
                                                                        break;
                                                                    }
                                                                    if(((prg_specific_setting_ram[withstand_test_index] == RESET)&&(prg_specific_setting_ram[ir_test_index] == RESET)))
                                                                    {
                                                                        Display_String_20(test_error,3,14,2); 
                                                                        DelayS(1);
                                                                        state_code = ST_IDLE;
                                                                        Clear_Display();
                                                                        Idle_Display();
                                                                        break;
                                                                    }
                                                                    Pre_Display_Before_State(menu_counter,TRUE,local_ram);
                                                                    while(1)
                                                                    { 
                                                                        if(key_pressed_flag==TRUE)
                                                                        {
                                                                            key_pressed_flag=FALSE;
                                                                            key_data_glb = IC8574_ByteRead();
                                                                            key_code =(key_data_glb & 0X0F);
                                                                            
                                                                            if((key_data_glb == 0xDF)&&(digital_IP_flag_1 == TRUE))
                                                                            {
                                                                                digital_IP_flag_1 = FALSE;
                                                                            }
                                                                            else
                                                                            {
                                                                                digital_IP_flag_1 = TRUE;
                                                                            }
                   
                                                                            if((test_settings_ram[remote_key_index]== SET)&&(digital_IP_flag_1 == FALSE))
                                                                            {
                                                                                  key_code=ENTER_KEY ;
                                                                                  
                                                                            }                                                                         
                                                                            if(key_code==ESC_KEY)			
                                                                            {
                                                                              state_code = ST_IDLE;
                                                                              Clear_Display();
                                                                              Idle_Display();
                                                                              break;  
                                                                            }
                                                                            else if(key_code==ENTER_KEY)
                                                                            {
                                                                               DelayMs(500);
                                                                               Harness_Test();
                                                                               Switch_Interface_Relays_To_LV(); 
                                                                               Clear_Display();
    //                                                                         Initialize_Lcd();
                                                                               Pre_Display_Before_State(ST_IDLE_TEST,TRUE,local_ram);
    //                                                                            Store_UUT_Variables_To_Eeprom(current_cable_number);
    //                                                                            Retrive_Prg_Specific_Settings_and_data(current_cable_number);
    //                                                                            Clear_Lcd_Line(4);
    //                                                                            continuity_test(last_ckt_no_glb);
    //                                                                            if(continuity_test_pass_flag_glb == TRUE)
    //                                                                            {
    //                                                                                continuity_test_pass_flag_glb = FALSE;
    //                                                                                if(prg_specific_setting_ram[ir_test_index] == SET)
    //                                                                                {
    //                                                                                    Clear_Lcd_Line(2);
    //                                                                                    Display_String_20(ir_test,1,7,2);
    //                                                                                    DelayS(2);
    //                                                                                    Clear_Lcd_Line(2);
    //                                                                                    HV_IR_test(last_ckt_no_glb);
    //                                                                                }// end of if(prg_specific_setting_ram[ir_test_index] == SET)
    //                                                                                if(((prg_specific_setting_ram[withstand_test_index] == SET)&&(prg_specific_setting_ram[ir_test_index] == RESET)) || ((prg_specific_setting_ram[withstand_test_index] == SET)&&(ir_test_pass_flag_glb == TRUE)))
    //                                                                                {
    //                                                                                    Clear_Lcd_Line(2);
    //                                                                                    Display_String_20(withstand_test,1,14,2);
    //                                                                                    DelayS(2);
    //                                                                                    Clear_Lcd_Line(2);
    //                                                                                    HV_Withstand_test(last_ckt_no_glb); 
    //                                                                                }
    //                                                                                
    //                                                                                if(((ir_test_pass_flag_glb == TRUE) && (prg_specific_setting_ram[withstand_test_index] == RESET)) || (withstand_test_pass_flag_glb == TRUE))
    //                                                                                {
    //                                                                                   ir_test_pass_flag_glb = FALSE; 
    //                                                                                   withstand_test_pass_flag_glb= FALSE;
    //                                                                                   Display_String_20(pass,1,5,3);	
    //                                                                                    // display pass count
    //                                                                                    Read_Write_RTC(digit_ram,pass_count_bbram_base_address,3,READ);
    //                                                                                    Increment_Numeric_Array (digit_ram,3);
    //                                                                                    Display_Ram(digit_ram,7,0,3,3);
    //                                                                                    Read_Write_RTC(digit_ram,pass_count_bbram_base_address,3,WRITE);
    //                                                                                   
    //                                                                                   
    //                                                                                   
    //                                                                                }
    //                                                                                else if((ir_test_fail_flag_glb == TRUE) || (withstand_test_fail_flag_glb == TRUE))
    //                                                                                {
    //                                                                                    ir_test_fail_flag_glb = FALSE;
    //                                                                                    withstand_test_fail_flag_glb = FALSE;
    //                                                                                     // display FAIL count
    //                                                                                    Display_String_20(fail,1,5,3);	
    //                                                                                    Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,READ);
    //                                                                                    Increment_Numeric_Array (digit_ram,3);
    //                                                                                    Display_Ram(digit_ram,7,0,3,3);
    //                                                                                    Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,WRITE);
    //                                                                                    break;
    //                                                                                    
    //                                                                                }
    ////                                                                                Display_String_20(press_ent_start,1,18,4);
    //    //                                                                          state_code = ST_IDLE_TEST;
    //                                                                            }// end of if(continuity_test_pass_flag_glb == TRUE)
    //                                                                            else if(continuity_test_fail_flag_glb == TRUE)
    //                                                                            {
    //                                                                                continuity_test_fail_flag_glb = FALSE;
    //                                                                                Display_String_20(fail,1,5,3);	
    //                                                                                // display pass count
    //                                                                                Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,READ);
    //                                                                                Increment_Numeric_Array (digit_ram,3);
    //                                                                                Display_Ram(digit_ram,7,0,3,3);
    //                                                                                Read_Write_RTC(digit_ram,fail_count_bbram_base_address,3,WRITE);
    //                                                                                break;
    //                                                                            }

                                                                            }// End of else if(key_code==ENTER_KEY)
                                                                        }//end if(key_pressed_flag==TRUE).   
                                                                    }// End of while(1)
                                                                    state_code = ST_IDLE;
                                                                    Clear_Display();
                                                                    Idle_Display();
                                                                    break; 
                                                                   
                                        case ST_IDLE_SELECT_PRG :     
                                                                    state_code = menu_counter;
                                                                    Clear_Display();
                                                                    Pre_Display_Before_State(menu_counter,TRUE,local_ram);
                                                                    break;
                                        case ST_IDLE_BRD_CHK    :

                                                                    menu_counter=MIN_LIMIT_MENU;
                                                                    Clear_Display();
                                                                    Check_Board();
                        //                                            Display_String_20(notavailable,1,13,2);
                                                                    DelayS(1);
                                                                    Pre_Display_Before_State(ST_IDLE,TRUE,local_ram);
                                                                    state_code = ST_IDLE;
                                                                    break;

                                        case ST_ANALYSE         :  
                                                                    menu_counter=MIN_LIMIT_MENU;
                                                                    Clear_Display();
                                                                    Display_String_20(notavailable,1,13,2);
                                                                    DelayS(1);
                                                                    Pre_Display_Before_State(ST_IDLE,TRUE,local_ram);
                                                                    state_code = ST_IDLE;
                                                                    break;

                                        case ST_SET_UP          :   state_code = menu_counter;
                                                                    Clear_Display();
                                                                    Pre_Display_Before_State(menu_counter,TRUE,local_ram);
                                                                    break;

                                        case ST_IDLE_COUNT      :
                                                                    state_code = menu_counter;
                                                                    Clear_Display();
                                                                    Pre_Display_Before_State(menu_counter,TRUE,local_ram);
                                                                    break;

                                   }
                                   break;	

                case ESC_KEY   : 
                                   state_code = ST_IDLE;
                                   Pre_Display_Before_State(ST_IDLE,TRUE,local_ram);
                                   break;	
                default        :
                                   break;                 
            }
            break;
            
//        case ST_IDLE_TEST      :
//                                continuity_test(last_ckt_no_glb);
//                                state_code = ST_IDLE;
//                                break;    
            
            
        case ST_IDLE_SELECT_PRG:
          
            switch(key_code)
            {
                case UP_KEY     :
                                    current_prg_local=Navigate_Counter_3_Digit(current_prg_local,DOWN_KEY,cursor_position_global);
                                    Scroll_Programs_Hvct(current_prg_local);
                                    state_code = ST_IDLE_SELECT_PRG;                    		    
                                    break;
                                
                case DOWN_KEY   :
                                    current_prg_local=Navigate_Counter_3_Digit(current_prg_local,UP_KEY,cursor_position_global);
                                    Scroll_Programs_Hvct(current_prg_local);
                                    state_code = ST_IDLE_SELECT_PRG;                       
                                    break;
                                
                case ENTER_KEY  :
                                    if(current_prg_local > MAX_PROGRAM_NO && cursor_position_global==0x03)
                                    {
                                      Clear_Display();
                                      Remove_Blinking_Cursor();
                                      Display_String_20(lmt_exceeds,1,20,2);
                                      DelayS(1);	
                                      Display_String_20(lmt_exceeds,1,20,2);
                                      cursor_position_global=1;
                                      state_code = ST_IDLE;
                                      Pre_Display_Before_State(ST_IDLE,TRUE,local_ram);
                                    }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                    else if(cursor_position_global==0x03)
                                          {
                                            cursor_position_global=1;
                                            Remove_Blinking_Cursor();
                                            menu_counter=1;
                                            if (current_prg_local==0)
                                            {
                                                current_prg_local=1; 
                                                current_cable_number = current_prg_local;
                                                E2prom_Byte_Write(e2prom_2_page_2_add,((current_cable_number_address)&mask_higher_byte) >> 8,(current_cable_number_address)&mask_lower_byte,current_cable_number);
                                                Scroll_Programs_Hvct(current_prg_local);
                                                break;
                                            }
                                            current_cable_number=current_prg_local;
                                            E2prom_Byte_Write(e2prom_2_page_2_add,((current_cable_number_address)&mask_higher_byte) >> 8,(current_cable_number_address)&mask_lower_byte,current_cable_number);
                                            Display_String_20(space,1,20,2);
                                            Display_String_20(space,1,20,3);
                                            Display_String_20(space,1,20,4);
                                            Display_String_20(selected,1,8,2);
                                            DelayS(1);
                                            state_code = ST_IDLE;
                                            Pre_Display_Before_State(ST_IDLE,TRUE,local_ram);	
                                           }//end else if(cursor_position_global==0x03)
                                    else if(cursor_position_global<=0x03)     
                                           {
                                            cursor_position_global++;
                                            Start_Cursor_Blinking(cursor_position_global+16,1);
                                            state_code = ST_IDLE_SELECT_PRG;
                                           }//end else if(cursor_position_global<=0x03)     
                                           else;
                                    break;
                case ESC_KEY    :
                                    if( cursor_position_global==0x01)
                                    {
                                    Remove_Blinking_Cursor();
                                    cursor_position_global=1;
                                    state_code = ST_IDLE;
                                    Initialize_Lcd();
                                    Remove_Blinking_Cursor();
                                    Pre_Display_Before_State(ST_IDLE,TRUE,local_ram);
                                    break;
                                    }
                                    else if( cursor_position_global > 0x01)
                                    {
                                       cursor_position_global--;
                                            Start_Cursor_Blinking(cursor_position_global+16,1);
                                            state_code = ST_IDLE_SELECT_PRG; 
                                    }
                default         :
                                    break;
            }
            break;
            
        case ST_ANALYSE:  

            switch(key_code)
            {
                case UP_KEY    : 
                                  menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_ANALYSE_MENU);
                                  menu_counter=Display_Navigation(menu_counter,UP_KEY,MAX_LIMIT_ANALYSE_MENU);
                                  state_code = ST_ANALYSE;                    		    
                                  break;	
                                  
                case DOWN_KEY :   
                                  menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_ANALYSE_MENU);
                                  Display_Navigation(menu_counter,DOWN_KEY,MAX_LIMIT_ANALYSE_MENU);
                                  state_code = ST_ANALYSE;            
                                  break;
                                  
                case ENTER_KEY  :
                                  state_code=menu_counter+0X4F;
//									  Pre_Display_Before_State(menu_counter+0X4F,TRUE,local_ram);     
                                  break;
                                  
                case ESC_KEY:    
                                  state_code = ST_IDLE;
                                  Initialize_Lcd();
                                  Remove_Blinking_Cursor();
                                  Pre_Display_Before_State(ST_IDLE,TRUE,local_ram);
                                  break;
                                  
                default:
                                  break;
                                  
            }
            break;  
            
        case  ST_SET_UP:  
            
			switch(key_code)
            {
                case UP_KEY    : 
                                  menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_SETUP_MENU);
                                  menu_counter=Display_Navigation(menu_counter,UP_KEY,MAX_LIMIT_SETUP_MENU);	
                                  state_code = ST_SET_UP;                    		    
                                  break;	

                case DOWN_KEY :  
                                  menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_SETUP_MENU);
                                  Display_Navigation(menu_counter,DOWN_KEY,MAX_LIMIT_SETUP_MENU);
                                  state_code = ST_SET_UP;           
                                  break;	

                case ENTER_KEY  :
                                  state_code=menu_counter+0X0A;					
                                  Pre_Display_Before_State(menu_counter+0X0A,TRUE,local_ram);					     
                                  break;	

                case ESC_KEY:   
                                  state_code = ST_IDLE;
                                  Initialize_Lcd();
                                  Remove_Blinking_Cursor();
                                  Pre_Display_Before_State(ST_IDLE,TRUE,local_ram);
                                  break;	

                default:
                                  break;

            }
            break;
                
        case ST_SET_UP_OTH_DATA: 
            
			switch(key_code)
            {
                case UP_KEY    : 
                                  menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_OTH_DATA_MENU);
                                  menu_counter=Display_Navigation(menu_counter,UP_KEY,MAX_LIMIT_OTH_DATA_MENU);
                                  state_code = ST_SET_UP_OTH_DATA;                    		    
                                  break;	

                case DOWN_KEY :  
                                  menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_OTH_DATA_MENU);
                                  Display_Navigation(menu_counter,DOWN_KEY,MAX_LIMIT_OTH_DATA_MENU);
                                  state_code = ST_SET_UP_OTH_DATA;          
                                  break;	

                case ENTER_KEY:  
                                  Execute_Other_Data_Menu(menu_counter,local_ram);
                                  break;	

                case ESC_KEY  :    
                                    Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                    state_code = ST_SET_UP;
                                    break;	

                default:
                                  break;

            }
            break;  

        case ST_SET_UP_INIT_RTC:  
            
		    switch(key_code)
            {
                case UP_KEY    :  
                                  Navigate_Date_Time(digit_ram,UP_KEY,cursor_position_global);
                                  Display_Ram(digit_ram,1,0,10,2);
                                  Display_Ram(digit_ram,1,10,8,3);
                                  Display_Ram(digit_ram,1,18,1,4);		
                                  Start_Cursor_Blinking((cursor_position_global<=10) ? cursor_position_global:(cursor_position_global<=18)? cursor_position_global-10:cursor_position_global-18,(cursor_position_global<=10) ? 2:(cursor_position_global<=18)?3:4);
                                  state_code = ST_SET_UP_INIT_RTC;                   		    
                                  break;	

                case DOWN_KEY :  
                                  Navigate_Date_Time(digit_ram,DOWN_KEY,cursor_position_global);
                                  Display_Ram(digit_ram,1,0,10,2);
                                  Display_Ram(digit_ram,1,10,8,3);
                                  Display_Ram(digit_ram,1,18,1,4);
                                  Start_Cursor_Blinking((cursor_position_global<=10) ? cursor_position_global:(cursor_position_global<=18)? cursor_position_global-10:cursor_position_global-18,(cursor_position_global<=10) ? 2:(cursor_position_global<=18)?3:4);
                                  state_code =ST_SET_UP_INIT_RTC;          
                                  break;

                case ENTER_KEY:
                                  if(cursor_position_global==19)
                                  {
                                    Display_String_20(space,1,20,2);	
                                    Display_String_20(space,1,20,3);
                                    Display_String_20(space,1,20,4);
                                    if(Validate_Date_Time(digit_ram))
                                    {
                                        cursor_position_global=1;	
                                        Display_String_20(saving,1,6,2);
                                        Store_Date_Time_2(digit_ram);     
                                        WriteRtc(stored_day_bbram_base_address,(ReadRtc(0x05) & (0x3F)));
                                        WriteRtc(stored_month_bbram_base_address,(ReadRtc(0x06) & (0x1F)));
                                        WriteRtc(stored_year_lsb_bbram_base_address,ReadRtc(0x11));
                                        WriteRtc(day_code_bbram_base_address,(digit_ram[18] - 0x30));
                                    }//end if(Validate_Date_Time(digit_ram))
                                    else
                                        Display_String_20(dataerror,1,10,2);	

                                    DelayS(1);
                                    Remove_Blinking_Cursor();
                                    Pre_Display_Before_State(ST_SET_UP,TRUE,digit_ram);
                                    state_code = ST_SET_UP;
                                  }				  
                                  else
                                  {
                                    cursor_position_global++;
                                    if(cursor_position_global-1==2 || cursor_position_global-1==5 ||cursor_position_global-1==12 ||cursor_position_global-1==15)
                                        cursor_position_global++;
                                    //Start_Cursor_Blinking((cursor_position_global<=10) ? cursor_position_global:cursor_position_global-10,(cursor_position_global<=10) ? 2:3);
                                    Start_Cursor_Blinking((cursor_position_global<=10) ? cursor_position_global:(cursor_position_global<=18)? cursor_position_global-10:cursor_position_global-18,(cursor_position_global<=10) ? 2:(cursor_position_global<=18)?3:4);
                                  }								     
                                  break;	

                case ESC_KEY  :    
                                  Remove_Blinking_Cursor();
                                  cursor_position_global=1;
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  state_code = ST_SET_UP;
                                  break;	

                default        :
                                  break;

            }
            break;

        case ST_SET_UP_GEN_SETTINGS  :
            
            switch(key_code)
            {
                case UP_KEY    :  
                                  menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_GEN_SETT_MENU);
                                  arrow_pos_global=Display_Navigation_1(menu_counter,UP_KEY,MAX_LIMIT_GEN_SETT_MENU);
                                  state_code = ST_SET_UP_GEN_SETTINGS;                 		    
                                  break;	

                case DOWN_KEY : 
                                  menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_GEN_SETT_MENU);
                                  arrow_pos_global=Display_Navigation_1(menu_counter,DOWN_KEY,MAX_LIMIT_GEN_SETT_MENU);
                                  state_code = ST_SET_UP_GEN_SETTINGS;            
                                  break;	

                case ENTER_KEY : 
                                  Execute_General_Settings_Menu(menu_counter,arrow_pos_global); 
                                  state_code = ST_SET_UP_GEN_SETTINGS;						     
                                  break;	

                case ESC_KEY	:
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  arrow_pos_global=1;
                                  state_code = ST_SET_UP;
                                  break;	

                default:
                                  break;

            }
            break;
                
        case ST_SET_UP_TEST_SETTINGS:  
            
			switch(key_code)
            {
                case UP_KEY    : 
                                  menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_TEST_SETT_MENU);
                                  arrow_pos_global=Display_Navigation_1(menu_counter,UP_KEY,MAX_LIMIT_TEST_SETT_MENU);
                                  state_code = ST_SET_UP_TEST_SETTINGS;                 		    
                                  break;

                case DOWN_KEY :
                                  menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_TEST_SETT_MENU);
                                  arrow_pos_global=Display_Navigation_1(menu_counter,DOWN_KEY,MAX_LIMIT_TEST_SETT_MENU);
                                  state_code = ST_SET_UP_TEST_SETTINGS;            
                                  break;

                case ENTER_KEY : 
                                  Execute_Test_Settings_Menu (menu_counter,arrow_pos_global); 
                                  state_code = ST_SET_UP_TEST_SETTINGS;						     
                                  break;	

                case ESC_KEY	: 
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  arrow_pos_global=1;
                                  state_code = ST_SET_UP;
                                  break;

                default:
                                  break;

            }
            break; 
                
        case ST_SET_UP_PRG_SETTINGS:
            
            switch(key_code)
            {
                case UP_KEY    : 
                                 menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_PRG_SETT_MENU);
                                 arrow_pos_global=Display_Navigation_1(menu_counter,UP_KEY,MAX_LIMIT_PRG_SETT_MENU);
                                 state_code = ST_SET_UP_PRG_SETTINGS; 		    
                                 break;	

                case DOWN_KEY : 
                                 menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_PRG_SETT_MENU);
                                 arrow_pos_global=Display_Navigation_1(menu_counter,DOWN_KEY,MAX_LIMIT_PRG_SETT_MENU);
                                 state_code = ST_SET_UP_PRG_SETTINGS;   
                                 break;	

                case ENTER_KEY :
                                 Execute_Prg_Settings_Menu (menu_counter,arrow_pos_global); 
                                 state_code = ST_SET_UP_PRG_SETTINGS;						     
                                 break;	

                case ESC_KEY  : 
                                 Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                 arrow_pos_global=1;
                                 state_code = ST_SET_UP;
                                 break;	

                default:
                                 break;

            }
            break;

        case ST_SET_UP_HVCT_DATA_LAST_CKT:
            
            switch(key_code)
            {
                case UP_KEY     :
                                    ir_last_ckt_no_glb=Navigate_Counter_3_Digit(ir_last_ckt_no_glb,DOWN_KEY,cursor_position_global);
                                    Scroll_last_ckt_Hvct(ir_last_ckt_no_glb);
                                    state_code = ST_SET_UP_HVCT_DATA_LAST_CKT;                    		    
                                    break;
                                
                case DOWN_KEY   :
                                    ir_last_ckt_no_glb=Navigate_Counter_3_Digit(ir_last_ckt_no_glb,UP_KEY,cursor_position_global);
                                    Scroll_last_ckt_Hvct(ir_last_ckt_no_glb);
                                    state_code = ST_SET_UP_HVCT_DATA_LAST_CKT;   
                                    break;
                                
                case ENTER_KEY  :
                                    if(ir_last_ckt_no_glb> MAX_CKT_NO && cursor_position_global==0x03)
                                    {
                                      Clear_Display();
                                      Remove_Blinking_Cursor();
                                      Display_String_20(lmt_exceeds,1,20,2);
                                      DelayS(1);	
                                      cursor_position_global=1;
                                      state_code = ST_SET_UP_HVCT_DATA_LAST_CKT;
                                      Pre_Display_Before_State(ST_SET_UP_HVCT_DATA_LAST_CKT,TRUE,local_ram);
                                    }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                    if(ir_last_ckt_no_glb == 0 && cursor_position_global==0x03)
                                    {
                                      Clear_Display();
                                      Remove_Blinking_Cursor();
                                      cursor_position_global=1;
                                      state_code = ST_SET_UP_HVCT_DATA_LAST_CKT;
                                      Pre_Display_Before_State(ST_SET_UP_HVCT_DATA_LAST_CKT,TRUE,local_ram);
                                    }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                    else if(cursor_position_global==0x03)
                                          {
                                            cursor_position_global=1;
                                            Remove_Blinking_Cursor();
                                            E2prom_Byte_Write(e2prom_2_page_2_add,((last_ckt_number_base_address+(current_cable_number-1))&mask_higher_byte) >> 8,(last_ckt_number_base_address+(current_cable_number-1))&mask_lower_byte,ir_last_ckt_no_glb);//11/10/11.
//                                            Display_String_20(space,1,20,2);
//                                            Display_String_20(space,1,20,3);
//                                            Display_String_20(space,1,20,4);
//                                            Display_String_20(selected,1,8,2);
                                            state_code = ST_SET_UP_HVCT_DATA_VOLTAGE;
//                                            DelayS(1);
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_DATA_VOLTAGE,TRUE,local_ram);	
                                           }//end else if(cursor_position_global==0x03)
                                    else if(cursor_position_global<=0x03)     
                                           {
                                            cursor_position_global++;
                                            Start_Cursor_Blinking(cursor_position_global+8,2);
                                            state_code = ST_SET_UP_HVCT_DATA_LAST_CKT;
                                           }//end else if(cursor_position_global<=0x03)     
                                           else;
                                    break;
                                    
                case ESC_KEY    :
                                    Remove_Blinking_Cursor();
                                    cursor_position_global=1;
                                    state_code = ST_SET_UP;
                                    Initialize_Lcd();
                                    Remove_Blinking_Cursor();
                                    Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                    break;
                                    
                default         :
                                    break;
                                    
            }
            break;
            
        case ST_SET_UP_HVCT_DATA_VOLTAGE  :
            
            switch(key_code)
            {
                case UP_KEY      :      
                                    hv_voltage_glb=Navigate_Counter_4_Digit(hv_voltage_glb,DOWN_KEY,cursor_position_global);
                                    Scroll_voltage_Hvct(hv_voltage_glb);
                                    state_code = ST_SET_UP_HVCT_DATA_VOLTAGE; 
                                    break;
                                    
                case DOWN_KEY    :
                                    hv_voltage_glb=Navigate_Counter_4_Digit(hv_voltage_glb,UP_KEY,cursor_position_global);
                                    Scroll_voltage_Hvct(hv_voltage_glb);
                                    state_code = ST_SET_UP_HVCT_DATA_VOLTAGE; 
                                    break;
                                    
                case ENTER_KEY   :          
                                        if((hv_voltage_glb > MAX_VOLTAGE_LIMIT) && (cursor_position_global == 0x04))
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            Display_String_20(lmt_exceeds,1,20,2);
                                            DelayS(1);	
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_HVCT_DATA_VOLTAGE;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_DATA_VOLTAGE,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if((hv_voltage_glb < MIN_VOLTAGE_LIMIT) && (cursor_position_global == 0x04))
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            Display_String_20(minimum_voltage,1,16,2);
                                            DelayS(1);	
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_HVCT_DATA_VOLTAGE;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_DATA_VOLTAGE,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if(cursor_position_global==0x04)
                                        {
                                            cursor_position_global=1;                                                                                                                                                                                                                                      
                                            Remove_Blinking_Cursor();
                                            hv_msb_glb = ((0xFF00 & hv_voltage_glb) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
                                            hv_lsb_glb = (0x00FF & hv_voltage_glb);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

                                            E2prom_Byte_Write(e2prom_2_page_2_add,((high_voltage_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(high_voltage_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
                                            E2prom_Byte_Write(e2prom_2_page_2_add,((high_voltage_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(high_voltage_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);

                                            state_code = ST_SET_UP_HVCT_DATA_IR;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_DATA_IR,TRUE,local_ram);	
                                         }//end else if(cursor_position_global==0x03)
                                        else if(cursor_position_global<=0x04)     
                                        {
                                            cursor_position_global++;
                                            Start_Cursor_Blinking(cursor_position_global+15,2);
                                            state_code = ST_SET_UP_HVCT_DATA_VOLTAGE;
                                        }//end else if(cursor_position_global<=0x03)     
                                               else;
                                        break;
                                        
                case ESC_KEY     :      
                                        Remove_Blinking_Cursor();
                                        cursor_position_global=1;
                                        state_code = ST_SET_UP;
                                        Initialize_Lcd();
                                        Remove_Blinking_Cursor();
                                        Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                        break;
                                        
            }
            break;

        case ST_SET_UP_HVCT_DATA_IR  :
            
            switch(key_code)
            {
                case UP_KEY      :      
                                    insulation_resistance_glb=Navigate_Counter_4_Digit(insulation_resistance_glb,DOWN_KEY,cursor_position_global);
                                    Scroll_IR_Hvct(insulation_resistance_glb);
                                    state_code = ST_SET_UP_HVCT_DATA_IR; 
                                    break;

                case DOWN_KEY    :
                                    insulation_resistance_glb=Navigate_Counter_4_Digit(insulation_resistance_glb,UP_KEY,cursor_position_global);
                                    Scroll_IR_Hvct(insulation_resistance_glb);
                                    state_code = ST_SET_UP_HVCT_DATA_IR; 
                                    break;

                case ENTER_KEY   :          
                                        if(insulation_resistance_glb> MAX_IR_LIMIT && cursor_position_global==0x04)
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            Display_String_20(lmt_exceeds,1,20,2);
                                            DelayS(1);	
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_HVCT_DATA_IR;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_DATA_IR,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if(insulation_resistance_glb == 0 && cursor_position_global==0x04)
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_HVCT_DATA_IR;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_DATA_IR,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if(cursor_position_global==0x04)
                                        {
                                            cursor_position_global=1;                                                                                                                                                                                                                                      
                                            Remove_Blinking_Cursor();
                                            hv_msb_glb = ((0xFF00 & insulation_resistance_glb) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
                                            hv_lsb_glb = (0x00FF & insulation_resistance_glb);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

                                            E2prom_Byte_Write(e2prom_2_page_2_add,((insulation_resistance_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(insulation_resistance_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
                                            E2prom_Byte_Write(e2prom_2_page_2_add,((insulation_resistance_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(insulation_resistance_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);

                                            state_code = ST_SET_UP;//ST_SET_UP_HVCT_DATA_TIME;
                                            Pre_Display_Before_State(ST_SET_UP/*ST_SET_UP_HVCT_DATA_TIME*/,TRUE,local_ram);	
                                         }//end else if(cursor_position_global==0x03)
                                        else if(cursor_position_global<=0x04)     
                                        {
                                            cursor_position_global++;
                                            Start_Cursor_Blinking(cursor_position_global+3,3);
                                            state_code = ST_SET_UP_HVCT_DATA_IR;
                                        }//end else if(cursor_position_global<=0x03)     
                                               else;
                                        break;

                case ESC_KEY     :      
                                        Remove_Blinking_Cursor();
                                        cursor_position_global=1;
                                        state_code = ST_SET_UP;
                                        Initialize_Lcd();
                                        Remove_Blinking_Cursor();
                                        Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                        break;

            }
             break;  
                 
//        case ST_SET_UP_HVCT_DATA_TIME  :
//            
//            switch(key_code)
//            {
//                case UP_KEY      :      
//                                    hv_withstand_time_glb=Navigate_Counter_4_Digit(hv_withstand_time_glb,DOWN_KEY,cursor_position_global);
//                                    Scroll_Time_Hvct(hv_withstand_time_glb);
//                                    state_code = ST_SET_UP_HVCT_DATA_TIME; 
//                                    break;
//
//                case DOWN_KEY    :
//                                    hv_withstand_time_glb=Navigate_Counter_4_Digit(hv_withstand_time_glb,UP_KEY,cursor_position_global);
//                                    Scroll_Time_Hvct(hv_withstand_time_glb);
//                                    state_code = ST_SET_UP_HVCT_DATA_TIME; 
//                                    break;
//
//                case ENTER_KEY   :          
//                                        if(hv_withstand_time_glb > MAX_WITHSTAND_TIME_LIMIT && cursor_position_global==0x04)
//                                        {
//                                            Clear_Display();
//                                            Remove_Blinking_Cursor();
//                                            Display_String_20(lmt_exceeds,1,20,2);
//                                            DelayS(1);	
//                                            cursor_position_global=1;
//                                            state_code = ST_SET_UP_HVCT_DATA_TIME;
//                                            Pre_Display_Before_State(ST_SET_UP_HVCT_DATA_TIME,TRUE,local_ram);
//                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
//                                        else if(cursor_position_global==0x04)
//                                        {
//                                            cursor_position_global=1;                                                                                                                                                                                                                                      
//                                            Remove_Blinking_Cursor();
//                                            hv_msb_glb = ((0xFF00 & hv_withstand_time_glb) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
//                                            hv_lsb_glb = (0x00FF & hv_withstand_time_glb);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
//
//                                            E2prom_Byte_Write(e2prom_2_page_2_add,((hv_time_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(hv_time_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
//                                            E2prom_Byte_Write(e2prom_2_page_2_add,((hv_time_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(hv_time_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);
//                                            Display_String_20(space,1,20,2);
//                                            Display_String_20(space,1,20,3);
//                                            Display_String_20(space,1,20,4);
//                                            Display_String_20(selected,1,8,2);
//                                            DelayMs(700);
//                                            state_code = ST_SET_UP;
//                                            Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);	
//                                         }//end else if(cursor_position_global==0x03)
//                                        else if(cursor_position_global<=0x04)     
//                                        {
//                                            cursor_position_global++;
//                                            Start_Cursor_Blinking(cursor_position_global+14,3);
//                                            state_code = ST_SET_UP_HVCT_DATA_TIME;
//                                        }//end else if(cursor_position_global<=0x03)     
//                                               else;
//                                        break;
//
//                case ESC_KEY     :      
//                                        Remove_Blinking_Cursor();
//                                        cursor_position_global=1;
//                                        state_code = ST_SET_UP;
//                                        Initialize_Lcd();
//                                        Remove_Blinking_Cursor();
//                                        Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
//                                        break;
//
//            }
//            break;

        case ST_SET_UP_DIAGNO:  
            
		    switch(key_code)
            {
                case UP_KEY     :
                                  menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_DIAGNO_MENU);
                                  menu_counter=Display_Navigation(menu_counter,UP_KEY,MAX_LIMIT_DIAGNO_MENU);
                                  state_code = ST_SET_UP_DIAGNO;                    		    
                                  break;	

                case DOWN_KEY  : 
                                  menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_DIAGNO_MENU);
                                  Display_Navigation(menu_counter,DOWN_KEY,MAX_LIMIT_DIAGNO_MENU);
                                  state_code = ST_SET_UP_DIAGNO;            
                                  break;	

                case ENTER_KEY : 
                                  Execute_DIAGNO_Menu(menu_counter);
                                  //--------12_12_14_2_b-----start------//
                                  if(menu_counter!=9)
                                  {
                                    Pre_Display_Before_State(ST_SET_UP_DIAGNO,TRUE,local_ram);
                                    state_code = ST_SET_UP_DIAGNO;						     
                                  }//end if(meun_counter!=9).
                                  //--------12_12_14_2_b-----start------//
                                  break;

                case ESC_KEY   :
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  state_code = ST_SET_UP;
                                  break;

                default:
                                  break;

            }
            break;
                
        case ST_SET_UP_FUTURA_DND:  
            
		    switch(key_code)
            {
                case UP_KEY     : 
                                  menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_FDND_MENU);
                                  menu_counter=Display_Navigation(menu_counter,UP_KEY,MAX_LIMIT_FDND_MENU);
                                  state_code = ST_SET_UP_FUTURA_DND;                    		    
                                  break;

                case DOWN_KEY  : 
                                  menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_FDND_MENU);
                                  Display_Navigation(menu_counter,DOWN_KEY,MAX_LIMIT_FDND_MENU);
                                  state_code = ST_SET_UP_FUTURA_DND;            
                                  break;	

                case ENTER_KEY :
                                  Execute_FDND_Menu(menu_counter);
                                  state_code = ST_SET_UP_FUTURA_DND;
                                  Pre_Display_Before_State(ST_SET_UP_FUTURA_DND,TRUE,local_ram);						     
                                  break;

                case ESC_KEY   :
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  state_code = ST_SET_UP; 
                                  break;

                default:
                                  break;

            }
            break;

            Pre_Display_Before_State(ST_SET_UP_FUTURA_DND,TRUE,local_ram);
            break; 

        case ST_SET_UP_RELAY_TIME:  
            
		    switch(key_code)
            {
                case UP_KEY    :  
                                  menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_RELAY_ON);
                                  if(temp_global==0)
                                  {
                                    Display_Number_4digit(menu_counter,6,1,2);
                                    Start_Cursor_Blinking(6,2);
                                  }
                                  else
                                  {
                                    Display_Number_4digit(menu_counter,6,1,4);
                                    Start_Cursor_Blinking(6,4);
                                  }
                                  state_code = ST_SET_UP_RELAY_TIME;                 		    
                                  break;	

                case DOWN_KEY :  //call here volume decrement function.
                                  //Display_String_20(relay_on_time,1,13,1);
                                  menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_RELAY_ON);
                                  if(temp_global==0)
                                  {
                                    Display_Number_4digit(menu_counter,6,1,2);
                                    Start_Cursor_Blinking(6,2);
                                  }
                                  else
                                  {
                                    Display_Number_4digit(menu_counter,6,1,4);
                                    Start_Cursor_Blinking(6,4);
                                  }
                                  state_code = ST_SET_UP_RELAY_TIME;         
                                  break;	

                case ENTER_KEY  :
                                  if(temp_global==0)
                                    E2prom_Byte_Write(e2prom_2_page_2_add,((relay_1_on_time_address)&mask_higher_byte) >> 8,(relay_1_on_time_address)&mask_lower_byte,menu_counter);//11/10/11.
                                  else
                                    E2prom_Byte_Write(e2prom_2_page_2_add,((relay_2_on_time_address)&mask_higher_byte) >> 8,(relay_2_on_time_address)&mask_lower_byte,menu_counter);

                                  if(temp_global==1)
                                  {
                                      Clear_Display();
                                      Remove_Blinking_Cursor();
                                      temp_global=0;
                                      Display_String_20(selected,1,8,2);
                                      DelayS(1);
                                      Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                      state_code = ST_SET_UP;
                                  }
                                  else
                                  {
                                        menu_counter= E2prom_Byte_Read(e2prom_2_page_2_add,(relay_2_on_time_address & mask_higher_byte) >> 8,(relay_2_on_time_address & mask_lower_byte));
                                        temp_global++;
                                        Start_Cursor_Blinking(6,4);
                                        state_code = ST_SET_UP_RELAY_TIME;  
                                  }	   						     
                                  break;	

                case ESC_KEY:    
                                  Remove_Blinking_Cursor();
                                  temp_global=0;
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  state_code = ST_SET_UP;
                                  break;

                default:
                                  break;

            }
            break;        

        case ST_SET_UP_CLR_COUNTS:  
            
			switch(key_code)
            {
                case UP_KEY   :
                                menu_counter=Navigate_Menu_Counter(menu_counter,UP_KEY,MAX_LIMIT_CLR_COUNTS_MENU);//24_12_18_1.
                                arrow_pos_global=Display_Navigation_1(menu_counter,UP_KEY,MAX_LIMIT_CLR_COUNTS_MENU);//24_12_18_1.
                                state_code = ST_SET_UP_CLR_COUNTS; 	    
                                break;	

                case DOWN_KEY : 
                                menu_counter=Navigate_Menu_Counter(menu_counter,DOWN_KEY,MAX_LIMIT_CLR_COUNTS_MENU);//24_12_18_1.
                                arrow_pos_global=Display_Navigation_1(menu_counter,DOWN_KEY,MAX_LIMIT_CLR_COUNTS_MENU);//24_12_18_1.
                                state_code = ST_SET_UP_CLR_COUNTS;   
                                break;	

                case ENTER_KEY : 
                                Execute_Clr_Cnt_Menu(menu_counter,arrow_pos_global);
                                state_code = ST_SET_UP_CLR_COUNTS;						     
                                break;	

                case ESC_KEY   : 
                                Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                arrow_pos_global=1;
                                state_code = ST_SET_UP;
                                //----------------08_03_17_4-----ends--------------//
                                break;	

                default:
                                break;

            }//end switch(key_code)
            break;   
                
        case ST_SET_UP_VOLUME:  
            
            switch(key_code)
            {
                case UP_KEY    :  
                                  Display_String_20(volume_level,1,12,1);
                                  volume_level_global=Navigate_Menu_Counter(volume_level_global,DOWN_KEY,MAX_LIMIT_VOL);
                                  if(volume_level_global==MIN_LIMIT_MENU)//since above function is roll over this arrangment is made.
                                  volume_level_global=MAX_LIMIT_VOL;
                                  Configure_Stored_Volume_Setting(volume_level_global);
                                  Display_String_20(arrow,1,4,2);
                                  Display_Number_4digit(volume_level_global,6,2,2);
                                  Display_String_20(q,8,1,2);
                                  Gen_Tone(1,5);
                                  state_code = ST_SET_UP_VOLUME;                    		    
                                  break;

                case DOWN_KEY :  
                                  Display_String_20(volume_level,1,12,1);
                                  volume_level_global=Navigate_Menu_Counter(volume_level_global,UP_KEY,MAX_LIMIT_VOL);
                                  if(volume_level_global==MAX_LIMIT_VOL)//since above function is roll over this arrangment is made.
                                  volume_level_global=MIN_LIMIT_MENU;
                                  Configure_Stored_Volume_Setting(volume_level_global);
                                  Display_String_20(arrow,1,4,2);
                                  Display_Number_4digit(volume_level_global,6,2,2);
                                  Display_String_20(q,8,1,2);	
                                  Gen_Tone(1,5);
                                  state_code = ST_SET_UP_VOLUME;            
                                  break;	

                case ENTER_KEY  :
                                  E2prom_Byte_Write(e2prom_2_page_2_add,((volume_setting_address)&mask_higher_byte) >> 8,(volume_setting_address)&mask_lower_byte,volume_level_global);//11/10/11.
                                  volume_level_global=E2prom_Byte_Read(e2prom_2_page_2_add,((volume_setting_address)& mask_higher_byte)>>8,(volume_setting_address)& mask_lower_byte);//11/10/11.
                                  Display_String_20(selected,1,8,2);
                                  DelayMs(700);
                                  state_code = ST_SET_UP;		
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);			     
                                  break;

                case ESC_KEY:    
                                  state_code = ST_SET_UP;//before required
                                  Initialize_Lcd();//FACT_512_13.
                                  Remove_Blinking_Cursor();//FACT_512_13.
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  break;	

                 default:
                                  break;

            }
            break;  

        case ST_SET_UP_CALIBRATE  :
                                            
            switch(key_code)
            {
                case UP_KEY      :      
                                    input_resistance_value=Navigate_Counter_4_Digit(input_resistance_value,DOWN_KEY,cursor_position_global);
                                    Scroll_IR_Calibrate_Hvct(input_resistance_value);
                                    state_code = ST_SET_UP_CALIBRATE; 
                                    break;

                case DOWN_KEY    :
                                    input_resistance_value=Navigate_Counter_4_Digit(input_resistance_value,UP_KEY,cursor_position_global);
                                    Scroll_IR_Calibrate_Hvct(input_resistance_value);
                                    state_code = ST_SET_UP_CALIBRATE; 
                                    break;

                case ENTER_KEY   :          
                                        if(input_resistance_value> MAX_IR_LIMIT && cursor_position_global==0x04)
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            Display_String_20(lmt_exceeds,1,20,2);
                                            DelayS(1);	
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_CALIBRATE;
                                            Pre_Display_Before_State(ST_SET_UP_CALIBRATE,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if(input_resistance_value == 0 && cursor_position_global==0x04)
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_CALIBRATE;
                                            Pre_Display_Before_State(ST_SET_UP_CALIBRATE,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if(cursor_position_global==0x04)
                                        {                                                                                                                                                        
                                            if( calibrate_flag_glb == TRUE)
                                            {
                                                cursor_position_global=1;                                                                                                                                                                                                                                      
                                                Remove_Blinking_Cursor();
                                                input_resistance_value_y2 = input_resistance_value ;
                                                hv_msb_glb = ((0xFF00 & input_resistance_value) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
                                                hv_lsb_glb = (0x00FF & input_resistance_value);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

                                                E2prom_Byte_Write(e2prom_2_page_1_add,((input_resistance_value_y2_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(input_resistance_value_y2_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
                                                E2prom_Byte_Write(e2prom_2_page_1_add,((input_resistance_value_y2_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(input_resistance_value_y2_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);
                                                Display_String_20(measure_r2,1,20,3);
                                                HV_IR_test_1(1);
                                                Calibration( );
                                                state_code = ST_SET_UP;
                                                Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);	
                                                calibrate_flag_glb = FALSE ;
                                                break;
                                            }
                                            if( calibrate_flag_glb == FALSE)
                                            {
                                                cursor_position_global=1;                                                                                                                                                                                                                                      
                                                Remove_Blinking_Cursor();
                                                input_resistance_value_y1 = input_resistance_value ;
                                                hv_msb_glb = ((0xFF00 & input_resistance_value) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
                                                hv_lsb_glb = (0x00FF & input_resistance_value);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

                                                E2prom_Byte_Write(e2prom_2_page_1_add,((input_resistance_value_y1_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(input_resistance_value_y1_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
                                                E2prom_Byte_Write(e2prom_2_page_1_add,((input_resistance_value_y1_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(input_resistance_value_y1_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);
                                                Display_String_20(measure_r1,1,20,3);
                                                HV_IR_test_1(1);
                                                //Calibration( );
                                                input_resistance_value = 0;
                                                state_code = ST_SET_UP_CALIBRATE;
                                                calibrate_flag_glb = TRUE ;
                                                Pre_Display_Before_State(ST_SET_UP_CALIBRATE,TRUE,local_ram);

                                            }
                                         }//end else if(cursor_position_global==0x03)
                                        else if(cursor_position_global<=0x04)     
                                        {
                                            cursor_position_global++;
                                            Start_Cursor_Blinking(cursor_position_global+10,3);
                                            state_code = ST_SET_UP_CALIBRATE;      
                                        }//end else if(cursor_position_global<=0x03)     
                                        break;

                case ESC_KEY     :      
                                        Remove_Blinking_Cursor();
                                        cursor_position_global=1;
                                        state_code = ST_SET_UP;
                                        Initialize_Lcd();
                                        Remove_Blinking_Cursor();
                                        Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                        calibrate_flag_glb = FALSE ;
                                        break;

            }
            break; 

        case ST_SET_UP_FACTORY_SETT:  
            
            switch(key_code)
            {
                case UP_KEY    : 
                                  state_code = ST_SET_UP_FACTORY_SETT;                    		    
                                  break;

                case DOWN_KEY :  
                                  state_code = ST_SET_UP_FACTORY_SETT;           
                                  break;

                case ENTER_KEY  :
                                  state_code=ST_SET_UP;	
                                  Run_Factory_Settings();//FACT_512_2		
                                  asm("RESET");//FACT_512_2			     
                                  break;	

                case ESC_KEY:   
                                  state_code = ST_SET_UP;                                                 
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  break;	

                default:
                                  break;

            }
            break;
            
        case ST_SET_UP_LRN_LABEL:  
            
            switch(key_code)
            {
                case UP_KEY    : 
                                  state_code = ST_SET_UP_LRN_LABEL;                    		    
                                  break;

                case DOWN_KEY :  
                                  state_code = ST_SET_UP_LRN_LABEL;           
                                  break;

                case ENTER_KEY  :
                                  Clear_Display();
                                  
                                  Fill_Any_Char_Ram_With(digit_ram_1, 2050, 0x00);
                                  receive_label_data_glb = Receive_Data_Char_Ram(digit_ram_1,LABEL_DATA_SIZE);
                                  if(receive_label_data_glb==TRUE)
                                  {
                                      Store_Label_Data_In_E2prom( );
                                      Clear_Display();
                                      Display_String_20(received,5,13,2);
                                      Gen_Tone(2,4);
                                      DelayS(2);
                                  }
                                  state_code=ST_SET_UP;	
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  break;	

                case ESC_KEY:   
                                  state_code = ST_SET_UP;                                                 
                                  Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                  break;	

                default:
                                  break;

            }
            break;
     /*-----------------------------------------------------------------------------------------*/
     
        case ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT:
            
            switch(key_code)
            {
                case UP_KEY     :
                                    withstand_last_ckt_no_glb=Navigate_Counter_3_Digit(withstand_last_ckt_no_glb,DOWN_KEY,cursor_position_global);
                                    Scroll_last_ckt_Hvct(withstand_last_ckt_no_glb);
                                    state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT;                    		    
                                    break;
                                
                case DOWN_KEY   :
                                    withstand_last_ckt_no_glb=Navigate_Counter_3_Digit(withstand_last_ckt_no_glb,UP_KEY,cursor_position_global);
                                    Scroll_last_ckt_Hvct(withstand_last_ckt_no_glb);
                                    state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT;   
                                    break;
                                
                case ENTER_KEY  :
                                    if(withstand_last_ckt_no_glb> MAX_CKT_NO && cursor_position_global==0x03)
                                    {
                                      Clear_Display();
                                      Remove_Blinking_Cursor();
                                      Display_String_20(lmt_exceeds,1,20,2);
                                      DelayS(1);	
                                      cursor_position_global=1;
                                      state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT;
                                      Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT,TRUE,local_ram);
                                    }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                    if(withstand_last_ckt_no_glb == 0 && cursor_position_global==0x03)
                                    {
                                      Clear_Display();
                                      Remove_Blinking_Cursor();
                                      cursor_position_global=1;
                                      state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT;
                                      Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT,TRUE,local_ram);
                                    }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                    else if(cursor_position_global==0x03)
                                          {
                                            cursor_position_global=1;
                                            Remove_Blinking_Cursor();
                                            E2prom_Byte_Write(e2prom_2_page_2_add,((withstand_last_ckt_number_base_address+(current_cable_number-1))&mask_higher_byte) >> 8,(withstand_last_ckt_number_base_address+(current_cable_number-1))&mask_lower_byte,withstand_last_ckt_no_glb);//11/10/11.
//                                            Display_String_20(space,1,20,2);
//                                            Display_String_20(space,1,20,3);
//                                            Display_String_20(space,1,20,4);
//                                            Display_String_20(selected,1,8,2);
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE;
//                                            DelayS(1);
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE,TRUE,local_ram);	
                                           }//end else if(cursor_position_global==0x03)
                                    else if(cursor_position_global<=0x03)     
                                           {
                                            cursor_position_global++;
                                            Start_Cursor_Blinking(cursor_position_global+8,2);
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT;
                                           }//end else if(cursor_position_global<=0x03)     
                                           else;
                                    break;
                                    
                case ESC_KEY    :
                                    Remove_Blinking_Cursor();
                                    cursor_position_global=1;
                                    state_code = ST_SET_UP;
                                    Initialize_Lcd();
                                    Remove_Blinking_Cursor();
                                    Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                    break;
                                    
                default         :
                                    break;
                                    
            }
            break;
            
        case ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE  :
            
            switch(key_code)
            {
                case UP_KEY      :      
                                    withstand_hv_voltage_glb=Navigate_Counter_4_Digit(withstand_hv_voltage_glb,DOWN_KEY,cursor_position_global);
                                    Scroll_voltage_Hvct(withstand_hv_voltage_glb);
                                    state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE; 
                                    break;
                                    
                case DOWN_KEY    :
                                    withstand_hv_voltage_glb=Navigate_Counter_4_Digit(withstand_hv_voltage_glb,UP_KEY,cursor_position_global);
                                    Scroll_voltage_Hvct(withstand_hv_voltage_glb);
                                    state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE; 
                                    break;
                                    
                case ENTER_KEY   :          
                                        if((withstand_hv_voltage_glb > MAX_VOLTAGE_LIMIT) && (cursor_position_global == 0x04))
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            Display_String_20(lmt_exceeds,1,20,2);
                                            DelayS(1);	
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if((withstand_hv_voltage_glb < MIN_VOLTAGE_LIMIT) && (cursor_position_global == 0x04))
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            Display_String_20(minimum_voltage,1,16,2);
                                            DelayS(1);	
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if(cursor_position_global==0x04)
                                        {
                                            cursor_position_global=1;                                                                                                                                                                                                                                      
                                            Remove_Blinking_Cursor();
                                            hv_msb_glb = ((0xFF00 & withstand_hv_voltage_glb) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
                                            hv_lsb_glb = (0x00FF & withstand_hv_voltage_glb);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

                                            E2prom_Byte_Write(e2prom_2_page_2_add,((withstand_high_voltage_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(withstand_high_voltage_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
                                            E2prom_Byte_Write(e2prom_2_page_2_add,((withstand_high_voltage_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(withstand_high_voltage_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);

                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT,TRUE,local_ram);	
                                         }//end else if(cursor_position_global==0x03)
                                        else if(cursor_position_global<=0x04)     
                                        {
                                            cursor_position_global++;
                                            Start_Cursor_Blinking(cursor_position_global+15,2);
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE;
                                        }//end else if(cursor_position_global<=0x03)     
                                               else;
                                        break;
                                        
                case ESC_KEY     :      
                                        Remove_Blinking_Cursor();
                                        cursor_position_global=1;
                                        state_code = ST_SET_UP;
                                        Initialize_Lcd();
                                        Remove_Blinking_Cursor();
                                        Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                        break;
                                        
            }
            break;

        case ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT  :
            
            switch(key_code)
            {
                case UP_KEY      :      
                                    withstand_current_glb=Navigate_Counter_4_Digit(withstand_current_glb,DOWN_KEY,cursor_position_global);
                                    Scroll_Current_Withstand(withstand_current_glb);
                                    state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT; 
                                    break;

                case DOWN_KEY    :
                                    withstand_current_glb=Navigate_Counter_4_Digit(withstand_current_glb,UP_KEY,cursor_position_global);
                                    Scroll_Current_Withstand(withstand_current_glb);
                                    state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT; 
                                    break;

                case ENTER_KEY   :          
                                        if(withstand_current_glb> MAX_CURRENT_LIMIT && cursor_position_global==0x04)
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            Display_String_20(lmt_exceeds,1,20,2);
                                            DelayS(1);	
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if(withstand_current_glb == 0 && cursor_position_global==0x04)
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if(cursor_position_global==0x04)
                                        {
                                            cursor_position_global=1;                                                                                                                                                                                                                                      
                                            Remove_Blinking_Cursor();
                                            hv_msb_glb = ((0xFF00 & withstand_current_glb) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
                                            hv_lsb_glb = (0x00FF & withstand_current_glb);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

                                            E2prom_Byte_Write(e2prom_2_page_2_add,((withstand_current_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(withstand_current_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
                                            E2prom_Byte_Write(e2prom_2_page_2_add,((withstand_current_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(withstand_current_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);

                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_TIME;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_TIME,TRUE,local_ram);	
                                         }//end else if(cursor_position_global==0x03)
                                        else if(cursor_position_global<=0x04)     
                                        {
                                            cursor_position_global++;
                                            Start_Cursor_Blinking(cursor_position_global+4,3);
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT;
                                        }//end else if(cursor_position_global<=0x03)     
                                               else;
                                        break;

                case ESC_KEY     :      
                                        Remove_Blinking_Cursor();
                                        cursor_position_global=1;
                                        state_code = ST_SET_UP;
                                        Initialize_Lcd();
                                        Remove_Blinking_Cursor();
                                        Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                        break;

            }
             break;  
                 
        case ST_SET_UP_HVCT_WITHSTAND_DATA_TIME  :
            
            switch(key_code)
            {
                case UP_KEY      :      
                                    hv_withstand_time_glb=Navigate_Counter_5_Digit(hv_withstand_time_glb,DOWN_KEY,cursor_position_global);
                                    Scroll_Time_Hvct(hv_withstand_time_glb);
                                    state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_TIME; 
                                    break;

                case DOWN_KEY    :
                                    hv_withstand_time_glb=Navigate_Counter_5_Digit(hv_withstand_time_glb,UP_KEY,cursor_position_global);
                                    Scroll_Time_Hvct(hv_withstand_time_glb);
                                    state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_TIME; 
                                    break;

                case ENTER_KEY   :          
                                        if(hv_withstand_time_glb > MAX_WITHSTAND_TIME_LIMIT && cursor_position_global==0x05)
                                        {
                                            Clear_Display();
                                            Remove_Blinking_Cursor();
                                            Display_String_20(lmt_exceeds,1,20,2);
                                            DelayS(1);	
                                            hv_withstand_time_glb = 0;
                                            cursor_position_global=1;
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_TIME;
                                            Pre_Display_Before_State(ST_SET_UP_HVCT_WITHSTAND_DATA_TIME,TRUE,local_ram);
                                        }//end if(current_prg_local>max_number_programs && cursor_position_global==0x03) 
                                        else if((cursor_position_global==0x05)&&(hv_withstand_time_glb <= MAX_WITHSTAND_TIME_LIMIT))
                                        {
                                            cursor_position_global=1;                                                                                                                                                                                                                                      
                                            Remove_Blinking_Cursor();
                                            hv_msb_glb = ((0xFF00 & hv_withstand_time_glb) >> 8);    //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom
                                            hv_lsb_glb = (0x00FF & hv_withstand_time_glb);         //high voltage value is int hence masking upper and lower byte to store separate bytes in eprom

                                            E2prom_Byte_Write(e2prom_2_page_2_add,((withstand_time_base_address+(current_cable_number*2-2))&mask_higher_byte) >> 8,(withstand_time_base_address+(current_cable_number*2-2))&mask_lower_byte,hv_msb_glb);
                                            E2prom_Byte_Write(e2prom_2_page_2_add,((withstand_time_base_address+(current_cable_number*2-1))&mask_higher_byte) >> 8,(withstand_time_base_address+(current_cable_number*2-1))&mask_lower_byte,hv_lsb_glb);
                                            Display_String_20(space,1,20,2);
                                            Display_String_20(space,1,20,3);
                                            Display_String_20(space,1,20,4);
                                            Display_String_20(selected,1,8,2);
                                            DelayMs(700);
                                            state_code = ST_SET_UP;
                                            Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);	
                                         }//end else if(cursor_position_global==0x03)
                                        else if(cursor_position_global<=0x05)     
                                        {
                                            cursor_position_global++;
                                            Start_Cursor_Blinking(cursor_position_global+13,3);
                                            state_code = ST_SET_UP_HVCT_WITHSTAND_DATA_TIME;
                                        }//end else if(cursor_position_global<=0x03)     
                                               else;
                                        break;

                case ESC_KEY     :      
                                        Remove_Blinking_Cursor();
                                        cursor_position_global=1;
                                        state_code = ST_SET_UP;
                                        Initialize_Lcd();
                                        Remove_Blinking_Cursor();
                                        Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
                                        break;

            }
            break;
       
    /*-----------------------------------------------------------------------------------------*/
                                         
        case ST_IDLE_COUNT:  

            switch(key_code)
            {
                case UP_KEY    : 
                                  state_code = ST_IDLE_COUNT;                    		    
                                  break;

                case DOWN_KEY :  
                                  state_code = ST_IDLE_COUNT;            
                                  break;	

                case ENTER_KEY  : state_code = ST_IDLE_COUNT;  					     
                                  break;	

                case ESC_KEY:   
                                 Initialize_Lcd();//FACT_512_13.
                                 Remove_Blinking_Cursor();//FACT_512_13.
                                 Pre_Display_Before_State(ST_IDLE,TRUE,local_ram);
                                 state_code =ST_IDLE;
                                  break;	

                default:
                                  break;

            }
            break;        
        
    }
//    key_pressed_flag=FALSE;
//	key_code=0x00;
}

                                         
                                           /*
 Function:This function resets testers to factory settings.It sets general settings,clears pass,fail
          counts and serial number in label,set relay on time,and volume also. 
 Input:none.
 Output:sets various settings and counts.
 Calls:None
 Called By:....
*/
//09_03_15_5.
//void Run_Factory_Settings(void)
//{
//	unsigned char index_lcl=1;
//
//    Clear_Display();
//    Display_String_20(reset,1,14,2);
//	//---------------------volume and tone--start----------------------//
//	Configure_Stored_Volume_Setting(0);//pass 0 for setting volume level to 5.
//	//set type 1 tones.
//	E2prom_Byte_Write(e2prom_2_page_2_add,((tone_address)&mask_higher_byte) >> 8,(tone_address)&mask_lower_byte,1);
//	//---------------------volume and tone----end----------------------//
//
//    E2prom_Byte_Write(e2prom_2_page_2_add,((gen_settings_base_address)&mask_higher_byte) >> 8,(gen_settings_base_address)&mask_lower_byte,RESET);
//    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
//    {
//        E2prom_Byte_Write(e2prom_2_page_2_add,((test_setting_base_address)&mask_higher_byte) >> 8,(test_setting_base_address)&mask_lower_byte,RESET);
//    }
//    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
//    {
//        E2prom_Byte_Write(e2prom_2_page_2_add,((test_setting_base_address +1 )&mask_higher_byte) >> 8,(test_setting_base_address + 1)&mask_lower_byte,RESET);
//    }
//    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
//    {
//        E2prom_Byte_Write(e2prom_2_page_2_add,((withstand_test_setting_base_address + index_lcl)&mask_higher_byte) >> 8,(withstand_test_setting_base_address + index_lcl) & mask_lower_byte,RESET);
//    }
//    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
//    {
//        E2prom_Byte_Write(e2prom_2_page_2_add,((ir_test_setting_base_address +index_lcl)&mask_higher_byte) >> 8,(ir_test_setting_base_address + index_lcl) & mask_lower_byte,SET);
//    }
//	//---------------------pass count-----start----------------//
//	Read_Write_RTC(digit_ram,pass_count_bbram_base_address,pass_fail_count_size,WRITE);
//	for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
//		Write_Bulk_Data_Eeprom(e2prom_2_page_2_add,pass_count_base_address,pass_fail_count_size,index_lcl,digit_ram);//write in eeprom//11/01/11.
//	//---------------------pass count-----end------------------//
//	//---------------------fail count-----start----------------//
//	Read_Write_RTC(digit_ram,fail_count_bbram_base_address,pass_fail_count_size,WRITE);
//	for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
//		Write_Bulk_Data_Eeprom(e2prom_2_page_2_add,fail_count_base_address,pass_fail_count_size,index_lcl,digit_ram);//write in eeprom.
//	//---------------------fail count-----end------------------//
//	//set relay on time 1 sec.
//	E2prom_Byte_Write(e2prom_2_page_2_add,((relay_1_on_time_address)&mask_higher_byte) >> 8,(relay_1_on_time_address)&mask_lower_byte,1);
//	E2prom_Byte_Write(e2prom_2_page_2_add,((relay_2_on_time_address)&mask_higher_byte) >> 8,(relay_2_on_time_address)&mask_lower_byte,1);
//    
//
//    
//}//end function..


void Run_Factory_Settings(void)
{
	unsigned char index_lcl=1,local_ram[16];

    Clear_Display();
    Display_String_20(reset,1,14,2);
	//---------------------volume and tone--start----------------------//
	Configure_Stored_Volume_Setting(0);//pass 0 for setting volume level to 5.
	//set type 1 tones.
	E2prom_Byte_Write(e2prom_2_page_2_add,((tone_address)&mask_higher_byte) >> 8,(tone_address)&mask_lower_byte,1);
	//---------------------volume and tone----end----------------------//

    //--------------------------------------Test setting-------start-------------------------------------------------------//
    //Display result Setting
    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
    {
        E2prom_Byte_Write(e2prom_2_page_2_add,((test_setting_base_address)&mask_higher_byte) >> 8,(test_setting_base_address)&mask_lower_byte,RESET);
    }
    //Stop at 1st fault Setting
    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
    {
        E2prom_Byte_Write(e2prom_2_page_2_add,((test_setting_base_address +1 )&mask_higher_byte) >> 8,(test_setting_base_address + 1)&mask_lower_byte,RESET);
    }
    //Remote key setting
    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
    {
        E2prom_Byte_Write(e2prom_2_page_2_add,((test_setting_base_address +2 )&mask_higher_byte) >> 8,(test_setting_base_address + 2)&mask_lower_byte,RESET);
    }
    //Fail Bin setting
    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
    {
        E2prom_Byte_Write(e2prom_2_page_2_add,((test_setting_base_address +3 )&mask_higher_byte) >> 8,(test_setting_base_address + 3)&mask_lower_byte,RESET);
    }
    //--------------------------------------Test setting-------end-------------------------------------------------------//

    //-------------------------------------General setting-------end-------------------------------------------------------//
    //Label print Setting
    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
    {
         E2prom_Byte_Write(e2prom_2_page_2_add,((gen_settings_base_address)&mask_higher_byte) >> 8,(gen_settings_base_address)&mask_lower_byte,RESET);
    }
    //-------------------------------------General setting-------end-------------------------------------------------------//
    
    //--------------------------------------Program setting-------start-------------------------------------------------------//
    //withstand Test Setting
    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
    {
        E2prom_Byte_Write(e2prom_2_page_2_add,((withstand_test_setting_base_address + index_lcl)&mask_higher_byte) >> 8,(withstand_test_setting_base_address + index_lcl) & mask_lower_byte,RESET);
    }
    // IR Test Setting
    for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
    {
        E2prom_Byte_Write(e2prom_2_page_2_add,((ir_test_setting_base_address +index_lcl)&mask_higher_byte) >> 8,(ir_test_setting_base_address + index_lcl) & mask_lower_byte,SET);
    }
    //--------------------------------------Program setting-------end-------------------------------------------------------//
	
    //---------------------pass count-----start----------------//
	Read_Write_RTC(digit_ram,pass_count_bbram_base_address,pass_fail_count_size,WRITE);
	for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
		Write_Bulk_Data_Eeprom(e2prom_2_page_2_add,pass_count_base_address,pass_fail_count_size,index_lcl,digit_ram);//write in eeprom//11/01/11.
	//---------------------pass count-----end------------------//
	//---------------------fail count-----start----------------//
	Read_Write_RTC(digit_ram,fail_count_bbram_base_address,pass_fail_count_size,WRITE);
	for(index_lcl=1;index_lcl<=MAX_PROGRAM_NO;index_lcl++)//for(index_lcl=1;index_lcl<=max_number_programs;index_lcl++)
		Write_Bulk_Data_Eeprom(e2prom_2_page_2_add,fail_count_base_address,pass_fail_count_size,index_lcl,digit_ram);//write in eeprom.
	//---------------------fail count-----end------------------//
    
    //--------------------------------------Relay setting------start-------------------------------------------------------//
	//set relay on time 1 sec.
	E2prom_Byte_Write(e2prom_2_page_2_add,((relay_1_on_time_address)&mask_higher_byte) >> 8,(relay_1_on_time_address)&mask_lower_byte,1);
	E2prom_Byte_Write(e2prom_2_page_2_add,((relay_2_on_time_address)&mask_higher_byte) >> 8,(relay_2_on_time_address)&mask_lower_byte,1);
    //--------------------------------------Relay setting-------end-------------------------------------------------------//
  
    //--------------------------------------Printer Setting------start-------------------------------------------------------//
    //Serial number reset
    Fill_Any_Char_Ram_With(local_ram,16,0x30);
    Read_Write_RTC(local_ram,serial_number_bbram_base_address,9,WRITE);
    //--------------------------------------Printer setting-------end-------------------------------------------------------//
}//end function..









/*
 Function:This function will assign updated value for menu counter on every call depending on key type.
 Input:menu counter as a local(unsigned char),key type(unsigned char type),maximum boundary limit(unsigned char type) 
 Output:returns updated value of menu counter (unsigned char type)
 Calls:None
 Called By:....
*/
unsigned char Navigate_Menu_Counter(unsigned char counter_local,unsigned char key_local,unsigned char max_limit_local)
{
	
	switch(key_local)
	{
		case UP_KEY:counter_local--;
					break;

		case DOWN_KEY:counter_local++;
					break;

		case ENTER_KEY:
					break;

		case ESC_KEY:
					break;

		default:
			break;	
	}	
	if(counter_local>max_limit_local)
	{
		counter_local=MIN_LIMIT_MENU;
	}
	else if(counter_local<MIN_LIMIT_MENU)
	{
		counter_local=max_limit_local;	
	}
	return(counter_local);

}

/*
 Function:This function will navigate arrow cursor and calls Display_Screen functon with appropriate input.
		  a) arrow '>' will be navigated to appropriate position on behalf of 'UP' or 'DOWN' key.
		  	 this includes disappering of older arrow position and display to current position.
		  b) if arrow is gone bayond maximum display limit then scroll the display by triggering display_screen function. 
		  c) for  boundary value menu limit,provide appropriate inputs so that roll over is occured. 
		  
 Input: menu counter is an input, indication cuttent menu being present on arrow(unsigned char).
 Output:scrolling display with roll over.
 Calls:Display_Screen_Main_menu(),SetDDRamAddr_PIC30(),WriteDataXLCD_PIC30().
 Called By:
*/
unsigned char Display_Navigation(unsigned char counter_local,unsigned char key_code_local,unsigned char max_limit_local)
{

	unsigned char dd_ram_add_curr_local=0,dd_ram_add_prev_local=0,dd_ram_add_fowr_local=0;
	//--------------------------------------------------------------------------//
	//Display next screen only when arrow scrolls beyod screen.
	switch(key_code_local)
	{
		case DOWN_KEY:arrow_position++;
					  if(arrow_position>=7)
					  {
						arrow_position=5;
						top_position++;												     //increment menu position..locally.
						if(top_position>max_limit_local/2)
						{
								top_position=1;											//when menu limit is over initialise to one.
						}
						Display_Screen_Main_menu(top_position,max_limit_local);		    //display screen.
					 }				
			 		 break;
		case UP_KEY:arrow_position--;
					if(arrow_position<1)
					{
						arrow_position=1;
						counter_local=Navigate_Menu_Counter(counter_local,UP_KEY,max_limit_local);//to match limits
						top_position--;												//decrement menu position..locally.
						if(top_position < MIN_LIMIT_MENU)
						{
							top_position=max_limit_local/2;							//when menu limit is 1 initialise to max value.
						}
						Display_Screen_Main_menu(top_position,max_limit_local);		//display screen.
					}
					break;	
		case ENTER_KEY: arrow_position=1;
						top_position=1;
						Display_Screen_Main_menu(top_position,max_limit_local);	
			 		break;
		case ESC_KEY:	arrow_position=1;
						top_position=1;
						Display_Screen_Main_menu(top_position,max_limit_local);
					break;	
	}		
	//----------------------------------------------------------------------------------//
	//display arrow to current position and erase previous values.
	switch(arrow_position)
	{
		
		case 1: dd_ram_add_curr_local=0x40;
				dd_ram_add_prev_local=0x5E;
				dd_ram_add_fowr_local=0x4A;
				break;
		case 2: dd_ram_add_curr_local=0x4A;
				dd_ram_add_prev_local=0x40;
				dd_ram_add_fowr_local=0x14;
				break;
		case 3: dd_ram_add_curr_local=0x14;
				dd_ram_add_prev_local=0x4A;
				dd_ram_add_fowr_local=0x1E;
				break;
		case 4: dd_ram_add_curr_local=0x1E;
				dd_ram_add_prev_local=0x14;
				dd_ram_add_fowr_local=0x54;
				break;
		case 5: dd_ram_add_curr_local=0x54;
				dd_ram_add_prev_local=0x1E;
				dd_ram_add_fowr_local=0x5E;
				break;
		case 6:	dd_ram_add_curr_local=0x5E;
				dd_ram_add_prev_local=0x54;
				dd_ram_add_fowr_local=0x40;
				break;
		default://dd_ram_add_curr_local=0x40;
				break;
	}
	SetDDRamAddr_PIC30(dd_ram_add_prev_local);
	WriteDataXLCD_PIC30(' ');
	SetDDRamAddr_PIC30(dd_ram_add_fowr_local);
	WriteDataXLCD_PIC30(' ');
	SetDDRamAddr_PIC30(dd_ram_add_curr_local);
	WriteDataXLCD_PIC30('>');
	//-------------------------------------------------------------------------------//
	return(counter_local);
}


/*
 Function:Pre_Display_Before_State(unsigned char counter_local,unsigned char normal_display_flag_local,unsigned char* local_ram)
 Input:  state is the input.
 Output: This function will displays menus related to state before actually going into that state
 Calls:Clear_Display();Display_Navigation(meun_counter,ENTER_KEY,MAX_LIMIT_MAIN_MENU), Display_Number_4digit();
 Called By:
*/
void Pre_Display_Before_State(unsigned char counter_local,unsigned char normal_display_flag_local,unsigned char* local_ram)
{
	//unsigned char lcl_ram[10];
	menu_counter=MIN_LIMIT_MENU;	
	if(normal_display_flag_local==TRUE)
	{
		Clear_Display();
		
	}//end if(normal_display_flag_local==TRUE)
	switch(counter_local)
	{
		case ST_IDLE            : 	
                                    Display_String_20(menu,2,5,1);
                                    Display_String_20(main_1,7,4,1);
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    disp_start_ptr = start_main_menu;
                                    Display_Navigation(menu_counter,ENTER_KEY,MAX_LIMIT_MAIN_MENU);
                                    state_code = ST_IDLE;
                                    break;
        case ST_IDLE_TEST       :
                                    Display_String_20(prgno,1,16,1);
                                    Display_String_20(pn,16,2,1);
                                    Check_circuit_number( );
                                    Display_String_20(lst_ckt,1,8,2);
                                    Display_Number_4digit(last_ckt_no_glb,9,3,2);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    Display_String_20(test_continuity,1,15,3);
                                    Display_String_20(press_ent_start,1,18,4);
                                    state_code = ST_IDLE_TEST;
                                    break;
        case ST_IDLE_SELECT_PRG :
                                    Display_String_20(prgno,1,16,1);
                                    Display_Number_4digit(current_cable_number,17,3,1);
                                    Display_String_20(enter_program_no,2,16,2);
                                    Display_String_20(and,8,3,3);
                                    Display_String_20(press_ent_sel,1,19,4);
                                    Start_Cursor_Blinking(cursor_position_global+16,1);
                                    break;
                                    
        case ST_SET_UP          : 
                                    Display_String_20(menu,2,5,1);
                                    Display_String_20(start_main_menu+36,7,9,1);   
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    
                                    disp_start_ptr=start_setup_menu;      
                                   
                                    Display_Navigation(menu_counter,ENTER_KEY,MAX_LIMIT_SETUP_MENU);
                                    break;
                                 
        case ST_ANALYSE://FACT_256
						
							Display_String_20(menu,2,5,1);
							Display_String_20(start_main_menu+27,7,9,1);     
							Display_String_20(pn,16,3,1);
							Display_Number_4digit(current_cable_number,18,3,1);
//							analyse_flag=TRUE;
					    	disp_start_ptr=start_analyse_menu;     
							
							Display_Navigation(menu_counter,ENTER_KEY,MAX_LIMIT_ANALYSE_MENU);
                            break;                            
        case ST_SET_UP_OTH_DATA://25/11/11.
								Display_String_20(menu,2,5,1);
								Display_String_20(start_setup_menu+9,7,9,1);
								Display_String_20(pn,16,2,1);
								Display_Number_4digit(current_cable_number,18,3,1);
								menu_counter=MIN_LIMIT_MENU;
					    		
                                //disp_start_ptr=(int)start_oth_data_menu; 
					    		disp_start_ptr=start_oth_data_menu;        
								//disp_start_ptr++;                        
                               
								Display_Navigation(menu_counter,ENTER_KEY,MAX_LIMIT_OTH_DATA_MENU);	
                                break;	                    
        case ST_SET_UP_HVCT_DATA_VOLTAGE:   
                                    Display_String_20(menu,1,5,1);
                                    Display_String_20(ir_data,6,7,1);
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    Display_String_20(lst_ckt,1,8,2);
                                    Display_Number_4digit(ir_last_ckt_no_glb,9,3,2);
                                    Display_String_20(hv,13,3,2);
                                    Display_String_20(volts,20,1,2);
                                    Display_Number_4digit(hv_voltage_glb,16,4,2);
                                    Display_String_20(ir,1,3,3);
//                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    Display_String_20(mohm,8,4,3);
                                    Display_Number_4digit(insulation_resistance_glb,4,4,3);
//                                    Display_String_20(time,13,2,3);
//                                    Display_String_20(sec,19,2,3);
//                                    Display_Number_4digit(hv_withstand_time_glb,15,4,3);
                                    Start_Cursor_Blinking(cursor_position_global+15,2);
                                    break;
                                    
        case ST_SET_UP_HVCT_DATA_LAST_CKT:   
                                    Display_String_20(menu,1,5,1);
                                    Display_String_20(ir_data,6,7,1);
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    Display_String_20(lst_ckt,1,8,2);
                                    Display_Number_4digit(ir_last_ckt_no_glb,9,3,2);
                                    Display_String_20(hv,13,3,2);
                                    Display_String_20(volts,20,1,2);
                                    Display_Number_4digit(hv_voltage_glb,16,4,2);
                                    Display_String_20(ir,1,3,3);
                                    Display_String_20(mohm,8,4,3);
                                    Display_Number_4digit(insulation_resistance_glb,4,4,3);
//                                    Display_String_20(time,13,2,3);
//                                    Display_String_20(sec,19,2,3);
//                                    Display_Number_4digit(hv_withstand_time_glb,15,4,3);
                                    Start_Cursor_Blinking(cursor_position_global+8,2);
                                    break;  
                                    
        case ST_SET_UP_HVCT_DATA_IR:   
                                    Display_String_20(menu,1,5,1);
                                    Display_String_20(ir_data,6,7,1);
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    Display_String_20(lst_ckt,1,8,2);
                                    Display_Number_4digit(ir_last_ckt_no_glb,9,3,2);
                                    Display_String_20(hv,13,3,2);
                                    Display_String_20(volts,20,1,2);
                                    Display_Number_4digit(hv_voltage_glb,16,4,2);
                                    Display_String_20(ir,1,3,3);
                                    Display_String_20(mohm,8,4,3);
                                    Display_Number_4digit(insulation_resistance_glb,4,4,3);
//                                    Display_String_20(time,13,2,3);
//                                    Display_String_20(sec,19,2,3);
//                                    Display_Number_4digit(hv_withstand_time_glb,15,4,3);
                                    Start_Cursor_Blinking(cursor_position_global+3,3);
                                    break;                            
//        case ST_SET_UP_HVCT_DATA_TIME:   
//                                    Display_String_20(menu,1,5,1);
//                                    Display_String_20(ir_data,6,7,1);
//                                    Display_String_20(pn,16,2,1);
//                                    Display_Number_4digit(current_cable_number,18,3,1);
//                                    Display_String_20(lst_ckt,1,8,2);
//                                    Display_Number_4digit(ir_last_ckt_no_glb,9,3,2);
//                                    Display_String_20(hv,13,3,2);
//                                    Display_String_20(volts,20,1,2);
//                                    Display_Number_4digit(hv_voltage_glb,16,4,2);
//                                    Display_String_20(ir,1,3,3);
//                                    Display_String_20(mohm,8,4,3);
//                                    Display_Number_4digit(insulation_resistance_glb,4,4,3);
//                                    Display_String_20(time,13,2,3);
//                                    Display_String_20(sec,19,2,3);
//                                    Display_Number_4digit(hv_withstand_time_glb,15,4,3);
//                                    Start_Cursor_Blinking(cursor_position_global+14,3);
//                                    break;                            
        
        case ST_SET_UP_GEN_SETTINGS:
            					    menu_counter=MIN_LIMIT_MENU;	
									Display_String_20(menu,2,5,1);
									Display_String_20(start_analyse_menu+18,7,9,1);
									Display_Number_4digit(current_cable_number,18,3,1); 
                                    
						    		disp_start_ptr=start_gen_sett_menu; 
            
									Display_Navigation_1(menu_counter,ENTER_KEY,MAX_LIMIT_GEN_SETT_MENU);
                                    break;	

		case ST_SET_UP_TEST_SETTINGS:
									
									menu_counter=MIN_LIMIT_MENU;	
									Display_String_20(menu,2,5,1);
									Display_String_20(start_setup_menu+18,7,9,1);
									Display_Number_4digit(current_cable_number,18,3,1); 
						    		
						    		disp_start_ptr=start_test_sett_menu;        
								
									Display_Navigation_1(menu_counter,ENTER_KEY,MAX_LIMIT_TEST_SETT_MENU);
                                    break;
                                    
        case ST_SET_UP_PRG_SETTINGS:
                                    menu_counter=MIN_LIMIT_MENU;
                                    Display_String_20(menu,2,5,1);
                                    Display_String_20(start_analyse_menu+27,7,9,1);    
                                    Display_String_20(pn,16,2,1);//11/10/11.
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    disp_start_ptr=start_prg_sett_menu;       
                                    Display_Navigation_1(menu_counter,ENTER_KEY,MAX_LIMIT_PRG_SETT_MENU);
                                    break;
        case ST_SET_UP_DIAGNO:
								Display_String_20(menu,2,5,1);
								Display_String_20(start_setup_menu+45,7,9,1);  //diagno_n 17_05_18_1_e
								Display_String_20(pn,16,2,1);//11/10/11.
								Display_Number_4digit(current_cable_number,18,3,1);//11/10/11.   
								//------------17_05_18_1_e----start------//
								//disp_start_ptr=(int)start_diagno_menu; //earlier.
                                disp_start_ptr=start_diagno_menu;        //current.
							    //disp_start_ptr++;                      //earlier.
                                //------------17_05_18_1_e----end--------//
							    Display_Navigation(menu_counter,ENTER_KEY,MAX_LIMIT_DIAGNO_MENU);
								state_code=ST_SET_UP_DIAGNO;  
                                break;
        case ST_SET_UP_VOLUME:		
								volume_level_global= E2prom_Byte_Read(e2prom_2_page_2_add,(volume_setting_address & mask_higher_byte) >> 8,(volume_setting_address & mask_lower_byte));//A VERSION
								//Fill_Small_Data_Ram(local_ram,RING_CONST,E2prom_Byte_Read(e2prom_2_page_2_add,(tone_address & mask_higher_byte) >> 8,(tone_address & mask_lower_byte)),1,1,3);//A VERSION
								//if(!Send_Bulk_Data_To_I2C_Slave_A(0xE0,local_ram,5));//A VERSION
								Display_String_20(volume_level,1,12,1);
								Display_String_20(arrow,1,4,2);
								Display_Number_4digit(volume_level_global,6,2,2);
								Display_String_20(q,8,1,2);
								menu_counter=MIN_LIMIT_MENU;		
                                break;
        case ST_SET_UP_FUTURA_DND:  
                                    menu_counter=MIN_LIMIT_MENU;
									Display_String_20(menu,2,5,1);
									Display_String_20(start_setup_menu+54,7,9,1);   //f_dnd_n 17_05_18_1_e.
									Display_String_20(pn,16,2,1);
									Display_Number_4digit(current_cable_number,18,3,1);  
									disp_start_ptr=start_fdnd_menu;        
							    	Display_Navigation(menu_counter,ENTER_KEY,MAX_LIMIT_FDND_MENU);
                                	break;
            
//                                Clear_Display();
//                                Display_String_20(notavailable,1,13,2);
//                                DelayS(1);
//                                Pre_Display_Before_State(ST_SET_UP,TRUE,local_ram);
//                                state_code = ST_SET_UP;
//                                break;
        case ST_SET_UP_RELAY_TIME: 
								//----------------28_05_13_4---------start-----------------//
								Clear_Display();
								Display_String_20(relay_on_time,1,5,1);
								Display_Number_4digit(1,7,1,1);   
								Display_String_20(relay_on_time+5,9,8,1);
								menu_counter= E2prom_Byte_Read(e2prom_2_page_2_add,(relay_2_on_time_address & mask_higher_byte) >> 8,(relay_2_on_time_address & mask_lower_byte));//11/10/11.
								Display_Number_4digit(menu_counter,6,1,4);
								Display_String_20(arrow,1,4,2);
								Display_String_20(relay_on_time,1,5,3);
								Display_Number_4digit(2,7,1,3);   
								Display_String_20(relay_on_time+5,9,8,3);
								menu_counter= E2prom_Byte_Read(e2prom_2_page_2_add,(relay_1_on_time_address & mask_higher_byte) >> 8,(relay_1_on_time_address & mask_lower_byte));
								Display_Number_4digit(menu_counter,6,1,2);
								Display_String_20(arrow,1,4,4);
								Display_String_20(q,8,1,2);
								Display_String_20(q,8,1,4);
								Start_Cursor_Blinking(6,2); 
								state_code=ST_SET_UP_RELAY_TIME;
                                break;	    
        case ST_SET_UP_CLR_COUNTS:                         
                                  Display_String_20(prgno,1,16,1);
                                  Display_Number_4digit(current_cable_number,17,3,1);
                                  disp_start_ptr=start_clr_cnt_menu;   
                                  Display_Navigation_1(menu_counter,ENTER_KEY,MAX_LIMIT_CLR_COUNTS_MENU);
                                  break;
                                  
                                  
                                  
        case ST_IDLE_COUNT:                        
                               Display_String_20(prgno,1,16,1);
						  	   Display_Number_4digit(current_cable_number,17,3,1);
							   Display_String_20(pass,2,4,2);
							   Read_Write_RTC(digit_ram,pass_count_bbram_base_address,pass_fail_count_size,READ);
							   Display_Ram(digit_ram,8,0,3,2);
							   Fill_Any_Char_Ram_With(digit_ram,32,0x00);
							   Display_String_20(fail,2,4,3);
							   Read_Write_RTC(digit_ram,fail_count_bbram_base_address,pass_fail_count_size,READ);
							   Display_Ram(digit_ram,8,0,3,3);
							   Fill_Any_Char_Ram_With(digit_ram,32,0x00);
                                break; 
      case ST_SET_UP_CALIBRATE:   
                                    Display_String_20(menu,1,5,1);
                                    Display_String_20(calibration,6,9,1);
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    if(calibrate_flag_glb == FALSE)
                                    {
                                        Display_String_20(enter_r1_value,1,10,3);
                                    }
                                    if(calibrate_flag_glb == TRUE)
                                    {
                                        Display_String_20(enter_r2_value,1,10,3);
                                    }
                                    Display_String_20(mohm,15,4,3);
                                    Display_Number_4digit(input_resistance_value,11,4,3);

                                    Start_Cursor_Blinking(cursor_position_global+10,3);//Start_Cursor_Blinking(cursor_position_global+3,3);
                                    break; 
                                    

         case ST_SET_UP_FACTORY_SETT:   
                                     Display_String_20(press_enter_to_reset,1,20,1);
                                     Display_String_20(or,9,2,2);
                                     Display_String_20(press_escape,5,14,3);
                                     Display_String_20(return_to_main_menu,2,19,4);
                                     state_code = ST_SET_UP_FACTORY_SETT; 
                                     break;
                                     
         case ST_SET_UP_LRN_LABEL:   
                                     Display_String_20(press_enter_key,4,15,2);
                                     Display_String_20(and,9,3,3);
                                     Display_String_20(send_label_file,2,19,4);
                                     state_code = ST_SET_UP_LRN_LABEL; 
                                     break;
                                     
    /*----------------------------------------------------------------------------------------------*/
         case ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT:   
                                    Display_String_20(menu,1,5,1);
                                    Display_String_20(ws_data,6,7,1);
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    Display_String_20(lst_ckt,1,8,2);
                                    Display_Number_4digit(withstand_last_ckt_no_glb,9,3,2);
                                    Display_String_20(hv,13,3,2);
                                    Display_String_20(volts,20,1,2);
                                    Display_Number_4digit(withstand_hv_voltage_glb,16,4,2);
                                    Display_String_20(amp,1,4,3);
                                    Display_String_20(ua,9,2,3);
                                    Display_Number_4digit(withstand_current_glb,5,4,3);
                                    Display_String_20(time,12,2,3);
                                    Display_String_20(sec,19,2,3);
                                    Display_Number_5digit(hv_withstand_time_glb,14,5,3);
                                    Start_Cursor_Blinking(cursor_position_global+8,2);
                                    break;    
                                    
         case ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE:   
                                    Display_String_20(menu,1,5,1);
                                    Display_String_20(ws_data,6,7,1);
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    Display_String_20(lst_ckt,1,8,2);
                                    Display_Number_4digit(withstand_last_ckt_no_glb,9,3,2);
                                    Display_String_20(hv,13,3,2);
                                    Display_String_20(volts,20,1,2);
                                    Display_Number_4digit(withstand_hv_voltage_glb,16,4,2);
                                    Display_String_20(amp,1,4,3);
                                    Display_String_20(ua,9,2,3);
                                    Display_Number_4digit(withstand_current_glb,5,4,3);
                                    Display_String_20(time,12,2,3);
                                    Display_String_20(sec,19,2,3);
                                    Display_Number_5digit(hv_withstand_time_glb,14,5,3);
                                    Start_Cursor_Blinking(cursor_position_global+15,2);
                                    break;
                                    
                                                                        
        case ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT:   
                                    Display_String_20(menu,1,5,1);
                                    Display_String_20(ws_data,6,7,1);
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    Display_String_20(lst_ckt,1,8,2);
                                    Display_Number_4digit(withstand_last_ckt_no_glb,9,3,2);
                                    Display_String_20(hv,13,3,2);
                                    Display_String_20(volts,20,1,2);
                                    Display_Number_4digit(withstand_hv_voltage_glb,16,4,2);
                                    Display_String_20(amp,1,4,3);
                                    Display_String_20(ua,9,2,3);
                                    Display_Number_4digit(withstand_current_glb,5,4,3);
                                    Display_String_20(time,12,2,3);
                                    Display_String_20(sec,19,2,3);
                                    Display_Number_5digit(hv_withstand_time_glb,14,5,3);
                                    Start_Cursor_Blinking(cursor_position_global+4,3);
                                    break;   
                                    
        case ST_SET_UP_HVCT_WITHSTAND_DATA_TIME:   
                                    Display_String_20(menu,1,5,1);
                                    Display_String_20(ws_data,6,7,1);
                                    Display_String_20(pn,16,2,1);
                                    Display_Number_4digit(current_cable_number,18,3,1);
                                    Display_String_20(lst_ckt,1,8,2);
                                    Display_Number_4digit(withstand_last_ckt_no_glb,9,3,2);
                                    Display_String_20(hv,13,3,2);
                                    Display_String_20(volts,20,1,2);
                                    Display_Number_4digit(withstand_hv_voltage_glb,16,4,2);
                                    Display_String_20(amp,1,4,3);
                                    Display_String_20(ua,9,2,3);
                                    Display_Number_4digit(withstand_current_glb,5,4,3);
                                    Display_String_20(time,12,2,3);
                                    Display_String_20(sec,19,2,3);
                                    Display_Number_5digit(hv_withstand_time_glb,14,5,3);
                                    Start_Cursor_Blinking(cursor_position_global+13,3);
                                    break;   
    /*----------------------------------------------------------------------------------------------*/
                                   
        default                 :   
                                    menu_counter=MIN_LIMIT_MENU;

                                    state_code = ST_IDLE;
                                    break;        
    }
}


/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
//25/11/11.
void Scroll_Programs_Hvct(unsigned int counter_local)
{   
    unsigned char last_ckt_number_lcl=0;
    unsigned char hv_msb_lcl=0,hv_lsb_lcl=0;
    unsigned char ir_msb_lcl=0,ir_lsb_lcl=0;
    unsigned char time_msb_lcl=0,time_lsb_lcl=0;
    unsigned int hv_voltage_lcl=0,insulation_resistance_lcl=0;
    unsigned int hv_time_lcl=0;
    //---------------------------------------Read saved Last circuit no for selected program location from eeprom-------------------------------// 
    last_ckt_number_lcl = E2prom_Byte_Read(e2prom_2_page_2_add,((last_ckt_number_base_address+(counter_local-1))&mask_higher_byte) >> 8,(last_ckt_number_base_address+(counter_local-1))&mask_lower_byte);
    ir_last_ckt_no_glb = last_ckt_number_lcl;
    
    //---------------------------------------Read saved voltage for selected program location from eeprom---------------------------------------// 
    hv_msb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add,((high_voltage_base_address+(counter_local*2-2))&mask_higher_byte) >> 8,(high_voltage_base_address+(counter_local*2-2))&mask_lower_byte);
    hv_lsb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add,((high_voltage_base_address+(counter_local*2-1))&mask_higher_byte) >> 8,(high_voltage_base_address+(counter_local*2-1))&mask_lower_byte);
    hv_voltage_lcl = hv_msb_lcl << 8; 
    hv_voltage_glb = (hv_voltage_lcl | hv_lsb_lcl);
    
    //---------------------------------------Read saved IR(M ohm) value for selected program location from eeprom---------------------------------------// 
    ir_msb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add,((insulation_resistance_base_address+(counter_local*2-2))&mask_higher_byte) >> 8,(insulation_resistance_base_address+(counter_local*2-2))&mask_lower_byte);
    ir_lsb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add,((insulation_resistance_base_address+(counter_local*2-1))&mask_higher_byte) >> 8,(insulation_resistance_base_address+(counter_local*2-1))&mask_lower_byte);
    insulation_resistance_lcl = ir_msb_lcl << 8; 
    insulation_resistance_glb = (insulation_resistance_lcl | ir_lsb_lcl);
    
    //---------------------------------------Read saved Time(m Sec)for selected program location from eeprom---------------------------------------// 
    time_msb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add,((hv_time_base_address+(counter_local*2-2))&mask_higher_byte) >> 8,(hv_time_base_address+(counter_local*2-2))&mask_lower_byte);
    time_lsb_lcl = E2prom_Byte_Read(e2prom_2_page_2_add,((hv_time_base_address+(counter_local*2-1))&mask_higher_byte) >> 8,(hv_time_base_address+(counter_local*2-1))&mask_lower_byte);
    hv_time_lcl = time_msb_lcl << 8; 
    hv_withstand_time_glb = (hv_time_lcl | time_lsb_lcl);
 
    //
    Clear_Display();
    if(counter_local>128)
    {
        Display_String_20(prgno,1,16,1);
        Display_Number_4digit(counter_local,17,3,1);
        Display_String_20(lmt_exceeds,1,20,2);
        Display_String_20(lmt_exceeds,1,20,3);
        Display_String_20(lmt_exceeds,1,20,4);
        Start_Cursor_Blinking(cursor_position_global+16,1); 
        return;	
    }//end if(counter_local>max_number_programs)
    if(counter_local==0 )
    {
        Display_String_20(prgno,1,16,1);
        Display_Number_4digit(counter_local,17,3,1);
        Display_String_20(valid_program,1,19,2);
        Start_Cursor_Blinking(cursor_position_global+16,1); 
        return;	
    }
    else 
    {
        Display_String_20(prgno,1,16,1);
        Display_Number_4digit(counter_local,17,3,1);
        Display_String_20(enter_program_no,2,16,2);
        Display_String_20(and,8,3,3);
        Display_String_20(press_ent_sel,1,19,4);
        Start_Cursor_Blinking(cursor_position_global+16,1); 
        return;	
    }
    return;	
}


/*
 Function:
 Input:
 Output:
 Calls:None
 Called By:....
*/
void Scroll_voltage_Hvct(unsigned int counter_local)
{
    
    if(counter_local > MAX_VOLTAGE_LIMIT)
    {
        Display_String_20(hv_lmt_exceeds,1,16,4);
        Display_String_20(hv,13,3,2);
        Display_String_20(volts,20,1,2);
        Display_Number_4digit(counter_local,16,4,2);
        Start_Cursor_Blinking(cursor_position_global+15,2);
        return;	
    }
    else if(counter_local < MIN_VOLTAGE_LIMIT)
    {
        Display_String_20(minimum_voltage,1,17,4);
        Display_String_20(hv,13,3,2);
        Display_String_20(volts,20,1,2);
        Display_Number_4digit(counter_local,16,4,2);
        Start_Cursor_Blinking(cursor_position_global+15,2);
        return;	
    }
    else
    {
    Clear_Lcd_Line(4);  

    Display_Number_4digit(counter_local,16,4,2);

    Start_Cursor_Blinking(cursor_position_global+15,2);
   
    }
}
/*
 Function:
 Input:
 Output:
 Calls:None
 Called By:....
*/
void Scroll_IR_Hvct(unsigned int counter_local)
{
    if(counter_local > MAX_IR_LIMIT)
    {
        Display_String_20(ir_exceeds,1,16,4); 
        Display_String_20(ir,1,3,3);
        Display_String_20(mohm,8,4,3);
        Display_Number_4digit(counter_local,4,4,3);
        Start_Cursor_Blinking(cursor_position_global+3,3);
        return;	
    }
    else
    {
    Clear_Lcd_Line(4);  

    Display_Number_4digit(counter_local,4,4,3);

    Start_Cursor_Blinking(cursor_position_global+3,3);
   
    }
    
}

/*
 Function:
 Input:
 Output:
 Calls:None
 Called By:....
*/
void Scroll_last_ckt_Hvct(unsigned int counter_local)
{
    
    if(counter_local > MAX_CKT_NO)
        {
            Display_String_20(ckt_lmt_exceeds,1,16,4);
            Display_String_20(lst_ckt,1,8,2);
            Display_Number_4digit(counter_local,9,3,2);
            Start_Cursor_Blinking(cursor_position_global+8,2); 
            return;	
        
        }//end if(counter_local>max_number_programs)
    else
    {
       Clear_Lcd_Line(4); 

       Display_Number_4digit(counter_local,9,3,2);

       Start_Cursor_Blinking(cursor_position_global+8,2); 
    }        
    
}
/*
 Function:
 Input:
 Output:
 Calls:None
 Called By:....
*/
void Scroll_Time_Hvct(unsigned int counter_local)
{
    if(counter_local > MAX_WITHSTAND_TIME_LIMIT)
    {
        Display_String_20(time_exceeds,1,18,4);
        Display_String_20(time,12,2,3);
        Display_String_20(sec,19,2,3);
        Display_Number_5digit(counter_local,14,5,3);
        Start_Cursor_Blinking(cursor_position_global+13,3);
        return;	
        
    }
    else
    {
        Clear_Lcd_Line(4);    
        Display_Number_5digit(counter_local,14,5,3);
        Start_Cursor_Blinking(cursor_position_global+13,3);
    }
}


/*
 Function:
 Input:
 Output:
 Calls:None
 Called By:....
*/
void Scroll_Current_Withstand(unsigned int counter_local)
{
    if(counter_local > MAX_CURRENT_LIMIT)
    {
        Display_String_20(current_exceeds,1,20,4); 
        Display_String_20(amp,1,4,3);
        Display_String_20(ua,9,2,3);
        Display_Number_4digit(counter_local,5,4,3);
        Start_Cursor_Blinking(cursor_position_global+4,3);
        return;	
    }
    else
    {
    Clear_Lcd_Line(4);  

    Display_Number_4digit(counter_local,5,4,3);

    Start_Cursor_Blinking(cursor_position_global+4,3);
   
    }
    
}

/*
 Function:
 Input:
 Output:
 Calls:None
 Called By:....
*/
unsigned int Navigate_Counter_3_Digit(unsigned int counter_local,unsigned char key_local,unsigned char curser_position_local)
{
	
	long int bcdnum_local;
	unsigned char local_ram[4];

	bcdnum_local = HexToBCD_1(counter_local);	
	bcdnum_local = DAA(bcdnum_local);

	local_ram[3] =  bcdnum_local & 0x000F;
	local_ram[2] = (bcdnum_local & 0x00F0) >> 4;
	local_ram[1] = (bcdnum_local & 0x0F00) >> 8;
	local_ram[0] =0;


	switch(key_local)
	{
		case UP_KEY:
					local_ram[curser_position_local] = local_ram[curser_position_local]-1;
					if(local_ram[curser_position_local]==0XFF)
						local_ram[curser_position_local]=0x09;
					break;
		case DOWN_KEY:
					local_ram[curser_position_local] = local_ram[curser_position_local]+1;
					if(local_ram[curser_position_local]==0X0A)
						local_ram[curser_position_local]=0x00;
					break;
		default:
			break;	

	}//end switch(key_local).	
	counter_local=(local_ram[1]*100+local_ram[2]*10 + local_ram[3]);
	counter_local=counter_local+1;
	counter_local=counter_local-1;
	return(counter_local);
}//end unsigned int Navigate_Counter_3_Digit(unsigned int counter_local,unsigned char key_local,unsigned char curser_position_local)


/*
 Function:
 Input:
 Output:
 Calls:None
 Called By:....
*/
void Scroll_IR_Calibrate_Hvct(unsigned int counter_local)
{
    if(counter_local > MAX_IR_LIMIT)
    {
        Display_String_20(ir_exceeds,1,16,4); 
        Display_String_20(enter_r1_value,1,10,3);
        Display_String_20(mohm,15,4,3);
        Display_Number_4digit(counter_local,11,4,3);
        Start_Cursor_Blinking(cursor_position_global+10,3);
        return;	
    }
    else
    {
    Clear_Lcd_Line(4);  

    Display_Number_4digit(counter_local,11,4,3);

    Start_Cursor_Blinking(cursor_position_global+10,3);
   
    }
    
}


/*
 Function:
 Input:
 Output:
 Calls:None
 Called By:....
*/
unsigned int Navigate_Counter_4_Digit(unsigned int counter_local,unsigned char key_local,unsigned char curser_position_local)
{
	
	long int bcdnum_local;
	unsigned char local_ram[5];

	bcdnum_local = HexToBCD_1(counter_local);	
	bcdnum_local = DAA(bcdnum_local);

	local_ram[4] =  bcdnum_local & 0x000F;
	local_ram[3] = (bcdnum_local & 0x00F0) >> 4;
	local_ram[2] = (bcdnum_local & 0x0F00) >> 8;
	local_ram[1] = (bcdnum_local & 0xF000) >> 12;
	local_ram[0] = 0;

    
	switch(key_local)
	{
		case UP_KEY:
					local_ram[curser_position_local] = local_ram[curser_position_local]-1;
					if(local_ram[curser_position_local]==0XFF)
						local_ram[curser_position_local]=0x09;
					break;
		case DOWN_KEY:
					local_ram[curser_position_local] = local_ram[curser_position_local]+1;
					if(local_ram[curser_position_local]==0X0A)
						local_ram[curser_position_local]=0x00;
					break;
		default:
			break;	

	}//end switch(key_local).	
	counter_local=(local_ram[1]*1000+local_ram[2]*100 + local_ram[3]*10+local_ram[4]);
	counter_local=counter_local+1;
	counter_local=counter_local-1;
	return(counter_local);
}//end unsigned int Navigate_Counter_3_Digit(unsigned int counter_local,unsigned char key_local,unsigned char curser_position_local)






/*
 Function:
 Input:
 Output:
 Calls:None
 Called By:....
*/
unsigned int Navigate_Counter_5_Digit(unsigned int counter_local,unsigned char key_local,unsigned char curser_position_local)
{
	
	long int bcdnum_local;
	unsigned char local_ram[6];

	bcdnum_local = HexToBCD_1(counter_local);	
	bcdnum_local = DAA(bcdnum_local);

	local_ram[5] =  bcdnum_local & 0x000F;
	local_ram[4] = (bcdnum_local & 0x00F0) >> 4;
	local_ram[3] = (bcdnum_local & 0x0F00) >> 8;
	local_ram[2] = (bcdnum_local & 0xF000) >> 12;
    local_ram[1] = (bcdnum_local & 0xF0000) >> 16;
	local_ram[0] =0;

    
	switch(key_local)
	{
		case UP_KEY:
					local_ram[curser_position_local] = local_ram[curser_position_local]-1;
					if(local_ram[curser_position_local]==0XFF)
						local_ram[curser_position_local]=0x09;
					break;
		case DOWN_KEY:
					local_ram[curser_position_local] = local_ram[curser_position_local]+1;
					if(local_ram[curser_position_local]==0X0A)
						local_ram[curser_position_local]=0x00;
					break;
		default:
			break;	

	}//end switch(key_local).

    if(curser_position_local == 1)
    {
        if((local_ram[1]==2)||(local_ram[1]==1))
        {
            local_ram[1] = 0;
        }
        else
        {
            local_ram[1] = 1;
        }
    }
	counter_local=(local_ram[1]*10000+local_ram[2]*1000 + local_ram[3]*100+local_ram[4]*10+local_ram[5]);
	counter_local=counter_local+1;
	counter_local=counter_local-1;
	return(counter_local);
}//end unsigned int Navigate_Counter_4_Digit(unsigned int counter_local,unsigned char key_local,unsigned char curser_position_local)







unsigned char Navigate_Date_Time(unsigned char* ram_ptr_lcl,unsigned char key_lcl,unsigned char curser_position_local)
{

	switch(key_lcl)
	{
		case DOWN_KEY:
				
					ram_ptr_lcl[curser_position_local-1] = ram_ptr_lcl[curser_position_local-1]-1;
					if(curser_position_local<19)
					{
						if(ram_ptr_lcl[curser_position_local-1]==0X2F)
						{
							ram_ptr_lcl[curser_position_local-1]=0x39;
						}
					}
					else
					{
						if(ram_ptr_lcl[curser_position_local-1]==0X30)
						{
							ram_ptr_lcl[curser_position_local-1]=0x37;
						}
					}
					
					break;
		case UP_KEY:
					
						ram_ptr_lcl[curser_position_local-1] = ram_ptr_lcl[curser_position_local-1]+1;
						if(curser_position_local<19)
						{
							if(ram_ptr_lcl[curser_position_local-1]==0X3A)
							{
								ram_ptr_lcl[curser_position_local-1]=0x30;
							}
						}
						else
						{
							if(ram_ptr_lcl[curser_position_local-1]==0X38)
							{
								ram_ptr_lcl[curser_position_local-1]=0x31;
							}
						}
					
					break;
		default:
				break;	

	}	
	return(0);
}


void Execute_Other_Data_Menu(unsigned char counter_local,unsigned char* ram_prt_local)
{
	switch(counter_local)
	{
		case 1: 
				//provision for entering day code fascillity.
				Clear_Display();
				Display_String_20(date_time,1,15,1);
               // use this function with error display.
				Read_Date_Time_1(digit_ram,TRUE);
				if(Validate_Date_Time(digit_ram));
				else
				{
					Fill_Any_Char_Ram_With(digit_ram,20,0x30);
					digit_ram[2]='/';
					digit_ram[5]='/';
					digit_ram[12]=':';
					digit_ram[15]=':';
					
				}//end if(Validate_Date_Time(digit_ram));
				digit_ram[18]= ReadRtc(day_code_bbram_base_address)+0x30;
				if(digit_ram[18]<=0x31 || digit_ram[18]>= 0x37 )
					digit_ram[18]=0x31;
			
				Display_Ram(digit_ram,1,0,10,2);
				Display_Ram(digit_ram,1,10,8,3);
				Display_Ram(digit_ram,1,18,1,4);
				Display_String_20(day_code_1,3,17,4);		

				state_code = ST_SET_UP_INIT_RTC; 
				Start_Cursor_Blinking(1,2);
				break;
			
		case 2: Clear_Display();
//				Display_String_20(receiving,1,14,2);	 
				Fill_Any_Char_Ram_With(operator_code_1,9,' ');
				Receive_Data_Char_Ram(operator_code_1,8);
				Display_Ram(operator_code_1,7,0,9,3);
                Remove_Blinking_Cursor();
                Pre_Display_Before_State(ST_SET_UP_OTH_DATA,TRUE,digit_ram);
                state_code = ST_SET_UP_OTH_DATA;
 				DelayS(1);
   				break;
                
		case 3: Clear_Display();
//				Display_String_20(receiving,1,14,2);	 
				Fill_Any_Char_Ram_With(operator_code_2,9,' ');
				Receive_Data_Char_Ram(operator_code_2,8);
				Display_Ram(operator_code_2,7,0,9,3);
                Remove_Blinking_Cursor();
                Pre_Display_Before_State(ST_SET_UP_OTH_DATA,TRUE,digit_ram);
                state_code = ST_SET_UP_OTH_DATA;
 				DelayS(1);
   				break;
                
		case 4: Clear_Display();
//				
				break;
		default:
				break;
	}
}

/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
void Execute_General_Settings_Menu (unsigned char counter_local,unsigned char temp_local)
{
	unsigned int address_local=0;
	unsigned char *ram_ptr=0;
    
    
//    counter_local=counter_local+16;
    ram_ptr = gen_settings_ram;
    address_local = gen_settings_base_address+counter_local-1;
    Retrive_Specific_Data(e2prom_2_page_2_add,gen_settings_base_address,16,1,ram_ptr,SET);//11/10/11.	
  
    if(ram_ptr[counter_local]==RESET)
    {
        if(temp_local==0)
                temp_local=1;
				Display_String_20(done,14,7,temp_local+1);
				E2prom_Byte_Write(e2prom_2_page_2_add,((address_local)&mask_higher_byte) >> 8,(address_local)&mask_lower_byte,SET);
				DelayMs(250);
				DelayMs(250);
				Display_String_20(off,13,6,temp_local+1);
                Retrive_Specific_Data(e2prom_2_page_2_add,gen_settings_base_address,16,1,ram_ptr,SET);
    }
    
    else if(ram_ptr[counter_local]==SET)
    {
        
				if(temp_local==0)
                temp_local=1;
				Display_String_20(done,14,7,temp_local+1);
				E2prom_Byte_Write(e2prom_2_page_2_add,((address_local)&mask_higher_byte) >> 8,(address_local)&mask_lower_byte,RESET);
				DelayMs(250);
				DelayMs(250);
				Display_String_20(on,13,6,temp_local+1);
                 Retrive_Specific_Data(e2prom_2_page_2_add,gen_settings_base_address,16,1,ram_ptr,SET);
    }
    else
        ;
    Retrive_Specific_Data(e2prom_2_page_2_add,gen_settings_base_address,16,1,gen_settings_ram,SET);
}
/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
void Execute_Test_Settings_Menu (unsigned char counter_local,unsigned char temp_local)
{
	unsigned int address_local=0;
	unsigned char *ram_ptr=0;
    
    
    ram_ptr=test_settings_ram;
	address_local=test_setting_base_address+counter_local-1;
    Retrive_Specific_Data(e2prom_2_page_2_add,test_setting_base_address,6,1,ram_ptr,SET);
  
    if(ram_ptr[counter_local]==RESET)
    {
        if(temp_local==0)
                temp_local=1;
				Display_String_20(done,14,7,temp_local+1);
				E2prom_Byte_Write(e2prom_2_page_2_add,((address_local)&mask_higher_byte) >> 8,(address_local)&mask_lower_byte,SET);
				DelayMs(250);
				DelayMs(250);
				Display_String_20(off,13,6,temp_local+1);
                Retrive_Specific_Data(e2prom_2_page_2_add,test_setting_base_address,6,1,ram_ptr,SET);
    }
    
    else if(ram_ptr[counter_local]==SET)
    {
        
				if(temp_local==0)
					temp_local=1;
				Display_String_20(done,14,7,temp_local+1);
				E2prom_Byte_Write(e2prom_2_page_2_add,((address_local)&mask_higher_byte) >> 8,(address_local)&mask_lower_byte,RESET);
				DelayMs(250);
				DelayMs(250);
				Display_String_20(on,13,6,temp_local+1);
                Retrive_Specific_Data(e2prom_2_page_2_add,test_setting_base_address,6,1,ram_ptr,SET);
    }
    else
        ;
    Retrive_Specific_Data(e2prom_2_page_2_add,test_setting_base_address,6,1,test_settings_ram,SET);
}
/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
void Execute_Prg_Settings_Menu (unsigned char counter_local,unsigned char temp_local)
{
	unsigned int prg_address_local=0;
	unsigned char *ram_ptr=0;
    
    
    ram_ptr=prg_specific_setting_ram;
									switch(counter_local)
									{
                                               
										case 1: prg_address_local=ir_test_setting_base_address+current_cable_number;
                                                Retrive_Specific_Data(e2prom_2_page_2_add,prg_address_local,6,counter_local,ram_ptr,SET);
												break;
										case 2: prg_address_local=withstand_test_setting_base_address+current_cable_number;	
                                                Retrive_Specific_Data(e2prom_2_page_2_add,prg_address_local,6,counter_local,ram_ptr,SET);
												break;
										//case 3: 
                                                 
										default:
												break;
									}//end switch(counter_local).
//									Retrive_Cable_Specific_Settings(current_cable_number);
    
  
    if(ram_ptr[counter_local]==RESET)        //0xAA
    {
        if(temp_local==0)
                temp_local=1;
				Display_String_20(done,14,7,temp_local+1);
				E2prom_Byte_Write(e2prom_2_page_2_add,((prg_address_local)&mask_higher_byte) >> 8,(prg_address_local)&mask_lower_byte,SET);
				DelayMs(250);
				DelayMs(250);
				Display_String_20(off,13,6,temp_local+1);
    }
    
    else if(ram_ptr[counter_local]==SET)   //0x55
    {
        
				if(temp_local==0)
					temp_local=1;
				Display_String_20(done,14,7,temp_local+1);
				E2prom_Byte_Write(e2prom_2_page_2_add,((prg_address_local)&mask_higher_byte) >> 8,(prg_address_local)&mask_lower_byte,RESET);
				DelayMs(250);
				DelayMs(250);
				Display_String_20(on,13,6,temp_local+1);
    }
    else
        ;
    
        Retrive_Prg_Specific_Settings_and_data(current_cable_number);
}

/*
 Function:
 Input: 
 * 
 Output:
 Calls:
 Called By:
*/
void Execute_DIAGNO_Menu(unsigned char local_counter)
{
//	unsigned char local_ram[15];//15_01_18_4,index_lcl=0;//12_12_14_2_b.
	Clear_Display();
	switch(local_counter)
	{
		case 1:
				Cards_Test();
			    break;
		case 2:	
				//----------------21_06_16_2-----start------------//
				Display_String_20(conn_dongle,1,17,2);
				Display_String_20(Rs232_1_tesT,1,7,3);
				Display_String_20(and,9,3,3);
				Display_String_20(press_ent_sel,1,9,4);
				Caliberate_Serial_1(1);
				
				//----------------21_06_16_2-----ends------------//
			break;

		case 3:
                Display_String_20(e2prom1,1,12,1);
				Display_String_20(e2prom2,1,12,2);
                Display_String_20(e2prom3,1,12,3);
                Display_String_20(e2prom4,1,12,4);
				E2prom_test();
                break;
		case 4:	
                Display_String_20(buzz_tone,1,11,2);
                Gen_Tone(1,5);
                break;
		case 5:	
               // Display_String_20(date_time,1,13,2);
                Display_String_20(date,1,5,1);
                Display_String_20(time_1,1,11,3);
                Display_date_time();	
                break;
		case 6:	
				Display_String_20(conn_diobox,1,14,2);	
			    Display_String_20(and,1,3,3);	     
			    Display_String_20(pressent,5,9,3);
				Test_DigIO();
		        break;
		  
		case 7: 
                Check_circuit_number( );
                continuity_test(last_ckt_no_glb);
                if(continuity_test_pass_flag_glb == TRUE)
                {
                    continuity_test_pass_flag_glb = FALSE;
                    HV_IR_Value(last_ckt_no_glb);
                } 
                else
                {
                    Display_String_20(please,1,6,2);
                    Display_String_20(connect,8,7,2);
                    Display_String_20(the,16,3,2);
                    Display_String_20(harness,1,7,3);
                    Display_String_20(and,9,3,3);
                    Display_String_20(retry,13,8,3);
                    DelayS(2);
                }
                  break;
		  
//                   const static unsigned char please[]=         	{"PLEASE"};
//const static unsigned char connect[]=         	{"CONNECT"};
//const static unsigned char harness[]=         	{"HARNESS"};
//const static unsigned char retry[]=         	{"RETRY......."};
                //const static unsigned char and[]=    			{"AND"};
//                const static unsigned char the[]=         	{"THE"};
//                Display_String_20(conn_diobox,1,14,2);	
//			    Display_String_20(and,1,3,3);	     
//			    Display_String_20(pressent,5,9,3);
//				Test_DigIO();
		//-----------------12_12_14_2--------start------------------//
		case 8:
                  
                   Display_String_20(checking_io,1,11,2);
                   Display_String_20(points,1,10,3);
                   Self_IO_Test( );
            //////////////////calibration/////////////////
             /*Calibration(1);*/
//////////////////////////////////////////
            
//                Display_String_20(Chk_ledS,1,10,2);
//				HACG_All_Led_Off_On(MCMD_MAKE_ALL_LOW);
//				Delay_S(3);
//				HACG_All_Led_Off_On(MCMD_MAKE_ALL_HIGH);
		   break;
		case 9: 
//				lbl_counter_lmt_global=128; 
//				lbl_counter_val_global=1; 
//				ctr_typ_glb=VALUE;	
//				st_cntr_function_2_var_glb=1;
//				Display_String_20(start_diagno_menu+72,1,9,1);	//hacg_seq_led_n 17_05_18_1_e.
//				Display_String_20(arrow,1,4,2);	
//				HACG_All_Led_Off_On(MCMD_MAKE_ALL_HIGH);
//				Counter_Navigation_Function_1(1,ctr_ptr_glb+(lbl_counter_val_global-1)*2,ctr_typ_glb,cntr_size_glb);//FACT_512_1
//				Fill_Any_Char_Ram_With(local_ram,10,0x00);	
//    			Fill_Small_Data_Ram(local_ram,MCMD_BROAD_CAST_ADD,MCMD_MAKE_SINGLE_IO_PT_LOW,1,MCMD_NULL,MCMD_NULL);
//    			Send_HAAP_Data(local_ram,0x00,0x03);
//    			DelayMs(30);
//				state_counters_fun_cnt_glb=9;//state for digital_ip_2 delay.
//				state_code=ST_COUNTER;
			break;	
				//Display_String_20(Chk_ledS,1,10,2);
				//state_code=ST_DIAGNO_HACG_SEQ;	
		  // break;
		//-----------------12_12_14_2--------start------------------//
		//-----------------20_04_15_1--------start------------------//
		case 10:
//				Display_String_20(Connect_dig_iP,1,14,2);
//				Display_String_20("1",15,1,2);
//				while(Sence_Vibrating_Digital_Input(DIGITAL_IP_LOW,DIGITAL_IP_HIGH,DIGITAL_IP_1,10,10)!=TRUE)
//				{
//				}//end while(Sence_Vibrating_Digital_Input(DIGITAL_IP_LOW,DIGITAL_IP_HIGH,DIGITAL_IP_1,10,10)!=TRUE)
//				Display_String_20("PASS            ",1,16,2);
//				Ring_Buzzer_A(local_ram,PASS_TONE,E2prom_Byte_Read((e2prom_2_page_2_add & mask_lower_byte),((tone_address)&mask_higher_byte) >> 8,(tone_address)& mask_lower_byte),0x02);	
//				Display_String_20(Connect_dig_iP,1,14,3);
//				Display_String_20("2",15,1,3);
//				key_pressed_flag =0;
//				key_code=0;			
//				while(Sence_Vibrating_Digital_Input(DIGITAL_IP_LOW,DIGITAL_IP_HIGH,DIGITAL_IP_2,10,10)!=TRUE)
//				{
//				}//end while(Sence_Vibrating_Digital_Input(DIGITAL_IP_LOW,DIGITAL_IP_HIGH,DIGITAL_IP_1,10,10)!=TRUE)
//				Display_String_20("PASS            ",1,16,3);
//				Ring_Buzzer_A(local_ram,PASS_TONE,E2prom_Byte_Read((e2prom_2_page_2_add & mask_lower_byte),((tone_address)&mask_higher_byte) >> 8,(tone_address)& mask_lower_byte),0x02);	
//				Delay_S(1);
			break;
		//-----------------20_04_15_1--------end--------------------//
		default:
			    Display_String_20(notavailable,1,13,2);	
				DelayMs(250);
				DelayMs(250);
			break;
	}//end switch(local_counter).
}//end void Execute_DIAGNO_Menu(unsigned char local_counter).


//void Execute_Analyse_Menu(unsigned char local_counter)
//{
//    
//}


/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
void Execute_Clr_Cnt_Menu(unsigned char local_counter,unsigned char arr_pos_local)
{
	unsigned char local_ram[16];

	Fill_Any_Char_Ram_With(local_ram,16,0x30);

	switch(local_counter)
	{
		case 1://clear pass count.
				Read_Write_RTC(local_ram,pass_count_bbram_base_address,pass_fail_count_size,WRITE);
			break;
		case 2://clear fail count.
				Read_Write_RTC(local_ram,fail_count_bbram_base_address,pass_fail_count_size,WRITE);
			break;
        case 3://clear Serial number count.
				Read_Write_RTC(local_ram,serial_number_bbram_base_address,9,WRITE);
			break;
		
		
		//----------------------24_12_18_1-----end-------------------//
	}//end switch(local_counter)
	Display_String_20(done,13,6,arr_pos_local+1);
	DelayMs(250);
	DelayMs(250);
	DelayMs(250);
	Display_String_20(clear,13,6,arr_pos_local+1);	 
}//end void Execute_Clr_Cnt_Menu(unsigned char local_counter,unsigned char arr_pos_local)s


/*
 Function:This function will execute various futura dnd menu functions as per the attribute passed.
 Input:counter for menu selection.(unsigned char type) 
 Output:None.
 Calls:Clear_Display(),Display_String_20(),Configure_Tester_Sr_No_Eeprom(),DelayMs().
 Called By:
*/
void Execute_FDND_Menu(unsigned char counter_local)
{
	Clear_Display();										//clear display to display new menu.

	switch(counter_local)
	{
		case 1: //configure tester serial number....
                //-----------------05_08_19_1---start-------------------------//
                //for tester serial number config,baud rate is req. to initialize to 38400 for serial port 1. 
                Timer1_50ms_Start();
                Initialise_Serial_1_FACT_Editor();                              //Initialise to 38400 2 stop bits for FACT editor.
				Display_String_20(receiving,1,14,2);	 
				Display_String_20(send_data,1,9,3);		 
				Configure_Tester_Sr_No_Eeprom();		// configure serial number.	
                Initialize_Serial_1_RS232();                                          //Re-initialize to 9600 again.   
                Timer1_50ms_Stop();
                //-----------------05_08_19_1---ends--------------------------//
				break;
		//----------------15_01_18_4---------------start-------------------//
		case 2: 
				break;
		case 3: 
				break;
		case 4:
				break;
		//----------------15_01_18_4---------------ends--------------------//
		default:Display_String_20(notavailable,1,13,2);
				break;
	}//end switch(counter_local).
	DelayMs(250);							
	DelayMs(250);
}//end void Execute_FDND_Menu(unsigned char counter_local)



