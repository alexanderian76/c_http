all: gcc run 
file=async_tcp.cpp
gcc:
	gcc -xc++ -lstdc++ -shared-libgcc $(file)
g++:
	g++ $(file)

mac:
	gcc -xc++ -lstdc++ -shared-libgcc -std=c++11 $(file)

mac++:
	g++ -std=c++11 $(file)
run:
	./a.out