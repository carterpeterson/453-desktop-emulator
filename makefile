all:
	gcc -I /opt/X11/include -L /opt/X11/lib/ -lX11 -lm -lpthread ./src/emulator/*.c ./src/tasks/*.c  -o table

clean:
	rm table