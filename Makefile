CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC = $(wildcard src/*.cpp)

main: $(SRC)
	$(CXX) $(CXXFLAGS) -Iinclude -o main $(SRC)

clean:
	rm -f main

