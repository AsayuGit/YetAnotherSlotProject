all:
	gcc -o main main.c $$(sdl2-config --cflags --libs) -Wall

run:
	gcc -o main main.c $$(sdl2-config --cflags --libs) -Wall && ./main

runSDL:
	gcc -o main main.c $$(sdl2-config --cflags --libs) -Wall && ./main -SDL

clean:
	rm main