#include "HuffmanLeaf.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <deque>

using namespace std;

typedef unsigned char BYTE;

//Checks the given BYTE in the given list. If it exists, will return true
//and also set the index at which the BYTE was found. If it doesn't exist, will return true
//and set the index = -1
bool ExistsInVector(BYTE, vector<HuffmanLeaf*>, int*);

//Given a tree, finds the binary value for the leafs. While trying to reach the leaf,
//if the left path was taken, "1" will be appended to the string while "0" will be appended
//if the right path is taken. At the end of this function every leaf will have its own binary value
void SetBinaryValues(Node*, string);

//Prints the leafs' hex values and their corresponding binary value. This function was used for bug testing.
void PrintTree(Node*);

//Finds the number of nodes in a tree. This function was used for bug testing.
int FindNodeCount(Node*);

//This function writes the tree into a BYTE vector
void WriteTreePostOrder(Node*, vector<BYTE>*, int);

//Finds the binary value of an integer
int FindBinary(int);

//Converts an integer to a string
string IntToString(int);

int main()
{
	//variable initializations
	char command;
	char s_fileName[50];
	fstream myFile;
	streampos size;
	BYTE *fileContent;
	
	//program will work untill input 'q' is entered 
	cout<<"Enter c to compress, d to decompress, q to quit\nCommand:";
	cin>>command;
	while(command!='q')
	{
		//If command = 'c' , the program will do compression
		if(command == 'c')
		{
			// Get file name from user		
			cout<<"Enter file name:";
			cin>>s_fileName;
		
			
			// Read file into a BYTE array			
			myFile.open(s_fileName, 	ios::in | ios::out | ios::binary | ios::ate);
			if(myFile.is_open())
			{		
				cout<<"\nOpen!";
				size = myFile.tellg();
				fileContent = new BYTE[size];
				myFile.seekg(0, ios::beg);
				myFile.read((char*)fileContent, size);
				myFile.close();
			}
			else
			{
				cout<<"File "<<s_fileName<<" could not be found!\n";
				cout<<"Enter c to compress, d to decompress, q to quit\nCommand:";
				cin>>command;
				continue;				
			} 
			// Read file done!
			
			cout<<"\nRead complete!\nStarting array creation!";
			
			
			// Create array of bytes and their frequencies
			vector<HuffmanLeaf*> byteList;
			HuffmanLeaf *temp;
			
			int index;
			
			for(int i = 0; i < size; i++)
			{
				//If byte already exists in array, increase its frequency
				if(ExistsInVector(fileContent[i], byteList, &index) == true)
				{
					byteList[index]->IncFreq();
				}
				//If it doesn't exist, then create a new leaf node and add it to the list of bytes
				else
				{
					temp = new HuffmanLeaf(fileContent[i], 1);
					byteList.push_back(temp);
					temp = NULL;
				}
			}
			cout<<"\nSorting...";
			// Sorting the array by frequency. Bubble sort was implemented	
			for(int i = byteList.size(); i > 0; i--)
			{
				for(int j = 0; j<i-1; j++)
				{
					if( byteList[j]->GetFreq() > byteList[j+1]->GetFreq() )
					{
						temp = byteList[j];
						byteList[j] = byteList[j+1];
						byteList[j+1] = temp;
					}
				}
			}
			
			//nodeList will hold the list of nodes. The other list called byteList will change and only hold the root of the tree
			vector<HuffmanLeaf*> nodeList;
			
			for(int i = byteList.size()-1; i > -1; i--)
			{
				nodeList.push_back(byteList[i]);
			}
			
			Node *tempNode;
			long int totalFrequency;
			
			cout<<"\nthis probably takes long :(";
			
			/** This while loop will create the tree.
				it will get the 2 lowest frequency nodes, add their frequencies and make a new node with
				the added frequencies. Next it will add those 2 nodes as its children, remove them from the list
				and add the new node to the list. This will continue until only 1 node is left, and that node will be the
				root of the tree
			*/
			while(byteList.size() != 1)
			{
				totalFrequency = byteList[0]->GetFreq() + byteList[1]->GetFreq();
				tempNode = new Node(totalFrequency);
				tempNode->left = byteList[0];
				tempNode->right = byteList[1];
				byteList.erase(byteList.begin());
				byteList.erase(byteList.begin());
				byteList.push_back((HuffmanLeaf*)tempNode);
				
				//Sort array after insertion of new node
				for(int i = byteList.size(); i > 0; i--)
				{	
					for(int j = 0; j<i-1; j++)
					{
						if( byteList[j]->GetFreq() > byteList[j+1]->GetFreq() )
						{
							temp = byteList[j];
							byteList[j] = byteList[j+1];
							byteList[j+1] = temp;
						}
					}
				}
				
				tempNode = NULL;
			}
			
			//After the operation above, byteList will hold only 1 node, and that node will be the root
			Node *root;
			root = byteList[0];
			
			
			tempNode = root;
			
			string p_binVal;
			
			/** Setting binary values for each leaf in the tree
				Now we are ready to encode the file
			*/			
			SetBinaryValues(tempNode, p_binVal);
			
			tempNode = root;
			 
			
			BYTE tempByte;
			int nodeListIndex;
			bool found;
			string binaryString;
			
			/** This variable will hold the complete binary string for the file
				but it won't hold the tree as binary. That will be held in another string
			*/	
			string completeFileString;
			
			/** This for loop will check every byte of the file, find the corresponding binary value
				that was obtained from the tree, and append it to the string.
			*/
			for(int i = 0; i < size; i++)
			{
				tempByte = fileContent[i];
				nodeListIndex = 0;
				found = false;
				while(nodeListIndex < nodeList.size() && found == false)
				{
					if(nodeList[nodeListIndex]->GetHexVal() == tempByte)
					{
						binaryString = nodeList[nodeListIndex]->GetBinVal();
						found = true;
					}
					nodeListIndex++;
				}
				//If we can't find one of the bytes in the list, there is a major problem.
				if(found == false)
				{
					cout<<"THERE'S A BUG\n";
					exit(1);
				}
				completeFileString.append(binaryString);
				
			}
			
			
			/** After the binary string is ready, we need to check if the string length is
				divisible by 8 because file writing can only be done by bytes, not by bits.
				If the string length is not divisible by 8, zeros will be added to the end of the string
				to make it divisible by 8. In the worst case scenario we add 7 bits of zeros which is minimal
				in terms of memory. 
			*/
			int mod8;
			if(completeFileString.size() % 8 != 0)
			{
				mod8 = 8 - (completeFileString.size() % 8);
				for(int i = 0; i < mod8; i++)
				{
					completeFileString.append("0");
				}
			}
			
			
			/** Now the first part of the file will be created.
				The first byte of the compressed file will tell us how many zeros
				we added to the end of the file. The next set of bytes will give us the tree structure.
				After the tree structure, the rest of the file is the encrypted version of the file
			*/
			vector<BYTE> startPiece;
			//This function will write the tree into startPiece vector
			WriteTreePostOrder(tempNode, &startPiece, 0);
			
			BYTE *fileStartPiece = new BYTE[startPiece.size()+1];
			fileStartPiece[0] = mod8;
			for(int i = 0; i < startPiece.size(); i++)
			{
				fileStartPiece[i+1] = startPiece[i];
			}
			
					
			/** This next part will convert the binary string
				into a BYTE array. This is done so that we can write
				it into the file
			*/
			int newFileSize = completeFileString.size() / 8;
			
			BYTE *newFileContent = new BYTE[newFileSize];
			BYTE toWrite;
			index = 0;
			int endindex;
			unsigned int bit = 0;
			
			/** This for loop reads the binary string character by character,
				and performs bitwise operations to get the corresponding BYTE
			*/
			for(int i = 0; i < newFileSize; i++)
			{
				endindex = index + 8;
				for(; index < endindex; index++)
				{
					if(completeFileString[index] == '0' )
					{
						bit = bit << 1;
					}
					else
					{
						bit = bit << 1;
						bit = bit + 1;
					}
				}
				toWrite = bit;
				newFileContent[i] = toWrite;
			}
			
			//Create a new file called compressed.bin and write into the file the starting piece and the rest of the encryption
			fstream newFile;	
			newFile.open("compressed.bin", ios::out | ios::binary);
			if(newFile.is_open())
			{
				newFile.write((char*)fileStartPiece, startPiece.size()+1);
				newFile.write((char*)newFileContent, newFileSize);
				newFile.close();
			}
			else cout<<"\nThe file \"compressed.bin\" could not be created.";
			
			//Free the memory
			delete fileContent;
			fileContent = NULL;
			for(int i = 0; i < nodeList.size(); i++)
			{
				delete nodeList[i];
			}
			
			cout<<"\n\nCompress Done :)\n";
		}
		//If command = 'd' then the program will do decompression
		else if(command == 'd')
		{
			cout<<"\nEnter file name after decompress done \"example.txt\": ";
			cin>>s_fileName;
			
			//Open file compressed.bin and read it into fileContent BYTE array
			myFile.open("compressed.bin", ios::in | ios::binary | ios::ate);
			if(myFile.is_open())
			{		
				cout<<"Open!";
				size = myFile.tellg();
				fileContent = new BYTE[size];
				myFile.seekg(0, ios::beg);
				myFile.read((char*)fileContent, size);
				myFile.close();
			}
			else
			{
				cout<<"compressed.bin file could not be opened.\n";
				cout<<"Enter c to compress, d to decompress, q to quit\nCommand:";
				cin>>command;
				continue;
			} 
			
			
			/** After reading the file into the BYTE array, we will use a stack to construct the tree.
				A stack is used because the tree is written post order in the file.
				This is done by adding the leafs into the stack, and once we read a node we remove the leafs
				from the stack and make them the children of the node. Then we place the node back in the stack.
				At the end, the stack will contain only 1 element and that will be the root of the tree.
			*/
			deque<HuffmanLeaf*> stackOfNodes;
			bool foundRoot = false;
			int numberOfZeros = fileContent[0];
			int fileIndex = 1;
			BYTE tempByte2;
			HuffmanLeaf *tempLeaf;
			Node *tempNode2;
			/** The tree is encoded like this: For every node, there is 1 byte of data
				that defines the node. 
				If the byte equals to 0 it means the node is a leaf.
				If the byte equals to 1 it means the node is an internal node of the tree.
				If the byte equals to 2 it means the node is the root of the tree.
				So the tree data looks something like this in the encrypted file: 0 A0 0 F5 0 1D 1 1 2 2
				This is read by the program in this way: First byte is zero so it is a leaf and the hex value for it is in the next byte which is A0
												 		 The third byte is again zero so it is also a leaf and the hex value for it is F5
												 		 The fifth byte is another zero, meaning a leaf with the value 1D
												 		 The seventh byte is a 1 which means it is an internal node and the hex value doesn't matter, so is just shown as 1
												 		 The ninth byte is a 2 which means this is the root of the tree.			
			*/
			while(!foundRoot)
			{
				//If the defining byte is a 0, create a new leaf and add it to the stack
				if(fileContent[fileIndex] == 0)
				{
					tempLeaf = new HuffmanLeaf(fileContent[fileIndex+1], 0);
					stackOfNodes.push_front(tempLeaf);
					tempLeaf = NULL;
				}
				//If the defining byte is a 1, create a new internal node, make the first two nodes in the stack its children,
				//remove those from the stack and add the internal node to the stack.
				else if(fileContent[fileIndex] == 1)
				{
					tempNode2 = new Node(0);
					tempNode2->right = stackOfNodes[0];
					tempNode2->left = stackOfNodes[1];
					stackOfNodes.erase(stackOfNodes.begin());
					stackOfNodes.erase(stackOfNodes.begin());
					stackOfNodes.push_front((HuffmanLeaf*)tempNode2);
					tempNode2 = NULL;
				}
				//If the defining byte is a 2, then it is the root of our tree. We will do the same operations as we did with an internal node,
				//but also set foundRoot = true so our while loop ends.
				else
				{
					tempNode2 = new Node(0);
					tempNode2->right = stackOfNodes[0];
					tempNode2->left = stackOfNodes[1];
					stackOfNodes.erase(stackOfNodes.begin());
					stackOfNodes.erase(stackOfNodes.begin());
					stackOfNodes.push_front((HuffmanLeaf*)tempNode2);
					tempNode2 = NULL;
					//to end the while loop
					foundRoot = true;
				}
				fileIndex = fileIndex + 2;
			}
			//Constructing of tree is complete!
			
			/** Read rest of the file and start creation of binary string which will be used to decompress the file.
				This string will contain the rest of the file in binary. 
			*/
			Node *decompressRoot;
			cout<<endl;
			decompressRoot = stackOfNodes[0];
			string encryptedFile;
			/** This for loop will loop through the rest of the file contents and for every byte read, it will
				convert it to binary, and if the binary value is not 8 bits it will add zeros to the start so that
				it becomes 8 bits. After this loop, the string encryptedFile will contain the binary string of the compressed file.
			*/
			for(;fileIndex<size;fileIndex++)
			{
				int byteVal = fileContent[fileIndex];
				int binaryValue = FindBinary(byteVal);
				string temp = IntToString(binaryValue);
				string front = "";
				if(temp.size() != 8)
				{
					int append = 8 - temp.size();
					for(int i = 0; i<append; i++)
					{
						front.append("0");
					}
					front.append(temp);
				}
				else
				{
					front = temp;
				}
				encryptedFile.append(front);
				
			}
			//binary string creation complete, now to decipher it using the tree
			
			
			
			vector<BYTE> fileBytes;
			Node *tempNode;
			tempNode = decompressRoot;
			/** This for loop will loop through the binary string and for every character it will do the following:
				if the character is a 0 it will take the left path and check if that node is a leaf or not.
				if it is a leaf, the hex value will be added to the list of bytes which is called fileBytes.
				if it is not a leaf, it will not do anything.
				if the character is a 1 it will take the right path and do the same check.
			*/
			for(int i = 0; i < encryptedFile.size()-numberOfZeros; i++)
			{
				if(encryptedFile[i] == '0')
				{
					tempNode = tempNode->left;
					if(tempNode->left == NULL && tempNode->right == NULL)
					{
						HuffmanLeaf *tempLeaf = (HuffmanLeaf*)tempNode;
						fileBytes.push_back(tempLeaf->GetHexVal());
						tempNode = decompressRoot;
					}
				}
				else
				{
					tempNode = tempNode->right;
					if(tempNode->left == NULL && tempNode->right == NULL)
					{
						HuffmanLeaf *tempLeaf = (HuffmanLeaf*)tempNode;
						fileBytes.push_back(tempLeaf->GetHexVal());
						tempNode = decompressRoot;
					}
				}
			}
			//The vector fileBytes is filled with the data we need, now to convert it to a BYTE array
			
			delete fileContent;
			fileContent = NULL;
			fileContent = new BYTE[fileBytes.size()];
			//Copy contents of fileBytes into fileContent which is a BYTE array
			for(int i = 0; i < fileBytes.size(); i++)
			{
				fileContent[i] = fileBytes[i];
			}
			
			//Create a new file with the name the user specified and write fileContent into the file
			fstream newFile;
			newFile.open(s_fileName, ios::out | ios::binary);
			if(newFile.is_open())
			{
				newFile.write((char*)fileContent, fileBytes.size());
				newFile.close();
				cout<<"\nDecompress done!";
			}
			else
			{
				cout<<endl<<"Could not create file "<<s_fileName<<endl;
			}
		}
		//Get the new command
		cout<<"\n\nEnter c to compress, d to decompress, q to quit\nCommand:";
		cin>>command;
		
	}
	
	
	
	return 0;
}

