#ifndef HuffmanLeaf_H
#define HuffmanLeaf_H
#include "Node.h"
#include <iostream>
#include <string>

using namespace std;

class HuffmanLeaf : public Node
{
	private:
		unsigned char hexValue;
		string binaryValue;
		
		
	public:
		// Constructor
		HuffmanLeaf(unsigned char hexVal, long int freq);
		
		// Get the hex value of the leaf
		unsigned char GetHexVal();
		
		// Set the hex value of the leaf
		void SetHexVal(unsigned char hexVal);
		
		// Get binary value of the leaf
		string GetBinVal();
		
		// Set the binary value of the leaf
		void SetBinVal(string binaryVal);
			
};

#endif
