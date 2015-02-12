CXX=g++ -std=c++11
RM=rm -rf

all: main

debug: clean
	$(CXX) -o debug -g ./src/main.cpp

main: 
	$(CXX) -o main ./src/main.cpp

clean:
	$(RM) main debug