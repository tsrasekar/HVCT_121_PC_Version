#define  TRUE              	   				1
#define  FALSE                              0
#define  HIGH                               1
#define  LOW                                0
#define output_pin                          0    // port pin as output pin
#define input_pin                           1    // port pin as input pin
#define  SET     		       				0X55
#define  RESET                 				0XAA
#define  PASS     		       				0X55
#define  FAIL                 				0XAA
#define RELAY_ON                            0;
#define RELAY_OFF                           1;
#define SINGLE_RELAY						0XAA
#define BOTH_RELAYS							0X55

#define PASS_TONE                           1
#define FAIL_TONE                           5

/****************************************************************************
 *Interface Card Relay Port pins
 ***************************************************************************/
#define RLYLV11_rb2_pin                   PORTBbits.RB2     
#define TRIS_RLYLV11_rb2_pin              TRISBbits.TRISB2

#define RLYLV12_rb3_pin                   PORTBbits.RB3    
#define TRIS_RLYLV12_rb3_pin              TRISBbits.TRISB3

#define RLYLV21_rb4_pin                   PORTBbits.RB4
#define TRIS_RLYLV21_rb4_pin              TRISBbits.TRISB4

#define RLYLV22_rb5_pin                   PORTBbits.RB5
#define TRIS_RLYLV22_rb5_pin              TRISBbits.TRISB5

#define RLYHV11_re9_pin                   PORTEbits.RE9
#define TRIS_RLYHV11_re9_pin              TRISEbits.TRISE9

#define RLYHV21_re8_pin                   PORTEbits.RE8   
#define TRIS_RLYHV21_re8_pin              TRISEbits.TRISE8

#define RLYHL1_rg9_pin                    PORTGbits.RG9
#define TRIS_RLYHL1_rg9_pin               TRISGbits.TRISG9

#define RLYHL2_rg8_pin                    PORTGbits.RG8
#define TRIS_RLYHL2_rg8_pin               TRISGbits.TRISG8


/****************************************************************************
 *Digital Read Pin for contiuity
 ***************************************************************************/
#define DIG_READ_rg6_pin                  PORTGbits.RG6
#define TRIS_DIG_READ_rg6_pin             TRISGbits.TRISG6

/****************************************************************************
*KBD Interrupt pin
***************************************************************************/
#define TRIS_KBD_INTERRUPT_rd1_pin        TRISDbits.TRISD1                 

/****************************************************************************
 *SP3 Port Pins Start
 ***************************************************************************/
#define SDO_SPI3_rd5_pin				PORTDbits.RD5
#define TRIS_SDO_SPI3_rd5_pin			TRISDbits.TRISD5

#define SDI_SPI3_rd4_pin				PORTDbits.RD4
#define TRIS_SDI_SPI3_rd4_pin			TRISDbits.TRISD4

#define SCLK_SPI3_rd3_pin				PORTDbits.RD3
#define TRIS_SCLK_SPI3_rd3_pin			TRISDbits.TRISD3

#define CS1_4151_SPI3_rh0_pin           PORTHbits.RH0
#define TRIS_CS1_SPI3_rh0_pin			TRISHbits.TRISH0

#define CS2_3202_SPI3_rf7_pin           PORTFbits.RF7
#define TRIS_CS2_SPI3_rf7_pin			TRISFbits.TRISF7

#define CS3_3202_SPI3_rf8_pin           PORTFbits.RF8
#define TRIS_CS3_SPI3_rf8_pin			TRISFbits.TRISF8


 
/****************************************************************************
 *I2C Port Pins Start
 ***************************************************************************/
#define TRIS_SCL_ra14_pin               TRISAbits.TRISA14
#define TRIS_SDA_ra15_pin               TRISAbits.TRISA15


/****************************************************************************
 *LCD Port Pins
 ***************************************************************************/
#define tris_lcd_data_pin_7       				TRISEbits.TRISE4  
#define lcd_data_pin_7             				PORTEbits.RE4  //data pin 7                         

#define tris_lcd_data_pin_6       				TRISEbits.TRISE3 
#define lcd_data_pin_6            				PORTEbits.RE3  //data pin 6

#define tris_lcd_data_pin_5       				TRISEbits.TRISE2  
#define lcd_data_pin_5             				PORTEbits.RE2  //data pin 5

#define tris_lcd_data_pin_4       				TRISEbits.TRISE1
#define lcd_data_pin_4             				PORTEbits.RE1  //data pin 4

