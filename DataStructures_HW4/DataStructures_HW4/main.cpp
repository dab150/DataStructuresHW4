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
struct node
{
	char character;
	int frequency;
	node * left;
	node * right;

	node(char character, int frequency)
	{
		this->character = character;
		this->frequency = frequency;
		left = right = NULL;
	}
};

struct compare
{
	bool operator()(node * left, node* right)
	{
		return (left->frequency > right->frequency);
	}
};

void readFile();
void buildTree();
void getCodes(node * root, string code);

vector<node> frequencies;

int main()
{
	readFile();
	buildTree();

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
		node input (character, freq);
		frequencies.push_back(input);
	}

	//sort the frequencies from low to high


	//print out the frequencies
	for (int i = 0; i < frequencies.size(); i++)
		cout << frequencies[i].character << " " << frequencies[i].frequency << "\n";
}

void buildTree()
{
	node *left, *right, *sumNode;

	//create a min heap and insert characters from file we read in previously
	priority_queue<node*, vector<node*>, compare> minHeap;

	for (int i = 0; i < frequencies.size(); i++)
	{	
		minHeap.push(new node(frequencies[i].character, frequencies[i].frequency));
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
		sumNode = new node('+', left->frequency + right->frequency);
		sumNode->left = left;
		sumNode->right = right;
		//now add it back to the heap
		minHeap.push(sumNode);
	}

	getCodes(minHeap.top(), "");
}

void getCodes(node * root, string code)
{
	if (!root)
		return;
	if (root->character != '+')
	{
		cout << root->character << ": " << code << "\n";
	}

	//print to the left and add 0 to the code
	getCodes(root->left, code + "0");
	//print to the right and add 1 to the code
	getCodes(root->right, code + "1");
}

