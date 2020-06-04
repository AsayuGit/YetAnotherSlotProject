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

linuxBuild = cd ./build; gcc -c ../src/*.c; gcc -o ../YASP/yasp *.o $$(sdl2-config --cflags --libs) -lSDL2_mixer -lSDL2_image -Wall
windowsBuild = ./misc/buildIcon.bat; cd ./build; gcc -D WINDOWS -c ../src/*.c; gcc -o ../YASP/yasp *.o icon.res $$(sdl2-config --cflags --libs) -lSDL2_mixer -lSDL2_image -Wall -mconsole

linuxInstall = cp -r YASP /usr/local/games; cp misc/yasp /usr/local/bin; chmod +x /usr/local/bin/yasp; cp misc/YASP.desktop /usr/share/applications
windowsInstall = cp -r YASP "C:\Program Files"; echo "Yasp is installed in C:\Program Files"

ifeq (Windows_NT,Windows_NT)
	OSbuild = $(windowsBuild)
	OSInstall = $(windowsInstall)
else
	OSbuild = $(linuxBuild)
	OSInstall = $(linuxInstall)
endif

all: build copy

build: CreateDirectories
	$(OSbuild)

CreateDirectories:
	mkdir -p YASP build

copy: CreateDirectories
	cp -r assets/* YASP/
	cp LICENSE YASP/LICENSE

clean:
	rm -rf build
	rm -rf YASP

install:
	$(OSInstall)

uninstall:
	rm -rf /usr/local/games/YASP
	rm /usr/local/bin/yasp
	rm /usr/share/applications/YASP.desktop

dependencies:
	apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev