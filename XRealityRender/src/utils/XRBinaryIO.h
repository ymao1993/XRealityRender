#ifndef XRBINARYIO_H
#define XRBINARYIO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

namespace XRBinaryIO
{
	void readBinary(const string& filename, vector<char>& buffer);
	void writeBinary(const string& filename, vector<char>& buffer);
}

#endif