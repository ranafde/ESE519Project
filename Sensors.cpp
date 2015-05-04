//
//  Sensors.cpp
//  
//
//  Created by Saira Hussain on 4/20/15.
//
//

#include "Sensors.h"
#include <poll.h>

/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);

	return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, PIN_DIRECTION out_flag)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}

	if (out_flag == OUTPUT_PIN)
		write(fd, "out", 4);
	else
		write(fd, "in", 3);

	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, PIN_VALUE value)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
		return fd;
	}

	if (value==LOW)
		write(fd, "0", 2);
	else
		write(fd, "1", 2);

	close(fd);
	return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
	int fd;
	char buf[MAX_BUF];
	char ch;

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("gpio/get-value");
		return fd;
	}

	read(fd, &ch, 1);

	if (ch != '0') {
		*value = 1;
	} else {
		*value = 0;
	}

	close(fd);
	return 0;
}


/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int gpio_set_edge(unsigned int gpio, char *edge)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		return fd;
	}

	write(fd, edge, strlen(edge) + 1);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(unsigned int gpio)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd)
{
	return close(fd);
}


/*
 read the ADC pin
 */
unsigned int SELECTGPIO = 30;   // p9_12 - GPIO1_28 = (1x32) + 28 = 60
int adc_read(unsigned int pin){
    int fd, len, j;
    char buf[MAX_BUF2] = {0};
    char val[4] = {0};
    
    
    
    len = snprintf(buf, sizeof(buf), "/sys/devices/ocp.3/helper.10/AIN%d", pin);
    
    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        perror("adc/get-value");
        
    }
    
    read(fd, &val, 4);
    close(fd);
    //printf ("value of pin ADC%d =%.4s \n", pin, val);
    return atoi(val);
}

/*
 initialize the ADC
 */
int adc_init(){
    FILE * f = popen( "sudo echo cape-bone-iio > /sys/devices/bone_capemgr.8/slots", "r" );
    if ( f == 0 ) {
        fprintf( stderr, "Could not execute\n" );
        return 1;
    }
    const int BUFSIZE = 1000;
    char buf[ BUFSIZE ];
    while( fgets( buf, BUFSIZE,  f ) ) {
        fprintf( stdout, "%s", buf  );
    }
    pclose( f );
}

/*
 initialize the GPIO for teh select button of the joystick
 */
void joystick_init(){
    gpio_export(SELECTGPIO);    // The SELECT button
    gpio_set_dir(SELECTGPIO, INPUT_PIN);   // The select is an input
}

/**
 return "UP" -> when joystick is moved up
        "DOWN" -> when joystick is moved down
        "RIGHT" -> when joystick is moved right
        "LEFT" -> when joystick is moved left
        "SELECT" -> when select is pressed
        "" -> empty string returned if nothing is pressed.
 
    Vertical -> AN0
    Horizontal -> AN1
 */
string read_joystick(){
    float verticalIn, horizontalIn;
    float current;
    verticalIn = adc_read(0);
    horizontalIn = adc_read(1);
    
    if(verticalIn >= 760.0 && verticalIn <= 1690.0){
        return "UP";
    }else if(verticalIn >= 1.0 && verticalIn <= 680.0){
        return "DOWN";
    }
    
    
    if(horizontalIn >= 760.0 && horizontalIn <= 1690.0){
        return "LEFT";
    }else if(horizontalIn >= 0.0 && horizontalIn <= 710.0){
        return "RIGHT";
    }
    
    unsigned int value = HIGH;
    gpio_get_value(SELECTGPIO, &value);
    if(value == LOW){
        return "SELECT";
    }
    
    return "";

}

/*
 Read voltage value at AN2*/
float read_voltage(){
    float sensorValue, voltageValue;
    
    sensorValue = adc_read(2);
    voltageValue = (sensorValue - 14.33)/33.3;
    return voltageValue;
}
