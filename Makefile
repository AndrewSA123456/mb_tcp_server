.DEFAULT_GOAL: all

all: mb_server

mb_server: main.o mb_tcp.o
	gcc main.o mb_tcp.o -o mb_server `pkg-config --cflags --libs gtk+-3.0 libmodbus`

main.o: main.c
	gcc -c main.c -o main.o `pkg-config --cflags --libs gtk+-3.0 libmodbus`

mb_tcp.o: mb_tcp.c
	gcc -c mb_tcp.c -o mb_tcp.o `pkg-config --cflags --libs gtk+-3.0 libmodbus`

clean:
	rm -rf *.o mb_server

