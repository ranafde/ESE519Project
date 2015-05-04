
#Importing modules
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import smtplib
from email.mime.text import MIMEText
from email.mime.image import MIMEImage
from email.mime.multipart import MIMEMultipart
import sys
import Adafruit_ILI9341 as TFT
import Adafruit_GPIO as GPIO
import Adafruit_GPIO.SPI as SPI
import time
from evdev import InputDevice, categorize, ecodes
import workingTempSensor
import Sensors
import lxml.html as lh
import urllib2
import os
from Subfact_ina219 import INA219


# BeagleBone Black configuration for TFT LCD. TFT LCD works on SPI
DC = 'P9_15'
RST = 'P9_12'
SPI_PORT = 1
SPI_DEVICE = 0

# Create TFT LCD display class.
disp = TFT.ILI9341(DC, rst=RST, spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE, max_speed_hz=64000000))

# Initialize display.
disp.begin()



# Get a PIL Draw object to start drawing on the display buffer.
draw = disp.draw()
# Load default font.
font = ImageFont.load_default()

def draw_rotated_text(image, text, position, angle, font, fill=(255,255,255)):
        # Get rendered font width and height.
        draw = ImageDraw.Draw(image)
        width, height = draw.textsize(text, font=font)
        # Create a new image with transparent background to store the text.
        textimage = Image.new('RGBA', (width, height), (0,0,0,0))
        # Render the text.
        textdraw = ImageDraw.Draw(textimage)
        textdraw.text((0,0), text, font=font, fill=fill)
        # Rotate the text image.
        rotated = textimage.rotate(angle, expand=1)
        # Paste the text into the image, using it as a mask for transparency.
        image.paste(rotated, position, rotated)


#Method for reading barcode
def read_barcode():
    keys = {
    2: 1,
    3: 2,
    4: 3,
    5: 4,
    6: 5,
    7: 6,
    8: 7,
    9: 8,
    10: 9,
    11: 0,
    }

    barcode = ""
    # evdev module gives an instance of InputDevice
    dev = InputDevice('/dev/input/event1')
    # For loop to iterate over the barcode digits
    for event in dev.read_loop():
	 joystickPosn = Sensors.read_joystick()
	 print(joystickPosn)
	 if(joystickPosn == "SELECT"):
	    joypressed = False
	    break
        # ecodes.EV_KEY corresponds to a valid barcode digit
         if event.type == ecodes.EV_KEY:
            # used to signify end of barcode
             if event.code == 28:
                 break
            # used to signify valid digits
             if event.value == 00:
                # used for decoding digits
                 barcode += str(keys[event.code])
    # Clears LCD display
    disp.clear()
    # Puts "Processing..." in the display buffer
    draw_rotated_text(disp.buffer, "Processing...", (100, 160), 90, font, fill=(255,255,0))
    # Displays on LCD
    disp.display()
    # Creates required URL for barcode fetching
    url = 'https://afternoon-river-6706.herokuapp.com/barcodes/' + barcode
    # Opens URL
    doc=lh.parse(urllib2.urlopen(url))
    # Retrieves description of barcode using xpath
    desc = doc.xpath('//li/text()')
    desc = '\n'.join(desc)
    # Returns description of barcode
    return desc

def getTasks():
    # URL for worker 1
    url='https://afternoon-river-6706.herokuapp.com/workers/1/st'
    # Opens URL
    doc=lh.parse(urllib2.urlopen(url))
    # Retrieves tasks using xpath
    tasks = doc.xpath('//li/text()')
    tasks = '\n'.join(tasks)
    # Returns tasks
    return tasks    

def current_drawn():
    # Create an instance of INA219 library
    ina = INA219()
    data = ""
    # Get shunt voltage
    shuntVoltage =  str(ina.getShuntVoltage_mV())
    # Get bus voltage
    busVoltage = str(ina.getBusVoltage_V())
    # Get current
    curr = str(ina.getCurrent_mA())
    data = "Shunt: " + shuntVoltage + " mV \n" + "Bus: " + busVoltage + " V \n" + "Current: " + curr + " mA \n"
    # Return shunt voltage, bus voltage and current
    return data

def SendMail(ImgFileName):
    # Opens file name
    img_data = open(ImgFileName, 'rb').read()
    # Setting msg parameters
    msg = MIMEMultipart()
    msg['Subject'] = 'Image'
    msg['From'] = 'ese519project@gmail.com'
    msg['To'] = 'manager@gmail.com'

    text = MIMEText("test")
    msg.attach(text)
    image = MIMEImage(img_data, name=os.path.basename(ImgFileName))
    msg.attach(image)
    # Using smtplib to make an SMTP request
    s = smtplib.SMTP('smtp.gmail.com', 587)
    s.ehlo()
    s.starttls()
    s.ehlo()
    s.login('ese519project@gmail.com', '519project')
    # Sending email from ese519project@gmail.com to manager@gmail.com
    s.sendmail('ese519project@gmail.com', 'manager@gmail.com', msg.as_string())
    s.quit()
    #print("Success!")
    

# Total number of screens on LCD
state = ["First Screen", "Temperature", "Voltage", "Barcode", "Current", "Tasks", "Camera"]
index = 0

