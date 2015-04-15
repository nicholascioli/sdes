CXX=g++
FLAGS=-std=c++11 -Iinclude
RM=rm -rf

all: main

debug: clean
	$(CXX) $(FLAGS) -o debug -g ./src/main.cpp

main: clean
	$(CXX) $(FLAGS) -o sdes ./src/main.cpp

clean:
	$(RM) sdes debug