#define tris_enable_pin                			TRISGbits.TRISG0        
#define lcd_enable_pin                 			PORTGbits.RG0  //to enable lcd.       

#define tris_rdwr_pin                  			TRISGbits.TRISG1     
#define lcd_rdwr_pin                   			PORTGbits.RG1  //read write control pin.

#define tris_rselect_pin               			TRISEbits.TRISE0
#define lcd_rselect_pin                			PORTEbits.RE0  //selection pin.       

//-------------------------------------------------------------------------//
#define LCD_ADD_LINE_1							0X00
#define LCD_ADD_LINE_2							0X40
#define LCD_ADD_LINE_3							0X14
#define LCD_ADD_LINE_4							0X54



/****************************************************************************
 *Card Select pins
 ***************************************************************************/
#define tris_card_select_A0_rg7_pin             TRISGbits.TRISG7 
#define card_select_A0_rg7_pin                  PORTGbits.RG7

#define tris_card_select_A1_rd8_pin             TRISDbits.TRISD8
#define card_select_A1_rd8_pin                  PORTDbits.RD8

#define tris_card_select_A2_rd9_pin             TRISDbits.TRISD9
#define card_select_A2_rd9_pin                  PORTDbits.RD9

#define tris_enable_74hc138_rd10_pin            TRISDbits.TRISD10
#define enable_74hc138_rd10_pin                 PORTDbits.RD10

#define tris_strobe_74hc244_rd11_pin            TRISDbits.TRISD11
#define enable_74hc244_rd11_pin                 PORTDbits.RD11



#define volume_control             				PORTBbits.RB7  //volume_control_for_slave_controller
#define tris_volume_control						TRISBbits.TRISB7
#define TONE                                    PORTBbits.RB6
#define tris_tone                               TRISBbits.TRISB6

#define tris_rs485_control_ra10_pin             TRISAbits.TRISA10
#define rs485_control_ra10_pin                  PORTAbits.RA10

#define tris_board_check_ra9_pin                TRISAbits.TRISA9//both added for board check function dt 10.10.2023
#define board_check_ra9_pin                     PORTAbits.RA9   //
/****************************************************************************
 *hex to bcd constants
 ***************************************************************************/
#define two_power_zero							0x0001
#define two_power_one							0x0002
#define two_power_two							0x0004
#define two_power_three							0x0008
#define two_power_four							0x0016
#define two_power_five							0x0032	
#define two_power_six							0x0064
#define two_power_seven							0x0128
#define two_power_eight							0x0256
#define two_power_nine							0x0512
#define two_power_ten							0x1024
#define two_power_eleven						0x2048
#define two_power_twelve						0x4096
#define two_power_thirteen						0x8192
#define two_power_fourteen						0x16384
#define two_power_fifteen						0x32768
#define two_power_sixteen                       0x65536
#define two_power_seventeen                     0x131072
#define two_power_eignteen                      0x262144
#define two_power_ninteen                       0x524288
#define two_power_twenty                        0x1048576
#define two_power_twentyone                     0x2097152
#define two_power_twentytwo                     0x4194304
#define two_power_twentythree                   0x8388608



/****************************************************************************
 * I2C related Defines
 ***************************************************************************/
#define read_8574_1_input                       0x41
#define write_8574_2_output                     0x42

#define read_8574_3_input                       0x44
#define write_8574_3_output                     0x45

/****************************************************************************
 * Key Code
 ***************************************************************************/
#define UP_KEY									0X0B
#define DOWN_KEY								0X07
#define ENTER_KEY								0X0D
#define ESC_KEY									0X0E
#define REMOTE_KEY							    0XD0
#define FAIL_BIN_KEY						    0XE0

#define ST_IDLE									0X00
#define ST_IDLE_TEST							0X01
#define ST_IDLE_SELECT_PRG						0x02
#define ST_IDLE_BRD_CHK							0X03
#define ST_ANALYSE								0X04
#define ST_SET_UP								0X05
#define ST_IDLE_COUNT							0X06
#define NO_STATE                                0xFF   

/****************************************************************************
 * peripherals settings
 ***************************************************************************/
#define mask_higher_byte						0xFF00	 // retains upper byte
#define mask_lower_byte							0x00FF	 // retains lower byte


/****************************************************************************
 * LCD defines
 ***************************************************************************/
