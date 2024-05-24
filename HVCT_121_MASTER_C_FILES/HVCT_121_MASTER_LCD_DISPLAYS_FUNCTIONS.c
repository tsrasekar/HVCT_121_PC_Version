#include<p24fj512gu408.h>

#include "../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"


//-----------------------LCD Fundamental Functions------------------------------//
/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
void BusyXLCD_PIC30(void)
{
     /* Configure all pins as input */
	 
     tris_lcd_data_pin_7  = 1;
     DelayUs(50);

 	do
	{		
    	lcd_rselect_pin = 0;           /* for selecting AC for read */
        DelayUs(50);
		lcd_rdwr_pin = 1;           	/* for read operation */ 
		DelayUs(50);                   
        
    	lcd_enable_pin = 0;            /* for enabling read/write */
		DelayUs(50);                   
 		lcd_enable_pin = 1; 
		DelayUs(50);                   
	}
     while(lcd_data_pin_7 != 0);        /* busy flag */

 	lcd_rdwr_pin = 1; 
    DelayUs(50);
    
	//while leaving back to output mode
    tris_lcd_data_pin_7  = 0;
    DelayUs(50);
}

/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
void WriteCmdXLCD_PIC30(unsigned char cmd)
{
	//As 4 bit interface, at a time, out only 4 bits
	/* configure the common data pins as output */
    tris_lcd_data_pin_7  = 0;
    tris_lcd_data_pin_6  = 0;
    tris_lcd_data_pin_5  = 0;
    tris_lcd_data_pin_4  = 0;

   	lcd_rselect_pin   = 0;  /* select Instruction Reg */
    DelayUs(50); 
 	lcd_rdwr_pin=0;    		/*ENABLE WRITE*/
	DelayUs(50);                  
    
	lcd_data_pin_7 = (unsigned int)((cmd & 0x08)>>3);
    lcd_data_pin_6 = (unsigned int)((cmd & 0x04)>>2);
    lcd_data_pin_5 = (unsigned int)((cmd & 0x02)>>1);
    lcd_data_pin_4 = (unsigned int)(cmd & 0x01);
	DelayUs(50);                  
    
 	lcd_enable_pin= 1;
	DelayUs(500);                   
  	lcd_enable_pin = 0;
	DelayUs(50);                    
    
 	lcd_rdwr_pin=1;
	DelayUs(50);				   
	BusyXLCD_PIC30();
    DelayUs(50);
}

void OpenXLCD_PIC30(void)
{
	DelayMs(200);						
    
    tris_lcd_data_pin_7  = 0;
    tris_lcd_data_pin_6  = 0;
    tris_lcd_data_pin_5  = 0;
    tris_lcd_data_pin_4  = 0;    

    tris_rdwr_pin  =  0;   
    tris_rselect_pin =0;  
    tris_enable_pin = 0;     

    
	WriteCmdXLCD_PIC30(0x03);
	DelayMs(10);
	WriteCmdXLCD_PIC30(0x03);
	DelayMs(10);			
  	WriteCmdXLCD_PIC30(0x03);
		
 
 	WriteCmdXLCD_PIC30(0x02);
  
	WriteCmdXLCD_PIC30(0x02);
  	WriteCmdXLCD_PIC30(0x08);	

	WriteCmdXLCD_PIC30(0x00);
  	WriteCmdXLCD_PIC30(0x08);	

	WriteCmdXLCD_PIC30(0x00);
  	WriteCmdXLCD_PIC30(0x0F);	

	WriteCmdXLCD_PIC30(0x00);
  	WriteCmdXLCD_PIC30(0x06);	

	WriteCmdXLCD_PIC30(0x08);
  	WriteCmdXLCD_PIC30(0x00);	

	WriteCmdXLCD_PIC30(0x01);
  	WriteCmdXLCD_PIC30(0x04);	
	
	WriteCmdXLCD_PIC30(0x00);
  	WriteCmdXLCD_PIC30(0x01);
    
    WriteCmdXLCD_PIC30(0x00);  
  	WriteCmdXLCD_PIC30(0x0C);   
} // end of function


