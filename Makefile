CXX = g++
CFLAGS = -c -Wall -Wextra -Wpedantic -O2 -std=c++11 -Iinclude
LIBS = -lSDL2
SOURCES = main.cpp
SOURCES += $(wildcard src/*.cpp)
OBJECTS = $(subst .cpp,.o,$(subst src/,,$(SOURCES)))
EXEC = space_invaders


all: makeObj
	$(CXX) $(OBJECTS) $(LIBS) -o $(EXEC)

makeObj:
	$(CXX) $(CFLAGS) $(SOURCES)

clean:
	rm -rf $(OBJECTS) $(EXEC)

