all: build copy

build: CreateDirectories
	cd ./build; gcc -c ../src/*.c; gcc -o ../YASP/yasp *.o $$(sdl2-config --cflags --libs) -lSDL2_mixer -Wall

CreateDirectories:
	mkdir -p YASP build

copy: CreateDirectories
	cp -r assets/* YASP/

install: all

clean:
	rm main