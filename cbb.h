/* 
 * File:   cbb.h
 * Author: asfaq
 *
 * Created on 6 November, 2026, 12:40 PM
 */

#ifndef CBB_H
#define	CBB_H
#define I2CFREQUENCY         100000


typedef enum screen
{   
    ViewLogScreen,
    DashboardScreen,
    MainMenuScreen,
    SetTimeScreen, 
    ChangPasswordScreen,
    PasswordScreen,
    LoginScreen
} CarScreens;


static const char gears[6][3] = { "GN","G1","G2","G3","G4","GR" };

static const char set_password[5] = "1010";

typedef struct {
    unsigned int speed;       
    unsigned char gear_pos;   
    char event[3];           
} CarEvent;

static const char *main_menu[5] = {"View log", "Clear log", "Download log","Set Time", "ChangePassword"};
unsigned char menu_pos = 0, menu_pointer = 0;

static void get_time(void); 

void display_dashboard();
void update_car_event();
void get_password_from_user();
void log_car_event();
CarScreens get_password(unsigned char key);
void display_main_menu();
void change_password();
void change_rtc_time();
void display_logs();

#endif	/* CBB_H */


        