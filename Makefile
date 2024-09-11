all:
	gcc -xc++ -lstdc++ -shared-libgcc $(file)

g++:
	g++ $(file)

run:
	./a.out