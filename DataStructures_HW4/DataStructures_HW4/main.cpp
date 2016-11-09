//Data Structures HW4
//Daniel Barber-Cironi
//November 7, 2016
//Huffmann Tree

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

//-------------------buildtree---------------------//
struct freqNode
{
	char character;
	int frequency;
	freqNode * left;
	freqNode * right;

	freqNode(char character, int frequency)
	{
		this->character = character;
		this->frequency = frequency;
		left = right = NULL;
	}
};

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

struct compare
{
	bool operator()(freqNode * left, freqNode* right)
	{
		return (left->frequency > right->frequency);
	}
};

void readFile();
void buildTree();
void getCodes(freqNode * root, string code);
void encode();

vector<freqNode> frequencies;
vector<codeNode> codes;

int main()
{
	readFile();
	//buildTree();
	encode();

	cin.get();
}

void readFile()
{
	ifstream freqFile("basic_freq.txt");
	if (!freqFile)
		cout << "Error Opening Frequency File! \n";

	char character;
	int freq = 0;

	//store the frequencies in a vector of structs
	for (int i = 0; freqFile >> character && freqFile >> freq; i++)
	{
		//i need to handle special cases here because it makes it easier to encode later
		if (character == '-')
			character = ' ';
		else if (character == '!')
			character = '\n';

		freqNode input (character, freq);
		frequencies.push_back(input);
	}

	////print out the frequencies
	//for (int i = 0; i < frequencies.size(); i++)
	//	cout << frequencies[i].character << " " << frequencies[i].frequency << "\n";
}

void buildTree()
{
	freqNode *left, *right, *sumNode;

	//create a min heap and insert characters from file we read in previously
	priority_queue<freqNode*, vector<freqNode*>, compare> minHeap;

	for (int i = 0; i < frequencies.size(); i++)
	{	
		minHeap.push(new freqNode(frequencies[i].character, frequencies[i].frequency));
	}

	//now go through the heap and combine leaves until we have one node left
	while (minHeap.size() > 1)
	{
		left = minHeap.top();
		//get rid of this item from the minHeap once we have assigned it
		minHeap.pop();

		right = minHeap.top();
		//get rid of this item from the minHeap once we have assigned it
		minHeap.pop();

		//now we add these two nodes' values together and create a new with the frequency being the value of their sum
		//also, the two added nodes become the left and right child of this new node
		sumNode = new freqNode('+', left->frequency + right->frequency);
		sumNode->left = left;
		sumNode->right = right;
		//now add it back to the heap
		minHeap.push(sumNode);
	}

	getCodes(minHeap.top(), "");
}

void getCodes(freqNode * root, string code)
{
	if (!root)
		return;
	if (root->character != '+')
	{
		cout << root->character << " " << code << "\n";
	}

	//print to the left and add 0 to the code
	getCodes(root->left, code + "0");
	//print to the right and add 1 to the code
	getCodes(root->right, code + "1");
}

//-----------------------------------encode stuff------------------//

void encode()
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

	for (int i = 0; i < codes.size(); i++)
		cout << codes[i].character << " " << codes[i].code << "\n";
	
	//read in the non-encrypted message
	ifstream messageFile("basic_message.txt");
	if (!messageFile)
		cout << "Error Opening Message File!";
	
	char messageChar = ' ';
	
	string encodedMessage = "";

	while (messageFile >> noskipws >> messageChar)
	{
		//go through the list of codes until we find a char matching our char
		for (int i = 0; i < codes.size(); i++)
		{
			if (messageChar == codes[i].character)
			{
				encodedMessage += codes[i].code + ' '; //space is only for testing
			}
		}	
	}

	//after reading whole message we can just print out the code for the last item in the code vector because we know it is always last
	encodedMessage += codes[codes.size() - 1].code;

	cout << encodedMessage;
}

