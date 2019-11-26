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

void sendKeyReplica(string key, string val)
{
	NodeAddress temp = getClosestLeafNodeForReplica(key,nodeId);

	if(temp.nodeId == "empt")
		return;

	if( isNodeActive(temp) == false)
	{
		repairLeafSet(temp);
		temp = getClosestLeafNodeForReplica(key,nodeId);
		if(temp.nodeId == "empt")
			return;
	}

	string msg = "addkeyvalue "+ key +" "+val; 
	int fd = createConnection(temp.ip ,temp.port);
	send(fd ,msg.c_str() ,msg.size() ,0);
	close(fd);	
}

void sendReplicaonJoining(vector<string> token)
{
	for(int i=1;i<token.size();i+=2)	
	{
		string key = token[i];
		string value = token[i+1];
		hashTable[key] = value;
		sendKeyReplica(key,value);
	}
}

void redistributeHashTable(vector<string> token)
{
	string newNodeId = token[1];
	string newNodeIP = token[2];
	int newNodeport = stoi(token[3]);

	string msg = "rdaddkeyvalue ";
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
	if(keyhash == nodeId)
	{
		//"I am closest Node add key here ...."
		string key = token[1];
		string value = token[2];
		hashTable[key] = value;

		int fd = createConnection(token[4], stoi(token[5]));
		string msg = "msg_ack key successfully added to node " + nodeId;
		send(fd ,msg.c_str() ,msg.size() ,0);
		sendKeyReplica(key, value);
		
		return;
	}

	NodeAddress temp =  getClosestLeafNode(keyhash);

	if( temp.nodeId != selfAdd.nodeId)
	{
		//forward to temp
		string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4] + " "+token[5];
		
		if( isNodeActive(temp) == false )
		{
			//Repair algorithm
	        cout<<"Repairing leafSet for "<<temp.nodeId<<endl;
	        repairLeafSet(temp);
	        printleafSet();
	        temp =  getClosestLeafNode(keyhash);
		}
		if(temp.nodeId != selfAdd.nodeId )
		{
			int fd = createConnection(temp.ip, temp.port);
			send(fd ,msg.c_str() ,msg.size() ,0);
			//cout<<"forward key query Msg Sent 1 "<<temp.port<<endl;
			return ;
		}
	}

	//cout<<endl<<"Routing Set key checking starts here ..."<<endl;

	int l = prefixMatch(nodeId ,keyhash);
	int j = index(keyhash[l]);

	int fd = -1;
	if( routeTable[l][j].nodeId != "empt")
	{
		if(isNodeActive(routeTable[l][j]) == false)
    	{
	      // repair route table algorithm
	      cout << "Repairing Routetable for "<< routeTable[l][j].nodeId<< endl;
	      repairRouteTable(l, j);
	      if( routeTable[l][j].nodeId != "empt")
	      	fd = createConnection(routeTable[l][j].ip ,routeTable[l][j].port);
	      printrouteTable();
	    }  
	    if( fd != -1)
	    {
			string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4] + " "+token[5];
			send(fd ,msg.c_str() ,msg.size() ,0);
			//cout<<"forward Msg Sent 2 (from Set key)from routing table"<<routeTable[l][j].port<<endl;
			return ;
		}
	}

	//cout<<endl<<"Rare condition Set Key starts here ..."<<endl;
	temp = getClosestNode(keyhash);

	//cout<<"Temp = "<<temp.nodeId<<endl;
	if( temp.nodeId != selfAdd.nodeId && isNodeActive(temp))
	{
		string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4] + " "+token[5];
		int fd = createConnection(temp.ip, temp.port);
		send(fd ,msg.c_str() ,msg.size() ,0);
		// cout<<"forward Msg Sent 3 (from Set key)from rare conditon "<<temp.port<<endl;
	}
	else
	{
		// cout<<"I am closest Node add key here ...."<<endl;
		string key = token[1];
		string value = token[2];
		hashTable[key] = value;

		int fd = createConnection(token[4], stoi(token[5]));
		string msg = "msg_ack key successfully added to node " + nodeId;
		send(fd ,msg.c_str() ,msg.size() ,0);
		sendKeyReplica(key, value);
		//printhashTable();
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
		
		if( isNodeActive(temp) == false )
		{
			//Repair algorithm
	        cout<<"Repairing leafSet for "<<temp.nodeId<<endl;
	        repairLeafSet(temp);
	        //printleafSet();
	        temp =  getClosestLeafNode(keyhash);
		}
		if(temp.nodeId != selfAdd.nodeId )
		{
			int fd = createConnection(temp.ip, temp.port);
			send(fd ,msg.c_str() ,msg.size() ,0);
			//cout<<"forward get query Msg Sent 1 "<<temp.port<<endl;
			return ;
		}
	}

	int l = prefixMatch(nodeId ,keyhash);
	int j = index(keyhash[l]);

	// cout<<endl<<"Routing Get key checking starts here ..."<<endl;


	int fd = -1;
	if( routeTable[l][j].nodeId != "empt")
	{
		if(isNodeActive(routeTable[l][j]) == false)
    	{
	      // repair route table algorithm
	      cout << "Repairing Routetable for "<<routeTable[l][j].nodeId << endl;
	      repairRouteTable(l, j);
	      if( routeTable[l][j].nodeId != "empt")
	      	fd = createConnection(routeTable[l][j].ip ,routeTable[l][j].port);
	      //printrouteTable();
	    }  
	    if( fd != -1)
	    {
			string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4] ;
			send(fd ,msg.c_str() ,msg.size() ,0);
			//cout<<"forward Msg Sent 2 (from Get key) from routing table "<<routeTable[l][j].port<<endl;
			return ;
		}
	}
	
	//cout<<endl<<"Rare condition Get Key starts here ..."<<endl;
	temp = getClosestNode(keyhash);	
	if( temp.nodeId != selfAdd.nodeId && isNodeActive(temp))
	{
		string msg = token[0] + " " + token[1] + " " +token[2]+" "+token[3]+" "+token[4];
		int fd = createConnection(temp.ip, temp.port);
		send(fd ,msg.c_str() ,msg.size() ,0);
		// cout<<"forward Msg 3 sent (from Get key) from rear conditon "<<temp.port<<endl;
	}
	else
	{
		// cout<<"I have a key ...."<<endl;
		string key = token[1];
		string value;

		if(hashTable.find(key) != hashTable.end())
			value =  key + " --> " + hashTable[key];
		else
			value = "Key not found.";

		int fd = createConnection(token[3], stoi(token[4]));
		string msg = "msg_ack " + value;
		send(fd ,msg.c_str() ,msg.size() ,0);
		//printhashTable();
	}
		
}

