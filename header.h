#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_map>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <pthread.h>
#include <netinet/in.h>
#include <string.h>
#include <ifaddrs.h>
#include <sstream>
#include <algorithm>

#define BUFFER_SIZE 256
#define N 65536 // Total Number of node in Network
#define B 4
#define f(i,a,b) for(int i=(int)a; i<(int)b; i++)

using namespace std;

class NodeAddress
{
public:
	string nodeId;
	string ip;
	int port;
};


void print();
void printrouteTable();
void printneighbourSet();
void printleafSet();
void createNode(string ip, int port);
int index(char ch);


//Server functions
void * serverthread(void *args);
void startServer();
void sendLeafSet(vector<string> token);
void sendNeighbourSet(vector<string> token);
void receiveNeighbourSet(vector<string> token);
void recieveLeafSet(vector<string> token);
void addToNeighbourSet(NodeAddress newNeighbour);
void addToLeafSet(NodeAddress n);

//client functions
void joinHandler(vector<string> command);
void client();
vector<string> split(string s);
int createConnection(string ip ,int port);

//proximity
double getProximity(string ip, int port);