//////////////////////END OF MAIN





//Checks the given BYTE in the given list. If it exists, will return true
//and also set the index at which the BYTE was found. If it doesn't exist, will return true
//and set the index = -1
bool ExistsInVector(BYTE byte, vector<HuffmanLeaf*> list, int *index)
{
	for(int i = 0; i < list.size(); i++)
	{
		if(byte == list[i]->GetHexVal())
		{
			*index = i;
			return true;
		}
	}
	*index = -1;
	return false;
}

//Given a tree, finds the binary value for the leafs. While trying to reach the leaf,
//if the left path was taken, "1" will be appended to the string while "0" will be appended
//if the right path is taken. At the end of this function every leaf will have its own binary value
void SetBinaryValues(Node *temp, string binaryVal)
{
	if(temp != NULL)
	{
		if(temp->left == NULL && temp->right == NULL)
		{
			HuffmanLeaf *tempLeaf = (HuffmanLeaf*)temp;
			tempLeaf->SetBinVal(binaryVal);
		}
		string left = binaryVal;
		string right = binaryVal;
		left.append("0");
		right.append("1");
		SetBinaryValues(temp->left, left);
		SetBinaryValues(temp->right, right);
	}
}

//Prints the leafs' hex values and their corresponding binary value. This function was used for bug testing.
void PrintTree(Node *temp)
{
	if(temp != NULL)
	{
		if(temp->left == NULL && temp->right == NULL)
		{
			HuffmanLeaf *tempLeaf = (HuffmanLeaf*)temp;
			cout<<tempLeaf->GetHexVal()<<" "<<tempLeaf->GetBinVal()<<endl;
		}
		PrintTree(temp->left);
		PrintTree(temp->right);
	}
}