#define MIN_LIMIT_MENU							0X01
#define MAX_LIMIT_MAIN_MENU						0X06

#define MAX_LIMIT_MAIN_MENU						0X06
#define MIN_LIMIT_MENU							0X01
#define MAX_LIMIT_ANALYSE_MENU					0x08
#define MAX_LIMIT_SETUP_MENU					0x0E
#define MAX_LIMIT_TEST_DATA_MENU                0x08
#define MAX_LIMIT_OTH_DATA_MENU					0x06   
#define MAX_LIMIT_GEN_SETT_MENU					0x04
#define MAX_LIMIT_VOL							0x0A
#define MAX_LIMIT_TONE                          0X04
#define MAX_LIMIT_RELAY_ON						0X09
#define MAX_LIMIT_CLR_COUNTS_MENU				0X03
#define MAX_LIMIT_DIAGNO_MENU					0X08
#define MAX_LIMIT_TEST_SETT_MENU				0X04
#define MAX_LIMIT_PRG_SETT_MENU					0x03
#define MAX_LIMIT_FDND_MENU						0x06

/****************************************************************************
 * sub states of state setup.
 ***************************************************************************/
	#define ST_SET_UP_HVCT_DATA_LAST_CKT			0x0B	//decimal(11)-----IR Data
	#define ST_SET_UP_OTH_DATA						0X0C	//decimal(12)
	#define ST_SET_UP_TEST_SETTINGS					0X0D	//decimal(13)
	#define ST_SET_UP_GEN_SETTINGS					0X0E	//decimal(14)
	#define ST_SET_UP_PRG_SETTINGS					0X0F	//decimal(15)
	#define ST_SET_UP_DIAGNO						0X10	//decimal(16)
	#define ST_SET_UP_FUTURA_DND					0X11	//decimal(17)
	#define ST_SET_UP_VOLUME						0x12	//decimal(18)
	#define ST_SET_UP_CLR_COUNTS               		0x13	//decimal(19)
	#define ST_SET_UP_RELAY_TIME					0x14	//decimal(20)
    #define ST_SET_UP_CALIBRATE                     0x15   // decimal(21)  
    #define ST_SET_UP_FACTORY_SETT				    0x16	//decimal(22)
	#define ST_SET_UP_LRN_LABEL  					0x17	//decimal(23)
    #define ST_SET_UP_HVCT_WITHSTAND_DATA_LAST_CKT	0x18	//decimal(24)-----WS-Data
//    #define ST_SET_UP_TONE				        0x15	//decimal(21)
//	  #define ST_CONFIGURE                    		0x16	//decimal(22)
//	#define ST_SET_UP_RECEIVE_SHIFT					0x17	//decimal(23)
//	#define ST_SET_LOT_COUNT						0x18	//decimal(24)
	#define ST_SET_CUT_TIME							0x19	//decimal(25)
	#define ST_RELESE_HRN							0X1A	//decimal(26)
	
	#define ST_SET_UP_SENSOR_RLY_ON					0x1B	//decimal(27)
	#define ST_SET_UP_LRN_RESOURCE_BAR				0x1C	//decimal(28)
	
	#define ST_SET_UP_DIG_IP_1_COUNT				0x1D	//decimal(29)
	#define ST_SET_UP_DIG_IP_1_DELAY				0x1E	//decimal(30)
	#define ST_SET_UP_DIG_IP_2_COUNT				0x1F	//decimal(31)
	#define ST_SET_UP_DIG_IP_2_DELAY				0x20	//decimal(32)

	#define ST_SET_UP_PRN_DELY_CNT					0x21	//decimal(33)
	#define ST_SET_UP_SENSOR_DATA					0X22    //decimal(34)
//	#define ST_SET_UP_FACTORY_SETT							0X23	//decimal(35)
	#define ST_SET_UP_EXTND_SETTINGS				0x24	//decimal(36)
    /***********************HVCT IR Data -start**************************************************/
    #define ST_SET_UP_HVCT_DATA_VOLTAGE             0x25	//decimal(37)
    #define ST_SET_UP_HVCT_DATA_IR                  0x26    //decimal(38)
    #define ST_SET_UP_HVCT_DATA_TIME                0x27    //decimal(39)
    /***********************HVCT IR Data -end**************************************************/

    /***********************HVCT Withstand Data -start**************************************************/
    #define ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE   0x28	//decimal(40)
    #define ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT   0x29    //decimal(41)
    #define ST_SET_UP_HVCT_WITHSTAND_DATA_TIME      0x30    //decimal(42)
    /***********************HVCT Withstand Data -end**************************************************/
    #define	ST_SET_UP_INIT_RTC						0x42	//decimal(65)
