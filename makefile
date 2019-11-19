all:pastry

pastry: main.o nodeServer.o nodeClient.o proximity.o leafSet.o neighbourSet.o routingTable.o node.o helperFunctions.o hashTable.o
	g++  main.o nodeServer.o nodeClient.o proximity.o leafSet.o neighbourSet.o node.o routingTable.o helperFunctions.o hashTable.o -o pastry -lpthread

main.o:main.cpp 
	g++ -c main.cpp

nodeServer.o:nodeServer.cpp
	g++ -c nodeServer.cpp -lpthread

nodeClient.o:nodeClient.cpp
	g++ -c nodeClient.cpp 

proximity.o:proximity.cpp
	g++ -c proximity.cpp

leafSet.o:leafSet.cpp
	g++ -c leafSet.cpp

neighbourSet.o:neighbourSet.cpp
	g++ -c neighbourSet.cpp

routingTable.o:routingTable.cpp
	g++ -c routingTable.cpp

node.o:node.cpp
	g++ -c node.cpp

helperFunctions.o:helperFunctions.cpp
	g++ -c helperFunctions.cpp

hashTable.o:hashTable.cpp
	g++ -c hashTable.cpp

r:
	rm main.o nodeServer.o nodeClient.o proximity.o leafSet.o neighbourSet.o routingTable.o node.o helperFunctions.o hashTable.o
