#Simulator for 453 Gametable. Interfaces with the FreeRTOS control board, simulating the display and touch inputs
import serial, sys, thread, time, struct
from Tkinter import *

WINDOW_HEIGHT 	= 320
WINDOW_WIDTH 	= 1280
PIXEL_HEIGHT 	= WINDOW_HEIGHT / 8
PIXEL_WIDTH 	= WINDOW_WIDTH / 32

FRAME_BUFFER_SIZE = 768

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
					
		else:
			for i in range(8):
					if HUMAN_READABLE:
						ser.write(str(0 & 0xFFFFFFFF))
					else:
						ser.write(struct.pack("I", 0 & 0xFFFFFFFF))
						
		time.sleep(0.01)
			

def recieve_frame_buffer():
	global ser, canvas, touch_buffer_read, touch_buffer_write, update_display
	
	while 1:
		touch_buffer_write = ser.read(FRAME_BUFFER_SIZE)
		touch_buffer_write = struct.unpack(FRAME_BUFFER_SIZE * 'B', touch_buffer_write)
		
		touch_buffer_read = touch_buffer_write
		update_display = True
		
		
			
			
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
	
def update():
	global frame_buffer_read, root, update_display
	
	if update_display == True:
		for i in range(0, FRAME_BUFFER_SIZE, 3):
			mycolor = '#%02x%02x%02x' % (int(touch_buffer_read[i]), int(touch_buffer_read[i + 1]), int(touch_buffer_read[i + 2]))
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
		thread.start_new_thread(output_touch_buffer, ())
		thread.start_new_thread(recieve_frame_buffer, ())
	except:
		print "Failed to start simulator threads...exiting"
		sys.exit(2)
	
	root.after(1, update)
	root.mainloop()
		
	

if __name__ == "__main__":
	main()