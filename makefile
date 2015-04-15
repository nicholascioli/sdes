CXX=g++ -std=c++11
RM=rm -rf

all: main

debug: clean
	$(CXX) -Iinclude -o debug -g ./src/main.cpp

main: clean
	$(CXX) -Iinclude -o sdes ./src/main.cpp

clean:
	$(RM) sdes debug