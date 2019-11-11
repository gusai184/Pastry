all:pastry

pastry: main.o nodeServer.o nodeClient.o proximity.o
	g++  main.o nodeServer.o nodeClient.o proximity.o -o pastry -lpthread

main.o:main.cpp 
	g++ -c main.cpp

nodeServer.o:nodeServer.cpp
	g++ -c nodeServer.cpp -lpthread

nodeClient.o:nodeClient.cpp
	g++ -c nodeClient.cpp 

proximity.o:proximity.cpp
	g++ -c proximity.cpp

r:
	rm main.o nodeServer.o nodeClient.o proximity.o
