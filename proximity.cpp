#include <iostream>
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 
#include <netinet/ip_icmp.h> 
#include <time.h> 
#include <fcntl.h> 
#include <signal.h> 
#include <time.h> 
using namespace std;
// Define the Packet Constants 
// ping packet size 
#define PING_PKT_S 64 

// Automatic port number 
#define PORT_NO 0 

// Automatic port number 
#define PING_SLEEP_RATE 1000000 x 

// Gives the timeout delay for receiving packets 
// in seconds 
#define RECV_TIMEOUT 1 

// Define the Ping Loop 
int pingloop=1; 


// ping packet structure 
struct ping_pkt 
{ 
	struct icmphdr hdr; 
	char msg[PING_PKT_S-sizeof(struct icmphdr)]; 
}; 

// Calculating the Check Sum 
unsigned short checksum(void *b, int len) 
{ 	
	unsigned short *buf = (unsigned short *)b; 
	unsigned int sum=0; 
	unsigned short result; 

	for ( sum = 0; len > 1; len -= 2 ) 
		sum += *buf++; 
	if ( len == 1 ) 
		sum += *(unsigned char*)buf; 
	sum = (sum >> 16) + (sum & 0xFFFF); 
	sum += (sum >> 16); 
	result = ~sum; 
	return result; 
} 

// make a ping request 
double send_ping(int ping_sockfd, struct sockaddr_in *ping_addr, char *ping_ip) 
{ 
	int ttl_val=64, msg_count=0, i, addr_len, flag=1, 
			msg_received_count=0; 
	
	struct ping_pkt pckt; 
	struct sockaddr_in r_addr; 
	struct timespec time_start, time_end, tfs, tfe; 
	long double rtt_msec=0, total_msec=0; 
	struct timeval tv_out; 
	tv_out.tv_sec = RECV_TIMEOUT; 
	tv_out.tv_usec = 0; 

	clock_gettime(CLOCK_MONOTONIC, &tfs); 

	
	// set socket options at ip to TTL and value to 64, 
	// change to what you want by setting ttl_val 
	if (setsockopt(ping_sockfd, SOL_IP, IP_TTL, 
			&ttl_val, sizeof(ttl_val)) != 0) 
	{ 
		cout << "Setting socket options to TTL failed!" << endl; 
		return 9999; 
	} 
	// setting timeout of recv setting 
	setsockopt(ping_sockfd, SOL_SOCKET, SO_RCVTIMEO, 
				(const char*)&tv_out, sizeof tv_out); 

	// flag is whether packet was sent or not 
	flag=1; 

	//filling packet 
	bzero(&pckt, sizeof(pckt)); 
	
	pckt.hdr.type = ICMP_ECHO; 
	pckt.hdr.un.echo.id = getpid(); 
	
	for ( i = 0; i < sizeof(pckt.msg)-1; i++ ) 
		pckt.msg[i] = i+'0'; 
	
	pckt.msg[i] = 0; 
	pckt.hdr.un.echo.sequence = msg_count++; 
	pckt.hdr.checksum = checksum(&pckt, sizeof(pckt)); 

	//send packet 
	clock_gettime(CLOCK_MONOTONIC, &time_start); 
	if ( sendto(ping_sockfd, &pckt, sizeof(pckt), 0, 
	(struct sockaddr*) ping_addr, 
		sizeof(*ping_addr)) <= 0) 
	{ 
		cout << "Packet Sending Failed!" << endl; 
		flag=0; 
	} 

	//receive packet 
	addr_len=sizeof(r_addr); 

	if ( recvfrom(ping_sockfd, &pckt, sizeof(pckt), 0, 
		(struct sockaddr*)&r_addr, (socklen_t *)&addr_len) <= 0 
		&& msg_count>1) 
	{ 
		cout << "Packet receive failed! "<<endl; 
	} 

	else
	{ 
		clock_gettime(CLOCK_MONOTONIC, &time_end); 
		
		double timeElapsed = ((double)(time_end.tv_nsec - time_start.tv_nsec))/1000000.0; 
		rtt_msec = (time_end.tv_sec- time_start.tv_sec) * 1000.0 + timeElapsed; 
		
		// if packet was not sent, don't receive 
		if(flag) 
		{ 
			if(!(pckt.hdr.type ==69 && pckt.hdr.code==0)) 
			{ 
				cout << "Error..Packet received with ICMP type "<< pckt.hdr.type << "code " << pckt.hdr.code << endl;  
			} 
			else
			{ 
				msg_received_count++; 
			} 
		} 	 
	} 
	clock_gettime(CLOCK_MONOTONIC, &tfe); 

	return rtt_msec;
} 

double getProximity(string ip, int port)
{
	struct sockaddr_in server_add;
	int c_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if(c_socket == -1)
	{
		cout << "Error while opening socket." << endl;
		perror("Hello");
		return 9999;
	}

	struct hostent *server = gethostbyname(ip.c_str());

	if(server == NULL)
	{
		cout << "No such host exist." << endl;
		close(c_socket);
		return 9999;
	}

	bzero((char *)&server_add, sizeof(server_add));
	server_add.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&server_add.sin_addr.s_addr, server->h_length);
	server_add.sin_port = htons(port);

	return send_ping(c_socket, &server_add, (char *)ip.c_str());
}