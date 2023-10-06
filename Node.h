#pragma once
#include <string>
using namespace std;
class Node
{
public:
	Node* leftC;
	Node* rightC;
	bool labeled;
	char label;
	Node(string label);
	Node(char label);
};

