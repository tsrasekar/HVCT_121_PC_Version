//------------------------------SPI Start-------------------------------------------//
void SPI3_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData );
uint8_t SPI3_Exchange8bit( uint8_t data );
unsigned int read_adc_3202(unsigned char channel);
unsigned int read_adc_IR_MEASURE(void);
unsigned int read_adc_HV_MONITOR(void);
unsigned int read_adc_R_MEASURE(void);
void digital_pot_write(unsigned char value_lcl);
void digital_pot1_write(unsigned char value_lcl);
void Set_HighVoltage(unsigned int voltage_lcl);
void Reset_HighVoltage(void);
//------------------------------SPI End-------------------------------------------//


//------------------------------LCD Start-------------------------------------------//
void BusyXLCD_PIC30(void);
void WriteCmdXLCD_PIC30(unsigned char cmd);
void OpenXLCD_PIC30(void);
void WriteDataXLCD_PIC30(char data);
void SetDDRamAddr_PIC30(char address);

void Display_String_20(const unsigned char *string_pointer,unsigned char display_position,unsigned char size,unsigned char line);
void Display_Number_4digit(unsigned int num_local,unsigned char display_position, unsigned char size,unsigned char line);
void Display_Number_5digit(unsigned int num_local,unsigned char display_position, unsigned char size,unsigned char line);
void Display_Ram(unsigned char *ram_ptr,unsigned char display_position,unsigned char index_start,unsigned char size,unsigned char line);
void Display_Screen_Main_menu(unsigned char local_counter,unsigned char max_limit_local);
void Idle_Display();
void Clear_Display();
void Clear_Lcd_Line(unsigned char line);
void Start_Cursor_Blinking(unsigned char cursor_pos, unsigned char line_1ocal);
void Remove_Blinking_Cursor();
void Initialise_CN_Interrupt();
void Scroll_Programs(unsigned int counter_local);
unsigned int Navigate_Counter_3_Digit(unsigned int counter_local,unsigned char key_local,unsigned char curser_position_local);
unsigned int Navigate_Counter_4_Digit(unsigned int counter_local,unsigned char key_local,unsigned char curser_position_local);
unsigned int Navigate_Counter_5_Digit(unsigned int counter_local,unsigned char key_local,unsigned char curser_position_local);
unsigned char Display_Navigation_1(unsigned char counter_local,unsigned char key_code_local,unsigned char max_limit_local);
void Scroll_Programs_Hvct(unsigned int counter_local);
void Scroll_voltage_Hvct(unsigned int counter_local);
void Scroll_last_ckt_Hvct(unsigned int counter_local);
void Scroll_IR_Hvct(unsigned int counter_local);
void Scroll_Time_Hvct(unsigned int counter_local);
//------------------------------LCD End-------------------------------------------//

//------------------------------I2C Start-------------------------------------------//
void key_parser();
void AckI2C(void);
char DataRdyI2C(void);
void IdleI2C(void);
void CloseI2C(void);
unsigned char MasterReadI2C(void);
char MasterWriteI2C(unsigned char data_out);
void NotAckI2C(void);
void RestartI2C(void);
void StartI2C(void);
void StopI2C(void);
unsigned char IC8574_ByteRead();
void IC8574_ByteWrite(unsigned char data);
void IC8574_2_ByteWrite(unsigned char data);
unsigned char E2prom_Byte_Read(unsigned char address,unsigned char word_addr_msb,unsigned char word_addr_lsb);
void Read_Bulk_Data_Eeprom(unsigned char eeprom_address,unsigned int base_address,unsigned int variable_size,unsigned char cable_num_local,unsigned char* ram_ptr);
void E2prom_Byte_Write(unsigned char address,unsigned char word_addr_msb,unsigned char word_addr_lsb,unsigned char data);
unsigned char Navigate_Menu_Counter(unsigned char counter_local,unsigned char key_local,unsigned char max_limit_local);
unsigned char Display_Navigation(unsigned char counter_local,unsigned char key_code_local,unsigned char max_limit_local);
void Pre_Display_Before_State(unsigned char counter_local,unsigned char normal_display_flag_local,unsigned char* local_ram);
void WriteRtc(unsigned char word_addr,unsigned char data);
unsigned char ReadRtc(unsigned char word_addr);
void Store_Date_Time_2(unsigned char* ram_ptr);
void Display_Screen_Main_menu_1(unsigned char local_counter,unsigned char max_limit_local);
unsigned char Display_Navigation_1(unsigned char counter_local,unsigned char key_code_local,unsigned char max_limit_local);
void Read_Write_RTC(unsigned char* ptr_local,unsigned char address_local,unsigned char size_local,unsigned char type_local);
//------------------------------I2C End---------------------------------------------//


