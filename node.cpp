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


void createNode(string ip, int port)
{
	NodeAddress temp;
	temp.nodeId = "empt";
	pair<double, NodeAddress> t;
	t.first = 9999.0;
	t.second = temp;

	//nodeId = md5(ip + to_string(port)).substr(0, ROW);

	//Initialize leafSet, neighbourSet and routeTable
	selfAdd.nodeId = nodeId;
	selfAdd.ip = ip;
	selfAdd.port = port;

	leafSet.resize(L, temp);
	neighbourSet.resize(M, t);
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

NodeAddress getClosestNode(string newnodeId)
{
	int max_prefix_match = prefixMatch(nodeId, newnodeId);
	int min_diff = diff(nodeId, newnodeId);
	
	NodeAddress clostestNode = selfAdd;

	//check in leaf Node
	for(auto leafNode : leafSet)
	{
		if( leafNode.nodeId != "empt" && prefixMatch(leafNode.nodeId, newnodeId) == max_prefix_match && diff(leafNode.nodeId, newnodeId) < min_diff )
		{
			max_prefix_match = prefixMatch(leafNode.nodeId, newnodeId);
			clostestNode = leafNode;
		}
	}

	//check in neighbourset Node
	for(auto neighbourNode : neighbourSet)
	{
		NodeAddress nbNode = neighbourNode.second;

		if( nbNode.nodeId == "empt" || isNodeActive(nbNode) == false )
		{
			continue;
		}

		if(prefixMatch(nbNode.nodeId, newnodeId) > max_prefix_match || ( diff(nbNode.nodeId, newnodeId) < min_diff && prefixMatch(nbNode.nodeId, newnodeId) == max_prefix_match ) )
		{
			max_prefix_match = prefixMatch(nbNode.nodeId, newnodeId);
			clostestNode = nbNode;
		}
	}

	//check in Routing table
	for(auto routetableNode : routeTable[max_prefix_match])
	{
		if( routetableNode.nodeId == "empt")
		{
			continue;
		}
		
		if(prefixMatch(routetableNode.nodeId, newnodeId) > max_prefix_match || ( diff(routetableNode.nodeId, newnodeId) < min_diff && prefixMatch(routetableNode.nodeId, newnodeId) == max_prefix_match ))
		{
			max_prefix_match = prefixMatch(routetableNode.nodeId, newnodeId);
			clostestNode = routetableNode;
		}
	}

	return clostestNode;	
}

void updateStateTables(vector<string> token)
{
	for(int i=1 ;i<token.size() ;i+=3)
	{
		NodeAddress temp;
		temp.nodeId = token[i];
		temp.ip = token[i+1];
		temp.port = stoi(token[i+2]);
		addToLeafSet(temp);
		addToRouteTable(temp);
		addToNeighbourSet(temp);
	}	
}

bool isNodeActive(NodeAddress node)
{
  int fd = createConnection(node.ip, node.port);
  if(fd == -1)
  {
    return false;
  }
  else
  {
    close(fd);
    return true;
  }
}

void broadCast()
{
	//cout<<"Broadcast FUnction"<<endl;
	unordered_map <string, int> mp;
	string msg = "broadcast " + selfAdd.nodeId + " "+selfAdd.ip + " " + to_string(selfAdd.port) + " ";
	for(auto node : leafSet)
	{
		if(node.nodeId == "empt")
			continue;

		if(mp[node.nodeId])
			continue;

		msg += node.nodeId + " " + node.ip + " " + to_string(node.port) + " ";
		mp[node.nodeId] = 1;
	}

	for(auto nNode : neighbourSet)
	{
		NodeAddress node = nNode.second;

		if(node.nodeId == "empt")
			continue;

		if(mp[node.nodeId])
			continue;

		msg += node.nodeId + " " + node.ip + " " + to_string(node.port) + " ";
		mp[node.nodeId] = 1;
	}

	f(i, 0, ROW)
	{
		f(j, 0, COL)
		{
			NodeAddress node = routeTable[i][j];
			if(node.nodeId == "empt")
				continue;

			if(mp[node.nodeId])
				continue;

			msg += node.nodeId + " " + node.ip + " " + to_string(node.port) + " ";
			mp[node.nodeId] = 1;
		}
	}

	unordered_map <string, int> mp1;
	for(auto node : leafSet)
	{
		if(node.nodeId == "empt")
			continue;

		if(mp1[node.nodeId])
			continue;
		//cout << "broadcast msg send to " << node.nodeId << endl;
		int fd = createConnection(node.ip, node.port);
		send(fd, msg.c_str(), msg.size(), 0);
		mp1[node.nodeId] = 1;
	}

	for(auto nNode : neighbourSet)
	{
		NodeAddress node = nNode.second;

		if(node.nodeId == "empt")
			continue;

		if(mp1[node.nodeId])
			continue;
		//cout << "broadcast msg send to " << node.nodeId << endl;
		int fd = createConnection(node.ip, node.port);
		send(fd, msg.c_str(), msg.size(), 0);
		mp1[node.nodeId] = 1;
	}

	f(i, 0, ROW)
	{
		f(j, 0, COL)
		{
			NodeAddress node = routeTable[i][j];
			if(node.nodeId == "empt" || node.nodeId == nodeId)
				continue;

			if(mp1[node.nodeId])
				continue;
			//cout << "broadcast msg send to " << node.nodeId << endl;
			int fd = createConnection(node.ip, node.port);
			send(fd, msg.c_str(), msg.size(), 0);
			mp1[node.nodeId] = 1;
		}
	}

}
