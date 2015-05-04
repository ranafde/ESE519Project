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
#include "SimpleGPIO.h"

#define MAX_BUF 100
const int RS = 10;
const int VOLTAGE_REF = 5;
using namespace std;

unsigned int SELECTGPIO = 60;   // p9_12 - GPIO1_28 = (1x32) + 28 = 60

int adc_read(unsigned int pin);
void adc_init();
void joystick_init();
string read_joystick();
float read_voltage();

#endif /* defined(____Sensors__) */
