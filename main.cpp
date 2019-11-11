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
	struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
	  string ret;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr)
            continue;
        if (ifa->ifa_addr->sa_family == AF_INET)
        { // IP4
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      		if(ifa->ifa_name[0] == 'w')
            {
  				ret = string(addressBuffer);
  				if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
  				return ret;
      			}
        }
    }
    return ret;
}

int main()
{
	cout<<"Enter PORT For Node :";
	string p ;
	getline(cin,p);

	int port = stoi(p);
	
	//Gets IP of the machine in string
	string ip = getIP();
	cout<<"IP Address Of Node :"<<ip<<endl<<endl;
	
	//Create ID and Intialize Node Tables
	createNode(ip, port);
	cout<<"Node created with nodeId = "<<nodeId<<endl<<endl;
	
	//creates Server On thread 
	startServer();

	//Client Function takes commands from user 
	client();
	
	return 0;
}