/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
void WriteDataXLCD_PIC30(char data)
{		
   	/* configure the common data pins as output */
    tris_lcd_data_pin_7  = 0;
    tris_lcd_data_pin_6  = 0;
    tris_lcd_data_pin_5  = 0;
    tris_lcd_data_pin_4  = 0;
   
	lcd_rselect_pin  = 1;  /* select Data Reg */
    DelayUs(250);
    DelayUs(250);
  	lcd_rdwr_pin = 0;  	   /* enable write */
  	DelayUs(50);                 
    
    lcd_data_pin_7 = (unsigned int)((data & 0x80)>>7);
    lcd_data_pin_6 = (unsigned int)((data & 0x40)>>6);
    lcd_data_pin_5 = (unsigned int)((data & 0x20)>>5);
    lcd_data_pin_4 = (unsigned int)((data & 0x10)>>4);
	DelayUs(50);                  
    
    lcd_enable_pin= 1;
	DelayUs(250);                  
	DelayUs(250);                   
    lcd_enable_pin =0;
	DelayUs(50);                   

   	lcd_rdwr_pin = 1;   /* enable read */
	DelayUs(50);
	BusyXLCD_PIC30();
    DelayUs(250);


	lcd_rselect_pin  = 1;   /* select Data Reg */
    DelayUs(50);
  	lcd_rdwr_pin = 0;  		/* enable write */
    DelayUs(50);            
    
    lcd_data_pin_7 = (unsigned int)((data & 0x08)>>3);
    lcd_data_pin_6 = (unsigned int)((data & 0x04)>>2);
    lcd_data_pin_5 = (unsigned int)((data & 0x02)>>1);
    lcd_data_pin_4 = (unsigned int)(data & 0x01);
	DelayUs(50);                  
    
    lcd_enable_pin= 1;
	DelayUs(250);                   
	DelayUs(250);                  
    lcd_enable_pin =0;
	DelayUs(50);                   

   	lcd_rdwr_pin = 1;   	/* enable read */
	DelayUs(50);
	BusyXLCD_PIC30();
    DelayUs(250);
    DelayUs(250);

}  /* end of function */

/*
 Function:
 Input: 
 Output:
 Calls:
 Called By:
*/
void SetDDRamAddr_PIC30(char address)
{
    /* configure the common data pins as output */
    tris_lcd_data_pin_7 = 0;
    tris_lcd_data_pin_6  = 0;
    tris_lcd_data_pin_5  = 0;
    tris_lcd_data_pin_4  = 0;

   	lcd_rselect_pin = 0;   /* select Instruction Reg */
    DelayUs(50);
   	lcd_rdwr_pin = 0;      /* enable write */
	DelayUs(50);                  
    /*DB7 must be 1 for setting DDRam ADDR */
    lcd_data_pin_7 = 1;
    /* address is only of 7 bits in length */
    lcd_data_pin_6 = (unsigned int) ((address & 0x40)>>6); 
    lcd_data_pin_5 = (unsigned int) ((address & 0x20)>>5);
	lcd_data_pin_4 = (unsigned int) ((address & 0x10)>>4);
	DelayUs(50);                  
    
    lcd_enable_pin = 1;
	DelayUs(250);                 
	DelayUs(250);                 
    lcd_enable_pin = 0;
	DelayUs(50);                   

	lcd_rdwr_pin=1;
	DelayUs(50);

	BusyXLCD_PIC30();
	DelayUs(250);
	DelayUs(250);

 	lcd_rselect_pin = 0;   /* select Instruction Reg */
    DelayUs(50);
   	lcd_rdwr_pin = 0;      /* enable write */
	DelayUs(50);                   
    
    lcd_data_pin_7 = (unsigned int)((address & 0x08)>>3);
    lcd_data_pin_6 = (unsigned int)((address& 0x04)>>2);
    lcd_data_pin_5 = (unsigned int)((address & 0x02)>>1);
    lcd_data_pin_4 = (unsigned int)((address & 0x01)>>0);
    DelayUs(50);                   
    
	lcd_enable_pin = 1;
	DelayUs(250);
	DelayUs(250);
    lcd_enable_pin = 0;
	DelayUs(50);                   

	lcd_rdwr_pin=1;
	DelayUs(50);
	BusyXLCD_PIC30();
    DelayUs(250);
    DelayUs(250);
}  /*  end of function */



//---------------------------------LCD Display Functions----------------------------//

