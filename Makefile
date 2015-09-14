INCLUDES = gui.h glitch.h -I./
FILES = gui.o glitch.o
CC = g++ 


GTKLINK = `pkg-config --libs gtk+-3.0`
GTK = `pkg-config --cflags gtk+-3.0`
MAGICK = `Magick++-config --cppflags --cxxflags --ldflags --libs`

all: 	clean glitch.o
	$(CC) $(INCLUDES) glitch.o main.cpp -o glitch $(MAGICK)

clean:
	rm -f *.o; rm -f glitch;

glitch.o: 	glitch.h glitch.cpp
		$(CC) glitch.h glitch.cpp $(MAGICK) -c

gui.o: 		gui.h gui.cpp glitch.o
		$(CC) $(GTK) gui.h gui.cpp $(GTKLINK) -c
	        $(CC) $(GTK) $(INCLUDES) main.cpp $(FILES) -o glitch $(MAGICK) $(GTKLINK)	