//    /***********************HVCT Withstand Data -start**************************************************/
//    #define ST_SET_UP_HVCT_WITHSTAND_DATA_VOLTAGE   0x43	//decimal(40)
//    #define ST_SET_UP_HVCT_WITHSTAND_DATA_CURRENT   0x44    //decimal(41)
//    #define ST_SET_UP_HVCT_WITHSTAND_DATA_TIME      0x45    //decimal(42)
//    /***********************HVCT Withstand Data -end**************************************************/



/****************************************************************************
 * Peripherals Addresses
 ***************************************************************************/
#define rtc_address 						0xA2	// address of device conncted in I2C RTC
//#define e2prom_1_page_1_add		   			0xA0						
//#define e2prom_1_page_2_add				   	0xA2// Due to same address of rtc & e2prom 1,e2prom 1 address shift e2prom 2 & e2prom is remove in CPU PCB.		

#define e2prom_2_page_1_add					0xA4		
#define e2prom_2_page_2_add					0xA6	

#define e2prom_3_page_1_add					0xA8		
#define e2prom_3_page_2_add					0xAA		

#define e2prom_4_page_1_add					0xAC				
#define e2prom_4_page_2_add					0xAE	


/****************************************************************************
 * Various Sizes
 ***************************************************************************/
#define pass_fail_count_size                0x0003


/****************************************************************************
 * Various indices
 ***************************************************************************/
#define MAX_MODBUS_ARRAY_SIZE					256
#define DATA_BYTE_4_AA                          0xAA
#define MAX_CARD_NUMBER                         2
#define MAX_PROGRAM_NO                          128     // Total number of program locations
#define MAX_CKT_NO                              32      // maximum 1-1 ckt number
#define MAX_VOLTAGE_LIMIT                       1500    // 1500 Vdc
#define MIN_VOLTAGE_LIMIT                       50      // 50 Vdc
#define MAX_IR_LIMIT                            100     // 100 Mohm
#define MAX_WITHSTAND_TIME_LIMIT                10000     // 10000 msec
/////////////////////////////////////////////////////////////////////////
#define MAX_CURRENT_LIMIT                       1000     // 1000 Micro Amp

/****************************************************************************
 *RS485 Commands
 ***************************************************************************/
#define MCMD_NULL								0X00	//null command.
#define MCMD_BRODCAST                           0x80
#define MCMD_BRD_CHK                            0x43
#define MCMD_ASSIGN_ADDRESS                     0x42
#define MCMD_HOW_ARE_YOU                        0x59
#define MCMD_I_AM_FINE                          0x5A
#define MCMD_SINGLE_RLY_ON                      0x4A
#define MCMD_SINGLE_RLY_OFF                     0x4B
#define MCMD_ALL_RLY_OFF                        0x51
#define MCMD_ALL_RLY_ON                         0x52
#define MCMD_ALL_RLY_OFF_EXCEPT_RLY_NO          0x60
#define MCMD_MAKE_RLY_PAIR_ON                   0x61
#define MCMD_MAKE_RLY_PAIR_OFF                  0x62
#define MCMD_MAKE_RLYS_OFF_EXCEPT_PAIR          0x63
#define MCMD_MAKE_SINGLE_RLY_OFF_SINGLE_RLY_ON  0x64        
#define MCMD_MAKE_RLYS_ON_EXCEPT_PAIR           0x65


/****************************************************************************
 * Peripheral Settings
 ***************************************************************************/
#define WRITE            						1		 //for writing of bbram,pass this definition to the comman read write function for bbram. 
#define READ             						0	



/***************************************************************************************
 *===========================Memory Mapping E2prom=====================================
 ***************************************************************************************/

    //Out of 4 eeproms we are going to use only 2 eeprom i.e (U6, U7)// U6 e2prom is removed on dt.16.10.2023