# Initializing adc unit in c++ from python
adcinit = Sensors.adc_init()
# Initializing joystick in c++ from python 
Sensors.joystick_init()
joyPressed = False
# Initializing i2c temperature sensor in c++ from python
workingTempSensor.program_init()
while True:
    # Clear display
    disp.clear()
    # Reading joystick position in while loop
    joystickPosn = Sensors.read_joystick()
    print(joystickPosn)
    # Screen for displaying the text "Slide the joystick left/right"
    if(index == 0):
        draw_rotated_text(disp.buffer, "Slide the joystick left/right", (100, 100), 90, font, fill=(255,255,0))
        if joystickPosn == "LEFT":
            index = len(state)-1
        if(joystickPosn == "RIGHT"):
            index += 1
    # Screen for displaying the text "TEMPERATURE"
    elif(index == 1):
        draw_rotated_text(disp.buffer, "TEMPERATURE", (100, 160), 90, font, fill=(255,255,0))
        if(joystickPosn == "LEFT"):
            index -= 1
        elif(joystickPosn == "RIGHT"):
            index += 1
        elif(joystickPosn == "SELECT"):
            joyPressed = True
    # Screen for displaying the text "VOLTAGE"
    elif(index == 2):
        draw_rotated_text(disp.buffer, "VOLTAGE", (100, 160), 90, font, fill=(255,255,0))
        if(joystickPosn == "LEFT"):
            index -= 1
        elif(joystickPosn == "RIGHT"):
            index += 1
        elif(joystickPosn == "SELECT"):
            joyPressed = True
    # Screen for displaying the text "Press select then Scan Barcode"
    elif(index == 3):
        draw_rotated_text(disp.buffer, "Press select then Scan Barcode", (100, 160), 90, font, fill=(255,255,0))
        if(joystickPosn == "LEFT"):
            index -= 1
        elif(joystickPosn == "RIGHT"):
            index += 1
        elif(joystickPosn == "SELECT"):
            joyPressed = True
    # Screen for displaying the text "CURRENT"
    elif(index == 4):
        draw_rotated_text(disp.buffer, "CURRENT", (100, 160), 90, font, fill=(255,255,0))
        if(joystickPosn == "LEFT"):
            index -= 1
        elif(joystickPosn == "RIGHT"):
            index += 1
        elif(joystickPosn == "SELECT"):
            joyPressed = True
    # Screen for displaying the text "TASKS"
    elif(index == 5):
        draw_rotated_text(disp.buffer, "TASKS", (100, 160), 90, font, fill=(255,255,0))
        if(joystickPosn == "LEFT"):
            index -= 1
        elif(joystickPosn == "RIGHT"):
            index += 1
        elif(joystickPosn == "SELECT"):
            joyPressed = True
    # Screen for displaying the text "CAMERA"
    elif(index == 6):
	draw_rotated_text(disp.buffer, "CAMERA", (100, 160), 90, font, fill=(255,255,0))
        if(joystickPosn == "LEFT"):
            index -= 1
        elif(joystickPosn == "RIGHT"):
            index = 0
        elif(joystickPosn == "SELECT"):
            joyPressed = True

    # When joystick is pressed
    while(joyPressed):
        # Gets current temperature and displays on the screen
        if index == 1:
	       disp.clear()
           currentTemp = workingTempSensor.runI2C()
           draw_rotated_text(disp.buffer, currentTemp, (100, 160), 90, font, fill=(255,255,0))
	       disp.display()
	       time.sleep(0.1)

        # Gets voltage and displays on the screen
    	if index == 2:
    	    disp.clear()
    	    voltage = str(Sensors.read_voltage())
    	    draw_rotated_text(disp.buffer, voltage, (100, 160), 90, font, fill=(255,255,0))
    	    disp.display()
    	    time.sleep(0.1)
    	
        # Reads barcode, fetches the description from the web app and displays it on the LCD
    	if index == 3:
    	    barcode = read_barcode()
            disp.clear()
    	    draw_rotated_text(disp.buffer, barcode, (100, 160), 90, font, fill=(255,255,0))
    	    disp.display()
    	    joyPressed = False
    	    time.sleep(5)

        # Gets current and displays on the screen
    	if index == 4:
    	    disp.clear()
    	    data = current_drawn()
    	    draw_rotated_text(disp.buffer, data, (100, 50), 90, font, fill=(255,255,0))
    	    disp.display()
    	    time.sleep(0.1)
    	
        # Fetches tasks from web app and displays on the screen
    	if index == 5:
            disp.clear()
            tasks = getTasks()
            draw_rotated_text(disp.buffer, tasks, (100, 160), 90, font, fill=(255,255,0))
            disp.display()
            time.sleep(0.1)

        # Takes image through camera and sends it on email
    	if index == 6:
            disp.clear()
    	    os.system("streamer -f jpeg -o image.jpeg")
    	    time.sleep(4)
    	    image = Image.open('image.jpeg')
    	    image = image.rotate(90).resize((240, 320))
    	    disp.display(image)
    	    time.sleep(2)
            SendMail('image.jpeg')
    	    disp.clear()
            draw_rotated_text(disp.buffer, "Image sent as email", (100, 160), 90, font, fill=(255,255,0))
            disp.display()
    	    joyPressed = False
            time.sleep(0.1)
    	
        # Checks the position of joystick
    	joystickPosn = Sensors.read_joystick()
        if joystickPosn == "UP":
            joyPressed = False
	   
    disp.display()
    time.sleep(0.1)
