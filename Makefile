CreateDirectories:
	mkdir -p YASP

all: CreateDirectories
	gcc -o YASP/yasp src/main.c $$(sdl2-config --cflags --libs) -lSDL2_mixer -Wall

copy:
	cp -r assets/* YASP/

install: all copy

clean:
	rm main