//Finds the number of nodes in a tree. This function was used for bug testing.
int FindNodeCount(Node *temp)
{
	if(temp != NULL)
	{
		return (1 + FindNodeCount(temp->left) + FindNodeCount(temp->right));
	}
}

//This function writes the tree into a BYTE vector
void WriteTreePostOrder(Node *temp, vector<BYTE> *array, int root)
{
	if(temp != NULL)
	{
		WriteTreePostOrder(temp->left, array, root+1);
		WriteTreePostOrder(temp->right, array, root+1);
		if(temp->left == NULL && temp->right == NULL)
		{
			HuffmanLeaf *tempLeaf = (HuffmanLeaf*)temp;
			BYTE zero = 0;
			array->push_back(zero);
			array->push_back(tempLeaf->GetHexVal());
		}
		else
		{
			if(root == 0)
			{
				BYTE two = 2;
				
				array->push_back(two);
				array->push_back(two);
			}
			else
			{
				BYTE one = 1;
				array->push_back(one);
				array->push_back(one);
			}
			
		}
	}
}

//Finds the binary value of an integer
int FindBinary(int x)
{
	if(x == 0) return 0;
	else return (x % 2 + 10*FindBinary(x/2));
}

//Converts an integer to a string
string IntToString(int x)
{
	ostringstream temp;
	temp<<x;
	return temp.str();
}


/*
	fstream newFile;	
	newFile.open("nextvideo.mp4", ios::out | ios::binary);
	if(newFile.is_open())
	{
		newFile.write((char*)fileContent, size);
	}
	else cout<<"No";
	*/
