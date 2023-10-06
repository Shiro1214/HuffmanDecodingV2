#include "Node.h"
#include <string>
using namespace std;
Node::Node(string label) {
	this->label = NULL;
	leftC = nullptr;
	rightC = nullptr;
	labeled = false;
}
Node::Node(char label) {
	this->label = label;
	leftC = nullptr;
	rightC = nullptr;
	labeled = true;
}