//----------------------Standard Conversions Start----------------------------------//
unsigned char Convert(unsigned char* ram_ptr,unsigned char a,unsigned char b);

long int DAA(long int num);
long int HexToBCD_1(unsigned int hexnum); 
unsigned char BCDToHex(unsigned char a);

//----------------------Standard Conversions End------------------------------------//


void Transmit_Byte_Serial_1(unsigned char byte_local);
void Transmit_Byte_RS485(unsigned char byte_local);
//-----------------------Standard Functions start-----------------------------------------//
unsigned char assign_slave_address_and_card_detect();
void Fill_Any_Char_Ram_With(unsigned char* test_ptr,unsigned int size_local,unsigned char data_local);
void Fill_Any_float_Ram_With(float* test_ptr, unsigned int size_local, float data_local) ;
void Retrive_Last_Selected_Program_And_Hvct_Data(void);
void Retrive_Specific_Data(unsigned char eeprom_addr_local,unsigned int address_local,unsigned int size_local,int index_local,unsigned char* ptr_local,unsigned char type_local);
void Retrive_Prg_Specific_Settings_and_data(unsigned char current_cable_local);
void Display_Setreset_Settings(unsigned char state_local,unsigned char conter_local,unsigned char line_local);
void Caliberate_Serial_1(unsigned char type_lcl);
void E2prom_test();
void Display_date_time();
unsigned int CRC_Calculate(unsigned char * transmit_buffer_lcl,unsigned int len_lcl);
unsigned int Validate_CRC_Received_Data(unsigned char* array_ptr_lcl,unsigned int index_array_ptr_lcl);
void Send_Modbus_Data(unsigned char *array_ptr_lcl,unsigned int start_index_lcl,unsigned int size_lcl);
void Fill_Small_Data_Ram(unsigned char *array_ptr_lcl,unsigned char data_lcl_1,unsigned char data_lcl_2,unsigned char data_lcl_3,unsigned char data_lcl_4,unsigned char data_lcl_5,unsigned char data_lcl_6,unsigned char data_lcl_7,unsigned char data_lcl_8,unsigned char data_lcl_9,unsigned char data_lcl_10);
void check_slave_feedback(unsigned char slave_adress);
void Write_Bulk_Data_Eeprom(unsigned char eeprom_address,unsigned int base_address,unsigned int variable_size,unsigned char cable_num_local,unsigned char* ramptr);
void Initialise_Numeric_Counts_If_Garbage_Value(unsigned char * ram_ptr, unsigned char size_local);
void Increment_Numeric_Array (unsigned char* ram_ptr,unsigned char size_local);
unsigned char Receive_Data_Char_Ram(unsigned char* ramptr, unsigned int size_local);
void Test_DigIO();
void Relay_On_Operation_1(unsigned char relay_1_local,unsigned char relay_2_local,unsigned char type_lcl);
//-----------------------Standard Functions end-----------------------------------------//


