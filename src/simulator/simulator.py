#Simulator for 453 Gametable. Interfaces with the FreeRTOS control board, simulating the display and touch inputs
import serial, sys, thread, time, struct
from Tkinter import *

WINDOW_HEIGHT 	= 320
WINDOW_WIDTH 	= 1280
PIXEL_HEIGHT 	= WINDOW_HEIGHT / 8
PIXEL_WIDTH 	= WINDOW_WIDTH / 32

FRAME_BUFFER_SIZE = 768

# Make the COM_PORT 1 less than the port you wanna use
COM_PORT 		= 6
BAUDRATE 		= 460800
TEST_MODE 		= False
HUMAN_READABLE  = True

EVENT_CLICK 	= 4
EVENT_RELEASE 	= 5
EVENT_DRAG 		= 6

touch_active = False
current_touch_pos = 0,0
ser = serial.Serial()
rectangles = [[]]
canvas = None

def output_touch_buffer():
	global touch_active, current_touch_pos, ser

	while 1:
		touch_buffer = [0] * 8
	
		if touch_active:
			pixel_pos = current_touch_pos[0] / PIXEL_WIDTH, current_touch_pos[1] / PIXEL_HEIGHT
			
			if(pixel_pos[0] >= 0 and pixel_pos[0] < 32 and pixel_pos[1] >= 0 and pixel_pos[1] < 8):
				touch_buffer[pixel_pos[1]] = (1 << (31 - pixel_pos[0]))
				
				for i in range(8):
					if HUMAN_READABLE:
						ser.write(str(touch_buffer[i] & 0xFFFFFFFF))
					else:
						ser.write(struct.pack("I", touch_buffer[i] & 0xFFFFFFFF))
					
			
		time.sleep(0.01)
			

def recieve_frame_buffer():
	global ser, canvas
	
	while 1:
		recieved_buffer = ser.read(FRAME_BUFFER_SIZE)
		recieved_buffer = struct.unpack(FRAME_BUFFER_SIZE * 'B', recieved_buffer)
		
		print "Change Display!"
		
		for i in range(0, FRAME_BUFFER_SIZE, 3):
			mycolor = '#%02x%02x%02x' % (int(recieved_buffer[i]), int(recieved_buffer[i + 1]), int(recieved_buffer[i + 2]))
			canvas.itemconfig(rectangles[(i / 3) % 32][(i / 3) / 32], fill=mycolor)
			
			
def board_touch_event(event):
	global touch_active, current_touch_pos

	if int(event.type) == EVENT_CLICK:
		touch_active = True
		current_touch_pos = event.x, event.y
	elif int(event.type) == EVENT_DRAG:
		current_touch_pos = event.x, event.y
	elif int(event.type) == EVENT_RELEASE:
		touch_active = False

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
	canvas.bind("<Button-1>", board_touch_event)
	canvas.bind("<B1-Motion>", board_touch_event)
	canvas.bind("<ButtonRelease-1>", board_touch_event)
	canvas.pack()
	
	rectangle_array = [[canvas.create_rectangle(i * PIXEL_WIDTH, j * PIXEL_HEIGHT, (i+1) * PIXEL_WIDTH, (j+1) * PIXEL_HEIGHT, fill="blue") for j in range(8)] for i in range(32)]
	
	return rectangle_array
	

def main():
	global ser, rectangles, canvas

	if not TEST_MODE:
		open_serial()
		if not ser.isOpen():
			print "Failed to connect to device over COM Port: " + str(COM_PORT)
			sys.exit()
			
		
	root = Tk()
	
	rectangles = create_canvas(root)
	
	try:
		thread.start_new_thread(output_touch_buffer, ())
		thread.start_new_thread(recieve_frame_buffer, ())
	except:
		print "Failed to start simulator threads...exiting"
		sys.exit(2)
	
	root.mainloop()
		
	

if __name__ == "__main__":
	main()