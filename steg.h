#include <iostream>
#include <fstream>
using namespace std;

class Steganography {
public:
	const char* inputFile;
	const char* outputFile;
	const char* dataFile;
	bool message = false;
	bool makeFile = false;
	uint32_t bits = 1;
	Steganography(const char* _i, const char* _o, const char* _d);
	Steganography(const char* _i);
	~Steganography();
	void encodeFile();
	void encodeMessage();
	void decode();
	static uint32_t getSize(ifstream* stream);
	static string getExt(string s);
	static string getName(string s);
	static string uniqueName(char* file);
	static char* toAscii(char* s);
};