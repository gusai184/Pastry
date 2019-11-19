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



void client()
{
	cout<<"Node Client Started :"<<endl;

	while(1)
	{
		cout<<"# ";
		string command;

		getline(cin,command);

		if(command == "")
			continue;
		
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

void joinHandler(vector<string> command)
{
	int server_fd = createConnection(command[2] ,stoi(command[3]));

	if( server_fd == -1)
	{
		cout<<"Error in connection"<<endl;
		return ;
	} 

	cout<<"Connection Establishd"<<endl;
	
	string cmd = "join 0 " +selfAdd.ip + " " + to_string(selfAdd.port) + " " + selfAdd.nodeId;
	
	send(server_fd ,cmd.c_str() ,cmd.size() ,0);

}

