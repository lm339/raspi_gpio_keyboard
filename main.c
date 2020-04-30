#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <string.h>  

#include <glib.h>
#include <wiringPi.h>

unsigned char  gpio_key_init(void);
int keyboard_uinput_init(void);

static GMainLoop *m_main_loop;

int main(int argc, char *argv[])  
{  

    if(keyboard_uinput_init() < 0)
    {  
        printf("keyboard_uinput_init erro\n");  
        return -1;//error process.  
    }  
    gpio_key_init();

	m_main_loop = g_main_loop_new(NULL, 0);
	g_main_loop_run(m_main_loop);  
    return 0;  
}  
  
