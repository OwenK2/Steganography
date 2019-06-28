#include "steg.h"
using namespace std;

uint32_t toNum(string n) {
	if(n == "1") {return 1;}
	else if(n == "2") {return 2;}
	else if(n == "3") {return 3;}
	else if(n == "4") {return 4;}
	else if(n == "5") {return 5;}
	else if(n == "6") {return 6;}
	else if(n == "7") {return 7;}
	else if(n == "8") {return 8;}
	else {return 9;}
}

int main(int argc, char* argv[]) {
	const char* inputName;
	const char* outputName;
	const char* dataName;
	uint32_t bits = 1;
	bool msg = false;
	if(argc > 5 && argc < 10) {
		for(int i = 0;i < argc;++i) {
			string arg(argv[i]);
			if(arg == "-o") {
				outputName = argv[++i];
			}
			else if(arg == "-i") {
				inputName = argv[++i];
			}
			else if(arg == "-s") {
				msg = true;
			}
			else if(arg == "-b") {
				arg = string(argv[++i]);
				bits = toNum(arg);
				if(bits > 8) {
					cerr << "\e[31mYou can only set bits 1-8 (ex. -b 2)\e[0m" << endl;
					return 1;
				}
			}
		}
		dataName = argv[argc-1];
		if(strlen(dataName) > 255) {
			cerr << "\e[31m" << dataName << " is too long, must be 255 chars or less\e[0m" << endl;
			return 1;
		}
		Steganography steg(inputName,outputName,dataName);
		steg.bits = bits;
		if(msg) {
			steg.encodeMessage();
		}
		else {
			steg.encodeFile();
		}
	}
	else if(argc > 1 && argc < 6) {
		inputName = argv[argc-1];
		Steganography steg(inputName);
		for(int i = 0;i < argc;++i) {
			string arg(argv[i]);
			if(arg == (char*)"-f") {
				steg.makeFile = true;
			}
		}
		steg.bits = bits;
		steg.decode();
	}
	else {
		cerr << "\e[31m\e[1mUSAGE\e[0m\e[31m\n\tENCODING: " << argv[0] << " -i <INPUT FILE> -o <OUTPUT FILE> [-s|-b] <DATA/FILENAME>\nOR\nDECODING\t" << argv[0] << " [-f] <INPUT FILE>\e[0m" << endl;
		cerr << "\e[31m\e[1mFLAGS\e[0m\n\e[31m\t-s\t Send string as a text file instead of passing a file to the encoder" << endl;
		cerr << "\e[31m\t-b\t Set bit depth (1-8)" << endl;
		cerr << "\e[31m\t-f\t Always create file after decode" << endl;
		return 0;
	}


	return 0;
}