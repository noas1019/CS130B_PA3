# Makefile
prog3: prog3.cpp
	g++ -std=c++17 prog3.cpp -o prog3

debug: prog3.cpp
	g++ -ggdb3 -O0 -std=c++17 prog3.cpp -o debug

clean:
	-rm prog3
	-rm debug