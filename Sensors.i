
%module Sensors
%{
/* Includes the header in the wrapper code */
#include "Sensors.h"
%}
%include "std_string.i"
/* Parse the header file to generate wrappers */
%include "Sensors.h"
