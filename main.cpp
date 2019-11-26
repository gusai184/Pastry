#include "header.h"

string nodeId;
NodeAddress selfAdd;
vector<NodeAddress> leafSet;
vector<pair<double, NodeAddress>> neighbourSet;
vector<vector<NodeAddress>> routeTable;
unordered_map<string, string> hashTable;


int L = 4;//pow(2,B); //leafSet Size
int M = 4;//pow(2,B); //NeighbourSet Size
int ROW = ceil(log(N) / log(pow(2,B)));
int COL = pow(2,B);
int DIGITS = ROW;

string getIP()
{
	string line;
	ifstream IPFile;
	int offset;
	vector<string> ip;        
	system("ifconfig > ip.txt");

	IPFile.open ("ip.txt");
	if(IPFile.is_open())
	{
	    while(getline(IPFile, line))
	    {
		    string temp;
		    stringstream ss(line);
		    ss >> temp;
		    if(temp == "inet")
		    {
		    	ss >> temp;
		    	ip.push_back(temp);
		    }
	    }
	    system("rm ip.txt");
	    for(int i = 0; i < ip.size(); i++)
    		if(ip[i] != "127.0.0.1")
    			return ip[i];
  
  		return ip[0];
	}
	
}

void menu()
{
	cout<<"---------List of Commands-------------"<<endl;
	cout<<"\t1) join ID IP PORT"<<endl;
	cout<<"\t2) set key value"<<endl;
	cout<<"\t3) get key "<<endl;
	cout<<"\t4) lt"<<endl;
	cout<<"\t5) ht"<<endl;
	cout<<"\t6) rt"<<endl;
	cout<<"\t7) nt"<<endl;
	cout<<"\t8) all"<<endl;
	cout<<"\t9) exit"<<endl<<endl;
}

int main()
{
	cout<<"--------------------Pastry Node--------------------"<<endl<<endl;
	cout<<"PORT :";
	string p ;
	getline(cin,p);

	int port = stoi(p);
	
	//Gets IP of the machine in string
	string ip = getIP();
	cout<<"IP Address : "<<ip<<endl;
	
	string temp;
	//cout<<"Enter Node Id :";
	//getline(cin,temp);
	temp = p;
	nodeId = temp;
	//selfAdd.nodeId = temp;

	//Create ID and Intialize Node Tables
	createNode(ip, port);
	cout<<"NodeId = "<<nodeId<<endl<<endl;
	
	//creates Server On thread 
	startServer();
	menu();
	//Periodically Updates Neighbour set
	repairneighbourSet();
	
	// //Client Function takes commands from user 
	client();
	
	return 0;
}