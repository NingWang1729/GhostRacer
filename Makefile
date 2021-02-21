CC = c++
INCLUDES = -I/usr/X11/include/GL
LIBS = -L/usr/X11/lib -lglut -lGL -lGLU
STD = -std=c++17
CCFLAGS = -Wno-deprecated-declarations -Wall

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

.PHONY: default all clean

PRODUCT = GhostRacer

all: $(PRODUCT)
	./GhostRacer

%.o: %.cpp $(HEADERS)
	$(CC) -c $(STD) $(CCFLAGS) $(INCLUDES) $< -o $@

$(PRODUCT): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	rm -f *.o *~ *# $(PRODUCT)

run:
	./GhostRacer

add:
	echo -e "a\n*\nq\n"|git add -i

push:
	git push origin master
