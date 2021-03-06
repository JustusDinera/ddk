//#include <string>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

#define DEBUG
#define DEBUG_INPUT "WHZ_uncompressed.bmp"
//#define DEBUG_INPUT "Faust.txt"
#define DEBUG_OUTPUT "WHZ_uncompressed.bmp.huf"
//#define DEBUG_OUTPUT "Faust.txt.huf"

using namespace std;

// datatype for character counter
struct Tcounter
{
    unsigned char character;
    int count;
};

struct node {
  unsigned char character;
  int count;
  struct node *left = nullptr;
  struct node *right = nullptr;
};


union uPattern {
    uint32_t whole;
    struct bytes {
        unsigned char byte0;
        unsigned char byte1;
        unsigned char byte2;
        unsigned char byte3;
    } bytes;
};

// datatype for encode symbole table 
struct TsymTab
{
    unsigned char character;
    unsigned char patternLen = 0;
    uPattern bitPattern;
};

/* 
    variable declaration
*/
int currentChar;
int lastChar = '\377';

void traverseNodes(node * nodeTranvers, vector<TsymTab> * table){
    static unsigned char childes = 0;
    static int rootNode = 1;

    // increase childes
    if (rootNode)
        rootNode = 0;
    else
        childes++;
    
    if (nodeTranvers->right != nullptr)
    {
        // get char from lower right node
        traverseNodes(nodeTranvers->right, table);

        // delete childe node
        free(nodeTranvers->right);
        nodeTranvers->right = nullptr;
    }
    if (nodeTranvers->left != nullptr)
    {
        // get char from lower left node
        traverseNodes(nodeTranvers->left, table);

        // delete childe node
        free(nodeTranvers->left);
        nodeTranvers->left = nullptr;
    }
    else 
    {
        // return char if the node has no childe
        table[0][nodeTranvers->character].patternLen = childes;
    }

    // decrease childes
    childes--;
    //return retVal;
}

bool sortVectorDec(node * i, node * j){
    return (i->count > j->count);
}

bool sortVectorAsc(Tcounter i, Tcounter j){
    return (i.count < j.count);
}

// sort table entries by length first and char value secound
bool sortTableLenAsc(TsymTab i, TsymTab j){
    bool retVal = false;
    if (i.patternLen == j.patternLen)
        retVal = (i.character < j.character);
    else
        retVal = (i.patternLen < j.patternLen);
    return retVal;
}

bool sortTableCharAsc(TsymTab i, TsymTab j){
    return (i.character < j.character);
}

