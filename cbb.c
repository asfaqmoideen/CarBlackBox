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
#include "timers.h"
#define PASSWORD_OFFSET 6

unsigned char time[7]; //HHMMSS
extern CarEvent myCar;
unsigned char log[11];
unsigned char log_pos = 0;
unsigned char pass_char_count = 0;
unsigned char user_password[5] = {0};
unsigned char attempts_remaining = 3;
extern volatile unsigned char sec, return_time;



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

    log_pos++;

    if(log_pos == 10)
    {
        log_pos = 0;
    }
    
   
}

CarScreens get_password(unsigned char key)
{
    return_time = 5;
    if (key == SW4 || key == SW5)
    {   

        user_password[pass_char_count] = (key == SW4) ? '0' : '1';
        clcd_putch('*', LINE2(pass_char_count + PASSWORD_OFFSET));
        pass_char_count++;
    }

    if(return_time == 0)
    {   
        clear_display();
        return Dashboard;
    }

    if (pass_char_count == 4) 
    {   
        
        clear_display();
        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
        user_password[4] = '\0';
        pass_char_count = 0;
        if (check_password(user_password) == success)
        {  
            clcd_print("Login Success !", LINE2(2));
            __delay_ms(4000);
            clear_display();
            return MainMenu;
        }

        if (--attempts_remaining == 0)
        {
            clear_display();
            clcd_print("You are blocked.", LINE1(0));
            sec = 20; 
            clcd_print("s remaining ", LINE2(2));
            while (sec)
            {
                    clcd_putch('0' + sec / 10, LINE2(0));
                    clcd_putch('0' + sec % 10, LINE2(1));
            }

            attempts_remaining = 3;
            get_password_from_user();
            return LoginScreen;
        }
        else
        {   
            clear_display();
            clcd_print("Wrong Password", LINE1(0));
            clcd_putch(attempts_remaining + '0', LINE2(0));
            clcd_print("attempts left.", LINE2(3));
            __delay_ms(2000);
            get_password_from_user();
            return LoginScreen;
        }

        
    }
    return LoginScreen;
}

AuthStatus check_password(unsigned char *user_password)
{
    if(strcmp(set_password, user_password) == 0)
    {
        return success;
    }
    else
    {
        return failure;
    }
}


void display_main_menu()
{
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