#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct node
{
	char character;
	node * left;
	node * right;
};

node* newNode(char character)
{
	node* temp = new node;
	temp->character = character;
	temp->left = temp->right = NULL;
	return (temp);
}

void readPreOrder();
void rebuildTree(node *&root, int &i);
void decode(node *root);
void printDecodedFile();

vector<char> preOrderTree;
vector<char> encodedMessage;
vector<char> decodedMessage;

int main()
{
	readPreOrder();

	//node to act as first node of rebuilt tree
	node *minHeapRoot = NULL;
	int x = -1;
	rebuildTree(minHeapRoot, x);

	decode(minHeapRoot);

	printDecodedFile();

	cin.get();
}

void readPreOrder()
{
	//read in the tree
	ifstream treeFile("basic_tree.txt");
	if (!treeFile)
		cout << "Error Opening Tree File!";

	char tempChar;

	for (int i = 0; treeFile >> tempChar; i++)
	{
		preOrderTree.push_back(tempChar);
	}
}

void rebuildTree(node *&root, int &i)
{
	i++;
	// Read next item from file. If there are no more items or next item is a '/', then return
	char character = preOrderTree[i];
	if (i >= preOrderTree.size() || character == '/')
		return;

	// Else create node with this item and recur for children
	root = newNode(character);	
	rebuildTree(root->left, i);
	rebuildTree(root->right, i);
}

void decode(node *root)
{
	//read in the encrypted message
	ifstream messageFile("basic_encoded.txt");
	if (!messageFile)
		cout << "Error Opening Encoded File!";

	node *currentNode = newNode('x');
	currentNode = root;
	char messageInt = ' ';

	for (int i = 0; messageFile >> messageInt; i++)
		encodedMessage.push_back(messageInt);


	//go through the huffman tree
	for (int i = 0; i < encodedMessage.size(); i++)
	{
		//if we see a 1, go right through the huffman tree
		if (encodedMessage[i] == '1')
		{
			currentNode = currentNode->right;
			if (currentNode->character != '$')
			{
				decodedMessage.push_back(currentNode->character);
				currentNode = root;
			}
		}
		//else if we see a 0, go to the left
		if (encodedMessage[i] == '0')
		{
			currentNode = currentNode->left;
			if (currentNode->character != '$')
			{
				decodedMessage.push_back(currentNode->character);
				currentNode = root;
			}
		}
	}
}

void printDecodedFile()
{
	//print to file
	ofstream outputFile("decoded.txt");

	for (int i = 0; i < decodedMessage.size(); i++)
	{
		char output = decodedMessage[i];

		//handle special chars
		switch (output)
		{
		case '!':
			output = '\n';
			break;
		case '-':
			output = ' ';
			break;
		case '+':
			break;
		}

		outputFile << output;
	}

	cout << "Message Decoded! \n";
}
