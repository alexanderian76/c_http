all: gcc run 
file=async_tcp.cpp
gcc:
	gcc -xc++ -lstdc++ -shared-libgcc $(file)
g++:
	g++ $(file)

run:
	./a.out