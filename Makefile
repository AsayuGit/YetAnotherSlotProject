all:
	gcc -o main main.c $$(sdl2-config --cflags --libs) -lSDL2_mixer -Wall

run:
	gcc -o main main.c $$(sdl2-config --cflags --libs) -lSDL2_mixer -Wall && ./main

runSDL:
	gcc -o main main.c $$(sdl2-config --cflags --libs) -lSDL2_mixer -Wall && ./main -SDL

clean:
	rm main