/*
 Function:This function is made to display string for maximum 20 characters.
		  a) *string_pointer= starting address of static string defined.
		  b) display_positon= on 20*1 line of 20*4 lcd; starting position indicates ddram address of first character of the string.
		      i.e. display to be started location on lcd.
		  c) size= total number of characters to be displayed.
		  d) line=on 20*4 lcd; line indicates first,second,third,or forth line.
		  since each line can accomodate only 20 characters, if size and position increased beyond limit,then display will be
		  adjusted for liited characters per line.
 Input: (const unsigned char *,unsigned char,unsigned char,unsigned char).
 Output:Diplay on 20*4 lcd.
 Calls:SetDDRamAddr_PIC30(),WriteDataXLCD_PIC30.
 Called By:
*/

void Display_String_20(const unsigned char *string_pointer,unsigned char display_position,unsigned char size,unsigned char line)
{
    unsigned char data_local,index_local=0;

	//----------------------------------------------------------------//
	//this block adjusts total number of characters to be displayd according to display position and size mentioned 
	//so that next line remain intact.

	if(size+display_position>20)
		size=20-display_position+1;
	else
		size=size;	
	//---------------------------------------------------------------//
	//---------------------------------------------------------------//
	 // assign ddram address according to line selected in attributes.

	switch(line)
	{
		case 1:SetDDRamAddr_PIC30(LCD_ADD_LINE_1+display_position-1);
				break;
		case 2:SetDDRamAddr_PIC30(LCD_ADD_LINE_2+display_position-1);
				break;
		case 3:SetDDRamAddr_PIC30(LCD_ADD_LINE_3+display_position-1);
				break;
		case 4:SetDDRamAddr_PIC30(LCD_ADD_LINE_4+display_position-1);
				break;
		default:
				break;
	}//end switch.

	//----------------------------------------------------------------//
	//----------------------------------------------------------------//
	// display according to size.
	for(index_local=0;index_local<size;index_local++)
	{
		data_local= string_pointer[index_local];						//assign pointer.
		WriteDataXLCD_PIC30(data_local);								//write data byte.						
	}//end for
	//-----------------------------------------------------------------//  
}


/*
 Function:This function will convert hex number in to bcd format and display it accordingly.
		  a)Maximum 4 digit number is allowed.
		  b)num_local= number to be displayed hex value.
		  c)display_position=on 20*1 line of 20*4 lcd; starting position indicates ddram address of first character of the string.
		      i.e. display to be started location on lcd.
		  d)size=total number of characters to be displayed.
		  e)line=n 20*4 lcd; line indicates first,second,third,or forth line.
          since each line can accomodate only 20 characters, if size and position increased beyond limit,then display will be
		  adjusted for liited characters per line.
 Input:(unsigned int,unsigned char,unsigned char,unsigned char).
 Output:Displays number.
 Calls:HexToBCD_1(),DAA(),SetDDRamAddr_PIC30(), WriteDataXLCD_PIC30().
 Called By:
*/
void Display_Number_4digit(unsigned int num_local,unsigned char display_position, unsigned char size,unsigned char line)
{
	long int bcdnum_local=0;
	unsigned char digit_ram_local[5];
	
	//----------------------------------------------------------------//
	//this block adjusts total number of characters to be displayd according to display position and size mentioned 
	//so that next line remain intact.
	if(size+display_position>20)
		size=20-display_position+1;
	else
		size=size;	
	//---------------------------------------------------------------//
	
	//---------------------------------------------------------------//
	//convert in to bcd format for display.
	bcdnum_local = HexToBCD_1(num_local);
	bcdnum_local = DAA(bcdnum_local);

	digit_ram_local[4] = ((bcdnum_local & 0x000F)+0x30);				//lsb
	digit_ram_local[3] = (((bcdnum_local & 0x00F0) >> 4)+0x30);
	digit_ram_local[2] = (((bcdnum_local & 0x0F00) >> 8)+0x30);
	digit_ram_local[1] = (((bcdnum_local & 0xF000) >> 12)+0x30);
    digit_ram_local[0] = (((bcdnum_local & 0xF0000) >> 16)+0x30);         //msb.
	//---------------------------------------------------------------//
	//---------------------------------------------------------------//
	// assign ddram address according to line selected in attributes.
	switch(line)
	{
		case 1:SetDDRamAddr_PIC30(0x00+display_position-1);
				break;
		case 2:SetDDRamAddr_PIC30(0x40+display_position-1);
				break;
		case 3:SetDDRamAddr_PIC30(0x14+display_position-1);
				break;
		case 4:SetDDRamAddr_PIC30(0x54+display_position-1);
				break;
		default:
				break;
	}//end switch.
	//---------------------------------------------------------------//
	//----------------------------------------------------------------//
	// display according to size i.e. no of digits. 
//	switch(size)
//	{
//		case 1: WriteDataXLCD_PIC30(digit_ram_local[3]);
//				break;
//		case 2: WriteDataXLCD_PIC30(digit_ram_local[2]);
//				WriteDataXLCD_PIC30(digit_ram_local[3]);
//				break;
//		case 3: WriteDataXLCD_PIC30(digit_ram_local[1]);
//				WriteDataXLCD_PIC30(digit_ram_local[2]);
//				WriteDataXLCD_PIC30(digit_ram_local[3]);
//				break;
//		case 4: WriteDataXLCD_PIC30(digit_ram_local[0]);
//				WriteDataXLCD_PIC30(digit_ram_local[1]);
//				WriteDataXLCD_PIC30(digit_ram_local[2]);
//				WriteDataXLCD_PIC30(digit_ram_local[3]);
//				break;
//		default:
//				break;
//	}//end switch.
    switch(size)
	{
		case 1: WriteDataXLCD_PIC30(digit_ram_local[4]);
				break;
		case 2: WriteDataXLCD_PIC30(digit_ram_local[3]);
				WriteDataXLCD_PIC30(digit_ram_local[4]);
				break;
		case 3: WriteDataXLCD_PIC30(digit_ram_local[2]);
				WriteDataXLCD_PIC30(digit_ram_local[3]);
				WriteDataXLCD_PIC30(digit_ram_local[4]);
				break;
		case 4: WriteDataXLCD_PIC30(digit_ram_local[1]);
				WriteDataXLCD_PIC30(digit_ram_local[2]);
				WriteDataXLCD_PIC30(digit_ram_local[3]);
				WriteDataXLCD_PIC30(digit_ram_local[4]);
				break;
        case 5: WriteDataXLCD_PIC30(digit_ram_local[0]);
				WriteDataXLCD_PIC30(digit_ram_local[1]);
				WriteDataXLCD_PIC30(digit_ram_local[2]);
				WriteDataXLCD_PIC30(digit_ram_local[3]);
                WriteDataXLCD_PIC30(digit_ram_local[4]);
				break;
		default:
				break;
	}//end switch.
   //----------------------------------------------------------------//

}




