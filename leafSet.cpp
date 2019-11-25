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

void addToLeafSet(NodeAddress n)
{

	if( n.nodeId == selfAdd.nodeId )
		return ;

	int fd = createConnection(n.ip, n.port);
	if(fd == -1)
	{
		//node is not active
		return;
	}
	close(fd);

	for(int i = 0; i < L; i++)
		if( leafSet[i].nodeId == n.nodeId )
			return;

	int idnew = hax_dec(n.nodeId);
	int idself = hax_dec(selfAdd.nodeId);

	if( idnew < idself) 
	{
		int index = 0;

		while( index<(L/2) && leafSet[index].nodeId != "empt")
		{
			index++;
		}

		if( index < L/2 )
		{
			leafSet[index].nodeId = n.nodeId;
			leafSet[index].ip = n.ip;
			leafSet[index].port = n.port;

			sort(leafSet.begin() ,leafSet.begin()+index + 1 ,compare);
		}
		else
		{
			int idfirst = hax_dec(leafSet[0].nodeId);

			if( idnew > idfirst)
			{
				leafSet[0].nodeId = n.nodeId;
				leafSet[0].ip = n.ip;
				leafSet[0].port = n.port;

				sort(leafSet.begin() ,leafSet.begin() + L/2 ,compare);
			}
		}
	}
	else
	{
		int index = L/2;

		while( index<(L) && leafSet[index].nodeId != "empt")
		{
			index++;
		}

		if( index < L )
		{
			leafSet[index].nodeId = n.nodeId;
			leafSet[index].ip = n.ip;
			leafSet[index].port = n.port;

			sort( leafSet.begin()+L/2 ,leafSet.begin() + index+1 ,compare);
		}
		else
		{
			int idlast = hax_dec(leafSet[L-1].nodeId);

			if( idnew < idlast)
			{
				leafSet[L-1].nodeId = n.nodeId;
				leafSet[L-1].ip = n.ip;
				leafSet[L-1].port = n.port;

				sort(leafSet.begin()+L/2 ,leafSet.end(),compare);
			}
		}
	}

}

void sendLeafSet(vector<string> token)
{
	string msg;

	if(token[0] == "join")
		msg = "leafBroadcast ";
	else
		msg = "leafSet ";

	msg+= selfAdd.nodeId + " " + selfAdd.ip + " " + to_string(selfAdd.port) + " ";

	f(i,0,L)
	{
		if(leafSet[i].nodeId != "empt")
			msg += leafSet[i].nodeId + " " + leafSet[i].ip + " " + to_string(leafSet[i].port) + " "; 
	}

	int client_fd = createConnection(token[2], stoi(token[3]));
	send(client_fd ,msg.c_str() ,msg.size() ,0);
	cout<<endl<<"LeafSet sent .."<<msg<<endl; 	
}


void recieveLeafSet(vector<string> token)
{
	cout<<endl<<"LeafSet received ... "<<endl;
	for(int i=1 ;i<token.size() ;i+=3)
	{
		NodeAddress temp;
		temp.nodeId = token[i];
		temp.ip = token[i+1];
		temp.port = stoi(token[i+2]);
		addToLeafSet(temp);
	}
}

NodeAddress getClosestLeafNode(string newnodeId)
{
	int max_prefix_match = prefixMatch(nodeId, newnodeId);
	NodeAddress clostestNode = selfAdd;

	for(auto leafNode : leafSet)
	{
		if(prefixMatch(leafNode.nodeId, newnodeId) > max_prefix_match)
		{
			max_prefix_match = prefixMatch(leafNode.nodeId, newnodeId);
			clostestNode = leafNode;
		}
	}

	return clostestNode;
}

void repairLeafSet(NodeAddress failednode)
{
	f(i,0,L)
	{
		if(!isNodeActive(leafSet[i]))
			leafSet[i].nodeId = "empt";
	}

	NodeAddress node = selfAdd;
	int fd;
	if(failednode.nodeId < nodeId)
	{
		f(i,0,L/2)
		{
			if(leafSet[i].nodeId != "empt")
			{
				node = leafSet[i];
				break;
			}

		}
	}
	else
	{
		rf(i,L-1,L/2)
		{
			if(leafSet[i].nodeId != "empt")
			{
				node = leafSet[i];
				break;
			}
		}
	}

	if(node.nodeId == selfAdd.nodeId)
	{
		return;
	}
	
	string msg = "getleafset dummytoken " + selfAdd.ip + " " + to_string(selfAdd.port);
	send(fd, msg.c_str(), msg.length(), 0);

}

NodeAddress getClosestLeafNodeForReplica(string key)
{
	int max_prefix_match = -1 ;
	int min_diff = INT_MAX;

	NodeAddress clostestNode;

	f(i,0,L)
	{
		NodeAddress node = leafSet[i];

		if( node.nodeId != "empt" )
		{
			int prematch =  prefixMatch(node.nodeId, key);

			if( prematch > max_prefix_match || (prematch == max_prefix_match && diff(node.nodeId, key) < min_diff) )
			{
				max_prefix_match = prematch;
				clostestNode = node;
				min_diff = diff(node.nodeId ,key);
			}
		}
	}

	return clostestNode;
}

void printleafSet()
{
	cout<<"-------------------Leaf Table---------------------"<<endl;
	f(i,0,L)
		cout<<leafSet[i].nodeId<<" ";
	cout<<endl;
}
