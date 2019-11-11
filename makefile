all:pastry

pastry: main.o
	g++ main.o -o pastry

main.o:main.cpp
	g++ -c main.cpp