/*
 Function:This function will convert hex number in to bcd format and display it accordingly.
		  a)Maximum 4 digit number is allowed.
		  b)num_local= number to be displayed hex value.
		  c)display_position=on 20*1 line of 20*4 lcd; starting position indicates ddram address of first character of the string.
		      i.e. display to be started location on lcd.
		  d)size=total number of characters to be displayed.
		  e)line=n 20*4 lcd; line indicates first,second,third,or forth line.
          since each line can accomodate only 20 characters, if size and position increased beyond limit,then display will be
		  adjusted for liited characters per line.
 Input:(unsigned int,unsigned char,unsigned char,unsigned char).
 Output:Displays number.
 Calls:HexToBCD_1(),DAA(),SetDDRamAddr_PIC30(), WriteDataXLCD_PIC30().
 Called By:
*/
void Display_Number_5digit(unsigned int num_local,unsigned char display_position, unsigned char size,unsigned char line)
{
	long int bcdnum_local=0;
	unsigned char digit_ram_local[5];
	
	//----------------------------------------------------------------//
	//this block adjusts total number of characters to be displayd according to display position and size mentioned 
	//so that next line remain intact.
	if(size+display_position>20)
		size=20-display_position+1;
	else
		size=size;	
	//---------------------------------------------------------------//
	
	//---------------------------------------------------------------//
	//convert in to bcd format for display.
	bcdnum_local = HexToBCD_1(num_local);
	bcdnum_local = DAA(bcdnum_local);

	digit_ram_local[4] = ((bcdnum_local & 0x000F)+0x30);				//lsb
	digit_ram_local[3] = (((bcdnum_local & 0x00F0) >> 4)+0x30);
	digit_ram_local[2] = (((bcdnum_local & 0x0F00) >> 8)+0x30);
	digit_ram_local[1] = (((bcdnum_local & 0xF000) >> 12)+0x30);		//msb.
    digit_ram_local[0] = (((bcdnum_local & 0xF0000) >> 16)+0x30);
	//---------------------------------------------------------------//
	//---------------------------------------------------------------//
	// assign ddram address according to line selected in attributes.
	switch(line)
	{
		case 1:SetDDRamAddr_PIC30(0x00+display_position-1);
				break;
		case 2:SetDDRamAddr_PIC30(0x40+display_position-1);
				break;
		case 3:SetDDRamAddr_PIC30(0x14+display_position-1);
				break;
		case 4:SetDDRamAddr_PIC30(0x54+display_position-1);
				break;
		default:
				break;
	}//end switch.
	//---------------------------------------------------------------//
	//----------------------------------------------------------------//
	// display according to size i.e. no of digits. 
	switch(size)
	{
		case 1: WriteDataXLCD_PIC30(digit_ram_local[3]);
				break;
		case 2: WriteDataXLCD_PIC30(digit_ram_local[2]);
				WriteDataXLCD_PIC30(digit_ram_local[3]);
				break;
		case 3: WriteDataXLCD_PIC30(digit_ram_local[1]);
				WriteDataXLCD_PIC30(digit_ram_local[2]);
				WriteDataXLCD_PIC30(digit_ram_local[3]);
				break;
		case 4: WriteDataXLCD_PIC30(digit_ram_local[0]);
				WriteDataXLCD_PIC30(digit_ram_local[1]);
				WriteDataXLCD_PIC30(digit_ram_local[2]);
				WriteDataXLCD_PIC30(digit_ram_local[3]);
				break;
 		case 5: WriteDataXLCD_PIC30(digit_ram_local[0]);
				WriteDataXLCD_PIC30(digit_ram_local[1]);
				WriteDataXLCD_PIC30(digit_ram_local[2]);
				WriteDataXLCD_PIC30(digit_ram_local[3]);
                WriteDataXLCD_PIC30(digit_ram_local[4]);
				break;               
		default:
				break;
	}//end switch.
   //----------------------------------------------------------------//

}







