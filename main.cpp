#include "header.h"

string nodeId;
NodeAddress selfAdd;
vector<NodeAddress> leafSet;
vector<NodeAddress> neighbourSet;
vector<vector<NodeAddress>> routeTable;
unordered_map<string, string> hashTable;


int L = pow(2,B); //leafSet Size
int M = pow(2,B); //NeighbourSet Size
int ROW = ceil(log(N) / log(pow(2,B)));
int COL = pow(2,B);
int DIGITS = ROW;

int main()
{
	createNode("127.0.0.1", 2000);
	cout<<"Node created with nodeId = "<<nodeId<<endl;
	print();
	startServer();
	//node.startClient();
	return 0;
}