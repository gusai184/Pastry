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

	int fd = createConnection(n.ip, n.port);
	if(fd == -1)
	{
		//node is not active
		return;
	}
	close(fd);

	int pm = prefixMatch(n.nodeId, nodeId);
	int in = index(n.nodeId[pm]);
	if(routeTable[pm][in].nodeId == "----")
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
			if(routeTable[i][j].nodeId != "----")
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

void repairRouteTable(int row, int col)
{
	routeTable[row][col].nodeId = "----";

	f(i, row, row+2)
	{
		f(j, 0, COL)
		{
			if(j == col)
				continue;

			if(routeTable[i][j].nodeId == "----" || routeTable[i][j].nodeId == nodeId)
				continue;

			if(!isNodeActive(routeTable[i][j]))
			{
				routeTable[i][j].nodeId = "----";
				continue;
			}
			
			string msg = "getRTentry " + to_string(row) + " " + to_string(col) + " " + selfAdd.ip + " " + to_string(selfAdd.port);
			int fd = createConnection(routeTable[i][j].ip, routeTable[i][j].port);
			send(fd, msg.c_str(), msg.size(), 0);
			close(fd);
		}
	}
}

void sendRTentry(vector<string> token)
{
	int i = stoi(token[1]);
	int j = stoi(token[2]);

	if( routeTable[i][j].nodeId == "----")
		return;
	string msg = "routingTable " + routeTable[i][j].nodeId + " " + routeTable[i][j].ip + " " + to_string(routeTable[i][j].port);
	int fd = createConnection(token[3], stoi(token[4]));
	send(fd, msg.c_str(), msg.size(), 0);
	close(fd);
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
	cout<<endl<<endl;
}


