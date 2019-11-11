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

int index(char ch)
{
	if(ch >= 'a')
		return ch - 'a' + 10;
	else
		return ch - '0';
}

void createNode(string ip, int port)
{
	NodeAddress temp;
	temp.nodeId = "empt";
	pair<double, NodeAddress> t;
	t.first = 9999.0;
	t.second = temp;

	nodeId = md5(ip + to_string(port)).substr(0, ROW);

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
	f(i,0,M)
		if(neighbourSet[i].second.nodeId!="empt")
			cout<<neighbourSet[i].second.nodeId<<":"<<neighbourSet[i].first << endl;
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

bool compare(pair<double, NodeAddress> p1, pair<double, NodeAddress> p2) 
{ 
    return (p1.first < p2.first);
} 

void addToNeighbourSet(NodeAddress newNeighbour)
{
	double dist = getProximity(newNeighbour.ip, newNeighbour.port);
	if(neighbourSet[M-1].first > dist && newNeighbour.nodeId != nodeId)
	{
		neighbourSet[M-1] = make_pair(dist, newNeighbour);
		sort(neighbourSet.begin(), neighbourSet.end(), compare);
	}
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
		cout<<token[i]<<" "<<token[i+1]<<" "<<token[i+2]<<endl;
		//add to leaf set
	}
}

void sendLeafSet(vector<string> token)
{
	string msg = "leafSet "+ selfAdd.nodeId + " " + selfAdd.ip + " " + to_string(selfAdd.port) + " ";

	f(i,0,L)
	{
		if(leafSet[i].nodeId != "empt")
			msg += leafSet[i].nodeId + " " + leafSet[i].ip + " " + to_string(leafSet[i].port) + " "; 
	}

	int client_fd = createConnection(token[2], stoi(token[3]));
	send(client_fd ,msg.c_str() ,msg.size() ,0);
	cout<<endl<<"LeafSet sent .."<<msg<<endl; 	
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
	cout<<endl<<"NeighbourSet sent .."<<endl; 
}

void receiveNeighbourSet(vector<string> token)
{
	cout<<endl<<"NeighbourSet received ... "<<endl;
	for(int i=1 ;i<token.size() ;i+=3)
	{
		NodeAddress temp;
		temp.nodeId = token[i];
		temp.ip = token[i+1];
		temp.port = stoi(token[i+2]);
		addToNeighbourSet(temp);
	}
	printneighbourSet();
}

void serverJoinHandler(vector<string> token)
{
	sendNeighbourSet(token);
	NodeAddress temp;
	temp.nodeId = token[1];
	temp.ip = token[2];
	temp.port = stoi(token[3]);
	addToNeighbourSet(temp);
	printneighbourSet();
	cout<<endl;
	// sendLeafSet(token);
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
        }
        else if(token[0] == "leafSet")
        {
        	recieveLeafSet(token);
        }

   }

}

void startServer()
{
   pthread_t thread_id; 
   pthread_create(&thread_id, NULL, serverthread, (void *)NULL); 

   cout<<"Node Server Running on : "<<selfAdd.ip<<" "<<selfAdd.port<<endl<<endl;
}
