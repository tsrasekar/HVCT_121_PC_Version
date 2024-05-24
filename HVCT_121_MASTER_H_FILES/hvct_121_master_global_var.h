volatile unsigned char temp_modbus_glb;
volatile unsigned int modbus_data_array_index_glb=0;
unsigned char my_address_glb;
unsigned char modbus_array_glb[50];
unsigned char modbus_receive_enable_flag_glb=0;
unsigned volatile int modbus_array_index_glb=0;
unsigned volatile int time_count_mdbus_glb=0;
volatile unsigned int receive_data_index_glb=0;
volatile unsigned char i_am_fine_flag_glb=0;
volatile unsigned char CRC_valid_flag_glb;
volatile unsigned int timer1_counter=0, RS485_data_receive_cnt=0;
volatile unsigned char timer2_counter=0;
volatile unsigned char sec_counter=0;
volatile unsigned char AA_count_valid_flag_glb;
volatile unsigned char data_reception_complete_flag_glb;
volatile unsigned char assign_slave_address_flag_glb;
volatile unsigned char ack_valid_flag_glb;
volatile unsigned char continuity_test_pass_flag_glb=0;
volatile unsigned char continuity_test_fail_flag_glb=0;
volatile unsigned char ir_test_pass_flag_glb=0;
volatile unsigned char ir_test_fail_flag_glb=0;
volatile unsigned char withstand_test_pass_flag_glb=0;
volatile unsigned char withstand_test_fail_flag_glb=0;
volatile unsigned char byte_received_flag=0;//sets in reception isr after every byte is received in U2RXREG,made false in learn.c where there is reception.(PHTM_1_0.c,Learn.c,ISR.c)
volatile int timecount=0;//this is incremented at every isr,unless cleared externally in other functon.used to count time i.e.for each count t=50ms.(PHTM_1_0.c,Power_On_Status.c,ISR.c)
volatile unsigned int ByteReceived;//this register is used to collect a data byte received from U2RXREG through rs232.(Learn.c,ISR.c).
unsigned char volume_level_global=0;
unsigned int read_data = 0;
float monitor_voltage_glb = 0;
float IR_Measure_Voltage_glb = 0;
unsigned int voltage_integer_part = 0;
unsigned int voltage_fractional_part = 0;
float read_voltage_glb = 0;
float Volts_glb[50];
float ir_result_arr[];
unsigned char store_result_glb[33];
unsigned char gen_settings_ram[17];
unsigned char test_settings_ram[6];
unsigned char prg_specific_setting_ram[6];
unsigned char analyse_flag=0;
unsigned char* disp_start_ptr;//current
unsigned char current_cable_number=0;
unsigned int  current_prg_local=0;
unsigned int selected_hv_voltage_glb=0;
unsigned int hv_voltage_glb=0;
unsigned int insulation_resistance_glb=0;
unsigned int hv_withstand_time_glb=0;
unsigned int IR_glb=0;
unsigned char hv_msb_glb=0;
unsigned char hv_lsb_glb=0;
unsigned int ir_last_ckt_no_glb=0;
unsigned int last_ckt_no_glb=0;
volatile unsigned char key_pressed_flag;
unsigned char key_data_glb;
unsigned char byte_glb;
unsigned char temp_global =0;
volatile unsigned char key_code=0;//current
unsigned char cards;//indicates number of cards present in tester.
unsigned char self_test_pass_flag;//if set indicates board is empty i.e.no points are connected.
unsigned char digit_ram[34];
unsigned char top_position=1;
unsigned char menu_counter=0;
unsigned char arrow_position=0;
unsigned char cursor_position_global=1;
unsigned char arrow_pos_global=1;
volatile unsigned char state_code;
unsigned int input_resistance_value_y1=0;
unsigned int input_resistance_value_y2=0;
unsigned int measured_resistance_x1;
unsigned int measured_resistance_x2;
unsigned int input_resistance_value;
volatile unsigned char calibrate_flag_glb=0;
volatile unsigned char last_ckt_number_flag=0;
volatile unsigned char self_test_flag=0;
volatile unsigned char digital_IP_flag_1;
volatile unsigned char fail_bin_data;
unsigned char label_data[186];
volatile unsigned int serial_number_count;
unsigned char fct_array[50];
unsigned char operator_code_1[10];
unsigned char operator_code_2[10];
unsigned char first_shift_hh_glb=0,second_shift_hh_glb=0,third_shift_hh_glb=0,first_shift_mm_glb=0,second_shift_mm_glb=0,third_shift_mm_glb=0;
unsigned char first_shift_character_glb=0,second_shift_character_glb=0,third_shift_character_glb=0;
unsigned char temp_char_glb=0;//11_10_18_1.
unsigned char srl_isr_glb_index=0;
unsigned char digit_ram_1[2050];
volatile unsigned int receive_label_data_glb=0;
unsigned char serial_number_searched_flag_glb=0;//26_06_19.

unsigned int withstand_last_ckt_no_glb;
unsigned int withstand_hv_voltage_glb=0;
unsigned int withstand_current_glb=0;
volatile unsigned char combine_test_flag_glb=0;