//Note:- All addresses in e2prom_1page_2 shifted to e2prom_2page_2
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^e2prom_2(U7) memory map^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
	//+++++++++++++++++++++++++++ e2prom_2 page 2 starts here+++++++++++++++++++++++++//
	//
	//	0x0000 to OX0020(decimal 0 to 32)		----> are reserved for all general setting.
	//	0x0021 to 0x0050(decimal 33 to 80)		----> can be used for other required settings, counts etc.
	//	0x0051 to ------(decimal 81 to  )       ----> can be used for cable specific settings, and cable related data.
	
	//this block is dedicated for general settings of size one byte each.
    #define memory_chk_reserved_address                  0x0000
	#define gen_settings_base_address                    0x0001          //(size 16 decimal) 1 to 16
    #define test_setting_base_address                    0x0011         //(size 16 decimal)  17 to 32
	//------------------------------------------------------------------------------//
	//bytes from 0x21(decimal 33 to 80) can be used for other settings.
	#define tester_sr_no_eeprom_address                  0x0021		//(size=8 bytes)
	#define current_cable_number_address                 0x0029		//(size=1 byte)	
	#define relay_1_on_time_address                      0X002A		//(size=1 byte)
	#define relay_2_on_time_address                      0x002B		//(size=1 byte)
	#define volume_setting_address                       0x002C	    //(size=1 byte)
    #define tone_address                                 0x002D		//(size=1 byte)
	#define fail_tone_address                            0x002E 	//(size=1 byte)
    #define LEAP_COUNT_ADDRESS                           0X002F     //(size=1 byte)
    #define YEAR_LSB_COUNT_ADDRESS                       0X0030     //(size=1 byte)
    //-------------------------------------------------------------------------------------//
// bytes from 0x0051 to 0x5D0 can be used for Cable specific data and settings 
/****************************************************************************
 * IR Test Data Addresses
 ***************************************************************************/
	#define high_voltage_base_address                    0X0051		//2 X 128 bytes (size=256 bytes) decimal 81 to 208 (0x51 to 0xD0)
	#define last_ckt_number_base_address                 0X0151     //1 X 128(size=128 bytes) decimal 337 to 464 (0x151 to 0x1D0)
    #define insulation_resistance_base_address           0x01D1     //4 X 128(size=512 bytes) decimal 465 to 976 (0x1D1 to 0x3D0)
    #define hv_time_base_address                         0x03D1     //2 X 128(size=256 bytes) decimal 977 to 1232 (0x3D1 to 0x4D0) 
/***************************************************************************/
    #define withstand_test_setting_base_address          0x04D1     //1 X 128(size=128 bytes) decimal 1233 to 1360(0x4D1 t0 0x550))
    #define ir_test_setting_base_address                 0x0551     //1 X 128(size=128 bytes) decimal 1361 to 1488 (0x551 to 0x5D0)
    #define pass_count_base_address                      0x05D1     //3 X 128(size=384 bytes) decimal 1489 to 1872 (0x5D1 to 0x750)
    #define fail_count_base_address                      0x0751     //3 X 128(size=384 bytes) decimal 1873 to 2256 (0x5D1 to 0x8D0)
	#define serial_no_count_base_address                 0x08D1 	//2 X 128(size=256 bytes) decimal 2257 to 2512 (0x8D1 to 0x9D0)
    #define SHIFT_DETAILS_ADDRESS					     0x09D1 	//1 X 128(size=128 bytes) decimal 2513 to 2640 (0x9D1 to 0xA50)
	#define serial_number_base_address				     0x0A51 	//4 X 128(size=512 bytes) decimal 2641 to 3152 (0xA51 to 0xC50)
/****************************************************************************
 * Withstand Test Data Addresses
 ***************************************************************************/
	#define withstand_high_voltage_base_address          0X0C51		//2 X 128 bytes (size=256 bytes) decimal 3153 to 3408 (0xC51 to 0xD50)
	#define withstand_last_ckt_number_base_address       0X0D51     //1 X 128(size=128 bytes) decimal 3409 to 3536 (0xD51 to 0xDD0)
    #define withstand_current_base_address               0x0DD1     //4 X 128(size=512 bytes) decimal 3537 to 4048 (0xDD1 to 0xFD0)
    #define withstand_time_base_address                  0x0FD1     //2 X 128(size=256 bytes) decimal 4049 to 4304 (0xFD1 to 0x10D0) 
