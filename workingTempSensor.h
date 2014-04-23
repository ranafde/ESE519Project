#ifndef _workingTempSensor
#define _workingTempSensor

#include <errno.h>
#include <string>
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
#include "/usr/include/python2.7/Python.h"

std::string runI2C();
void program_init();
void signal_handler(int signum);
#endif
