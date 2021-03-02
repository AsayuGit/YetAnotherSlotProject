#	<YetAnotherSlotProject a (yet another) Slotmachine simulator> \
    Copyright (C) <2020>  <RAIMBAUD Killian & TOUGARD Enzo> \
	\
    This program is free software; you can redistribute it and/or modify \
    it under the terms of the GNU General Public License as published by \
    the Free Software Foundation version 2 of the License. \
	\
    This program is distributed in the hope that it will be useful, \
    but WITHOUT ANY WARRANTY; without even the implied warranty of \
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the \
    GNU General Public License for more details. \
	\
    You should have received a copy of the GNU General Public License along \
    with this program; if not, write to the Free Software Foundation, Inc., \
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

all: build copy

CreateDirectories:
	mkdir -p YASP build

build: CreateDirectories
ifeq ($(OS),Windows_NT)
	./misc/buildIcon.bat
	cd ./build; gcc -D WINDOWS -c ../src/*.c; gcc -o ../YASP/yasp *.o icon.res $$(sdl2-config --cflags --libs) -lSDL2_mixer -lSDL2_image -Wall -mconsole
else
	cd ./build; gcc -c ../src/*.c; gcc -o ../YASP/yasp *.o $$(sdl2-config --cflags --libs) -lSDL2_mixer -lSDL2_image -Wall
endif

copy: CreateDirectories
	cp -r assets/* YASP/
	cp LICENSE YASP/LICENSE

clean:
	rm -rf build
	rm -rf YASP

install:
ifeq ($(OS),Windows_NT)
	cp -r YASP "C:\Program Files"; echo "Yasp is installed in C:\Program Files"
else
	mkdir -p $(DESTDIR)/usr/local/games $(DESTDIR)/usr/local/bin $(DESTDIR)/usr/share/applications
	cp -r YASP $(DESTDIR)/usr/local/games
	cp misc/yasp $(DESTDIR)/usr/local/bin
	chmod +x $(DESTDIR)/usr/local/bin/yasp
	cp misc/YASP.desktop $(DESTDIR)/usr/share/applications
endif
	
uninstall:
	rm -rf /usr/local/games/YASP
	rm /usr/local/bin/yasp
	rm /usr/share/applications/YASP.desktop

aptDep:
	apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev

pacDep:
	pacman -S sdl2 sdl2_image sdl2_mixer