//-----------------------ANALYSE start-----------------------------------------//
void Harness_Test();
void Self_Test();
void Cards_Test();
void Display_Power_On_A(unsigned int cards_local);
void Read_Tester_Sr_No_Display();	
void Configure_Stored_Volume_Setting(unsigned char volume_level_local);
void Mute_Volume();
void continuity_test(unsigned char last_ckt_number_lcl);
void HV_IR_test(unsigned char last_ckt_number_lcl);
void Switch_Interface_Relays_To_LV(void);
void Switch_Interface_Relays_To_HV(void);
void Configure_Tester_Sr_No_Eeprom();
//--------------------------ANALYSE end-----------------------------------------//

//----------------------------state diagram functions-----------------------------------------------//
unsigned char Read_Date_Time_1(unsigned char* ram_ptr,unsigned char display_error_flag_lcl);
unsigned char Navigate_Date_Time(unsigned char* ram_ptr_lcl,unsigned char key_lcl,unsigned char curser_position_local);

unsigned char Validate_Date_Time(unsigned char* ram_ptr_lcl);
void Execute_Other_Data_Menu(unsigned char counter_local,unsigned char* ram_prt_local);
void Execute_General_Settings_Menu (unsigned char counter_local,unsigned char temp_local);
void Execute_Test_Settings_Menu (unsigned char counter_local,unsigned char temp_local);
void Execute_Prg_Settings_Menu (unsigned char counter_local,unsigned char temp_local);
void Execute_DIAGNO_Menu(unsigned char local_counter);
void card_select(unsigned char card_local);


//--------------------------TONE-----------------------------------------//
void Gen_Tone(unsigned char which_tone, unsigned char Tone_Time);
void PlayNote(unsigned int note_temp, unsigned char octave_temp, unsigned int duration_temp,unsigned int beat_speed_temp);


void HV_IR_Value(unsigned char last_ckt_number_lcl);
void Calibration();
void HV_IR_test_1(unsigned char last_ckt_number_lcl);
void Scroll_IR_Calibrate_Hvct(unsigned int counter_local);
void Run_Factory_Settings(void);
void Self_IO_Test(void);
void Check_Board();
void fail_bin_signal();
void Print_Lbl_Data ();
void Print(unsigned char);
void Read_And_Print_Lbl_Data (unsigned char *,unsigned char,unsigned char,unsigned char,unsigned char);//25/11/11.
void Print_Ram(unsigned char* ram_ptr, unsigned char size_local);
void Test_Print();
void InitializeByteArray();
void Increment_Ram_Data(unsigned char * ram_ptr,unsigned char size_local);	
void storeDataInEEPROM(unsigned char* data_array, unsigned int dataSize);
unsigned int Print_Day_2(unsigned char type_lcl,unsigned char type_lcl_2);
unsigned char Month_Data(unsigned char mnth_lcl,unsigned char year_lcl);
unsigned char Week_By_Julian_Date(void);
unsigned int Print_Day_1(unsigned char type_lcl_1,unsigned char type_lcl_2);
void Read_Convert_Shift_Data_1(unsigned char* shift_details_array_lcl);
unsigned char Allocate_E2prom_Address(unsigned char,unsigned char);//25/11/11.
unsigned long int Base_To_BCD_Converter(unsigned long int val_lcl,unsigned char base_lcl,unsigned char* result_array_lcl);
unsigned char Allocate_E2prom_Address_1(unsigned char max_prg_local);
unsigned char check_addresses_of_E2prom(unsigned char program_number );
unsigned char Search_Sr_No(unsigned char prg_lcl, unsigned char* op_ram,unsigned int p_address_lcl,unsigned int size_lcl);
unsigned char Search_Sr_No_1(unsigned char prg_lcl, unsigned char* op_ram,unsigned int p_address_lcl,unsigned int size_lcl);
void Scroll_Current_Withstand(unsigned int counter_local);
void Check_circuit_number( );
void Same_Voltage_Withstand_n_IR_Test(unsigned char last_ckt_number_lcl);



///////////////////////////////////////////////////
void Different_Voltage_Withstand_n_IR_Test(unsigned char last_ckt_number_lcl);