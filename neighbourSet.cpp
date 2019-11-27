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

void addToNeighbourSet(NodeAddress newNeighbour)
{
	if(newNeighbour.nodeId == nodeId)
		return;

	int fd = createConnection(newNeighbour.ip, newNeighbour.port);
	if(fd == -1)
	{
		//node is not active
		return;
	}
	close(fd);

	f(i,0,M)
		if(newNeighbour.nodeId == neighbourSet[i].second.nodeId)
			return;

	double dist = getProximity(newNeighbour.ip, newNeighbour.port);
	if(neighbourSet[M-1].first > dist)
	{
		neighbourSet[M-1] = make_pair(dist, newNeighbour);
		sort(neighbourSet.begin(), neighbourSet.end(), compare1);
	}
}


void sendNeighbourSet(vector<string> token)
{
	string msg = "neighbourSet "+ selfAdd.nodeId + " " + selfAdd.ip + " " + to_string(selfAdd.port) + " ";

	f(i,0,M)
	{
		if(neighbourSet[i].second.nodeId != "empt")
			msg += neighbourSet[i].second.nodeId + " " + neighbourSet[i].second.ip + " " + to_string(neighbourSet[i].second.port) + " "; 
	}

	int client_fd = createConnection(token[2], stoi(token[3]));
	send(client_fd ,msg.c_str() ,msg.size() ,0);
	//cout<<endl<<"NeighbourSet sent .."<<endl; 
}

void receiveNeighbourSet(vector<string> token)
{
	//cout<<endl<<"NeighbourSet received ... "<<endl;
	for(int i=1 ;i<token.size() ;i+=3)
	{
		NodeAddress temp;
		temp.nodeId = token[i];
		temp.ip = token[i+1];
		temp.port = stoi(token[i+2]);
		addToNeighbourSet(temp);
	}
}



void * neighbourThread(void * args)
{
	while(1)
	{
		sleep(60);
		bool isNodeFail = false;
		//cout<<"Periodic Updation Started"<<endl;
		f(i,0,M)
		{
			NodeAddress node = neighbourSet[i].second;

			if( node.nodeId != "empt" && isNodeActive(node)==false)
			{
				neighbourSet[i].second.nodeId = "empt";
				neighbourSet[i].first = 9999.0;	
				isNodeFail = true;
			}
		}

		if(isNodeFail == false)
			continue;

		sort(neighbourSet.begin(), neighbourSet.end(), compare1);

		f(i,0,M)
		{
			NodeAddress node = neighbourSet[i].second;

			if( node.nodeId != "empt")
			{
				string msg = "getneighbourset "+ nodeId+" "+selfAdd.ip+" "+to_string(selfAdd.port);

				int fd = createConnection(node.ip ,node.port);
				send(fd ,msg.c_str() ,msg.size() ,0);
				close(fd);
			}
		}
	}
}

void repairneighbourSet()
{
   pthread_t thread_id; 
   pthread_create(&thread_id, NULL, neighbourThread, (void *)NULL); 
}


void printneighbourSet()
{

	cout<<"-------------------Neighbour Table---------------------"<<endl;
	f(i,0,M)
		if(neighbourSet[i].second.nodeId!="empt")
			cout<<neighbourSet[i].second.nodeId<<":"<<neighbourSet[i].first << endl;
	cout<<endl;
}