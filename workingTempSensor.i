%module workingTempSensor
%{
/* Includes the header in the wrapper code */
#include "workingTempSensor.h"
%}
%include "std_string.i"
/* Parse the header file to generate wrappers */
%include "workingTempSensor.h"
