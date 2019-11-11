#include "header.h"

int main()
{
	Node node("127.0.0.1", 2000);
	cout<<"Node created with nodeId = "<<node.nodeId<<endl;
	node.print();
	return 0;
}