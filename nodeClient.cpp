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
			join(command_splited);	
		}
		else if (command_splited[0] == "set" && command_splited.size() == 3) 
		{
			setKey(command_splited);
		}
		else if (command_splited[0] == "get" && command_splited.size() == 2) 
		{
			getKey(command_splited);
		}
		else if( command_splited[0] == "exit" && command_splited.size() == 1)
		{
			gracefulExit();
		}
		else
		{
			cout<<"Invalid command"<<endl;
		}
	}
}

void setKey(vector<string> command)
{

	int server_fd = createConnection(selfAdd.ip , selfAdd.port);
	if( server_fd == -1)
	{
		cout<<"Error in connection"<<endl;
		return ;
	} 
		
	//string keyhash = md5(command[1]).substr(0,DIGITS);
	string keyhash = command[1];
	cout<<"key hash : "<<keyhash<<endl;
		string cmd = command[0] + " " + command[1] + " " + command[2] + " " +keyhash + " "+selfAdd.ip + " " + to_string(selfAdd.port);
	send(server_fd ,cmd.c_str() ,cmd.size() ,0);
}

void getKey(vector<string>  command)
{
	int server_fd = createConnection(selfAdd.ip , selfAdd.port);
	if( server_fd == -1)
	{
		cout<<"Error in connection"<<endl;
		return ;
	} 
		
	//string keyhash = md5(command[1]).substr(0,DIGITS);
	string keyhash = command[1];
	cout<<"key hash : "<<keyhash<<endl;
	string cmd = command[0] + " " + command[1] + " " +keyhash + " "+selfAdd.ip + " " + to_string(selfAdd.port);
	send(server_fd ,cmd.c_str() ,cmd.size() ,0);

}
void join(vector<string> command)
{
	int server_fd = createConnection(command[2] ,stoi(command[3]));
	if( server_fd == -1)
	{
		cout<<"Error in connection"<<endl;
		return ;
	} 

	string cmd = "join 0 " +selfAdd.ip + " " + to_string(selfAdd.port) + " " + selfAdd.nodeId;
	send(server_fd ,cmd.c_str() ,cmd.size() ,0);
}