/*
 Function:This function will display ram contents according to attributes passed.
		  a)*ram_ptr=starting address of ram.
		  b)display_position=on 20*1 line of 20*4 lcd; starting position indicates ddram address of first character of the string.
		      i.e. display to be started location on lcd.
		  c)index_start=starting index of ram from which display to be started.
		  d)size=total number of characters to be displayed.
		  e)line=n 20*4 lcd; line indicates first,second,third,or forth line.
          since each line can accomodate only 20 characters, if size and position increased beyond limit,then display will be
		  adjusted for liited characters per line.
 Input:(unsigned char*,unsigned char,unsigned char,unsigned char).
 Output:Displays number.
 Calls:SetDDRamAddr_PIC30(), WriteDataXLCD_PIC30().
 Called By:
*/
void Display_Ram(unsigned char *ram_ptr,unsigned char display_position,unsigned char index_start,unsigned char size,unsigned char line)
{
	unsigned char index_local=0;
	//----------------------------------------------------------------//
	//this block adjusts total number of characters to be displayd according to display position and size mentioned 
	//so that next line remain intact.
	//if(size+display_position>20)
	//	size=20-display_position+1;
	//else
	//	size=size;	
	//----------------------------------------------------------------//
	//----------------------------------------------------------------//
	// assign ddram address according to line selected in attributes.
	switch(line)
	{
		case 1:SetDDRamAddr_PIC30(0x00+display_position-1);
				break;
		case 2:SetDDRamAddr_PIC30(0x40+display_position-1);
				break;
		case 3:SetDDRamAddr_PIC30(0x14+display_position-1);
				break;
		case 4:SetDDRamAddr_PIC30(0x54+display_position-1);
				break;
		default:
				break;
	}//end switch.
	//----------------------------------------------------------------//
	//----------------------------------------------------------------//
	// display according to size.
	for(index_local=1;index_local<=size;index_local++)
	{
		WriteDataXLCD_PIC30(ram_ptr[index_start]);
		index_start++;
	}//end for.
	//----------------------------------------------------------------//

}

/*
 Function:This functions clears display completely.	  
 Input:none.
 Output:none.
 Calls:none.
 Called By:
*/
void Clear_Display()
{
	WriteCmdXLCD_PIC30(0X00);	
	WriteCmdXLCD_PIC30(0X01);
}
/*
 Function: This Functions Clears the the line
 *  Clear_Lcd_Line(unsigned char line)
 Input:line number
 Output:none.
 Calls:none.
 Called By:
*/
void Clear_Lcd_Line(unsigned char line)
{
    switch(line)
    {
        case 1:  
                Display_String_20(clr_line,1,20,line);
                break;
        case 2:  
                Display_String_20(clr_line,1,20,line);
                break;
        case 3:  
                Display_String_20(clr_line,1,20,line);
                break;
        case 4:  
                Display_String_20(clr_line,1,20,line);
                break;
        default:
                break;
    }
	
}

