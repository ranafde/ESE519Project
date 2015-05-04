//
//  Sensors.h
//  
//
//  Created by Saira Hussain on 4/20/15.
//
//

#ifndef ____Sensors__
#define ____Sensors__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <iostream>
//#include "SimpleGPIO.h"

#define MAX_BUF2 100
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

enum PIN_DIRECTION{
	INPUT_PIN=0,
	OUTPUT_PIN=1
};

enum PIN_VALUE{
	LOW=0,
	HIGH=1
};

const int RS = 10;
const int VOLTAGE_REF = 5;
using namespace std;

//unsigned int SELECTGPIO = 60;   // p9_12 - GPIO1_28 = (1x32) + 28 = 60

int adc_read(unsigned int pin);
int adc_init();
void joystick_init();
std::string read_joystick();
float read_voltage();




/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_dir(unsigned int gpio, PIN_DIRECTION out_flag);
int gpio_set_value(unsigned int gpio, PIN_VALUE value);
int gpio_get_value(unsigned int gpio, unsigned int *value);
int gpio_set_edge(unsigned int gpio, char *edge);
int gpio_fd_open(unsigned int gpio);
int gpio_fd_close(int fd);



#endif /* defined(____Sensors__) */
