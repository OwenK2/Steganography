#include "steg.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
using namespace std;

Steganography::Steganography(const char* _i, const char* _o, const char* _d) : inputFile(_i), outputFile(_o), dataFile(_d) {}
Steganography::Steganography(const char* _i) : inputFile(_i) {}
Steganography::~Steganography() {};

void Steganography::encodeFile() {
	ifstream dataInput(dataFile, ios::binary);
	if(dataInput.is_open()) {
		int w, h, channels;
		string ext = getExt(outputFile);
		string inputExt = getExt(inputFile);
		int force = 0;
		if(inputExt != "png" || ext != "png") {force = 3;}
		cout << "\e[94mOpening \e[1m" << inputFile << "\e[0m\e[94m...\e[0m" << endl;
	  unsigned char* imageData = stbi_load(inputFile, &w, &h, &channels, force);
	  if(imageData != NULL) {
	  	cout << "\e[32m  \e[1m✓\e[0m\e[32m Image Opened\e[0m" << endl;
			uint32_t dataSize = getSize(&dataInput) + 260;
			if(dataSize*8 > w*h*channels*bits) {
				cerr << "\e[31m" << dataFile << " (" << (dataSize*8) << " bits) is too big for " << inputFile << " (" << (w*h*channels*bits) << " available bits)\nPlease use a larger input file, a smaller data file, or use a larger bit depth (ex. -b 2)\e[0m" << endl;
				return;
			}
			else {
				cout << "\e[94mEncoding \e[1m" << dataSize << " bytes\e[0m\e[94m of data from \e[1m" << dataFile << "\e[0m\e[94m...\e[0m" << endl;
				char* data = new char[dataSize-1];
				memcpy(data,dataFile,strlen(dataFile));
				memcpy(&data[255],&dataSize,4);
				dataInput.read(&data[259], dataSize-259);
				dataInput.close();
				for(int i = 0;i < 8;++i) {
					if(bits >> i & 0x1) {
						imageData[i] |= 0x1;
					}
					else {
						imageData[i] &= 0xA98AC6;
					}
				}
				for(int i = 8;i < dataSize*8;++i) {
					int j = i - 8;
					if(data[j/8] >> j%8 & 0x1) {
						imageData[i/bits+7] |= 0x1 << (i%bits);
					}
					else {
						imageData[i/bits+7] &= ~(0x1 << (i%bits));
					}
				}
				cout << "\e[32m  \e[1m✓\e[0m\e[32m Data Encoded\e[0m" << endl;
				cout << "\e[94mWriting \e[1m" << outputFile << "\e[0m\e[94m...\e[0m" << endl;
			  if(ext == "bmp") {
			  	stbi_write_bmp(outputFile, w, h, channels, imageData);
			  }
			  else {
			  	string name = getName(outputFile) + string(".png");
			  	if(ext != "png") {cout << "\e[2m\e[31m  \e[1m⚠\e[0m\e[31m\e[2m Converting \e[1m" << outputFile << "\e[0m\e[2m\e[31m to \e[1m" << name << "\e[0m\e[2m\e[31m to protect data from lossy compression\e[0m" << endl;}
			  	stbi_write_png(name.c_str(), w, h, channels, imageData, 0);
			  }
			  stbi_image_free(imageData);
			 	delete[] data;
			 	cout << "\e[32m  \e[1m✓\e[0m\e[32m Data Written\e[0m" << endl;
			}
	  }
	  else {
			cerr << "\e[31m" << inputFile << " could not be opened\e[0m" << endl;
			return;
	  }
	}
	else {
		cerr << "\e[31m" << dataFile << " could not be opened\e[0m" << endl;
		return;
	}
}

void Steganography::encodeMessage() {
	int w, h, channels;
	const char* message = dataFile;
	dataFile = "message.txt";
	string ext = getExt(outputFile);
	string inputExt = getExt(inputFile);
	int force = 0;
	if(inputExt != "png" || ext != "png") {force = 3;}
	cout << "\e[94mOpening \e[1m" << inputFile << "\e[0m\e[94m...\e[0m" << endl;
  unsigned char* imageData = stbi_load(inputFile, &w, &h, &channels, force);
  if(imageData != NULL) {
  	cout << "\e[32m  \e[1m✓\e[0m\e[32m Image Opened\e[0m" << endl;
		uint32_t dataSize = strlen(message) + 260;
		if(dataSize*8 > w*h*channels*bits) {
			cerr << "\e[31mYour message (" << (dataSize*8) << " bits) is too big for " << inputFile << " (" << (w*h*channels*bits) << " available bits)\nPlease use a larger input file, a smaller data file, or use a larger bit depth (ex. -b 2)\e[0m" << endl;
			return;
		}
		else {
			cout << "\e[94mEncoding \e[1m" << dataSize << " bytes\e[0m\e[94m of data...\e[0m" << endl;
			char* data = new char[dataSize-1];
			memcpy(data,dataFile,strlen(dataFile));
			memcpy(&data[255],&dataSize,4);
			memcpy(&data[259],message,dataSize-260);
			for(int i = 0;i < 8;++i) {
				if(bits >> i & 0x1) {
					imageData[i] |= 0x1;
				}
				else {
					imageData[i] &= 0xA98AC6;
				}
			}
			for(int i = 8;i < dataSize*8;++i) {
				int j = i - 8;
				if(data[j/8] >> j%8 & 0x1) {
					imageData[i/bits+7] |= 0x1 << (i%bits);
				}
				else {
					imageData[i/bits+7] &= ~(0x1 << (i%bits));
				}
			}
			cout << "\e[32m  \e[1m✓\e[0m\e[32m Data Encoded\e[0m" << endl;
			cout << "\e[94mWriting \e[1m" << outputFile << "\e[0m\e[94m...\e[0m" << endl;
		  if(ext == "bmp") {
		  	stbi_write_bmp(outputFile, w, h, channels, imageData);
		  }
		  else {
		  	string name = getName(outputFile) + string(".png");
		  	if(ext != "png") {cout << "\e[2m\e[31m  \e[1m⚠\e[0m\e[31m\e[2m Converting \e[1m" << outputFile << "\e[0m\e[2m\e[31m to \e[1m" << name << "\e[0m\e[2m\e[31m to protect data from lossy compression\e[0m" << endl;}
		  	stbi_write_png(name.c_str(), w, h, channels, imageData, 0);
		  }
		  stbi_image_free(imageData);
		 	delete[] data;
		 	cout << "\e[32m  \e[1m✓\e[0m\e[32m Data Written\e[0m" << endl;
		}
  }
  else {
		cerr << "\e[31m" << inputFile << " could not be opened\e[0m" << endl;
		return;
  }
}