//-------------------------e2prom_2 page_2 ends here------------------------------------------------------------//

	//+++++++++++++++++++++++++++ e2prom_2 page_1 starts here+++++++++++++++++++++++++//e2prom used to store calibrate values
    #define input_resistance_value_y1_base_address       0x0001     //(size=4 bytes)
    #define measured_resistance_x1_base_address          0x0005     //(size=4 bytes)
    #define input_resistance_value_y2_base_address       0x0009     //(size=4 bytes)
    #define measured_resistance_x2_base_address          0x000D     //(size=4 bytes)
    #define calculated_slope_value_m_base_address        0x0011     //(size=4 bytes)
    #define calculated_offset_value_c_base_address       0x0015     //(size=4 bytes)
//-------------------------e2prom_2 page_1 ends here------------------------------------------------------------//

//+++++++++++++++++++++++++++ e2prom_3 & 4_page_1&2 starts here+++++++++++++++++++++++++//eeprom used to store label data

//Note:- EEPROM 3 & 4 are completely used for Label data storing.

//-------------------------e2prom_3 & 4_page_1&2 ends here------------------------------------------------------------//

/****************************************************************************
 * bbram 8583(rtc) memory map
 ***************************************************************************/
	//
	//	Addresses from 0x00---0x0F bytes of rtc are reserved for clock calender mode,so dont use it.
	//	next 240 bytes are available.
	//	reserved addresses
	//	0x00----0x0F---->clock calender mode(decimal 0 to 15)
	//	0x10------------>year lsb. (decimal 16)
	//	0x11------------>year msb. (decimal 17)
	//	0x12------------>year count.(decimal 18)(to identify next year change.)
	//
	#define stored_day_bbram_base_address			0x0013 //(size=1 byte) decimal 19
	#define stored_month_bbram_base_address			0x0014 //(size=1 byte) decimal 20
	#define stored_year_lsb_bbram_base_address		0x0015 //(size=1 byte) decimal 21
	#define day_code_bbram_base_address				0x0016 	//(size=1 byte) decimal 22
	#define pass_count_bbram_base_address	    	0x0017	//(size=3 byte) decimal 23 (0x0017 to 0x0019)
	#define fail_count_bbram_base_address	    	0x001A	//(size=3 byte) decimal 26 (0x001A to 0x001C)
	#define fail_bin_bbram_base_address			    0x001D //(size=1 byte) decimal 29
	#define serial_number_bbram_count_address   	0x001E//(size=1 byte) decimal 30
	#define serial_number_bbram_base_address		0x001F//(size=1 byte) decimal 31
//	//0x20 to 0x22											//problamatic	 
//	#define last_cableno_bbram_base_address			
//	#define prg_name_bbram_base_address	


	//index 1 to index 16 are reserved for test settings.
    # define every_ckt_display_result_index         1
    # define stop_at_first_fault_index              2
//	# define testing_mode_setting_index				3 
    # define remote_key_index                       3
    # define fail_bin_index                         4
	# define testing_mode_setting_index				5

	//index 17 to index 32 are reserved for gen settings.
//	# define glb_prn_lbl_index						0x11 //(decimal 17)
    # define glb_prn_lbl_index						1 //(decimal 1)
    # define read_prn_lbl_index						2 //(decimal 2)


    // Program Specific Settings indeces
    #define ir_test_index                           1
    #define withstand_test_index                    2


/****************************************************************************
 * time counts
 ***************************************************************************/
	#define count_for_5sec  	   					100		// 50 ms ISR =>(5 sec * 1000)/50ms = 5000/50 = 100.
	#define count_for_2sec  	   					40		// 50 ms ISR =>(2 sec * 1000)/50ms = 2000/50 = 40.
	#define count_for_1sec		   					20		// 50 ms ISR =>(1 sec * 1000)/50ms = 1000/50 = 20.
    #define time_out_for_serial_reception			5		// 50 ms ISR =>(1 sec * 1000)/50ms =  250/50 =  5.

/****************************************************************************
 * Label Data
 ***************************************************************************/
	#define  PRINT_DAY_CODE							0x55
	#define  PRINT_DAY_YEAR							0xAA	
	#define  PRINT_AND_RETURN						0x55
	#define  RETURN_ONLY							0xAA
    #define  PRINT_DAY_CODE							0x55
	#define  PRINT_JULIAN_DAY						0xAA	
	#define  SHIFT_DETAILS_SIZE						0X0020
    #define  serial_number_max_size              	0x0009
	#define  ESC_FUNCTION							0XFE
   	#define  LABEL_DATA_SIZE						2048     
	#define  SIGNETURE								TRUE
	#define	 NO_SIGNETURE							FALSE