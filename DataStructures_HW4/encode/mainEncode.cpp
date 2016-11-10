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
void printEncodedMessageFile();
void readCodeFile();

//data structures
vector<codeNode> codes;

int main()
{
	readCodeFile();
	printEncodedMessageFile();

	cin.get();
}

void readCodeFile()
{
	//read in the codes
	ifstream codeFile("basic_code.txt");
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

void printEncodedMessageFile()
{
	//read in the non-encrypted message
	ifstream messageFile("basic_message.txt");
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
	ofstream outputFile("encoded.txt");
	outputFile << encodedMessage;
}
