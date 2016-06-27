#include "Node.h"
#include <iostream>

Node::Node()
{
	frequency = 0;
	left = NULL;
	right = NULL;
}

Node::Node(long int freq)
{
	frequency = freq;
	left = NULL;
	right = NULL;	
}

int Node::GetFreq()
{
	return frequency;
}

void Node::SetFreq(long int freq)
{
	frequency = freq;
}

void Node::IncFreq()
{
	frequency++;
}
