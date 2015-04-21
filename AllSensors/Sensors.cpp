//
//  Sensors.cpp
//  
//
//  Created by Saira Hussain on 4/20/15.
//
//

#include "Sensors.h"

/*
 read the ADC pin
 */

int adc_read(unsigned int pin){
    int fd, len, j;
    char buf[MAX_BUF] = {0};
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
void adc_init(){
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


