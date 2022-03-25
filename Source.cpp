#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <unordered_map>
#include <queue> 

using namespace std;

struct Node {
	//This is a node that we will use to create huffman tree. Contains char and its frequency.
	char ch;  
	int freq;
	//Since its a tree we need left and right branches.
	Node *left, *right;

};

Node* newNode(char ch, int freq, Node* left, Node* right) {
	// This is for creating a child for the tree.
	Node* node = new Node(); // Creates a new node.
	// To connect these nodes together.
	node->left = left; 
	node->right = right;

	node->ch = ch;
	node->freq = freq;
	// Returns the new node.
	return node;
}

void print_input() {

	ifstream myfile("input.txt"); // Opens the file and reads it.
	string line;
	while (getline(myfile, line)) { // Takes a line from the input file. Until there is another line loop can take.
		cout << line << "\n"; // Prints the line.
	}
	myfile.close(); // Closes the file so there is no loss of memory.
}

int binaryConvert() {

	int bitNumber = 0; // Bit number for how many bits it contains at the start.
	ifstream myfile("input.txt");
	string line;
	cout << "--Before Huffman Coding--\n";
	while (getline(myfile, line)) { // Gets the line.

		for (int i = 0; i < line.size(); i++) { // Repeats for every char

			cout << bitset<8>(line[i]) << ' '; bitNumber += 8; // Takes fixed 8 bits from every char in string. It adds 8 bits for every char to bitNumber.
		}
	}

	myfile.close();

	return bitNumber; // Returns first bit size.
}

unordered_map<char, int> frequency() {

	unordered_map<char, int> frequency; // Create a unordered map that contains char and int.
	ifstream myfile("input.txt");
	string line, data;
	while (getline(myfile, line)) {

		data += line; // Copies lines to a single string named data.
	}
	for (char ch : data) { // For every char that matches a char in data

		frequency[ch]++ ; // It increases the value of int(frequency) for that char.
	}
	cout << "Frequency\n";

	for (auto frq : frequency) { // It iterates and get the values that is contained in the unorderedmap

		cout << "'" << frq.first << "' = " << frq.second << "\n"; // Prints the values.
	}

	return frequency; // Returns unorderedmap for huffman tree.
}
// To constrate the queue we compare the Frequencys
struct comp {
	
	bool operator()(Node* l, Node* r) {
		// Takes lowest freq to the top of the queue
		return l->freq > r->freq;
	}
};

void huffmancodes(struct Node* root, string code, unordered_map<char, string> &huffmanCode) {
	// Checks left first.
	if (root->left) 
		huffmancodes(root->left, code + "0", huffmanCode);
	// takes the left node then the function sends left node's location to function again and adds 0 to string since it is the path that it goes.
	if (root->right)
		huffmancodes(root->right, code + "1", huffmanCode);
	// takes the right node then the function sends right node's location to function again and adds 1 to string since it is the path that it goes.
	if (root->left == '\0'&&root->right == '\0') { // if there no other node left that means we found a character.
		huffmanCode[root->ch] = code; // takes character and pairs it with it is code into unorderedmap.
	}
}

unordered_map<char, string> huffmantree(unordered_map<char, int> &frequency) {
	unordered_map<char, string> huffmanCode;
	int sum;
	string code;

	priority_queue<Node*, vector<Node*>, comp> pq; // Creates the queue pq.

	for (auto frq : frequency) { // uses auto to iterate and get the pairs.
		pq.push(newNode(frq.first, frq.second, nullptr, nullptr)); // pushes char and freq to the queue and their left and right ptr.
	}

	while (pq.size() != 1) { // until there is only one member left in the queue we assign top members of queue to left first then right nodes.

		Node *left = pq.top();    pq.pop(); // assigns top member of the queue to left then removes it.
		Node *right = pq.top();   pq.pop(); // assigns top member of the queue to right then removes it.

		sum = left->freq + right->freq; // takes sum of right and left members freq.

		pq.push(newNode('\0', sum, left, right)); // pushes the sumation as freq takes char as null then pushes it into queue.
	}

	Node* root = pq.top(); // takes last member of queue to root.
	huffmancodes(root, code, huffmanCode); // sends information to huffmancodes to find direction for char.

	return huffmanCode; // returns unorderedmap that contains chars and their codes.
}

int HuffmanCode(unordered_map<char, string> &HuffmanCodes) {

	int bits;
	unordered_map<char, int>frequency;
	// takes input to data as one string.
	ifstream myfile("input.txt");
	string line, data;
	while (getline(myfile, line)) {

		data += line; 
	}

	cout << "\n";

	string huffman = "";

	for (char ch : data) { // checks matching characters in data

		huffman += HuffmanCodes[ch]; // puts the matching code into the string.
	}

	cout << huffman; // after the loop it prints the string that is encoded.
	bits = huffman.length(); // takes lenght of the string for calculating how much bit space we saved.

	return bits; // returns bit size.
}

int main() {

	int firstBits, secondBits; // for calculating saved data.
	cout << "Initial text: ";
	print_input(); // calls print of input text 
	cout << "\n";

	unordered_map<char, int>temp = frequency(); // gets frequency of unique characters.
	cout << "\n";

	firstBits = binaryConvert(); // converts the inital input to binary and gets it's bit size.
	cout << "\n\n";

	unordered_map<char, string>temp2 = huffmantree(temp); // creates the huffmantree and returns the huffmancodes.
	cout << "HuffmanCodes\n"; 
	for (auto frq : temp2) { //prints the huffman codes with their pair char.

		cout << "'" << frq.first << "' = " << frq.second << "\n";
	}
	cout << "\n--After Huffman Coding--";

	secondBits = HuffmanCode(temp2); cout << endl << endl << "Savings:" << secondBits * 100 / firstBits << "%" << endl; //creates huffmancode from codes. and prints the saved amount of size.

	return 0;
}