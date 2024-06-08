CXX = g++
#CXXFLAGS = -std=c++11 -O2 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic
FILES = main.cpp apostolico_giancarlo.cpp functions.cpp parsing.cpp
NAME = solution

all: example

example:
	$(CXX) $(CXXFLAGS) -o $(NAME) $(FILES)

clean:
	rm -f *.o $(NAME)