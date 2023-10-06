#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <queue>
#include "Node.h"
using namespace std;
const static string SPACE = "space";
const static string NEWLINE = "newline";
const static string TAB = "tab";
const static string RETURN = "return";
const static string ASTERISK = "*****";
int condition = 0;

int main(int argc, char** argv)
{
    /*
            FILE INPUT CHECKs
    
    */
    //If there is no command line parameter
    if (argc == 1) {
        cout << "You forgot the command line parameter" << endl;
        exit(0);
    }
    //Check for file
    ifstream file;
    file.open(argv[1], ios::binary);
    if (!file) {
        cout << "could not find file " << argv[1] << endl;
        exit(0);
    }

    //Check for file extension
    string filename = argv[1];
    string ext = filename.substr(filename.find_last_of(".") + 1);
    if (ext != "zip301") {
        cout << "Invalid file type. Only .zip301 files are allowed." << endl;
        exit(0);
    }

    /*
            PROGRAM START

    */
    double totalTimeTaken = 0;
    clock_t tStart = clock();
    cout << "Reading file header ... ";

    

    vector<Node*> huffTree;
    huffTree.push_back(new Node(""));
    Node* root = huffTree[0];
    ///// READ THE ENCODINGS, Reconstructing Huffman Tree //////

    string p;
    string l;
    l.reserve(16);
    p.reserve(8);
    Node* x = root;
    char label;

    /*
         Generating Huffman Tree

    */

    while (true)
    {
        file >> l;      // read the encoding
        file >> p;      // read the corresponding character

        if (l != ASTERISK)       //if it's not the asterisks continue
        {

            x = root;
            //Check define label
            if (p == SPACE) label = ' ';
            else if (p == NEWLINE) label = '\n';
            else if (p == RETURN) label = '\r';
            else if (p == TAB) label = '\t';
            else label = p[0];

            //Add node to tree following its path
            for (auto& ch : l) {
                if (ch == '0') {
                    if (x->leftC == nullptr) {
                        x->leftC = new Node("");
                    }
                    x = x->leftC;
                }
                else {
                    if (x->rightC == nullptr) {
                        x->rightC = new Node("");
                    }
                    x = x->rightC;
                }
            }
            x->label = label;
            x->labeled = true;
        }
        else break;                 // break when you reach the asteris           
    }
    double duration = (double)(clock() - tStart) / CLOCKS_PER_SEC;
    totalTimeTaken += duration;
    cout << (" %.3fs\n", duration) << "s" << endl;


    /*
            Start decrypting each character
    
    */

    cout << "Reading file body ... ";
    tStart = clock();
    //print all nodes
    string line;
    string temp;
    int n;
    n = stoi(p);    // P already read the number of bits so convert it into an int


    string binstr = "";  //variable to store the binary string

    getline(file, temp); //skip a blank space

    //int numB = (n + (n%8) ) / 8;
    int numB = n / 8;
    if (numB % 8 != 0) {
        numB++;
        condition = (8 - (n % 8));
    }
    char c;
    binstr.reserve(32);


    /*
    
            Setting UP OUTPUT SETTING
    
    */

    //.zip301 to 2.txt format
    string path = argv[1];
    path = path.substr(0, path.length() - 7);
    path += "2.txt";
    ofstream ofs(path, ios::out | ios::binary);

    string output;
    output.reserve(2 * numB);

    Node* result = root;
    int endloop = 0;


    duration = (double)(clock() - tStart) / CLOCKS_PER_SEC;
    totalTimeTaken += duration;
    cout << (" %.3fs\n", duration) << "s" << endl;


    tStart = clock();

    /*
        ACTUAL DECRYPTING
    
    */
    cout << "Decrypting file ... "; 

    //read all the bytes
    string byteStr(numB, '\0');
    file.read(&byteStr[0], numB);

    //LOOP through each byte
    for (int i = 0; i < numB; i++)
    {
        //Adjusting the last byte, remove extra bits generated
        if (i >= numB - 1) {
            endloop = condition;
        }
        c = byteStr[i];  //get byte

        //Loop through the byte
        for (int j = 7; j >= endloop; j--) {
            
            if ((c >> j) & 1) {  //TRAVELLING down the tree bit by bit 1 or 0
                result = result->rightC;
                if (result->labeled) {    //when reach a node labeled stop, go back to root
                    output += result->label;
                    result = root;
                }

            }
            else {
                result = result->leftC;
                if (result->labeled) {
                    output += result->label;
                    result = root;
                }
            }

        }

    }
    duration = (double)(clock() - tStart) / CLOCKS_PER_SEC;
    totalTimeTaken += duration;
    cout << (" %.3fs\n", duration) << "s" << endl;


    // outputing
    tStart = clock();
    ofs << output;


    duration = (double)(clock() - tStart) / CLOCKS_PER_SEC;
    totalTimeTaken += duration;

    cout << "Wrote output to: " << path << endl;

    cout << "TOTAL TIME: ";

    cout << (" %.3fs\n", totalTimeTaken) << "s" << endl;

    ofs.close();

   
}

