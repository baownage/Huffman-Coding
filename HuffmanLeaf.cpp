#include "HuffmanLeaf.h"
#include <iostream>

HuffmanLeaf::HuffmanLeaf(unsigned char hexVal, long int freq):Node(freq)
{
	hexValue = hexVal;
}

unsigned char HuffmanLeaf::GetHexVal()
{
	return hexValue;
}

void HuffmanLeaf::SetHexVal(unsigned char hexVal)
{
	hexValue = hexVal;
}

string HuffmanLeaf::GetBinVal()
{
	return binaryValue;
}

void HuffmanLeaf::SetBinVal(string binaryVal)
{
	binaryValue = binaryVal;
}




