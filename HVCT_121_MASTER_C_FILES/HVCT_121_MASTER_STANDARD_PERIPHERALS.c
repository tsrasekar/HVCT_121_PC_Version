#include <p24FJ512GU408.h>

#include "../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"

//void SPI3_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData );
//===========================SPI Functions Start===================================//
void SPI3_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData )
{

    while( SPI3STATLbits.SPITBF == true )
    {

    }
        
    SPI3BUFL = *((uint8_t*)pTransmitData);

    while ( SPI3STATLbits.SPIRBE == true)
    {
    
    }

    *((uint8_t*)pReceiveData) = SPI3BUFL;
}

uint8_t SPI3_Exchange8bit( uint8_t data )
{
    uint8_t receiveData;
    
    SPI3_Exchange(&data, &receiveData);

    return (receiveData);
}


/*
Function:
 unsigned int read_adc_3202(unsigned char channel)
	
Description:
 This function Reads analog value from given channel. channels can be channel 0 or 1
  
Parameters:
unsigned char channel. The Channel 0 or 1 can be passed 

Return Values: It returns 12 bit value read from  ADC 3202 and is type of unsigned int.

Remarks: Nil
*/
 unsigned int read_adc_3202(unsigned char channel)
{
    unsigned int msb_lcl,lsb_lcl;
    unsigned int commandbyte;
    if(channel == 1)
    {
        commandbyte=0b11100000;     //  COMMAND FOR CHANNEL 1
    }
    else
    {
        commandbyte=0b10100000;     //  COMMAND FOR CHANNEL 0
    }
    PORTFbits.RF7= 0; // selects slave
    SPI3_Exchange8bit(0x01);                 //First send START BIT
    msb_lcl=SPI3_Exchange8bit(commandbyte); // command send to select channel
    msb_lcl= ((msb_lcl & 0x0f)<<8);// msb_lcl contains upper 4 bits  of 12 bit adc data
    lsb_lcl=SPI3_Exchange8bit(0x00);// lsb_lcl contains  lower 8 bits of 12 bit adc data
    PORTFbits.RF7= 1; // disselects the slave
    msb_lcl=(msb_lcl | lsb_lcl);  // msb_lcl contains all 12 bits data
    return (unsigned int)msb_lcl;
    
}
 

  /*
Function:
 unsigned int read_adc_IR_MEASURE(void)
	
Description:
 This function Reads analog value from channel 1 of  slave IC U13 MCP3202 
  
Parameters:
 

Return Values: It returns 12 bit value read from  ADC 3202 and is type of unsigned int.

Remarks: Nil
*/
unsigned int read_adc_IR_MEASURE(void)
{
//     CS3_3202_SPI3_rf8_pin = 1 ;
    CS1_4151_SPI3_rh0_pin = 1 ;
    unsigned int msb_lcl,lsb_lcl;
    PORTFbits.RF7 = 0; //CS2  selects slave  U13 MCP3202 (Active Low Pin)
    SPI3_Exchange8bit(0x01);                 //First send START BIT
    msb_lcl = SPI3_Exchange8bit(0b11100000); // send  0b11100000 command to select channel 1 
    msb_lcl = ((msb_lcl & 0x0f)<<8);// msb_lcl contains upper 4 bits  of 12 bit adc data
    lsb_lcl = SPI3_Exchange8bit(0b00000000);// lsb_lcl contains  lower 8 bits of 12 bit adc data
    PORTFbits.RF7 = 1; //CS2  Deselects the slave U13 MCP3202
    msb_lcl = (msb_lcl | lsb_lcl);  // msb_lcl contains all 12 bits data
    return (unsigned int)msb_lcl;
}


