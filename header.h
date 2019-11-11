#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_map>
#include "md5.cpp"

#define N 65536 // Total Number of node in Network
#define b 4 
#define f(i,a,b) for(int i=(int)a; i<(int)b; i++)

using namespace std;

int L = pow(2,b); //leafSet Size
int M = pow(2,b); //NeighbourSet Size
int ROW = ceil(log(N) / log(pow(2,b)));
int COL = pow(2,b);
int DIGITS = ROW;

class NodeAddress
{
public:
	string nodeId;
	string ip;
	int port;
};

class Node
{
public:
	string nodeId;
	NodeAddress selfAdd;
	vector<NodeAddress> leafSet;
	vector<NodeAddress> neighbourSet;
	vector<vector<NodeAddress>> routeTable;
	unordered_map<string, string> hashTable;

	int index(char ch)
	{
		if(ch >= 'a')
			return ch - 'a' + 10;
		else
			return ch - '0';
	}

	Node(string ip, int port)
	{
		NodeAddress temp;
		temp.nodeId = "empt";

		nodeId = md5(ip + to_string(port)).substr(0, ROW);

		//Initialize leafSet, neighbourSet and routeTable
		selfAdd.nodeId = nodeId;
		selfAdd.ip = ip;
		selfAdd.port = port;

		leafSet.resize(L, temp);
		neighbourSet.resize(M, temp);
		routeTable.resize(ROW);

		f(i,0,ROW)
		{
			routeTable[i].resize(COL, temp);
		}

		f(i,0,DIGITS)
		{
			int y = index(nodeId[i]);
			int x = i;
			routeTable[x][y] = selfAdd;
		}

	}

	void printleafSet()
	{
		cout<<"-------------------Leaf Table---------------------"<<endl;
		f(i,0,L)
			cout<<leafSet[i].nodeId<<" ";
		cout<<endl;
	}

	void printneighbourSet()
	{

		cout<<"-------------------Neighbour Table---------------------"<<endl;
		f(i,0,L)
			cout<<neighbourSet[i].nodeId<<" ";
		cout<<endl;
	}

	void printrouteTable()
	{
		cout<<"-------------------Routing Table---------------------"<<endl;
		f(i,0,ROW)
		{
			f(j,0,COL)
				cout<<routeTable[i][j].nodeId << " ";
			cout<<endl;
		}
		cout<<endl;
	}

	void print()
	{
		printleafSet();
		cout<<endl;
		printneighbourSet();
		cout<<endl;
		printrouteTable();
		cout<<endl;
	}


};
