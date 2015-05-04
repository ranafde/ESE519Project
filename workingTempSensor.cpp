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

using namespace std;

#define BUFF_READ_SIZE 2
#define BUFF_WRITE_SIZE 3

#define MCP9808_REG_TEMP 0x05
#define MCP9808_REG_CONF 0x01
#define MCP9808_ADDR 0x18 //0x18 << 1

int fd;
char bufRead[BUFF_READ_SIZE] = {0};
char bufWrite[BUFF_WRITE_SIZE] = {0};

void signal_handler(int signum)
{
    assert(0 == close(fd));
    exit(signum);
}

void program_init(){
    char *bus = "/dev/i2c-1"; /* Pins P9_17 and P9_18 */
<<<<<<< HEAD
=======
    //int addr = MCP9808_ADDR; /* The I2C address of TMP102 */
    //char bufRead[BUFF_READ_SIZE] = {0};
    //char bufWrite[BUFF_WRITE_SIZE] = {0};
>>>>>>> 6234c5938c94971fd32bb80b5c06603ab3e5b355

    if ((fd = open(bus, O_RDWR)) < 0) {
        /* ERROR HANDLING: you can check errno to see what went wrong */
        perror("Failed to open the i2c bus");
        exit(1);
    }
    
    if (ioctl(fd, I2C_SLAVE, MCP9808_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
    
    /* Register the signal handler */
    signal(SIGINT, signal_handler);
    
<<<<<<< HEAD
    // Write to I2C bus
=======
>>>>>>> 6234c5938c94971fd32bb80b5c06603ab3e5b355
    bufWrite[0] = MCP9808_REG_CONF;
    bufWrite[1] = 0x00;
    bufWrite[2] = 0x00;

    if(write(fd, bufWrite, 3) !=3){
           perror("Failed to Reset Address in readFullSensorState()\n ");
    }
}

string runI2C(){
    char TempCelsiusDisplay[] = "+abc.dd C";
    int tempval;
<<<<<<< HEAD
    // Write temp register on i2c bus
=======
>>>>>>> 6234c5938c94971fd32bb80b5c06603ab3e5b355
    bufWrite[0] = MCP9808_REG_TEMP;
    if(write(fd, bufWrite, 1) !=1){
        perror("Failed to Reset Address in readFullSensorState()\n ");
    }
<<<<<<< HEAD
    // Using I2C Read
=======
        // Using I2C Read
>>>>>>> 6234c5938c94971fd32bb80b5c06603ab3e5b355
    if (read(fd,bufRead,BUFF_READ_SIZE) != BUFF_READ_SIZE) {
         /* ERROR HANDLING: i2c transaction failed */
         perror("Failed to read from the i2c bus.\n");
    }
<<<<<<< HEAD
    // Performs the required conversion of data to celsius after reading from i2c bus
=======
>>>>>>> 6234c5938c94971fd32bb80b5c06603ab3e5b355
    else {

         if(bufRead[0] & 0xE0) {
             printf("\r\n");
             bufRead[0] = bufRead[0] & 0x1F;  // clear flag bits
         }
         if((bufRead[0] & 0x10) == 0x10) {
            bufRead[0] = bufRead[0] & 0x0F;
            TempCelsiusDisplay[0] = '-';
            tempval = 256 - (bufRead[0] << 4) + (bufRead[1] >> 4);
        } else {
<<<<<<< HEAD
            TempCelsiusDisplay[0] = '+';
            tempval = (bufRead[0] << 4) + (bufRead[1] >> 4);
        }
        if (bufRead[1] & 0x08) {
            if(bufRead[1] & 0x04) {
                TempCelsiusDisplay[5] = '7';
                TempCelsiusDisplay[6] = '5';
            } else {
                TempCelsiusDisplay[5] = '5';
                TempCelsiusDisplay[6] = '0';
            }
        } 
        else {
            if(bufRead[1] & 0x04) {
                TempCelsiusDisplay[5] = '2';
                TempCelsiusDisplay[6] = '5';
            }else{
                TempCelsiusDisplay[5] = '0';
                TempCelsiusDisplay[6] = '0';
            }
        }

        TempCelsiusDisplay[1] = (tempval / 100) + 0x30;
        TempCelsiusDisplay[2] = ((tempval % 100) / 10) + 0x30;
        TempCelsiusDisplay[3] = ((tempval % 100) % 10) + 0x30;
        string tempDisplay = string(TempCelsiusDisplay);
 
	    return tempDisplay;
    
    }

=======
        TempCelsiusDisplay[0] = '+';
        tempval = (bufRead[0] << 4) + (bufRead[1] >> 4);
        }
                if (bufRead[1] & 0x08) {
        if(bufRead[1] & 0x04) {
            TempCelsiusDisplay[5] = '7';
            TempCelsiusDisplay[6] = '5';
        } else {
            TempCelsiusDisplay[5] = '5';
            TempCelsiusDisplay[6] = '0';
        }
    } else {
        if(bufRead[1] & 0x04) {
            TempCelsiusDisplay[5] = '2';
            TempCelsiusDisplay[6] = '5';
        }else{
            TempCelsiusDisplay[5] = '0';
            TempCelsiusDisplay[6] = '0';
        }
    }

            TempCelsiusDisplay[1] = (tempval / 100) + 0x30;
            TempCelsiusDisplay[2] = ((tempval % 100) / 10) + 0x30;
            TempCelsiusDisplay[3] = ((tempval % 100) % 10) + 0x30;
            string tempDisplay = string(TempCelsiusDisplay);
         // Display result
         //string command = "python displayOnLCD.py ";
         //command.append(tempDisplay);

         //cout << command << "\r\n";
           //system(command.c_str());
          // cout << tempDisplay << "\r\n";
	   return tempDisplay;
         //   printf("temp = %s\r\n", tempDisplay);
//            printf("Temp Fahrenheit: %f Celsius: %f\n", f, c);
        }
        sleep(1);
>>>>>>> 6234c5938c94971fd32bb80b5c06603ab3e5b355
    
}
