#include <cstdio>
#include <vector>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <map>
#include <fstream>

using namespace std;

class Node 
{
		unsigned long long int count;
		Node* left;
		Node* right;
		char ch;


	public:

		Node(){
			;
		}

		Node(uint ncount, char nch)
		{
			Node(ncount, nch, NULL, NULL);
		}

		Node(uint ncount, char nch, Node* l, Node* r) 
			:ch(nch), count(ncount), left(l), right(r) 
		{;}

		Node(Node* l, Node* r): ch(0), count(l->getCount() + r->getCount()), left(l), right(r)
		{
			;
		}

		~Node() {
			free();
        }

		void Create(unsigned long long int ncount, char nch)
		{
			count = ncount;
			ch = nch;
			left = nullptr;
			right = nullptr;
		}

		void Create(unsigned long long int ncount, char nch, Node* l, Node* r) {
			count = ncount;
			ch = nch;
			left = l; 
			right = r;
		}


//------getters
		char getChar(){
			return ch;
		}

		uint getCount(){
			return count;
		}

		Node* getLeft(){
			return left;
		}

		Node* getRight(){
			return right;
		}
		//finished getters

		bool isLeaf() { 
			if (NULL != left || NULL != right)
			{
				//not a leaf
				return false;
			}
			return true;
		 }

		void free() {
			//std::cout << "Free: " << ch << " = " << count << '\n';
			//return;
			if (NULL != left) { delete left; } 
			if (NULL != right) { delete right; } 
			left = NULL;
			right = NULL;
		}


		unsigned long long int encodeMe(map<char, string> &binAssociation, string soFar){
			//encodeThe thingy
			unsigned long long int bitSize = 0;
			if(isLeaf()){
				//it's a leaf put it in the dict!!!
				//and update a file!!!
				ofstream keyFile;
				keyFile.open ("compressKey.huf", ios::app | ios::out | ios::binary);

				char valPair[9];
				uint n = getCount();

				valPair[1] = getChar();

				int ii = 2;
				for(ii = 2; n != 0; ++ii){
					valPair[ii] = (n & 0xFF);
				//	std::cout <<ii<<" Get count in bytes is " << (unsigned char)valPair[ii]<<'\n';
					n = n >> 8;
				}
				//negative one delims values
				valPair[0] = -1;

				//std::cout << "Negative 1 is " << (unsigned int)valPair[ii] << '\n';
				keyFile.write(valPair, ii);
				keyFile.close();

				binAssociation[getChar()] = soFar;
				//computenegative
				if(soFar.length() == 0){
					soFar = "1";
				}
				bitSize = getCount() * soFar.length();
				char showChar = getChar();
				if(showChar == '\n' ){ showChar = 'N';}
				if(showChar == '\t' ){ showChar = 'T';}
				if(showChar == ' ') { showChar = 'S';}

				std::cout << showChar << " | Code = " << soFar << " | weight = " << bitSize << '\n';
			}else{
					bitSize += getLeft()->encodeMe(binAssociation, soFar + "0");
					bitSize += getRight()->encodeMe(binAssociation, soFar + "1");
			}
			return bitSize;
		}



		string decompressMe(ifstream &compressedFile, int &curBit, unsigned int &byte, int &size){
			string myString;
			if( isLeaf() ){
				myString += getChar();
				return myString;
			}

			if(curBit == 8){
				curBit = 0;
				byte = compressedFile.get();
				if(byte == -1){// || 0 == byte){
					return "";
				}
			}


			// std::cout << "Val = " << (( byte << curBit) & 0x0080) << '\n';
			if ( 0 == ( ( byte << curBit) & 0x0080) ){
				//go left
				curBit += 1;
				//std::cout << "Go left" << '\n';
				myString += getLeft()->decompressMe(compressedFile, curBit, byte, size);
				
			}else{
				//go right
				curBit += 1;
				//std::cout << "Go right" << '\n';
				myString += getRight()->decompressMe(compressedFile, curBit, byte, size);

			}//else
			return myString;
		}//decompressstd::cout << "*Shift = " << *shiftP << " | *Pos = " << *posP << '\n';
	


		
		





		bool operator>(Node& N2) // Returns true if t1 is earlier than t2
    	{
	    	if ( getCount() > N2.getCount() ) {
	    		return true;
	    	}
	        
	       	return false;
    	}

    	bool operator<(Node& N2) // Returns true if t1 is earlier than t2
    	{
	    	if ( getCount() < N2.getCount() ) {
	    		return true;
	    	}
	        
	       	return false;
    	}




};


class CompareNode {
    public:
    bool operator()(Node* N1, Node* N2) // Returns true if t1 is earlier than t2
    {
    	if ( N1->getCount() > N2->getCount() ) {
    		return true;
    	}
        
       	return false;
    }
};


void populateThePQ( priority_queue<Node*, vector<Node*>, CompareNode> &minPQ, vector<unsigned long long int> charFreq)
{
	for (int ii = 0; ii < 255; ++ii)
	{
		//create a node for the character if it's count was more than 0.
		if(charFreq[ii] > 0)
		{
			Node* n = new Node();
			n->Create(charFreq[ii], (char)ii);
			minPQ.push(n);
		}
	}
}




