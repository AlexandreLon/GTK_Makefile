all:
	gcc `pkg-config --cflags gtk+-3.0` -o bin/main src/main.c `pkg-config --libs gtk+-3.0`