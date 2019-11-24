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


int prefixMatch(string id1, string id2)
{
	int match = 0;

	f(i,0,DIGITS)
	{
		if(id1[i] == id2[i])
			match++;
		else
			break;
	}

	return match;
}



int diff(string id1, string id2)
{
	return abs(hax_dec(id1) - hax_dec(id2));
}


int index(char ch)
{
	if(ch >= 'a')
		return ch - 'a' + 10;
	else
		return ch - '0';
}

int hax_dec(string s)
{
	int base = 1; 
    int dec_val = 0; 
     
    int len = s.size();

    for (int i=len-1; i>=0; i--) 
    {    
        if (s[i]>='0' && s[i]<='9') 
        { 
            dec_val += (s[i] - 48)*base;    
            base = base * 16; 
        } 
        else if (s[i]>='a' && s[i]<='f') 
        { 
        	if( s[i] == 'a')
        	{
        		dec_val += (10)*base;	
        	}
        	else if( s[i] == 'b')
        	{
        		dec_val += (11)*base;
        	}
        	else if( s[i] == 'c')
        	{
        		dec_val += (12)*base;
        	}
        	else if( s[i] == 'd')
        	{
        		dec_val += (13)*base;
        	}
        	else if( s[i] == 'e')
        	{
        		dec_val += (14)*base;
        	}
        	else if( s[i] == 'f')
        	{
        		dec_val += (15)*base;
        	}
                 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
}

bool compare(NodeAddress i1, NodeAddress i2) 
{ 
    return (i1.nodeId < i2.nodeId); 
} 

bool compare1(pair<double, NodeAddress> p1, pair<double, NodeAddress> p2) 
{ 
    return (p1.first < p2.first);
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
        cout<<"ERROR connecting "<<ip<<" "<<to_string(port)<<endl;
        return -1;
    }

    return server_fd;
}

void print()
{
	printleafSet();
	cout<<endl;
	printneighbourSet();
	cout<<endl;
	printrouteTable();
	cout<<endl;
	printhashTable();
	cout<<endl;
}

