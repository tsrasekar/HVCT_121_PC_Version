#include<p24fj512gu408.h>
#include "../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"

void AckI2C(void)
{
	I2C1CONLbits.ACKDT = 0;
	I2C1CONLbits.ACKEN = 1;
}

char DataRdyI2C(void)
{
     return I2C1STATbits.RBF;
}
void IdleI2C(void)
{
    //---------Wait until I2C Bus is Inactive------------------------------/
    while(I2C1CONLbits.SEN || I2C1CONLbits.PEN || I2C1CONLbits.RCEN || 
          I2C1CONLbits.ACKEN || I2C1STATbits.TRSTAT);	
}
void CloseI2C(void)
{
	// clear the I2CEN bit //
	I2C1CONLbits.I2CEN = 0;

	// clear the SI2C & MI2C Interrupt enable bits //
	_SI2C1IE = 0;
	_MI2C1IE = 0;

	// clear the SI2C & MI2C Interrupt flag bits //
	_SI2C1IF = 0;
	_MI2C1IF = 0;
}


unsigned char MasterReadI2C(void)
{
   I2C1CONLbits.RCEN = 1;
    while(I2C1CONLbits.RCEN);
    I2C1STATbits.I2COV = 0;
    return(I2C1RCV);
}
char MasterWriteI2C(unsigned char data_out)
{
	I2C1TRN = data_out;
    if(I2C1STATbits.IWCOL)        /* If write collision occurs,return -1 */
	{
		I2C1STATbits.IWCOL=0;
        return -1;
	}
    else
    {
        return 0;
    }
}
void NotAckI2C(void)
{
    I2C1CONLbits.ACKDT = 1;
    I2C1CONLbits.ACKEN = 1;
}

void RestartI2C(void)
{ 
    I2C1CONLbits.RSEN = 1;	/* initiate restart on SDA and SCL pins	*/
}
void StartI2C(void)
{
     I2C1CONLbits.SEN = 1;	/* initiate Start on SDA and SCL pins */
}
void StopI2C(void)
{
     I2C1CONLbits.PEN = 1;	/* initiate Stop on SDA and SCL pins */
}



unsigned char IC8574_ByteRead()
{
//    IdleI2C();
    StartI2C();
    DelayUs(30);
    IdleI2C();
    if(!MasterWriteI2C(read_8574_1_input))
		IdleI2C();
    byte_glb = MasterReadI2C();
    NotAckI2C();
    StopI2C();		
    DelayMs(1);
    return(byte_glb);
}

void IC8574_ByteWrite(unsigned char data)
{
//    IdleI2C();
    StartI2C();
    DelayUs(30);
    IdleI2C();
    if(!MasterWriteI2C(write_8574_3_output))
		IdleI2C();
    if(!MasterWriteI2C(data))
		IdleI2C();
    StopI2C();	
    DelayMs(1);
}
void IC8574_2_ByteWrite(unsigned char data)
{
//    IdleI2C();
    StartI2C();
    DelayUs(30);
    IdleI2C();
    if(!MasterWriteI2C(write_8574_2_output))
		IdleI2C();
    if(!MasterWriteI2C(data))
		IdleI2C();
    StopI2C();	
    DelayMs(1);
}

void Read_Write_RTC(unsigned char* ptr_local,unsigned char address_local,unsigned char size_local,unsigned char type_local)
{
     unsigned char index_local=0;		
    
		for(index_local=0;index_local<size_local;index_local++)
		{
			if(type_local==READ)
			{
				ptr_local[index_local]=ReadRtc(address_local);
			}//end if(type_local==READ)
			else if(type_local==WRITE)
			{
				WriteRtc(address_local,ptr_local[index_local]);
			}//end else if(type_local==WRITE)
			address_local++;	
		}//end for(index_local=0;index_local<size_local;index_local++)
		
 }//end void Read_Write_RTC(unsigned char* ptr_local,unsigned char address_local,unsigned char size_local,unsigned char type_local)