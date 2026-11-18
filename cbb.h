/* 
 * File:   cbb.h
 * Author: asfaq
 *
 * Created on 6 November, 2026, 12:40 PM
 */

#ifndef CBB_H
#define	CBB_H
#define I2CFREQUENCY         100000


#define DASHBOARD_SCREEN     0x01
#define PASSWORD_SCREEN      0x02
#define MAIN_MENU_SCREEN     0x03
#define VIEW_LOG_SCREEN      0x04
#define SET_TIME_SCREEN      0x05
#define CHANGE_PASSWORD      0x06
#define LOGIN_SCREEN         0x07



typedef enum screen
{
    Dashboard,
    Password,
    MainMenu,
    ViewLog,
    SetTime, 
    ChangPassword,
    LoginScreen
} CarScreens;

typedef enum authentication
{
    failure,
    success        
}AuthStatus;

static const char gears[6][3] = { "GN","G1","G2","G3","G4","GR" };

static const char set_password[5] = "1010";

typedef struct {
    unsigned int speed;       
    unsigned char gear_pos;   
    char event[3];           
} CarEvent;



static void get_time(void); 

void display_dashboard();
void update_car_event();
void get_password_from_user();
void log_car_event();
CarScreens get_password(unsigned char key);
AuthStatus check_password(unsigned char *pass);
void display_main_menu();

#endif	/* CBB_H */


        