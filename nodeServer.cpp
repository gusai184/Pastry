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


void joinHandler(vector<string> token)
{
	string newNodeId = token[4];

	int l = prefixMatch(nodeId ,newNodeId);

	if( token[1] == "0")
	{
		//first node
		sendNeighbourSet(token);
	}

	//cout<<endl<<"Routing Table Send"<<endl;
	sendRoutingTable(token);

  int flag = 1, fd;
  NodeAddress temp;

	//check in leafset for closest node
	temp =  getClosestLeafNode(newNodeId);

	if( temp.nodeId != selfAdd.nodeId)
	{
		//forward to temp
		string msg = "join " + to_string(l+1) + " " +token[2]+" "+token[3] + " "+token[4];
		
    if(isNodeActive(temp) == false)
    {
        //Repair algorithm
        cout<<"Repairing leafSet Running for "<<temp.nodeId<<endl;
        repairLeafSet(temp);
        printleafSet();
        temp =  getClosestLeafNode(newNodeId);
    }
    if(temp.nodeId != selfAdd.nodeId)
    {
      int fd = createConnection(temp.ip, temp.port);
      send(fd ,msg.c_str() ,msg.size() ,0);
      //cout<<"forward Msg Sent 1 "<<temp.port<<endl;
      return;
    }
	}

  //cout<<endl<<"Routing checking starts here ..."<<endl;
	int j = index(newNodeId[l]);
  fd = -1;
	if( routeTable[l][j].nodeId != "empt")
	{
		string msg = "join " + to_string(l+1) + " " +token[2]+" "+token[3] + " "+token[4];
		
    if(isNodeActive(routeTable[l][j]) == false)
    {
      // repair route table algorithm
      cout << "Repairing Routetable for "<< routeTable[l][j].nodeId << endl;
      repairRouteTable(l, j);
      if( routeTable[l][j].nodeId != "empt")
        fd = createConnection(routeTable[l][j].ip ,routeTable[l][j].port);
      printrouteTable();
    }
    

    if(fd != -1)
    {
      send(fd ,msg.c_str() ,msg.size() ,0);
      //cout<<"forward Msg Sent 2 "<<routeTable[l][j].port<<endl;
      return;
    }
	}

 // cout<<endl<<"Rare condition starts here ..."<<endl;
  //rare condition
	temp = getClosestNode(newNodeId);

	if( temp.nodeId != selfAdd.nodeId && isNodeActive(temp) )
	{
		string msg = "join " + to_string(l+1) + " " +token[2]+" "+token[3] + " "+token[4];
		int fd = createConnection(temp.ip, temp.port);
		send(fd ,msg.c_str() ,msg.size() ,0);
		//cout<<"forward Msg Sent 3 "<<temp.port<<endl;
	}
	else
	{
		//cout<<"I am closest in Rear conditon"<<endl;

		NodeAddress newNode;

		newNode.nodeId = newNodeId;
		newNode.ip = token[2];
		newNode.port = stoi(token[3]);
	
		routeTable[l][j] = newNode; 

		sendLeafSet(token);

    string msg = "msg_ack Joined successfully to Pastry Network.";
    int fd = createConnection(newNode.ip ,newNode.port);
    send(fd ,msg.c_str() ,msg.size() ,0);

		//cout<<"Leaf Set Send"<<endl;
	}

}

void * clientRequestThread(void * fd)
{
	int client_fd = *(int *) fd;
	char cmd[BUFFER_SIZE];
  	bzero(cmd,BUFFER_SIZE);
    int n = recv(client_fd ,cmd ,BUFFER_SIZE ,0);

    if(n==0 || cmd=="")
    {
      return NULL;
    }

    //cout<<"==> Got message "<<cmd<<endl;
    vector<string> token = split(string(cmd));

    if( token[0] == "join")
    {
    	joinHandler(token);
    }
    else if( token[0] == "set")
    {
    	setKeyHandler(token);
    }
    else if( token[0] == "get")
    {
    	getKeyHandler(token);	
    }
    else if( token[0] == "msg_ack")
    {
     	getAck(token);
    }
    else if( token[0] == "leafBroadcast")
    {
      recieveLeafSet(token);
      //printleafSet();
      broadCast();

    }
    else if( token[0] == "neighbourSet")
    {
    	receiveNeighbourSet(token);
    	//printneighbourSet();
    }
    else if(token[0] == "leafSet")
    {
    	recieveLeafSet(token);
    	//printleafSet();
    }
    else if(token[0] == "getleafSet")
    {
      sendLeafSet(token);
    }
    else if(token[0] == "routingTable")
    {
    	receiveRoutingTable(token);
    	//printrouteTable();
    }
    else if(token[0] == "broadcast")
    {
    	updateStateTables(token);
    	//Redistribute hash table
    	redistributeHashTable(token);
    	//print();
    	
    }
    else if(token[0] == "addkeyvalue")     
    {
    	addToHashTable(token);
      cout<<endl<<"Replica Recieved"<<endl;
      for(int i=1 ;i<token.size() ;i+=2)
      {
        cout<<token[i]<<" ";
      }
      cout<<endl;
    	printhashTable();
    }

    else if(token[0] == "rdaddkeyvalue")     
    {
      addToHashTable(token);
      sendReplicaonJoining(token);
      //sprinthashTable();
    }
    else if(token[0] == "getRTentry")
    {
      sendRTentry(token);
    }
    else if(token[0] == "getneighbourset")
    {
      sendNeighbourSet(token);
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
       int *client_fd = (int *) malloc(sizeof(int));
       *client_fd = accept(listener_fd, (struct sockaddr *)&peer_address,(socklen_t *)&peerlen);

        if (client_fd < 0) {
        	perror("ERROR on accepting peer request");
        	continue;
      	}
      
      	pthread_t *client_handler = (pthread_t *) malloc(sizeof(pthread_t));
		    pthread_create(client_handler, NULL, clientRequestThread, client_fd);
        pthread_join(*client_handler, NULL);
   }

}



void startServer()
{
   pthread_t thread_id; 
   pthread_create(&thread_id, NULL, serverthread, (void *)NULL); 

   cout<<"Node Server Running on : "<<selfAdd.ip<<" "<<selfAdd.port<<endl<<endl;
}


void getAck(vector<string> token)
{
	f(i,1,token.size())
		cout<<token[i]<<" ";
	cout<<endl;

}