void gracefulExit()
{
	for(auto pair:hashTable)
	{
		string key = pair.first;
		string val = pair.second;

		NodeAddress temp = getClosestLeafNodeForReplica(key,nodeId);
		
		if(temp.nodeId == "empt")
			break;

		if( isNodeActive(temp) == false)
		{
			repairLeafSet(temp);
			temp = getClosestLeafNodeForReplica(key,nodeId);
			if(temp.nodeId == "empt")
				break;
		}
		cout<<"Replica of key "<<key<<" send to "<<temp.nodeId<<endl;
		string msg = "addkeyvalue "+ key +" "+val; 
		int fd = createConnection(temp.ip ,temp.port);
		send(fd ,msg.c_str() ,msg.size() ,0);
		close(fd);

		temp = getClosestLeafNodeForReplica(key,temp.nodeId);

		if(temp.nodeId != "empt")
		{
			//second node exists for replica sending 
			fd = createConnection(temp.ip ,temp.port);
			send(fd ,msg.c_str() ,msg.size() ,0);
			close(fd);
			cout<<"Replica Send to "<<temp.nodeId<<endl;
		}

	}

	exit(0);
}

void printhashTable()
{
	cout<<"-------------------HashTable Table---------------------"<<endl;
	for(auto pair : hashTable)
	{
		cout<<pair.first <<" : "<<pair.second<<endl;
	}
} 