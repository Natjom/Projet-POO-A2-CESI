# Exemple de Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SOURCES = main.cpp FontManager.cpp
OBJECTS = $(SOURCES:.cpp=.o)

prog: $(OBJECTS)
	$(CXX) $(OBJECTS) -o prog $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