/*
Function:
unsigned int read_adc_HV_MONITOR(void)
	
Description:
 This function Reads analog value from channel 0 of  slave IC U13 MCP3202 
  
Parameters: 

Return Values: It returns 12 bit value read from  ADC 3202 and is type of unsigned int.

Remarks: Nil
*/
unsigned int read_adc_HV_MONITOR(void)
{   
    CS1_4151_SPI3_rh0_pin = 1 ;
    unsigned int msb_lcl=0,lsb_lcl=0;
    PORTFbits.RF7 = 0; //CS2  selects slave U13 MCP3202
    SPI3_Exchange8bit(0x01);                 //First send START BIT
    msb_lcl=SPI3_Exchange8bit(0b10100000); // command send to select channel 0
     DelayMs(1);
    msb_lcl= ((msb_lcl & 0x0f)<<8);// msb_lcl contains upper 4 bits  of 12 bit adc data
    lsb_lcl=SPI3_Exchange8bit(0x00);// lsb_lcl contains  lower 8 bits of 12 bit adc data
    PORTFbits.RF7 = 1; //CS2  disselects the slave U13 MCP3202
    msb_lcl=(msb_lcl | lsb_lcl);  // msb_lcl contains all 12 bits data
    return (unsigned int)msb_lcl;
}


/*
Function:
unsigned int read_adc_R_MEASURE(void)
	
Description:
 This function Reads analog value from channel 0 of  slave IC U12 MCP3202 
  
Parameters: 

Return Values: It returns 12 bit value read from  ADC 3202 and is type of unsigned int.

Remarks: Nil
*/
unsigned int read_adc_R_MEASURE(void)
{
    unsigned int msb_lcl,lsb_lcl;
    CS3_3202_SPI3_rf8_pin = 0; // selects slave U12 MCP3202 (Active Low Pin))
    SPI3_Exchange8bit(0x01);                 //First send START BIT
    msb_lcl=SPI3_Exchange8bit(0b10100000); // command send to select channel 0
    msb_lcl= ((msb_lcl & 0x0f)<<8);// msb_lcl contains upper 4 bits  of 12 bit adc data
    lsb_lcl=SPI3_Exchange8bit(0x00);// lsb_lcl contains  lower 8 bits of 12 bit adc data
    CS3_3202_SPI3_rf8_pin = 1; // disselects the slave U12 MCP3202
    msb_lcl=(msb_lcl | lsb_lcl);  // msb_lcl contains all 12 bits data
    return (unsigned int)msb_lcl;
        
}

/*
Function:
  	void digital_pot_write(unsigned char value_lcl)
Description:
 This functions sets the wiper position of digital potentiometer(MCP4151 10E) to selected value.  
Parameters:
unsigned char value_lcl . value_lcl can vary from 0 to 255.
Return Values: Nil

Remarks: Nil
*/
void digital_pot_write(unsigned char value_lcl)  // value 0 to 255
{
    PORTFbits.RF7 = 1;
    DelayUs(100);
    PORTHbits.RH0=0; // CS1
    SPI3_Exchange8bit(0x00);
    SPI3_Exchange8bit(value_lcl);
    PORTHbits.RH0=1;  //CS1
}

void digital_pot1_write(unsigned char value_lcl)  // value 0 to 255
{
    PORTFbits.RF7 = 1;
    DelayUs(100);
    PORTHbits.RH0=0; // CS1
    SPI3_Exchange8bit(0x11);
    SPI3_Exchange8bit(value_lcl);
    PORTHbits.RH0=1;  //CS1
}

/*
Function:
  	void Set_HighVoltage(unsigned int voltage_lcl)
Description:
 This functions sets voltage to selected value. (voltage varies from 0 to 1500)  
Parameters:
unsigned char value_lcl . voltage_lcl can vary from 0 to 1500.
Return Values: Nil

Remarks: Nil
*/

void Set_HighVoltage(unsigned int voltage_lcl)
{
    unsigned char value_lcl;
    float step_voltage = 5.859;     // voltage per step (total 256 steps)
    value_lcl = voltage_lcl / step_voltage;
    value_lcl = (256 - (value_lcl));
    
//    digital_pot_write(value_lcl); 
    digital_pot_write(value_lcl); 
}

/*
Function:
  	void Reset_HighVoltage()
Description:
 This functions Resets voltage to O volts
Parameters:
 *      Nil
Return Values: Nil

Remarks: Nil
*/

void Reset_HighVoltage(void)
{
    
    digital_pot_write(255);     //To set voltage to 0V
    
}

