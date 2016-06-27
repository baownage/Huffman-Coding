#ifndef Node_H
#define Node_H
#include <iostream>

using namespace std;

class Node
{
	private:
		long int frequency;
	public:
		
		// Constructor
		Node();
		
		// Constructor 2
		Node(long int freq);
		
		// Pointers for the left and right child
		Node *left;
		Node *right;
		
		// Get the frequency value of the node
		int GetFreq();
		
		// Set the frequency value of the node
		void SetFreq(long int freq);
		
		// Increase the frequency by 1
		void IncFreq();
};

#endif
