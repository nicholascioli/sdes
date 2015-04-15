CXX=g++ -std=c++11
RM=rm -rf

all: main

debug: clean
	$(CXX) -o debug -g ./src/main.cpp

main: clean
	$(CXX) -o sdes ./src/main.cpp

clean:
	$(RM) sdes debug