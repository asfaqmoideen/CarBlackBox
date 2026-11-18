#include <xc.h>



volatile unsigned char sec = 0, return_time = 0;
volatile unsigned int count = 0; 


void __interrupt() isr(void)
{
    static unsigned int blink_count = 0;


    if (TMR0IF == 1)
    {
        TMR0 = TMR0 + 6 + 2;

        if (++count == 20000)
        {
            count = 0;

            if(sec > 0)
            {
                sec--;
            }
            

            if(return_time > 0)
            {
                return_time--;
            }
        }

        TMR0IF = 0;

    }
}