/*
Function:This function will display blinking cursor at position an line specified in attributes. 
Inputs:cursor position (unsigned char), line to be selected (unsigned char).
Outouts:none.
Calls:SetDDRamAddr_PIC30(),WriteCmdXLCD_PIC30().
Called By:
*/
void Start_Cursor_Blinking(unsigned char cursor_pos, unsigned char line_1ocal)
{
	//----------------------------------------------------------//
	//set ddram address according to line attribute.
	switch(line_1ocal)
	{
		case 1:SetDDRamAddr_PIC30(0x00+cursor_pos-1);
				break;
		case 2:SetDDRamAddr_PIC30(0x40+cursor_pos-1);
				break;
		case 3:SetDDRamAddr_PIC30(0x14+cursor_pos-1);
				break;
		case 4:SetDDRamAddr_PIC30(0x54+cursor_pos-1);
				break;
		default:
				break;
	}//end switch.
	//----------------------------------------------------------//
	//command to blink cursor.
	WriteCmdXLCD_PIC30(0x00);
	WriteCmdXLCD_PIC30(0x0f);
	//----------------------------------------------------------//
}

/*
Function:This function will remove blinking cursor on the display. 
Inputs:--None.
Outouts:--None.
Calls:WriteCmdXLCD_PIC30().
Called By:
*/
void Remove_Blinking_Cursor()
{
	//--------------------------------------------------------//
	//command to stop blinking.
	WriteCmdXLCD_PIC30(0x00);
	WriteCmdXLCD_PIC30(0x0C);
	//--------------------------------------------------------//
}

/*
 Function:This function is made to display various menus on nova tester.
		  a) it will displays three lines i.e. total six menus at atime.
		  b) each line will comprised of two menus with maximum width of 9 characters.
		  c) for menuus reaching boundary values i.e.(MAX and MIN limits), next line will be roll over.
		  d) proper input is to be provided for correct working.
 Input: (unsigned char), menu counter; this menu will be displayed as a starting menu on first line and so on....
 Output:Diplay on 20*4 three lines (line 2,line 3,line 4).
 Calls:Display_String_20.
 Called By:
*/
void Display_Screen_Main_menu(unsigned char local_counter,unsigned char max_limit_local)
{
	unsigned char pos_local,index_local;
    const unsigned char* ptr_local;
    
		max_limit_local=max_limit_local/2;
		pos_local=0;                                                            //initialise position.
		//----------------------------------------------------------------------//
		//display three line every times(i.e. 2'nd,3'rd, and 4'th line)
		for(index_local=1;index_local<=3;index_local++)
		{
         
			ptr_local=(disp_start_ptr+((local_counter-1)*18));                  //calculate display pointer(start address from menu counter).
        //for menu counter, grater than maximum or less than minium value, initialise it to one.(for rolling display)
			if(local_counter>max_limit_local)
			{
				ptr_local=disp_start_ptr;
				local_counter=1;
			}
			else if(local_counter==MIN_LIMIT_MENU)
			{
				ptr_local=disp_start_ptr;
			}	
            
			Display_String_20(ptr_local,pos_local+2,9,index_local+1);           //display first LHS section
			Display_String_20(ptr_local+9,pos_local+12,9,index_local+1);      //display first RHS section
            
			local_counter++;												    //increment counter 
		}//end for(index_local=1;index_local<=3;index_local++).
		//------------------------------------------------------------------------//
}//end void Display_Screen_Main_menu(unsigned char local_counter,unsigned char max_limit_local).


/*
 Function:After all initialisations on power on this function displays idle state menu and initialises display variables accrdingly,
		  so that user can start operation.
 Input:none.
 Output:none.
 Calls:Display_Number_4digit,Display_Screen_Main_menu,SetDDRamAddr_PIC30, WriteDataXLCD_PIC30().
 Called By:
*/
void Idle_Display()
{
	Display_String_20(menu,2,5,1);
	Display_String_20(main_1,7,4,1);
	Display_String_20(pn,16,2,1);
	Display_Number_4digit(current_cable_number,18,3,1);
	Display_Screen_Main_menu(top_position,MAX_LIMIT_MAIN_MENU);
	SetDDRamAddr_PIC30(0x40);
	WriteDataXLCD_PIC30('>');
	menu_counter=1;
	arrow_position=1;
}

