#include "header.h"

extern string nodeId;
extern NodeAddress selfAdd;
extern vector<NodeAddress> leafSet;
extern vector<NodeAddress> neighbourSet;
extern vector<vector<NodeAddress>> routeTable;
extern unordered_map<string, string> hashTable;

extern int L;
extern int M;
extern int ROW;
extern int COL;
extern int DIGITS;

int createConnection(string ip ,int port)
{
	int listener_port = port, n, server_fd;
	struct sockaddr_in listener_address;
	struct hostent *listener_ip;
	char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
    	
    	perror("ERROR opening socket");
        return -1;
        //exit(1);
    }
	
    listener_ip = gethostbyname(ip.c_str());
    if (listener_ip == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return -1;
        //exit(0);
    }

    bzero((char *) &listener_address, sizeof(listener_address));
    listener_address.sin_family = AF_INET;
    bcopy((char *)listener_ip->h_addr, (char *)&listener_address.sin_addr.s_addr, listener_ip->h_length);
    listener_address.sin_port = htons(listener_port);
   
    if (connect(server_fd, (struct sockaddr*)&listener_address, sizeof(listener_address)) < 0) {
        cout<<"error no is "<<errno<<endl;
        perror("ERROR connecting");
        return -1;
    }

    return server_fd;
}

void joinHandler(vector<string> command)
{
	int server_fd = createConnection(command[2] ,stoi(command[3]));

	if( server_fd == - 1)
	{
		cout<<"Error in connection"<<endl;
		return ;
	} 

	cout<<"Connection Establishd"<<endl;
	
	string cmd = "join " + selfAdd.nodeId + " " + selfAdd.ip + " " + to_string(selfAdd.port);

	//cout<<cmd<<endl;
	s
	send(server_fd ,cmd.c_str() ,cmd.size() ,0);
}

vector<string> split(string s)
{
	istringstream ss(s);
	string token;

	vector<string> temp;

	while(getline(ss, token, ' ')) 
	{
    	temp.push_back(token);
	}

	return temp;
}

void client()
{
	cout<<"Node Client Started :"<<endl;

	while(1)
	{
		cout<<"# ";
		string command;

		getline(cin,command);

		vector<string> command_splited = split(command);


		if( command_splited[0] == "join" && command_splited.size() == 4) 
		{
			joinHandler(command_splited);	
		}
		else
		{
			cout<<"Invalid command"<<endl;
		}
	}
}