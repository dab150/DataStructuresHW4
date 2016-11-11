#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct codeNode
{
	char character;
	string code;

	codeNode(char character, string code)
	{
		this->character = character;
		this->code = code;
	}
};

//function prototypes
void readCodeFile(string file);
void printEncodedMessageFile(string messageFile, string encodedFile);

//data structures
vector<codeNode> codes;

int main(int argc, char *argv[])
{
	//read in the arguments from the command line
	//arg1 = code file
	//arg2 = message file
	//arg3 = encoded message file
	if (argc != 4)
		cout << "Incorrect Number of Arguments!\n";

	string codeFileTxt(argv[1]);
	string messageFileTxt(argv[2]);
	string encodedMessageFileTxt(argv[3]);

	readCodeFile(codeFileTxt);
	cout << "Code File Read\n";

	printEncodedMessageFile(messageFileTxt, encodedMessageFileTxt);
	cout << "Encoded Message Created\n";

	cin.get();
}

void readCodeFile(string file)
{
	//read in the codes
	ifstream codeFile(file);
	if (!codeFile)
		cout << "Error Opening Code File! \n";

	char character;
	string code;

	//store the chars and codes in a vector of structs
	for (int i = 0; codeFile >> character && codeFile >> code; i++)
	{
		codeNode currentCode(character, code);
		codes.push_back(currentCode);
	}
}

void printEncodedMessageFile(string msgFile, string encodedFile)
{
	//read in the non-encrypted message
	ifstream messageFile(msgFile);
	if (!messageFile)
		cout << "Error Opening Message File!";

	char messageChar = ' ';

	string encodedMessage = "";

	while (messageFile >> noskipws >> messageChar)
	{
		if (messageChar == ' ')
			messageChar = '-';
		else if (messageChar == '\n')
			messageChar = '!';

		//go through the list of codes until we find a char matching our char
		for (int i = 0; i < codes.size(); i++)
		{
			if (messageChar == codes[i].character)
			{
				encodedMessage += codes[i].code;
			}
		}
	}

	//after reading whole message we can just print out the code for the last item in the code vector because we know it is always last
	encodedMessage += codes[codes.size() - 1].code;

	//now that we have an entire message we can write it to a file
	ofstream outputFile(encodedFile);
	outputFile << encodedMessage;
}
