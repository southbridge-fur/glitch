all: 
	g++ main.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs`

