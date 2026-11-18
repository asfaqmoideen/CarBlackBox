/*
 * File:   cbb.c
 * Author: asfaq
 *
 * Created on 6 November, 2026, 12:40 PM
 */


#include <xc.h>
#include "cbb.h"
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "external_eeprom.h"
#include "i2c.h"
#include "string.h"

unsigned char time[7]; //HHMMSS
extern CarEvent myCar;
unsigned char log[11];
unsigned char pos = 0;
unsigned char count = 0;
unsigned char user_password[5] = {0};

static void get_time()
{
    unsigned char clock_reg[3];
    
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
    
    /* To store the time in HH:MM:SS format */
    char time[7];  // "HH:MM:SS"
    // HH -> 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
    clcd_putch(time[0],LINE2(0));
    clcd_putch(time[1],LINE2(1));
    clcd_putch(':',LINE2(2));
    
    //time[2] = ':';
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    
    clcd_putch(time[2],LINE2(3));
    clcd_putch(time[3],LINE2(4));
    clcd_putch(':',LINE2(5));
    
    //time[5] = ':';
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';
    
    clcd_putch(time[4],LINE2(6));
    clcd_putch(time[5],LINE2(7));
    
}

void display_dashboard()
{
    clcd_print("  TIME   EV  SP ", LINE1(0));
    
    get_time();
 
    clcd_print(myCar.event, LINE2(9));
    
    clcd_putch((myCar.speed/10) + '0', LINE2(13));
    
    clcd_putch((myCar.speed % 10) + '0', LINE2(14));
}

void update_car_event()
{
    strcpy(myCar.event, gears[myCar.gear_pos]);
}

void log_car_event()
{
    unsigned char address;
    
    get_time();
    strncpy(log, time, 6);
    strncpy(&log[6], myCar.event, 2);
    log[8] = (myCar.speed / 10) + '0';
    log[9] = (myCar.speed % 10) + '0';
    log[10] = '\0';
    
    eeprom_write_string(address, log);

    pos++;

    if(pos == 10)
    {
        pos = 0;
    }
    
   
}

AuthStatus check_password(unsigned char key)
{   
    
    key = ALL_RELEASED;
    
    if(count < 4)
    {   
        clcd_putch('*', LINE2(count));
        if(key == SW4)
        {
            user_password[count++] = '0';
        }
        if(key == SW5)
        {
            user_password[count++] = '1';
        }
    }
    
    if(count == 4)
    {
        if(strcmp(set_password, user_password) == 0)
        {
            return success;
        }
    }
    else
    {
        return failure;
    }
    
}

void display_main_menu()
{
    clear_display();
    clcd_print("Main Menu", LINE1(0));
}


void get_password_from_user()
{
    //clear screen 
    clear_display();

    //display enter password
    clcd_print("ENTER PASSWORD", LINE1(0));
    clcd_print("    ", LINE2(0)); // move cursor to next line
    //turn on the cursor
    clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
    
    
    //start timer

    //blink the cursor 
}