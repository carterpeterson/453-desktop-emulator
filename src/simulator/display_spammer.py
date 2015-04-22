#Simulator for 453 Gametable. Interfaces with the FreeRTOS control board, simulating the display and touch inputs
import serial, sys, thread, time, struct
from Tkinter import *

WINDOW_HEIGHT 	= 160
WINDOW_WIDTH 	= 640
PIXEL_HEIGHT 	= WINDOW_HEIGHT / 8
PIXEL_WIDTH 	= WINDOW_WIDTH / 32

FRAME_BUFFER_SIZE = 768

# Make the COM_PORT 1 less than the port you wanna use
COM_PORT 		= 9
BAUDRATE 		= 460800
TEST_MODE 		= False
HUMAN_READABLE  = True

EVENT_CLICK 	= 4
EVENT_RELEASE 	= 5
EVENT_DRAG 		= 6

ser = serial.Serial()

def open_serial():
	global ser
	
	ser.baudrate = BAUDRATE
	ser.port = COM_PORT
	try:
		ser.open()
	except ValueError:
		print "Failed to connect to device over COM Port: " + str(COM_PORT)
		sys.exit()

def send_buffer(buffer):
	global ser
	
	for i in range(FRAME_BUFFER_SIZE):
		ser.write(struct.pack("B", buffer[i] & 0xFF))
		
def main():
	global ser
	buffer = [0] * 768
	
	if not TEST_MODE:
		open_serial()
		if not ser.isOpen():
			print "Failed to connect to device over COM Port: " + str(COM_PORT)
			sys.exit()
			
	current_red = 0
	
	while 1:
		send_buffer(buffer)
		time.sleep(1)
		
	

if __name__ == "__main__":
	main()