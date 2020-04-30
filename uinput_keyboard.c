#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <string.h>

#include <linux/uinput.h>  
#include <linux/input.h>  

#define KEY_CUSTOM_UP 0x20  
#define KEY_CUSTOM_DOWN 0x30  

static struct uinput_user_dev uinput_dev;  
static int uinput_fd;  

int keyboard_uinput_init(void)  
{  
    int i;  
    int ret = 0;  
  
    uinput_fd = open("/dev/uinput", O_RDWR | O_NDELAY);  
    if(uinput_fd < 0)
    {  
        printf("open /dev/uintput erro\n");  
        return -1;//error process.  
    }  
      
    //to set uinput dev  
    memset(&uinput_dev, 0, sizeof(struct uinput_user_dev));  
    snprintf(uinput_dev.name, UINPUT_MAX_NAME_SIZE, "raspi-gpio-key");  
    uinput_dev.id.version = 1;  
    uinput_dev.id.bustype = BUS_VIRTUAL;  
      
    ioctl(uinput_fd, UI_SET_EVBIT, EV_SYN);  
    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);  
    ioctl(uinput_fd, UI_SET_EVBIT, EV_MSC);  
      
    for(i = 0; i < 256; i++)
    {  
        ioctl(uinput_fd, UI_SET_KEYBIT, i);  
    }  
    ioctl(uinput_fd, UI_SET_MSCBIT, KEY_CUSTOM_UP);  
    ioctl(uinput_fd, UI_SET_MSCBIT, KEY_CUSTOM_DOWN);
      
    ret = write(uinput_fd, &uinput_dev, sizeof(struct uinput_user_dev));  
    if(ret < 0)
    {  
        printf("dev write erro \r\n");  
        return ret;//error process.  
    }  
      
    ret = ioctl(uinput_fd, UI_DEV_CREATE);  
    if(ret < 0)
    {  
        printf("dev write erro \r\n");  
        close(uinput_fd);  
        return ret;//error process.  
    }  
    return 0;
}  
  
int keyboard_uinput_key_send(unsigned int keycode, unsigned int value)  
{  
    struct input_event key_event;  
    int ret;  
      
    memset(&key_event, 0, sizeof(struct input_event));  
      
    gettimeofday(&key_event.time, NULL);  
    key_event.type = EV_KEY;  
    key_event.code = keycode;  
    key_event.value = value;  
    ret = write(uinput_fd, &key_event, sizeof(struct input_event));  
    if(ret < 0)
    {  
        printf("dev write erro \r\n");
        return ret;//error process.  
    }  
      
    gettimeofday(&key_event.time, NULL);  
    key_event.type = EV_SYN;  
    key_event.code = SYN_REPORT;  
    key_event.value = 0;//event status sync  
    ret = write(uinput_fd, &key_event, sizeof(struct input_event));  
    if(ret < 0)
    {  
        printf("dev write erro \r\n"); 
        return ret;//error process.  
    }  
      
    return 0;  
}  