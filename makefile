all:pastry

pastry: main.o nodeServer.o nodeClient.o
	g++  main.o nodeServer.o nodeClient.o -o pastry -lpthread

main.o:main.cpp 
	g++ -c main.cpp

nodeServer.o:nodeServer.cpp
	g++ -c nodeServer.cpp -lpthread

nodeClient.o:nodeClient.cpp
	g++ -c nodeClient.cpp 
