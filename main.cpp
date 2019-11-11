#include "header.h"

string nodeId;
NodeAddress selfAdd;
vector<NodeAddress> leafSet;
vector<NodeAddress> neighbourSet;
vector<vector<NodeAddress>> routeTable;
unordered_map<string, string> hashTable;


int L = pow(2,B); //leafSet Size
int M = pow(2,B); //NeighbourSet Size
int ROW = ceil(log(N) / log(pow(2,B)));
int COL = pow(2,B);
int DIGITS = ROW;

string getIP()
{
	char hostbuffer[256]; 
    struct hostent *host_entry; 
  
    // To retrieve hostname 
    gethostname(hostbuffer, sizeof(hostbuffer)); 
  
    // To retrieve host information 
    host_entry = gethostbyname(hostbuffer);  
  
    // To convert an Internet network 
    // address into ASCII string 
    char *IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0])); 

    return string(IPbuffer);
}

vector<string> split(string s ,string del)
{
	int pos = 0;
	
	string token;
	
	vector<string> res;

	while ((pos = s.find(del)) != std::string::npos) 
	{
    	token = s.substr(0, pos);
    	res.push_back(token);
    	s.erase(0, pos + del.length());
	}

	return res;
}

void client()
{
	cout<<"Input Commands :"<<endl;

	while(1)
	{
		string command;

		getline(cin,command);

		vector<string> command_splited = split(command ," ");

		f(i,0,command_splited.size())
		{
			cout<<command_splited[i]<<endl;
		}

		break;
	}
}

int main()
{
	cout<<"Enter PORT For Node :";
	string p ;
	getline(cin,p);

	int port = stoi(p);
	cout<<port;
	//Gets IP of the machine in string
	string ip = getIP();
	cout<<"IP Address Of Node :"<<ip<<endl;

	ip = "127.0.0.1";
	
	//Create ID and Intialize Node Tables
	createNode(ip, port);
	cout<<"Node created with nodeId = "<<nodeId<<endl;
	
	//creates Server On thread 
	startServer();

	//Client Function takes commands from user 
	client();
	
	return 0;
}