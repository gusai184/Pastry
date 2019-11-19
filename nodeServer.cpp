#include "header.h"
#include "md5.cpp"

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




void serverJoinHandler(vector<string> token)
{
	string newNodeId = token[4];

	int l = prefixMatch(nodeId ,newNodeId);

	if( token[1] == "0")
	{
		//first node
		cout<<endl<<"Neighbour set send"<<endl;
		sendNeighbourSet(token);
	}

	cout<<endl<<"Routing Table Send"<<endl;
	sendRoutingTable(token);

	//check in leafset for closest node
	NodeAddress temp =  getClosestLeafNode(newNodeId);

	if( temp.nodeId != selfAdd.nodeId)
	{
		//forward to temp
		string msg = "join " + to_string(l+1) + " " +token[2]+" "+token[3] + " "+token[4];

		int fd = createConnection(temp.ip ,temp.port);

		send(fd ,msg.c_str() ,msg.size() ,0);

		cout<<"forward Msg Sent 1"<<endl;

		return ;
	}

	int j = index(newNodeId[l]);

	//print();

	if( routeTable[l][j].nodeId != "empt")
	{
		string msg = "join " + to_string(l+1) + " " +token[2]+" "+token[3] + " "+token[4];

		int fd = createConnection(routeTable[l][j].ip ,routeTable[l][j].port);

		send(fd ,msg.c_str() ,msg.size() ,0);

		cout<<"forward Msg Sent 2"<<endl;
	}
	else // Rare Condition
	{
		temp = getClosestNode(newNodeId);

		cout<<"Temp = "<<temp.nodeId<<endl;
		if( temp.nodeId != selfAdd.nodeId )
		{
			string msg = "join " + to_string(l+1) + " " +token[2]+" "+token[3] + " "+token[4];

			int fd = createConnection(temp.ip, temp.port);

			send(fd ,msg.c_str() ,msg.size() ,0);

			cout<<"forward Msg Sent 3"<<endl;
		}
		else
		{
			cout<<"I am closest in Rear conditon"<<endl;

			NodeAddress newNode;

			newNode.nodeId = newNodeId;
			newNode.ip = token[2];
			newNode.port = stoi(token[3]);
		
			routeTable[l][j] = newNode; 

			sendLeafSet(token);

			cout<<"Leaf Set Send"<<endl;
		}
	}

}


void * serverthread(void *args)
{
   int listener_port = selfAdd.port;
   int listener_fd,   peerlen, n;
   struct sockaddr_in listener_address, peer_address;

   listener_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (listener_fd < 0) {
      perror("ERROR opening socket");
      return NULL;
   }
   
   bzero((char *) &listener_address, sizeof(listener_address));
   
   listener_address.sin_family = AF_INET;
   listener_address.sin_addr.s_addr = INADDR_ANY;
   listener_address.sin_port = htons(listener_port);

   if (bind(listener_fd, (struct sockaddr *) &listener_address, sizeof(listener_address)) < 0) {
      perror("ERROR on binding");
      return NULL;
   }
   
   listen(listener_fd,SOMAXCONN);
   peerlen = sizeof(peer_address);
   
   while(1)
   {
       int client_fd;
       client_fd = accept(listener_fd, (struct sockaddr *)&peer_address,(socklen_t *)&peerlen);

        if (client_fd < 0) {
        	perror("ERROR on acceptingg peer request");
        	continue;
      	}
      
      	char cmd[BUFFER_SIZE];
      	bzero(cmd,BUFFER_SIZE);
	    int n = recv(client_fd ,cmd ,BUFFER_SIZE ,0);
        cout<<"Got message "<<cmd<<endl;
        vector<string> token = split(string(cmd));

        if( token[0] == "join")
        {
        	serverJoinHandler(token);
        }
        else if( token[0] == "neighbourSet")
        {
        	receiveNeighbourSet(token);
        	printneighbourSet();
        }
        else if(token[0] == "leafSet")
        {
        	recieveLeafSet(token);
        	printleafSet();
        	broadCast();
        }
        else if(token[0] == "routingTable")
        {
        	receiveRoutingTable(token);
        	printrouteTable();
        }
        else if(token[0] == "broadcast")
        {
        	updateStateTables(token);
        	print();
        }     
   }

}

void startServer()
{
   pthread_t thread_id; 
   pthread_create(&thread_id, NULL, serverthread, (void *)NULL); 

   cout<<"Node Server Running on : "<<selfAdd.ip<<" "<<selfAdd.port<<endl<<endl;
}
