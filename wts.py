import workingTempSensor
workingTempSensor.program_init()
while True:
    a = workingTempSensor.runI2C()
    print("The Temperature is + " + str(a))
