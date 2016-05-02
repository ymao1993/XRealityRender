#include "XRBinaryIO.h"

using namespace std;

namespace XRBinaryIO
{
	void readBinary(const string& filename, vector<char>& buffer)
	{
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file.good()) return;
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		buffer.resize(size);
		file.read(&buffer[0], size);
		file.close();
	}

	void writeBinary(const string& filename, vector<char>& buffer)
	{
		std::ofstream file(filename, std::ios::out | std::ios::binary);
		file.write(&buffer[0], buffer.size());
		file.close();
	}
}