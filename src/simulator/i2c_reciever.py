#Simulator for 453 Gametable. Interfaces with the FreeRTOS control board, simulating the display and touch inputs
import serial, sys, thread, time, struct
from Tkinter import *

WINDOW_HEIGHT 	= 320
WINDOW_WIDTH 	= 1280
PIXEL_HEIGHT 	= WINDOW_HEIGHT / 8
PIXEL_WIDTH 	= WINDOW_WIDTH / 32

FRAME_BUFFER_SIZE = 768
TOUCH_BUFFER_SIZE = 16

# Make the COM_PORT 1 less than the port you wanna use
COM_PORT 		= 3
BAUDRATE 		= 460800
TEST_MODE 		= False
HUMAN_READABLE  = False

EVENT_CLICK 	= 4
EVENT_RELEASE 	= 5
EVENT_DRAG 		= 6

touch_active = False
update_display = False
current_touch_pos = 0,0
ser = serial.Serial()
rectangles = [[]]
canvas = None
root = None
frame_buffer_read = None
frame_buffer_write = None
count = 0
			

def recieve_touch_buffer():
	global ser, canvas, frame_buffer_read, frame_buffer_write, update_display, count
	
	while 1:
		touch_buffer = ser.read(TOUCH_BUFFER_SIZE)
		touch_buffer = struct.unpack(TOUCH_BUFFER_SIZE * 'B', touch_buffer)
		
		print str(count)
			
		count = count + 1
		
		frame_buffer_read = [0] * 768
		
		for i in range(TOUCH_BUFFER_SIZE):
			for bit in range(8):
				if (touch_buffer[i] & (0x01 << (7 - bit))) == (0x01 << (7 -bit)):
					frame_buffer_read[(((3 - (i%4)) + ((i / 4) * 4)) * 8 + bit) * 3] = 255
		
		update_display = True
		

def open_serial():
	global ser
	
	ser.baudrate = BAUDRATE
	ser.port = COM_PORT
	try:
		ser.open()
	except ValueError:
		print "Failed to connect to device over COM Port: " + str(COM_PORT)
		sys.exit()

# Creates a canvas given inside a Tk object and fills the
# canvas with rectangle array.
def create_canvas(master):
	global canvas

	canvas = Canvas(master, width=WINDOW_WIDTH, height=WINDOW_HEIGHT)
	canvas.pack()
	
	rectangle_array = [[canvas.create_rectangle(i * PIXEL_WIDTH, j * PIXEL_HEIGHT, (i+1) * PIXEL_WIDTH, (j+1) * PIXEL_HEIGHT, fill="blue") for j in range(8)] for i in range(32)]
	
	return rectangle_array
	
def update():
	global frame_buffer_read, root, update_display
	
	if update_display == True:
		for i in range(0, FRAME_BUFFER_SIZE, 3):
			mycolor = '#%02x%02x%02x' % (int(frame_buffer_read[i]), int(frame_buffer_read[i + 1]), int(frame_buffer_read[i + 2]))
			canvas.itemconfig(rectangles[(i / 3) % 32][(i / 3) / 32], fill=mycolor)
			
		update_display = False
			
	root.after(1, update)
	

def main():
	global ser, rectangles, canvas, root

	if not TEST_MODE:
		open_serial()
		if not ser.isOpen():
			print "Failed to connect to device over COM Port: " + str(COM_PORT)
			sys.exit()
			
		
	root = Tk()
	
	rectangles = create_canvas(root)
	
	try:
		thread.start_new_thread(recieve_touch_buffer, ())
	except:
		print "Failed to start simulator threads...exiting"
		sys.exit(2)
	
	root.after(1, update)
	root.mainloop()
		
	

if __name__ == "__main__":
	main()