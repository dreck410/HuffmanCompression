#include <cstdio>
#include <vector>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <stdio.h>
#include <fstream>
#include <queue>          // std::priority_queue
#include "huffmanComp.h"


using namespace std;

// Returns true if successful. False if unsuccessful
bool getCounts(const char *filename, std::vector<unsigned long long int> &counts) {
	FILE * pFile;
	counts.clear();
	pFile=fopen (filename,"r");
	if (pFile==NULL) {
		perror ("Error opening file");
		return false;
	}
	// Resize and reset counts
	counts.resize(256,0);
	char c;
	while (EOF != (c=fgetc(pFile))) {
		if (c >= 0) {
			++counts[c];
		} else {
			puts("A negative values was encountered.");
			return false;
		}
	}
    fclose (pFile);
    return true;
}


std::vector<int> getEncoding(char c, map<char, string> &binAssociation, int &sizeOfCode){
//	unsigned long long int toWrite = 0;
	string bEncoding = binAssociation[c];
	int bEncodingSize = bEncoding.size();
//	char bitArry[bEncodingSize];
	std::vector<int> bitVect;
	bitVect.resize(bEncodingSize);

	for (int jj = 0; bEncodingSize > jj ; ++jj){
	//std::cout << "Bit = " << bEncoding[jj] << '\n';
		if('1' == bEncoding[jj]){
			//add 1
		//	toWrite += 0x01;
			bitVect[jj] = 1;

		}else{
			bitVect[jj] = 0;
		}
	//	toWrite = toWrite << 1;
		++sizeOfCode;
	}
//	toWrite = toWrite >> 1;
	return bitVect;
	//return toWrite;
}


void compress( unsigned long long int binSize, map<char, string> &binAssociation, const char* filename){

		unsigned int buffSize = 0;
		if((binSize % 8 != 0)){
			buffSize = 8 - (binSize % 8);
		}

		unsigned long long int toPrintSize = 1 + ceil(((buffSize + binSize)/8));
		unsigned int toPrint = 0;
	//std::cout << "To Print Size in bytes " << toPrintSize << '\n';
	//std::cout << "Buff Size in bits " << buffSize << '\n';
	//std::cout << "Bin Size in bits " << binSize << '\n';

		//Print buff size to file
		char buffSizeArray[1];
		buffSizeArray[0] = buffSize;
    	ofstream compressFile;
    	compressFile.open ("compressed.bin", ios::out | ios::binary);
		// LittleEndian so it writes it backwards....
		compressFile.write(buffSizeArray, 1);
		compressFile.close();


		ifstream inputFile (filename, ios::in | ios::binary | ios::ate );
		inputFile.seekg(0, ios::end);
		long long int sizeOfFile = inputFile.tellg();
		inputFile.seekg(0,ios::beg);
		//reads letter from file

		std::queue<char> queueOfBinary;
		//padd queue with buffer value
		for (int rr = 0; rr < buffSize; ++rr){
			queueOfBinary.push(0);
		}


		int sizeOfCode = 0;
		std::vector<int> bitVect;

		while(inputFile.tellg() != sizeOfFile){
			//get's a byte worth of code
	//std::cout << "Cur loc " << inputFile.tellg() << '\n';
			unsigned long long int toWrite = 0;
			int bit7, bit6, bit5, bit4,bit3,bit2,bit1,bit0 = 0;
			sizeOfCode = 0;

			char c = inputFile.get();
			//toWrite = getEncoding(c, binAssociation, sizeOfCode);
			bitVect = getEncoding(c, binAssociation, sizeOfCode);
			for(int ss = 0; ss < sizeOfCode; ++ss){
	//std::cout << "\tTo the queue " << bitVect.at(ss)<< '\n';
				queueOfBinary.push(bitVect.at(ss));

				if(queueOfBinary.size() > 7){
					bit7 = queueOfBinary.front();
					queueOfBinary.pop();
					bit6 = queueOfBinary.front();
					queueOfBinary.pop();
					bit5 = queueOfBinary.front();
					queueOfBinary.pop();
					bit4 = queueOfBinary.front();
					queueOfBinary.pop();
					bit3 = queueOfBinary.front();
					queueOfBinary.pop();
					bit2 = queueOfBinary.front();
					queueOfBinary.pop();
					bit1 = queueOfBinary.front();
					queueOfBinary.pop();
					bit0 = queueOfBinary.front();
					queueOfBinary.pop();

					toWrite = bit7*128 + bit6*64 + bit5*32 + bit4*16 +
							  bit3*8   + bit2*4  + bit1*2  + bit0*1;

	//std::cout << "Bits " << bit7 << bit6 << bit5 << bit4 << bit3 << bit2 << bit1 << bit0 << '\n';
	//std::cout << "To Write " << toWrite << '\n';
					compressFile.open ("compressed.bin", ios::out | ios::binary | ios::app);
					unsigned char bits = toWrite;

					compressFile << bits;
					compressFile.close();

				}
			}//for
			//clean up the queue
		}//while
		int finalBits = 0;
		if(queueOfBinary.size() != 0){
			while(queueOfBinary.size() != 0){
				finalBits += queueOfBinary.front();
				queueOfBinary.pop();
				finalBits = finalBits << 1;
			}
			finalBits = finalBits >> 1;
	//std::cout << "Extra " << finalBits << '\n';
			compressFile.open ("compressed.bin", ios::out | ios::binary | ios::app);
			unsigned char bits = finalBits;

			compressFile << bits;
			compressFile.close();
		}

}


int main(int argc, char const *argv[])
{
	/* code */
	vector<unsigned long long int> charFreq;
	if (getCounts(argv[1], charFreq)) {
		// next stepcharFreq

		//std::vector<Node> vNodes;
		priority_queue<Node*, vector<Node*>, CompareNode> minPQ;

		populateThePQ(minPQ, charFreq);
		//store the freq of each char (do a long long int cause DR K will hammer it)
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


		map<char, string> binAssociation;
		//get the first valueg lg l
		root = minPQ.top();
		//clear the file
		ofstream keyFile;
		keyFile.open ("compressKey.huf");
		keyFile.close();

		//Create the compressed file;
		unsigned long long int binSize = root->encodeMe(binAssociation, "");

		const char* file = argv[1];
		compress(binSize, binAssociation, file);
		
		delete root;

	}//if statmet
	return 0;
}//main