/*
 Function:This function is made to display various menus on nova tester.
		  a) it will displays three lines i.e. total six menus at atime.
		  b) each line will comprised of two menus with maximum width of 9 characters.
		  c) for menuus reaching boundary values i.e.(MAX and MIN limits), next line will be roll over.
		  d) proper input is to be provided for correct working.
 Input: (unsigned char), menu counter; this menu will be displayed as a starting menu on first line and so on....
 Output:Diplay on 20*4 three lines (line 2,line 3,line 4).
 Calls:Display_String_20.
 Called By:
*/
void Display_Screen_Main_menu_1(unsigned char local_counter,unsigned char max_limit_local)
{
	unsigned char pos_local,index_local;
	unsigned int ptr_local;


		max_limit_local=max_limit_local;
		pos_local=0;												//initialise position.
		//-----------------------------------------------------------------------//
		//display three line every times(i.e. 2'nd,3'rd, and 4'th line)
		for(index_local=1;index_local<=3;index_local++)
		{
            //------17_05_18_1_e------start----------------//
			ptr_local=(disp_start_ptr+((local_counter-1)*9));		//current //calculate dsiplay pointer(start address from menu counter).
			//ptr_local=(disp_start_ptr+((local_counter-1)*10));	//earlier //calculate dsiplay pointer(start address from menu counter).
			//for menu counter grater than maximum or less than minium value initialise it.(for rolling display)
            //------17_05_18_1_e------ends------------------//
			if(local_counter>max_limit_local)
			{
				ptr_local=disp_start_ptr;
				local_counter=1;
			}
			else if(local_counter==MIN_LIMIT_MENU)
			{
				ptr_local=disp_start_ptr;
			}	

			Display_String_20(ptr_local,pos_local+2,9,index_local+1);//display first LHS section beta version
			Display_Setreset_Settings(state_code,local_counter,index_local+1);
			local_counter++;									//increment counter 
		}//end for(index_local=1;index_local<=3;index_local++).
		//------------------------------------------------------------------------//
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
unsigned char Display_Navigation_1(unsigned char counter_local,unsigned char key_code_local,unsigned char max_limit_local)
{

	unsigned char dd_ram_add_curr_local=0,dd_ram_add_prev_local=0,dd_ram_add_fowr_local=0;
	//--------------------------------------------------------------------------//
	//Display next screen only when arrow scrolls beyond screen.
	switch(key_code_local)
	{
		case DOWN_KEY:arrow_position++;
					  if(arrow_position>=4)
					  {
						arrow_position=3;
						top_position++;												//increment menu position..locally.
						if(top_position>max_limit_local)
						{
								top_position=1;											//when menu limit is over initialise to one.
						}
						Display_Screen_Main_menu_1(top_position,max_limit_local);		//display screen.
					 }				
			 		 break;
		case UP_KEY:arrow_position--;
					if(arrow_position<1)
					{
						arrow_position=1;
						top_position--;												//decrement menu position..locally.
						if(top_position<MIN_LIMIT_MENU)
						{
							top_position=max_limit_local;							//when menu limit is 1 initialise to max value.
						}
						Display_Screen_Main_menu_1(top_position,max_limit_local);						//display screen.
					}
					break;	
		case ENTER_KEY: arrow_position=1;
						top_position=1;
						Display_Screen_Main_menu_1(top_position,max_limit_local);	
			 		break;
		case ESC_KEY:	arrow_position=1;
						top_position=1;
						Display_Screen_Main_menu_1(top_position,max_limit_local);
					break;	
	}		
	//----------------------------------------------------------------------------------//
	//display arrow to current position and erase previous values.

		switch(arrow_position)
		{
			
			case 1:	dd_ram_add_curr_local=0x40;
					dd_ram_add_prev_local=0x14;
					dd_ram_add_fowr_local=0x54;	
					break;
			case 2: dd_ram_add_curr_local=0x14;
					dd_ram_add_prev_local=0x40;
					dd_ram_add_fowr_local=0x54;
					break;
			case 3:	dd_ram_add_curr_local=0x54;
					dd_ram_add_prev_local=0x40;
					dd_ram_add_fowr_local=0x14;
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
		return(arrow_position);
	//-------------------------------------------------------------------------------//

}

