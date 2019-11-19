#include "header.h"
extern string nodeId;
extern NodeAddress selfAdd;
extern vector<NodeAddress> leafSet;
extern vector<pair<double, NodeAddress>> neighbourSet;
extern vector<vector<NodeAddress>> routeTable;
extern unordered_map<string, string> hashTable;

extern int L;
extern int M;
extern int ROW;
extern int COL;
extern int DIGITS;


void addToRouteTable(NodeAddress n)
{
	if(n.nodeId == nodeId)
		return;

	int pm = prefixMatch(n.nodeId, nodeId);
	int in = index(n.nodeId[pm]);
	if(routeTable[pm][in].nodeId == "empt")
	{
		routeTable[pm][in] = n;		
	}
	else
	{
		double pnew = getProximity(n.ip, n.port);
		double pcurr = getProximity(routeTable[pm][in].ip, routeTable[pm][in].port);

		if(pnew < pcurr)
			routeTable[pm][in] = n;
	}
}

void sendRoutingTable(vector<string> token)
{
	int l = prefixMatch(nodeId ,token[4]);

	int start = stoi(token[1]);
	int end = l+1;

	string rows = "routingTable ";

	f(i,start,end)
	{
		f(j,0,COL)
		{
			if(routeTable[i][j].nodeId != "empt")
				rows += routeTable[i][j].nodeId + " " + routeTable[i][j].ip + " " + to_string(routeTable[i][j].port) + " "; 
		}
	}

	int client_fd = createConnection(token[2], stoi(token[3]));
	send(client_fd ,rows.c_str() ,rows.size() ,0);
}

void receiveRoutingTable(vector<string> token)
{
	for(int i=1 ;i<token.size() ;i+=3)
	{
		NodeAddress temp;
		temp.nodeId = token[i];
		temp.ip = token[i+1];
		temp.port = stoi(token[i+2]);

		addToRouteTable(temp);
	}
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


