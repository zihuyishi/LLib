// CodeComplete.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include <fstream>
#include <map>
using std::map;
using std::string;
typedef map<string, string> TokenMap;

TokenMap TokenList;

static const string TOKENBEGIN(".TOKEN");
static const string TOKENEND(".END");
static const string SOURCEBEGIN(".SOURCE");
static const string SOURCEEND(".END");
static const string LTOKEN("%"), RTOKEN("%");
static const string LVALUE("'"), RVALUE("'");
static const char	FIXCHAR('\\');

void Init(int argc, char* argv[]);
string ReadSource(const string &filePath);
void ParseToken(const string &source);
string MakeCode(const string &source);
void WriteToFile(const string &source, const string &filePath);

int main(int argc, char* argv[])
{
	Init(argc, argv);
	string source, code;
	source = ReadSource(string(argv[1]));
	ParseToken(source);
	code = MakeCode(source);
	WriteToFile(code, string(argv[2]));
	return 0;
}

void Init(int argc, char* argv[])
{
	if (argc != 3) {
		std::cout << "Usage : CodeComplete [source file] [target file]" << std::endl;
	}
}

string ReadSource(const string &filePath)
{
	using std::ifstream;
	ifstream inFile;
	string outString;
	inFile.open(filePath, ifstream::binary);
	if (inFile) {
		inFile.seekg(0, inFile.end);
		int length = static_cast<int>(inFile.tellg());
		inFile.seekg(0, inFile.beg);
		char *inBuffer = new char[length + 1];
		inFile.read(inBuffer, length);
		inBuffer[length] = 0;
		inFile.close();
		outString = string(inBuffer);
		delete[] inBuffer;
	}
	return outString;
}
void ParseToken(const string &source)
{
	size_t tokenPosition = source.find(TOKENBEGIN);
	size_t tokenEnd = source.find(TOKENEND, tokenPosition);
	do {
		tokenPosition = source.find(LTOKEN, tokenPosition);
		if (tokenPosition == string::npos || tokenEnd <= tokenPosition) break;
		tokenPosition += LTOKEN.length();
		size_t tokenLen = source.find(RTOKEN, tokenPosition) - tokenPosition;
		string token = source.substr(tokenPosition, tokenLen);
		tokenPosition += tokenLen;
		tokenPosition = source.find(LVALUE, tokenPosition);
		tokenPosition += LVALUE.length();
		tokenLen = source.find(RVALUE, tokenPosition) - tokenPosition;
		string value = source.substr(tokenPosition, tokenLen);
		tokenPosition += tokenLen;
		TokenList.insert(std::make_pair(token, value));
	} while (tokenPosition < tokenEnd);
}
string MakeCode(const string &source)
{
	int SourceBegin = source.find(SOURCEBEGIN);
	if (SourceBegin == string::npos) return string();
	int SourceLen = source.find(SOURCEEND, SourceBegin) - SOURCEBEGIN.length() - SourceBegin;
	SourceBegin += SOURCEBEGIN.length();
	string outString = source.substr(SourceBegin, SourceLen);
	int sourcePosition = 0;
	do {
		sourcePosition = outString.find(RTOKEN, sourcePosition);
		if (sourcePosition == string::npos) break;
		if (sourcePosition == 0 || outString[sourcePosition - 1] == FIXCHAR) {
			outString.replace(sourcePosition - 1, 1, "");
			sourcePosition++;
			continue;
		}
		sourcePosition += RTOKEN.length();
		int tokenLen = outString.find(LTOKEN, sourcePosition) - sourcePosition;
		string token = outString.substr(sourcePosition, tokenLen);
		string value = TokenList[token];
		outString.replace(sourcePosition - RTOKEN.length(), 
			tokenLen + LTOKEN.length() + RTOKEN.length(), value);
		sourcePosition += value.length() - RTOKEN.length();
	} while (sourcePosition < SourceLen);
	return outString;
}
void WriteToFile(const string &source, const string &filePath)
{
	using std::ofstream;
	ofstream outFile;
	outFile.open(filePath, ofstream::out);
	outFile.write(source.c_str(), source.length());
	outFile.close();
}