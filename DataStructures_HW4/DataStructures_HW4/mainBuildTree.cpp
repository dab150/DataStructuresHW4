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

//structures
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

struct nodeCompare
{
	bool operator()(freqNode *left, freqNode *right) 
	{
		if (left == NULL)
			return false;
		if (right == NULL)
			return true;

		if (left->frequency == right->frequency)
		{
			if (left->character < 'a' && left->character != '$')
			{
				if (right->character < 'a' && right->character != '$')
					return left->character > right->character;
				return true;
			}
			if (right->character < 'a' && right->character != '$')
				return false;
			return left->character > right->character;
		}
		return left->frequency > right->frequency;
	}
};

//data structures
vector<freqNode> frequencies;
priority_queue<freqNode*, vector<freqNode*>, nodeCompare> minHeap;

//function prototypes
void readFile(string file);
void buildTree();
void printTreeFile(freqNode *root, ofstream &file);
void printCodesFile(freqNode * root, string code, ofstream &file);


int main(int argc, char *argv[])
{
	//read in the arguments from the command line
	//arg1 = frequency file
	//arg2 = tree file
	//arg3 = code file
	if (argc != 4)
		cout << "Incorrect Number of Arguments!\n";

	string freqFileTxt(argv[1]);
	string treeFileTxt(argv[2]);
	string codeFileTxt(argv[3]);

	ofstream treeFile(treeFileTxt);
	ofstream codeFile(codeFileTxt);

	readFile(freqFileTxt);
	cout << "File Read\n";

	buildTree();

	printTreeFile(minHeap.top(), treeFile);
	cout << "Huffman Tree Built\n";

	printCodesFile(minHeap.top(), "", codeFile);
	cout << "Huffmann Codes Assigned\n";

	cin.get();
}

void readFile(string file)
{
	ifstream freqFile(file);
	if (!freqFile)
		cout << "Error Opening Frequency File! \n";

	char character;
	int freq = 0;

	//store the frequencies in a vector of structs
	for (int i = 0; freqFile >> character && freqFile >> freq; i++)
	{
		freqNode input (character, freq);
		frequencies.push_back(input);
	}
}

void buildTree()
{
	freqNode *left, *right, *sumNode;

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
		sumNode = new freqNode('$', left->frequency + right->frequency);
		sumNode->left = left;
		sumNode->right = right;
		//now add it back to the heap
		minHeap.push(sumNode);
	}
}

void printTreeFile(freqNode * root, ofstream &file)
{
	if (root == NULL)
		file << "/";
	else
		file << root->character;

	if (root->left != NULL)
		printTreeFile(root->left, file);
	else
		file << "/";

	if (root->right != NULL)
		printTreeFile(root->right, file);
	else
		file << "/";
}

void printCodesFile(freqNode * root, string code, ofstream &file)
{
	char tempChar;

	if (!root)
		return;

	tempChar = root->character;

	if (root->character != '$')
	{
		file << tempChar << " " << code << "\n";
	}

	//print to the left and add 0 to the code
	printCodesFile(root->left, code + "0", file);
	//print to the right and add 1 to the code
	printCodesFile(root->right, code + "1", file);

}