void Steganography::decode() {
	int w, h, channels;
	cout << "\e[94mOpening \e[1m" << inputFile << "\e[0m\e[94m...\e[0m" << endl;
  unsigned char* imageData = stbi_load(inputFile, &w, &h, &channels, 0);
  if(imageData != NULL) {
  	cout << "\e[32m  \e[1m✓\e[0m\e[32m Image Opened\e[0m" << endl;
  	cout << "\e[94mDecoding Data...\e[0m" << endl;
  	char* header = new char[259];
  	for(int i = 0;i < 8;++i) {
  		if(imageData[i] & 0x1) {
  			bits |= 0x1 << i;
  		}
  		else {
  			bits &= ~(0x1 << i);
  		}
  	}
	  for(int i = 8;i < 260*8;++i) {
	  	int j = i - 8;
  		if((imageData[i/bits+7] >> i%bits) & 0x1) {
  			header[j/8] |= 0x1 << (j%8);
  		}
  		else {
  			header[j/8] &= ~(0x1 << (j%8));
  		}
  	}
  	char file[255];
  	uint32_t size = 0;
  	memcpy(file,header,255);
  	memcpy(&size,&header[255],4);
		if(size == 0) {
			cerr << "\e[1m\e[95mNo File Found In Photo" << endl;
			return;
		}
		else {
			char* data = new char[size - 260];
			cout << "\e[32m  \e[1m✓\e[0m\e[32m Found \e[1m" << file << "\e[0m\e[32m (\e[1m" << size << " bytes\e[0m\e[32m) with a bit depth of \e[1m" << bits << "\e[0m" << endl;
	  	for(int i = 260*8;i < size*8;++i) {
	  		int j = i - 260*8;
	  		if((imageData[i/bits+7] >> i%bits) & 0x1) {
	  			data[j/8] |= 0x1 << (j%8);
	  		}
	  		else {
	  			data[j/8] &= ~(0x1 << (j%8));
	  		}
	  	}
	  	string name = uniqueName(file);
		  cout << "\e[32m  \e[1m✓\e[0m\e[32m Decoded \e[1m" << size << " bytes\e[0m\e[32m into \e[1m" << name << "\e[0m" << endl;
		  if(getExt(name) == "txt" && !makeFile) {
		  	cout << "\n\e[32m\e[1mMessage Decoded:\e[0m\n" << endl;
		  	char msg[size-260];
		  	strncpy(msg,data,size-260);
		  	cout << msg << endl << endl;
		  }
		  else {
		  	cout << "\e[94mWriting \e[1m" << name << "\e[0m\e[94m...\e[0m" << endl;
				ofstream output(name, ios::out|ios::binary);
				if(output.is_open()) {
			  	output.write(data,size-260);
			  	output.close();
			  	cout << "\e[32m  \e[1m✓\e[0m\e[32m Output File Written\e[0m" << endl;
				}
				else {
					cerr << "\e[31mUnable to create output file\e[0m" << endl;
				}
		  }
	  	delete[] data;
		}
		delete[] header;
  }
  else {
		cerr << "\e[31m" << inputFile << " could not be opened\e[0m" << endl;
		return;
  }
}

uint32_t Steganography::getSize(ifstream* stream) {
	streampos fsize = 0;
	fsize = stream->tellg();
	stream->seekg(0, ios::end);
	fsize = stream->tellg() - fsize;
	stream->seekg(0, ios::beg);
	return (uint32_t)fsize;
}
string Steganography::getExt(string s) {
  size_t i = s.rfind('.', s.length());
  if(i != string::npos) {
    return(s.substr(i+1, s.length() - i));
  }
  return("");
}
string Steganography::getName(string s) {
	size_t dotPos = s.rfind('.');
	return s.substr(0,dotPos);
}
string Steganography::uniqueName(char* f) {
	bool taken = true;
	int i = 1;
	string file = string(f);
	string name = getName(string(file));
	string ext = getExt(string(file));
	while(taken) {
		ifstream test(file);
		if(test.is_open()) {
			file = name + string("(") + to_string(i) + string(").") + ext;
			i++;
		}
		else {
			taken = false;
		}
	}
	return file;
}