//===========================SPI Functions End===================================//



//===========================Serial Functions Start===================================//
/*
 Function: This function transmits a single  character byte from tester through rs232. 
 Input:Character byte to be transmitted.
 Output:None
 Calls:None.
 Called By:
*/
void Transmit_Byte_Serial_1(unsigned char byte_local)  
{													 
	 U1TXREG = byte_local;						 									 
	 while(!U1STAbits.TRMT);						 
     IFS0bits.U1TXIF = 0;	
}
/*
 Function: This function transmits a single  character byte from tester  through rs485. 
 Input:Character byte to be transmitted.
 Output:None
 Calls:None.
 Called By:SendHarnessDataToPC().
*/
void Transmit_Byte_RS485(unsigned char byte_local)  
{						
    U2TXREG = byte_local;
	 while(!U2STAbits.TRMT);						     
}//end void Transmit_Byte_Serial_1(unsigned char byte_local)


//===========================Serial Functions End===================================//



//===========================Standard Conversions Start===================================//
// this function is called by HexToBCD function mostly,and whereever BCD conversion is needed 
// and it performs similar function as assembly Decimal Adjust Accumulator instruction 
// now the function is valid only for DAA of 4 digits of the number passed.
// if any of the digit is found to be(A/B/C/D/E/F),it is changed to (0,1,2,3,4,5) and its next
// most significant digit is incremented by 1.
// special care is taken of the next MSD is F prior to increment,or it gives wrong result.

long int DAA(long int num)
{
	long int d1,d2,d3,d4,d5;
	d1 = num & 0x000F;
	d2 = (num & 0x00F0) >> 4;
	d3 = (num & 0x0F00) >> 8;
	d4 = (num & 0xF000) >> 12;
	d5 = (num & 0xF0000) >> 16;
	// before incrementing any next msd ensure that it is not F, if it is F, it should me made first 15,then incremented
	if(d2 == 0x0F)
	{
		d2 = 0x05;
		d3 = d3 + 1;
	}	
	switch(d1)
	{
		case 0x0A : 	d1 = 0x00;
						d2 = d2 + 0x01;
						break;
		case 0x0B : 	d1 = 0x01;
						d2 = d2 + 0x01;
						break;
		case 0x0C : 	d1 = 0x02;
						d2 = d2 + 0x01;
						break;
		case 0x0D : 	d1 = 0x03;
						d2 = d2 + 0x01;
						break;
		case 0x0E : 	d1 = 0x04;
						d2= d2 + 0x01;
						break;
		case 0x0F : 	d1 = 0x05;
						d2 = d2 + 0x01;
						break;
		default : 		break;	
	}
	if(d3 == 0x0F)
	{
		d3 = 0x05;
		d4 = d4 + 1;
	}	
	switch(d2)
	{
		case 0x0A : 	d2 = 0x00;
						d3 = d3 + 0x01;
						break;
		case 0x0B : 	d2 = 0x01;
						d3 = d3 + 0x01;
						break;
		case 0x0C : 	d2 = 0x02;
						d3 = d3 + 0x01;
						break;
		case 0x0D : 	d2 = 0x03;
						d3 = d3 + 0x01;
						break;
		case 0x0E : 	d2 = 0x04;
						d3= d3 + 0x01;
						break;
		case 0x0F : 	d2 = 0x05;
						d3 = d3 + 0x01;
						break;
		default : 		break;	
	}
	if(d4 == 0x0F)
	{
		d4 = 0x05;
		d5 = d5 + 1;
	}	
	switch(d3)
	{
		case 0x0A : 	d3 = 0x00;
						d4 = d4 + 0x01;
						break;
		case 0x0B : 	d3 = 0x01;
						d4 = d4 + 0x01;
						break;
		case 0x0C : 	d3 = 0x02;
						d4 = d4 + 0x01;
						break;
		case 0x0D : 	d3 = 0x03;
						d4 = d4 + 0x01;
						break;
		case 0x0E : 	d3 = 0x04;
						d4= d4 + 0x01;
						break;
		case 0x0F : 	d3 = 0x05;
						d4 = d4 + 0x01;
						break;
		default : 		break;	
	}
	switch(d4)
	{
		case 0x0A : 	d4 = 0x00;
						d5= d5 + 0x01;
						break;
		case 0x0B : 	d4 = 0x01;
						d5 = d5 + 0x01;
						break;
		case 0x0C : 	d4 = 0x02;
						d5 = d5 + 0x01;
						break;
		case 0x0D : 	d4 = 0x03;
						d5 = d5 + 0x01;
						break;
		case 0x0E : 	d4 = 0x04;
						d5= d5 + 0x01;
						break;
		case 0x0F : 	d4 = 0x05;
						d5 = d5 + 0x01;
						break;
		default : 		break;	
	}
	num = d1 | (d2 << 4) | (d3 << 8) | (d4 << 12) | (d5 << 16);
	return(num);
}

