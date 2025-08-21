

all:main

main:main.cpp
	g++ main.cpp -o main -Wall -std=gnu++17

clean:
	rm -f main
