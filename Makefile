all: build copy

build: CreateDirectories
	cd ./build; gcc -c ../src/*.c; gcc -o ../YASP/yasp *.o $$(sdl2-config --cflags --libs) -lSDL2_mixer -Wall

CreateDirectories:
	mkdir -p YASP build

copy: CreateDirectories
	cp -r assets/* YASP/

cleanup:
	rm -rf build
	rm -rf YASP

install:
	cp -r YASP /usr/local/games
	cp misc/yasp /usr/local/bin; chmod +x /usr/local/bin/yasp

uninstall:
	rm -rf /usr/local/games/YASP
	rm /usr/local/bin/yasp

clean:
	rm main