// this function converts given hexnum (one nibble) to its equivalent decimal
// the desired nible and the position of nibble(1,2,3,4 for 2 byte number) needs to be passed.
// its responsibility of calling function to call it the no of times required.
// this function converts a number passed to it in HEX ot its equivalent unpacked BCD
// the hex number and position of byte needs to be passed.

long int HexToBCD_1(unsigned int hexnum) 
{	
	
	long int multiply_0,multiply_1,multiply_2,multiply_3,multiply_4,multiply_5,multiply_6,multiply_7,multiply_8,multiply_9,multiply_10,multiply_11,multiply_12,multiply_13,multiply_14,multiply_15,multiply_16,multiply_17,multiply_18,multiply_19,multiply_20,multiply_21,multiply_22,multiply_23,answer;
	answer = 0;
	
{
				 multiply_0 = two_power_zero;                 //1
			 	 multiply_1 = two_power_one;                  //2
			 	 multiply_2 = two_power_two;                  //4 
			 	 multiply_3 = two_power_three;                //8
 			 	 multiply_4 = two_power_four;                 //16
			 	 multiply_5 = two_power_five;                 //32
			 	 multiply_6 = two_power_six;                  //64
			 	 multiply_7 = two_power_seven;                //128
 			 	 
				 multiply_8 = two_power_eight;                //256
			 	 multiply_9 = two_power_nine;                 //512
			 	 multiply_10 = two_power_ten;                 //1024
			 	 multiply_11 = two_power_eleven;              //2048
 			 	 multiply_12 = two_power_twelve;              //4096
			 	 multiply_13 = two_power_thirteen;            //8192
			 	 multiply_14 = two_power_fourteen;            //16384
			 	 multiply_15 = two_power_fifteen;             //32768
                 
                 multiply_16 = two_power_sixteen;             //65536
                 multiply_17 = two_power_seventeen;           //131072
                 multiply_18 = two_power_eignteen;            //262144
                 multiply_19 = two_power_ninteen;             //524288
                 multiply_20 = two_power_twenty;              //1048576
                 multiply_21 = two_power_twentyone;           //2097152
                 multiply_22 = two_power_twentytwo;           //4194304
                 multiply_23 = two_power_twentythree;         //8388608
                 
 			 
	}
	if(hexnum & 0x01)
	{
		answer = answer + multiply_0;
		answer = DAA(answer);
	}
	if(hexnum & 0x02)
	{
		answer = answer + multiply_1;
		answer = DAA(answer);
	}
	if(hexnum & 0x04)
	{
		answer = answer + multiply_2;
		answer = DAA(answer);
	}
	if(hexnum & 0x08)
	{
		answer = answer + multiply_3;	
		answer = DAA(answer);
	}
	if(hexnum & 0x10)
	{
		answer = answer + multiply_4;	
		answer = DAA(answer);
	}
	if(hexnum & 0x20)
	{
		answer = answer + multiply_5;	
		answer = DAA(answer);
	}
	if(hexnum & 0x40)
	{
		answer = answer + multiply_6;	
		answer = DAA(answer);
	}
	if(hexnum & 0x80)
	{
		answer = answer + multiply_7;	
	  	if((answer & 0x000F) == 0x0000 || (answer & 0x000F) == 0x0001 )//||(answer & 0x000F) == 0x0009 ||(answer & 0x000F) == 0x0008)
			answer = answer + 6;
		answer = DAA(answer);
		
	}

	if(hexnum & 0x100)
	{
		answer = answer + multiply_8;	
		answer = DAA(answer);
	}

	if(hexnum & 0x200)
	{
		answer = answer + multiply_9;	
		answer = DAA(answer);
	}

	if(hexnum & 0x400)
	{
		answer = answer + multiply_10;	
		answer = DAA(answer);
	}
	if(hexnum &0x800)
	{
		answer = answer + multiply_11;	
		answer = DAA(answer);
	}
	if(hexnum & 0x1000)
	{
		answer = answer + multiply_12;	
		answer = DAA(answer);
	}
	if(hexnum & 0x2000)
	{
		answer = answer + multiply_13;	
		answer = DAA(answer);
	}
	if(hexnum & 0x4000)
	{
		answer = answer + multiply_14;	
		answer = DAA(answer);
	}
	if(hexnum & 0x8000)
	{
		answer = answer + multiply_15;	
		answer = DAA(answer);
	}
    if(hexnum & 0x10000)
	{
		answer = answer + multiply_16;	
		answer = DAA(answer);
	}
    if(hexnum & 0x20000)
	{
		answer = answer + multiply_17;	
		answer = DAA(answer);
	}
    if(hexnum & 0x40000)
	{
		answer = answer + multiply_18;	
		answer = DAA(answer);
	}
    if(hexnum & 0x80000)
	{
		answer = answer + multiply_19;	
		answer = DAA(answer);
	}
    if(hexnum & 0x100000)
	{
		answer = answer + multiply_20;	
		answer = DAA(answer);
	}
    if(hexnum & 0x200000)
	{
		answer = answer + multiply_21;	
		answer = DAA(answer);
	}
    if(hexnum & 0x400000)
	{
		answer = answer + multiply_22;	
		answer = DAA(answer);
	}
    if(hexnum & 0x800000)
	{
		answer = answer + multiply_23;	
		answer = DAA(answer);
	}

	return(answer);
}


