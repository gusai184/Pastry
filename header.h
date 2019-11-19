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
#include <fstream>

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

//leafset.cpp
void sendLeafSet(vector<string> token);
void recieveLeafSet(vector<string> token);
void addToLeafSet(NodeAddress n);
void printleafSet();
NodeAddress getClosestLeafNode(string newnodeId);

//neighbourset.cpp
void sendNeighbourSet(vector<string> token);
void receiveNeighbourSet(vector<string> token);
void addToNeighbourSet(NodeAddress newNeighbour);
void printneighbourSet();

//routingTable.cpp
void sendRoutingTable(vector<string> token);
void receiveRoutingTable(vector<string> token);
void addToRouteTable(NodeAddress n);
void printrouteTable();

//helperfunctions.cpp
int prefixMatch(string id1, string id2);
int diff(string id1, string id2);
int index(char ch);
int hax_dec(string s);
bool compare(NodeAddress i1, NodeAddress i2);
bool compare1(pair<double, NodeAddress> p1, pair<double, NodeAddress> p2);
vector<string> split(string s);
int createConnection(string ip ,int port);
void print();

//proximity
double getProximity(string ip, int port);

//client functions
void join(vector<string> command);
void client();
void setKey(vector<string> command);
void getKey(vector<string> command);

//Server functions
void * serverthread(void *args);
void startServer();
void joinHandler(vector<string> token);
void setKeyHandler(vector<string> token);
void getKeyHandler(vector<string> token);
void printhashTable();
void getAck(vector<string> token);

//node.cpp
void createNode(string ip, int port);
NodeAddress getClosestNode(string newnodeId);
void updateStateTables(vector<string> token);
void broadCast();

//md5
std::string md5(const std::string str);