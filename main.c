/*
 * File:   main.c
 * Author: Asfaq
 *
 * Created on 6 November, 2026, 12:09 PM
 */


#include <xc.h>
#include <string.h>
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "external_eeprom.h"
#include "i2c.h"
#include "cbb.h"    

#pragma config WDTE = OFF


CarEvent myCar = {
    .speed = 0,
    .event = "ON",
    .gear_pos = 0
};

static void init_config(void) 
{
    init_clcd();
    init_digital_keypad();
    init_i2c(I2CFREQUENCY);
    init_adc();
    init_ds1307();
    
}

void main(void) 
{
    
    unsigned char key = ALL_RELEASED;
    
    CarScreens screen_flag = Dashboard;
   
    
    init_config();
    
    while (1) 
    {
        myCar.speed = (unsigned int)(read_adc(CHANNEL0) / 10.23);
        
        if(myCar.speed > 99)
        {
            myCar.speed = 99;
        }
        
        key = read_digital_keypad(STATE);
        
        if(key == SW1)
        {
            //increment the gear
              if (myCar.gear_pos < 5) 
                  myCar.gear_pos++;
            
            update_car_event();
            log_car_event();
        }
        else if(key == SW2)
        {
            //decrement the gear
              if (myCar.gear_pos > 0) 
                  myCar.gear_pos--;
            
             update_car_event();
             log_car_event();
            
        }
        else if(key == SW3)
        {
            //Collision
            strcpy(myCar.event, "C ");
            
            //Neutral after collision
            myCar.gear_pos = 0;
            
            log_car_event();
        }
        else if((key == SW4 || key == SW5) && screen_flag == Dashboard )
        {
             screen_flag = LoginScreen;
             clear_display();

            //display enter password
            clcd_print("ENTER PASSWORD", LINE1(0));
            clcd_print("    ", LINE2(4)); // move cursor to next line
            //turn on the cursor
//            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            
        }
        
        switch(screen_flag)
        {
            case Dashboard:
                display_dashboard();
                break;   
            case LoginScreen:
                if(check_password(key) == success)
                {
                    screen_flag = MainMenu;
                    break;
                }
                break;
            case MainMenu:
                display_main_menu();
        }
    }
    return;
}
