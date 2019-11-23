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


void addToHashTable(vector<string> token)
{
	for(int i=1;i<token.size();i+=2)	
	{
		string key = token[i];
		string value = token[i+1];
		hashTable[key] = value;
	}
}

void redistributeHashTable(vector<string> token)
{
	string newNodeId = token[1];
	string newNodeIP = token[2];
	int newNodeport = stoi(token[3]);

	string msg = "addkeyvalue ";
	vector<string> veckey;
	for(auto pair : hashTable)
	{
		string key = pair.first;
		string value = pair.second;
		//string keyhash = md5(key).substr(0,DIGITS);
		string keyhash = key;

		int max_prefix_match = prefixMatch(nodeId, keyhash);
		int min_diff = diff(nodeId, keyhash);
		
		if((prefixMatch(keyhash, newNodeId) > max_prefix_match) ||
			  (diff(keyhash, newNodeId) < min_diff && prefixMatch(keyhash, newNodeId) == max_prefix_match))
		{
			msg += key + " " + value + " ";
			veckey.push_back(key);
			
		}

	}
	
	for(int i=0;i<veckey.size();i++)
	{
		hashTable.erase(veckey[i]);
	}

	int fd = createConnection(newNodeIP, newNodeport);
	send(fd ,msg.c_str() ,msg.size() ,0);
			
}

void setKeyHandler(vector<string> token)
{
	
	string keyhash = token[3];
	NodeAddress temp =  getClosestLeafNode(keyhash);

	if( temp.nodeId != selfAdd.nodeId)
	{
		//forward to temp
		string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4] + " "+token[5];
		int fd = createConnection(temp.ip ,temp.port);
		send(fd ,msg.c_str() ,msg.size() ,0);
		cout<<"forward Msg Sent 1 from leafset"<<endl;
		return ;
	}

	int l = prefixMatch(nodeId ,keyhash);
	int j = index(keyhash[l]);

	if( routeTable[l][j].nodeId != "empt")
	{
		string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4] + " "+token[5];
		int fd = createConnection(routeTable[l][j].ip ,routeTable[l][j].port);
		send(fd ,msg.c_str() ,msg.size() ,0);
		cout<<"forward Msg Sent 2 from routing table"<<endl;
	}
	else
	{
		temp = getClosestNode(keyhash);

		cout<<"Temp = "<<temp.nodeId<<endl;
		if( temp.nodeId != selfAdd.nodeId )
		{
			string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4] + " "+token[5];
			int fd = createConnection(temp.ip, temp.port);
			send(fd ,msg.c_str() ,msg.size() ,0);
			cout<<"forward Msg Sent 3 from rear conditon"<<endl;
		}
		else
		{
			cout<<"I am closest Node add key here ...."<<endl;
			string key = token[1];
			string value = token[2];
			hashTable[key] = value;

			int fd = createConnection(token[4], stoi(token[5]));
			string msg = "msg_ack Successfully added to node " + nodeId;
			send(fd ,msg.c_str() ,msg.size() ,0);
			printhashTable();
		}
	}
}

void getKeyHandler(vector<string> token)
{
	string keyhash = token[2];
	NodeAddress temp =  getClosestLeafNode(keyhash);

	if( temp.nodeId != selfAdd.nodeId)
	{
		//forward to temp
		string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4];
		int fd = createConnection(temp.ip ,temp.port);
		send(fd ,msg.c_str() ,msg.size() ,0);
		cout<<"forward Msg Sent 1 from leafset"<<endl;
		return ;
	}

	int l = prefixMatch(nodeId ,keyhash);
	int j = index(keyhash[l]);

	if( routeTable[l][j].nodeId != "empt")
	{

		string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4];
		int fd = createConnection(routeTable[l][j].ip ,routeTable[l][j].port);
		send(fd ,msg.c_str() ,msg.size() ,0);
		cout<<"forward Msg Sent 2 from routing table"<<endl;
	}
	else
	{
		temp = getClosestNode(keyhash);

	
		if( temp.nodeId != selfAdd.nodeId )
		{
			string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4];
			int fd = createConnection(temp.ip, temp.port);
			send(fd ,msg.c_str() ,msg.size() ,0);
			cout<<"forward Msg Sent 3 from rear conditon"<<endl;
		}
		else
		{
			cout<<"I have a key ...."<<endl;
			string key = token[1];

			string value;

			if(hashTable.find(key) != hashTable.end())
				value = hashTable[key];
			else
				value = "Key not found ";

			int fd = createConnection(token[3], stoi(token[4]));
			string msg = "msg_ack " + value + " " + nodeId;
			send(fd ,msg.c_str() ,msg.size() ,0);
			printhashTable();
		}
	}	
}


void printhashTable()
{
	cout<<"-------------------HashTable Table---------------------"<<endl;
	for(auto pair : hashTable)
	{
		cout<<pair.first <<" : "<<pair.second<<endl;
	}
}