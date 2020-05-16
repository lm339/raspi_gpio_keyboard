#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <string.h>  
#include <linux/uinput.h>  
#include <linux/input.h>  
  
#include <glib.h>
#include <wiringPi.h>

#include "gpio_key_config.h"

int keyboard_uinput_key_send(unsigned int keycode, unsigned int value) ;

unsigned char key_gpio_list[] =  KEY_GPIO_LIS;
char key_value_list[] = KEY_VALUE_LIS ;

unsigned int key_down =0;
void key_gpio_interrupt(void)
{
	unsigned int temp = 0;
	for(unsigned char i=0;i<KEY_COUNT;i++)
	{
		temp = 0x00000001 << i;
		if ((temp & key_down) == 0)
		{
			if((digitalRead(key_gpio_list[i]) == 0))
			{
				key_down |= temp;
				keyboard_uinput_key_send(key_value_list[i], 1);
			}
		}

	}
}

unsigned char key_io_init(void)
{
	wiringPiSetup();

	for(unsigned char i=0;i<KEY_COUNT;i++)
	{
		pinMode(key_gpio_list[i],INPUT);       //配置1脚为输入
		pullUpDnControl(key_gpio_list[i],PUD_UP);  //将1脚上拉到3.3v	
		if(wiringPiISR (key_gpio_list[i], INT_EDGE_FALLING,  key_gpio_interrupt) != 0)
		{
			printf("gpio init erro");
			return 1;
		}	
	}
	return 0;
}

unsigned char key_logic(void)
{
	if(key_down == 0)
	{
		return 1;
	}
	int i = 0;
	unsigned int temp =0;
	for(i=0;i<KEY_COUNT;i++)
	{
		temp = 0x00000001 << i;
		if(key_down & temp)
		{

			if(digitalRead(key_gpio_list[i]) == 1) 
			{
				key_down &= ~temp;
				printf("key_logic %x\r\n",i);
				keyboard_uinput_key_send(key_value_list[i],0);	
			}

		}	
	}
	return 1;
}

gboolean key_timer_h(gpointer data)
{
    key_logic();
    return 1;
} 

unsigned char  gpio_key_init(void)
{
	printf("key init,%x \r\n",key_io_init());
	g_timeout_add(5,key_timer_h,NULL);
	return 0;
}