node * createNode(unsigned char character, int count) {
    struct node *node = (struct node *)malloc(sizeof(struct node));
    node->character = character;
    node->count = count;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

// create the pattern of the characters
void createPattern(vector<TsymTab> &table){
    // pattern variable
    static unsigned int pattern = 0;
    
    // sort table by pattern length
    sort(table.begin(), table.end(), sortTableLenAsc);

    for (int i = 0; i < table.size(); i++)
    {
        if (0 != table[i].patternLen)
        {
            if (i > 0)
            {
                if ((table[i-1].patternLen != 0))
                {
                    pattern++;
                    pattern <<= (table[i].patternLen - table[i-1].patternLen);
                }
            }
            table[i].bitPattern.whole = pattern;
        }
    }
}

void writeFileLen(vector<node *> & charCount, ofstream &outfile)
{
    uint32_t len = 0;
    uPattern fileLen;

    for (int i = 0; i < charCount.size(); i++)
    {
        len += charCount[i]->count;
    }

    fileLen.whole = len;

    outfile.put(fileLen.bytes.byte0);
    outfile.put(fileLen.bytes.byte1);
    outfile.put(fileLen.bytes.byte2);
    outfile.put(fileLen.bytes.byte3);
}

void createFileHead(vector<TsymTab> & codeTable,  ofstream * outputFile){
    for (int i = 0; i < codeTable.size(); i++)
    {
        outputFile->put(codeTable[i].patternLen);
    }
}

void createFileBody(vector<TsymTab> & codeTable, ifstream * inputFile,  ofstream * outputFile){
    uPattern tempPat;
    unsigned char length = 0;
    unsigned char bytesToWrite = 0;
    tempPat.whole = 0;
    int character = inputFile->get();

    // access whole file 
    while (character !=  EOF)
    {
        // increase length by lenght of current pattern
        length += codeTable[character].patternLen;
        // shift the rest of the last pattern by length of current pattern
        tempPat.whole <<= codeTable[character].patternLen;
        // set the current pattern
        tempPat.whole |= codeTable[character].bitPattern.whole;
        // shift the pattern to align bytes to the left
        if (length % 8 != 0)
        {
            tempPat.whole <<= 8 - length % 8;
            bytesToWrite = length + 8 - length % 8;
        }
        else {
            bytesToWrite = length;
        }


        // decide how many bytes filled and put them in the file 
        switch (bytesToWrite)
        {
        case 32:
            outputFile->put(tempPat.bytes.byte3);
            tempPat.bytes.byte3 = 0;
            length -= 8;
        case 24:
            outputFile->put(tempPat.bytes.byte2);
            tempPat.bytes.byte2 = 0;
            length -= 8;
        case 16:
            outputFile->put(tempPat.bytes.byte1);
            tempPat.bytes.byte1 = 0;
            length -= 8;
        case 8:
            // dont put a byte if its not full
            if (length % 8 == 0)
            {
                outputFile->put(tempPat.bytes.byte0);
                tempPat.bytes.byte0 = 0;
                length -= 8;
            }
            break;
        default:
            break;
        }

        // align the remaining bits to the right
        if (length != 0)
        {
            tempPat.whole >>= 8 - length;
        }
        
        character = inputFile->get();
    }
    if (length !=0)
    {
        tempPat.whole <<= 8 - length;
        outputFile->put(tempPat.bytes.byte0);
    }
    
}

void shiftToFile(vector<TsymTab> & codeTable, ifstream  * counter,  ofstream * outputFile){
    sort(codeTable.begin(), codeTable.end(), sortTableCharAsc);
    createFileHead(codeTable, outputFile);
    createFileBody(codeTable, counter, outputFile);
}

int main(int argc, char const *argv[])
{
    ifstream inputFile;
    ofstream outputFile;
    vector<unsigned char> inFile;
    vector<node *> charCount;
    node tempCount;
    unsigned char tempCharacter;
    int size;
    vector<TsymTab> codeTable;
    TsymTab tempTabItem;
    vector<unsigned char> tempStringToCount;
    node * tempNode = nullptr;

    // inital fill charCounter
    for (int i = 0; i < 256; i++)
    {
        tempCharacter = (unsigned char)i;
        // fill counter
        charCount.push_back(createNode(tempCharacter, 0));
        tempTabItem.character = tempCharacter;
        tempTabItem.patternLen = 0;
        tempTabItem.bitPattern.whole = 0;
        codeTable.push_back(tempTabItem);
    }

    
#ifndef DEBUG
    if (argv[1] == nullptr)
    {
        printf("Keine Dateien angegebn\n");
    }
    else if (argv[2] == nullptr)
    {
        printf("Keine Ausgabe Datei angegeben\n");
    }
    else
#endif
    {
#ifndef DEBUG
        inputFile.open((string)(argv[1]), ios::in | ios::binary);
        outputFile.open((string)(argv[2]), ios::out | ios::binary);
#else
        inputFile.open(DEBUG_INPUT, ios::in | ios::binary);
        outputFile.open(DEBUG_OUTPUT, ios::out | ios::binary);
#endif
        // check state of files
        if (!(outputFile.is_open()))
        {
            printf("Ausgabedatei konnte nicht geoeffnet werden\n");
        }
        else if (!(inputFile.is_open()))
        {
            printf("Eingabefile konnte nicht geoeffnet werden\n");
        }
        
        // read Inputfile
        currentChar = inputFile.get();
        if (currentChar == EOF)
        {
            printf("Leere Datei geoeffnet\n");
        }
        else {
            // count character in the file 
            do
            {
                charCount[(unsigned char)currentChar]->count++;
                currentChar = inputFile.get();
            }
            while (currentChar != EOF);

            // sort the vector -> most common first
            sort(charCount.begin(), charCount.end(), sortVectorDec);

            // write count of symbols to file
            writeFileLen(charCount, outputFile);

            do
            {
                size = charCount.size();
                // fill code table with zero length characters
                if (charCount[size-1]->count == 0)
                {
                    charCount.pop_back();
                }
                else {
                    // set count with sum of two least elements
                    node* tempNode = createNode('\000', charCount[size-1]->count + charCount[size-2]->count);
                    // set childe nodes
                    tempNode->left = charCount[size-1];
                    tempNode->right = charCount[size-2];
                    // pop childe nodes
                    charCount.pop_back();
                    charCount.pop_back();
                    // now add new parent node
                    charCount.push_back(tempNode);
                    // rank new entry
                    sort(charCount.begin(), charCount.end(), sortVectorDec);
                }
            }
            while (size > 2);
            
            // traverse node tree
            traverseNodes(charCount[0], &codeTable);
            
            // create pattern
            createPattern(codeTable);

            // go to the begin of the input file
            inputFile.clear();
            inputFile.seekg(ios::beg);
            
            shiftToFile(codeTable, &inputFile, &outputFile);
        }
        
        // close files
        inputFile.close();
        //outputFile << EOF;
        outputFile.close();
    }
    
    return 0;
}