// this function writes a single byte at given address, in given eeprom
/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
void E2prom_Byte_Write(unsigned char address,unsigned char word_addr_msb,unsigned char word_addr_lsb,unsigned char data)
{
		IdleI2C();
		StartI2C();
		DelayUs(30); 	
		if(!MasterWriteI2C(address))					// device address
			IdleI2C();
		if(!MasterWriteI2C(word_addr_msb))				// msb -- word address
			IdleI2C();
		if(!MasterWriteI2C(word_addr_lsb))				// lsb -- word address
			IdleI2C();
		if(!MasterWriteI2C(data))						// data
			IdleI2C();
		StopI2C();
		DelayMs(5);										//FACT_512_14.
}



// this function reads a single byte from given address, from given eeprom
/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
unsigned char E2prom_Byte_Read(unsigned char address,unsigned char word_addr_msb,unsigned char word_addr_lsb)
{
	unsigned char temp;
    IdleI2C();
    StartI2C();
    IdleI2C();
    DelayUs(30);
    if(!MasterWriteI2C(address))
    	IdleI2C();
    if(!MasterWriteI2C(word_addr_msb))				
		IdleI2C();
	if(!MasterWriteI2C(word_addr_lsb))				
		IdleI2C();
    RestartI2C();
	IdleI2C();
    DelayMs(1);                     //17_05_18_1_g.
    if(!MasterWriteI2C(address | 0x01))
       IdleI2C();
    temp = MasterReadI2C();
    NotAckI2C();
    IdleI2C();
	StopI2C();
    IdleI2C();
    return(temp) ;  
}

