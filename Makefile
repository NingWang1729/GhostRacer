CC = c++
INCLUDES = -I/usr/X11/include/GL
LIBS = -L/usr/X11/lib -lglut -lGL -lGLU
STD = -std=c++17
CCFLAGS = -Wno-deprecated-declarations -Wall

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

.PHONY: default all clean

PRODUCT = GhostRacer


# Compilation commands

all: $(PRODUCT)
	./GhostRacer

%.o: %.cpp $(HEADERS)
	$(CC) -c $(STD) $(CCFLAGS) $(INCLUDES) $< -o $@

$(PRODUCT): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBS) -o $@

submit:
	zip p3.zip Actor.cpp Actor.h StudentWorld.cpp StudentWorld.h report.txt

view:
	zipinfo *.zip

clean:
	rm -f *.o *~ *# $(PRODUCT)

run:
	./GhostRacer


# Git commands via Makefile

git: add commit push

add: clean
	git add -A

commit:
	git commit -m "$(m)"

push:
	git push -u origin master

pull:
	git pull
