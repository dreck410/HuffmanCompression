HuffmanCompression
==================

A compression technique that will compress a file down and also unzip the file


to compress a file 

compile the compression program with:

	g++ -std=c++11 -o compress compress.cpp

And then run:

	./compress myData.dat

This will create a file named myData.dat.bin and a compressKey.dat file used to decompress the file

To decompress the file compile the decompress code

	g++ -std=c++11 -o decompress decompress.cpp

and run

	./decompress myData.dat.bin