/*-----------
Function:
Input:
Output:
Calls:
Called By:
-------------*/
void Read_Bulk_Data_Eeprom(unsigned char eeprom_address,unsigned int base_address,unsigned int variable_size,unsigned char cable_num_local,unsigned char* ram_ptr)
{
	unsigned int destination_address_local;
	unsigned int index_local=0;
	destination_address_local = base_address + ((cable_num_local-1) * variable_size);
	for(index_local=0;index_local<variable_size;index_local++)
	{
		//-------------25/11/11-------------------------//
		if(destination_address_local==0xFFFF)
		{
			destination_address_local=0x0000;
			eeprom_address=eeprom_address+0x02;
		}//end if(destination_address_local==0xFFFF)
		//----------------------------------------------//
		*(ram_ptr+index_local)=E2prom_Byte_Read((eeprom_address /*& mask_lower_byte*/),((destination_address_local)&mask_higher_byte) >> 8,(destination_address_local)& mask_lower_byte);
			destination_address_local++;
	}//end for(index_local=0;index_local<variable_size;index_local++)
}

void WriteRtc(unsigned char word_addr,unsigned char data)
{
	IdleI2C();
	StartI2C();
	DelayUs(30);
    IdleI2C();
	if(!MasterWriteI2C(rtc_address))
		IdleI2C();
	if(!MasterWriteI2C(word_addr))
		IdleI2C();
	if(!MasterWriteI2C(data))
		IdleI2C();
	StopI2C();	
	DelayMs(1);				
}



unsigned char ReadRtc(unsigned char word_addr)
{
	unsigned char data;
	IdleI2C();
	StartI2C();
	DelayUs(30);
	IdleI2C();
	if(!MasterWriteI2C(rtc_address))
		IdleI2C();
	if(!MasterWriteI2C(word_addr))
		IdleI2C();
	RestartI2C();
	IdleI2C();
    DelayMs(1);                    
	if(!MasterWriteI2C(rtc_address | 0x01))
		IdleI2C();
	data = MasterReadI2C();
	NotAckI2C();
	IdleI2C();
	StopI2C();
	DelayUs(5);
	return(data);
}


unsigned char BCDToHex(unsigned char a)
{
	unsigned char val=0;//17_05_18_1_h.
	if(a < 0x10 )
		val = a;
	else
	{
		switch(a)
		{
			case 0x10 : val = 0x0A;
					    break;
			case 0x11 : val = 0x0B;
					    break;
			case 0x12 : val = 0x0C;
					    break;
			case 0x13 : val = 0x0D;
					    break;
			case 0x14 : val = 0x0E;
					    break;
			case 0x15 : val = 0x0F;
					    break;
			case 0x16: 	val = 0x10;
						break;
			case 0x17:  val = 0x11;
						break;
			case 0x18:  val=  0x12;
						break;
			case 0x19:  val=  0x13;
						break;
			case 0x20:  val=  0x14;
						break;
			case 0x21:  val=  0x15;
						break;
			case 0x22:  val=  0x16;
						break;
			case 0x23:  val=  0x17;
						break;
			case 0x24:  val=  0x18;
						break;
			case 0x25:  val=  0x19;
						break;
			case 0x26:  val=  0x1A;
						break;
			case 0x27:  val=  0x1B;
						break;
			case 0x28:  val=  0x1C;
						break;
			case 0x29:  val=  0x1D;
						break;
			case 0x30:  val=  0x1E;
						break;
			case 0x31:  val=  0x1F;
						break;



		}
	}
	return(val);
}
void Store_Date_Time_2(unsigned char* ram_ptr)
{
	unsigned int set_year_local=0;
	unsigned char rtc_count_local=0;
	unsigned char year_lsb_local=0,day_local=0;
	unsigned char temp_local=0;

	day_local = Convert(ram_ptr,0,1);					// 0,1 date location
	year_lsb_local = Convert(ram_ptr,8,9);				// 8,9 lower 2 digits of year location

	WriteRtc(0x00,0x88);             					//control word stopcouting

	WriteRtc(0x05,Convert(ram_ptr,0,1));				//day.    
	WriteRtc(0x06,Convert(ram_ptr,3,4));				//month.
	WriteRtc(0x04,Convert(ram_ptr,10,11));				//hour.
	WriteRtc(0x03,Convert(ram_ptr,13,14));				//minute.
	WriteRtc(0x02,Convert(ram_ptr,16,17));     			//seconds.
    //-------------05_08_19_2----start--------------------//
    //following two lines are commented as year msb is hardcoded as 0x20 and year lsb is shifted to eeprom.
	//WriteRtc(0x10,0x20);    							//hardcode.
    //WriteRtc(0x11,Convert(ram_ptr,8,9));				//year lsb.
    E2prom_Byte_Write(e2prom_2_page_2_add,((YEAR_LSB_COUNT_ADDRESS)&mask_higher_byte) >> 8,(YEAR_LSB_COUNT_ADDRESS) & mask_lower_byte,year_lsb_local);       
    //-------------05_08_19_2---------------ends----------//
    rtc_count_local=0x00;
    set_year_local=(int)0x20;
	if(year_lsb_local>0x09)
	 year_lsb_local= BCDToHex(year_lsb_local);
    set_year_local=(set_year_local<<8)+ (int)year_lsb_local;
    temp_local=(char)(set_year_local%4);
    rtc_count_local|=temp_local;                    
    //-------------05_08_19_2-------start-------------------//
    //store leap count in eeprom as it is shifted from rtc ram.
    E2prom_Byte_Write(e2prom_2_page_2_add,((LEAP_COUNT_ADDRESS)&mask_higher_byte) >> 8,(LEAP_COUNT_ADDRESS) & mask_lower_byte,rtc_count_local);            
    //WriteRtc(0x12,rtc_count_local);
    temp_local=temp_local<<6;       //05_08_19_2 shifting logic is only required for day initialization.
    day_local|=temp_local;
    WriteRtc(0x05,day_local);
    WriteRtc(0x00,0x00);                                                        //start RTC counters.
    //-------------05_08_19_2-------end---------------------//
 }//end void Store_Date_Time_1(unsigned char* ram_ptr) 


