#include<p24fj512gu408.h>

#include "../HVCT_121_MASTER_H_FILES/hvct_121_master_common_includes.h"

const static unsigned char hvct121[]=           	    {"HVCT_121"}; //8
const static unsigned char version[]=           	    {"VER M:A.0    S:XXX"}; //7


void Display_Power_On_A(unsigned int cards_local)
{
    digit_ram[0] = 'A';
    digit_ram[1] = '.';
    digit_ram[2] = '0';
    //send command to recive slave version
    //store the slave version
    Display_String_20(hvct121,1,8,1);            // Displays "HVCT_121"

    Display_String_20(version,1,17,2);           //display "VER M:A.0    S:XXX"
    Display_Ram(digit_ram,16,0,3,2);
    Display_String_20(sn,1,6,3);                 //display  "SR.NO."
    Read_Tester_Sr_No_Display();				 //display serial number.
     if(cards_local!=0)
     {
         Display_String_20(showcards,12,6,4);
         Display_Number_4digit(cards_local,18,2,4);
     }
     else
     {
         Display_String_20(nocards,9,12,4);
     }
    DelayS(3);
    Clear_Display();
    Display_String_20(testpt,1,11,1);
	Display_Number_4digit(cards_local*32,13,4,1);    //Display Number of points (1 card = 32 points)
//    Display_String_20(prgno,1,16,2);
//	Display_Number_4digit(128,17,3,2); 
}