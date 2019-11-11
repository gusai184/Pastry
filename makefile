all:pastry

pastry: main.o nodeServer.o
	g++  main.o nodeServer.o -o pastry -lpthread

main.o:main.cpp 
	g++ -c main.cpp

nodeServer.o:nodeServer.cpp
	g++ -c nodeServer.cpp -lpthread