//Function:
//Input:
//Output:
//Calls:
//Called By:
//-------------//
void Write_Bulk_Data_Eeprom(unsigned char eeprom_address,unsigned int base_address,unsigned int variable_size,unsigned char cable_num_local,unsigned char* ramptr)
{
	unsigned int destination_address_local;
	unsigned int index_local=0;
	destination_address_local = base_address + ((cable_num_local-1) * variable_size);
	for(index_local=0;index_local<variable_size;index_local++)
	{
		E2prom_Byte_Write(eeprom_address,((destination_address_local)&mask_higher_byte) >> 8,(destination_address_local)&mask_lower_byte,(*(ramptr+index_local)));
		destination_address_local++;
	}//end for(index_local=0;index_local<variable_size;index_local++)
}//end void Write_Bulk_Data_Eeprom(unsigned char eeprom_address,unsigned int base_address,unsigned int variable_size,unsigned char cable_num_local,unsigned char* ramptr)


void Initialise_Numeric_Counts_If_Garbage_Value(unsigned char * ram_ptr, unsigned char size_local)
{
	unsigned char index_local=0;
	for(;index_local<size_local;index_local++)
	{
		if(ram_ptr[index_local] < '0' || ram_ptr[index_local] >'9')
		{
			ram_ptr[index_local]='0';
		}
	}
}


/*
Function:This function will increment numeric array i.e. arrays containing 0x30---0x39 containts by one
		 this function is extensively used to increment serial numbers and various counter which already exists in numeric format.
Inputs: numeric array with some data(unsigned char type), size of array(unsigned char type);  
Outputs: numeric array with incremented data.
Calls:
Called By:
*/
void Increment_Numeric_Array (unsigned char* ram_ptr,unsigned char size_local)
{
	char index_local=0;

	ram_ptr[size_local-1]++;
	index_local=size_local-1;
	while(index_local>=0)
	{			
		if(ram_ptr[(unsigned char)index_local] > '9')
		{
			ram_ptr[(unsigned char)index_local] = 0x30;
			ram_ptr[(unsigned char)index_local-1]++;
			
		}//end if(ram_ptr[(unsigned char)index_local] > '9')
		index_local--;
	}//end while(index_local>=0)

}//end void Increment_Numeric_Array (unsigned char* ram_ptr,unsigned char size_local)