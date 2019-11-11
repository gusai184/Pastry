#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_map>


#include <pthread.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

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

