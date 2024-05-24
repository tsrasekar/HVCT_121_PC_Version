#include<p24fj512gu408.h>
#include  "../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"


void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{
    ByteReceived = U1RXREG;
    IFS0bits.U1RXIF = 0;
	byte_received_flag = TRUE;
}

unsigned char received_first_valid_byte,AA_counter;

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U2RXInterrupt( void )
{            
    if(IFS1bits.U2RXIF == 1)
    {  
        RS485_data_receive_cnt = 0;

            modbus_array_glb[modbus_data_array_index_glb] = U2RXREG;

            if(modbus_array_glb[modbus_data_array_index_glb] == DATA_BYTE_4_AA)
            {
                AA_counter++;                      
                if(AA_counter==4)
                {
                    AA_counter = 0;
                    AA_count_valid_flag_glb = TRUE;
                }                       
            }

            modbus_data_array_index_glb++;    
    }
    IFS1bits.U2RXIF = 0;
}


//each clock tick is at every 50 ms.
void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt (  )
{
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin
    
    if (RS485_data_receive_cnt >= 2)
    {
//       received_first_valid_byte = FALSE;
       
       if (modbus_data_array_index_glb != 0 && AA_count_valid_flag_glb == TRUE)
       {
           assign_slave_address_flag_glb = TRUE;
            data_reception_complete_flag_glb = TRUE;
            timer1_counter =0;
            AA_count_valid_flag_glb = FALSE;
            AA_counter = 0;
       }
    }
    
   
    RS485_data_receive_cnt++;
    timer1_counter++;
    
    IFS0bits.T1IF = 0;
    
}

//each clock tick is at every 100 ms.
void __attribute__ ( ( interrupt, no_auto_psv ) ) _T2Interrupt (  )
{
    /* Check if the Timer Interrupt/Status is set */

    timer2_counter++;
    sec_counter++;
    IFS0bits.T2IF = false;
}




void __attribute__ (( interrupt, no_auto_psv )) _IOCInterrupt ( void )
{
    volatile unsigned int d_cnt_local=0;
    
    if((IFS1bits.IOCIF == 1) && (key_pressed_flag==FALSE))
    {
        
//        if(IOCFDbits.IOCFD1 == 1)
//        {
            d_cnt_local=8000;//earlier 2000; 17_05_18_1_n.
			while(d_cnt_local--);
            key_pressed_flag = TRUE;
    }
    IFS1bits.IOCIF = 0;// Clear the flag
}