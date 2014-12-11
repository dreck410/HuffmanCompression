//decompress.cpp
#include <cstdio>
#include <vector>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <stdio.h>
#include <fstream>
#include <queue>          // std::priority_queue
#include "huffmanComp.h"

using namespace std;

//instead read it into a queue a byte at a time. Then write the byte. then grab the next byte from file...
std::vector<unsigned char> readFileIntoByteVector(const char *filename , int &size){

	ifstream keyFile (filename, ios::in | ios::binary | ios::ate );
	keyFile.seekg(0, ios::end);
	size = keyFile.tellg();
	keyFile.seekg(0,ios::beg);

	std::vector<unsigned char> fileInfo;

	for (int i = 0; i < size; ++i){
		unsigned char c = keyFile.get();
		fileInfo.push_back(c);
	}
	return fileInfo;
}

bool getCounts(const char *filename, std::vector<unsigned long long int> &counts) {
	int size = 0;
	
	ifstream keyFile (filename, ios::in | ios::binary | ios::ate );
	keyFile.seekg(0, ios::end);
	size = keyFile.tellg();
	keyFile.seekg(0,ios::beg);
	counts.resize(256,0);
	unsigned int negOne = keyFile.get();
	while(keyFile.tellg() != -1){

		char character = keyFile.get();

		unsigned char val = keyFile.get();

		unsigned long long int sum = 0;
		int bytes = 24; 
		while(255 != (unsigned char)val){
			unsigned long long int placeHolder = val;
			sum += placeHolder << 24;
			val = keyFile.get();
			sum = sum >> 8;
			bytes -= 8;
		}
		sum = sum >> bytes;
		char showChar = character;
		if(showChar == '\n' ){ showChar = 'N';}
		if(showChar == '\t' ){ showChar = 'T';}
		if(showChar == ' ') { showChar = 'S';}
		std::cout << showChar << " appears " << sum << "\n";
		counts.at(character) = sum;
	}
	return true;
}

int main(int argc, char const *argv[])
{
	

	vector<unsigned long long int> charFreq;
	if ( (getCounts("compressKey.huf", charFreq)) ) {
		// next stepcharFreq
		//std::vector<Node> vNodes;
		priority_queue<Node*, vector<Node*>, CompareNode> minPQ;

		populateThePQ(minPQ, charFreq);

		Node* root;
		int treeSize = minPQ.size();

		while(minPQ.size() > 1){
			Node *newNode = new Node();
			Node *n1 = new Node();
			Node *n2 = new Node();
			n1 = minPQ.top();
			minPQ.pop();

			n2 = minPQ.top();
			minPQ.pop();
			
			//the smaller node goes on the right.
			newNode->Create((n1->getCount() + n2->getCount()), 0, n2, n1);
			treeSize += 1;
			minPQ.push(newNode);
		
		}

		root = minPQ.top(); 

		int size = 0;
		ifstream compressedFile ("compressed.bin", ios::in | ios::binary | ios::ate );
		compressedFile.seekg(0, ios::end);
		int sizeOfCompressedFile = compressedFile.tellg();
		compressedFile.seekg(0,ios::beg);

		//gets the buff size
		int buffInfo = compressedFile.get();
		int* buffP = &buffInfo;

		ofstream reCreatedFile;
		reCreatedFile.open("decompressed.dat");

		string foundCharacter = "";
		unsigned int byte = compressedFile.get();

		while (compressedFile.tellg() != -1)
		{

			foundCharacter = root->decompressMe(compressedFile, buffInfo, byte, sizeOfCompressedFile);


			reCreatedFile << foundCharacter;

			if (*buffP >= 8)
			{
				*buffP = 0;
				byte = compressedFile.get();
			}//if

		}
		return 0;
	}
}
