CXX = g++
CFLAGS = -c -Wall -Wextra -Wpedantic -O2 -std=c++11 -Iinclude
SOURCES = main.cpp
SOURCES += $(wildcard src/*.cpp)
OBJECTS = $(subst .cpp,.o,$(subst src/,,$(SOURCES)))
EXEC = space_invaders

all: makeObj
	$(CXX) $(OBJECTS) -o $(EXEC)

makeObj:
	$(CXX) $(CFLAGS) $(SOURCES)

clean:
	rm -rf $(OBJECTS) $(EXEC)

