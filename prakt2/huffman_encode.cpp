#include <string>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

#define DEBUG

using namespace std;

// datatype for character counter
typedef struct Tcounter
{
    unsigned char character;
    int count;
};

typedef struct node {
  unsigned char character;
  int count;
  struct node *left = nullptr;
  struct node *right = nullptr;
};

typedef union uPattern {
    uint32_t whole;
    struct bytes {
        unsigned char byte0;
        unsigned char byte1;
        unsigned char byte2;
        unsigned char byte3;
    } bytes;
};

// datatype for encode symbole table 
typedef struct TsymTab
{
    unsigned char character;
    unsigned char patternLen = 0;
    uPattern bitPatern;
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

void createPattern(vector<TsymTab> * table){
    // pattern variable
    static unsigned int pattern = 0;
    // sort table by pattern length
    sort(table[0].begin(), table[0].end(), sortTableLenAsc);
    
    for (int i = 0; i < table[0].size(); i++)
    {
        if (0 != table[0][i].patternLen)
        {
            table[0][i].bitPatern.whole = pattern;

            if ((table[0][i].patternLen != table[0][i+1].patternLen) && (i < table[0].size()-1))
            {
                pattern <<= 1;
            }
            pattern++;
        }
    }
}

void createFileHead(vector<TsymTab> * codeTable,  ofstream * outputFile){
    for (int i = 0; i < codeTable[0].size(); i++)
    {
        outputFile->put(codeTable[0][i].patternLen);
    }
}

void createFileBody(vector<TsymTab> * codeTable, ifstream * inputFile,  ofstream * outputFile){
    uPattern tempPat;
    unsigned char length;
    tempPat.whole = 0;
    int character = inputFile->get();

    // access whole file 
    while (character !=  EOF)
    {
        length += codeTable[0][character].patternLen;
        tempPat.whole <<= codeTable[0][character].patternLen;
        tempPat.whole |= codeTable[0][character].bitPatern.whole;

        if  (length%8 != 0) {
            tempPat.whole <<= 8 - (length % 8);
            length += 8 - (length % 8);
        }

        switch (length)
        {
        case 32:
            outputFile->put(tempPat.bytes.byte3);
            tempPat.bytes.byte3 = 0;
        case 24:
            outputFile->put(tempPat.bytes.byte2);
            tempPat.bytes.byte2 = 0;
        case 16:
            outputFile->put(tempPat.bytes.byte1);
            tempPat.bytes.byte1 = 0;
        case 8:
            length = codeTable[0][character].patternLen % 8;
            if (length == 0)
            {
                outputFile->put(tempPat.bytes.byte0);
                tempPat.bytes.byte0 = 0;
            }
            else {
                tempPat.whole >>=  8 - length;
            }
        default:
            break;
        }

        character = inputFile->get();
    }
}

void shiftToFile(vector<TsymTab> * codeTable, ifstream  * counter,  ofstream * outputFile){
    sort(codeTable[0].begin(), codeTable[0].end(), sortTableCharAsc);
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

    // inital fill charCounter
    for (int i = 0; i < 256; i++)
    {
        tempCharacter = (unsigned char)i;
        // fill counter
        charCount.push_back(createNode(tempCharacter, 0));
        tempTabItem.character = tempCharacter;
        tempTabItem.patternLen = 0;
        tempTabItem.bitPatern.whole = 0;
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
        inputFile.open("Faust.txt", ios::in | ios::binary);
        outputFile.open("Faust.txt.huf", ios::out | ios::binary);
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
            inputFile.close();

            // sort the vector -> most common first
            sort(charCount.begin(), charCount.end(), sortVectorDec);

            do
            {
                size = charCount.size();
                // fill code table with zero length characters
                if (charCount[size-1]->count == 0)
                {
                    /*
                    // build table item
                    //tempTabItem.character = charCount[size-1]->character;
                    //tempTabItem.patternLen = 0;
                    //tempTabItem.bitPatern = 0;
                    // push table item
                    //codeTable.push_back(tempTabItem);
                    // delete last entry of counter
                    */
                    charCount.pop_back();
                }
                else {
                    // build characer sting 
                    //tempCharacter.append(charCount[size-2]->character); 
                    // set count with sum of two least elements
                    charCount.push_back(createNode('\000', charCount[size-1]->count + charCount[size-2]->count));
                    
                    // set childe nodes
                    charCount[size]->left = charCount[size-1];
                    charCount[size]->right = charCount[size-2];
                    // rank new entry 
                    sort(charCount.begin(), charCount.end(), sortVectorDec);
                    // pop childe nodes
                    charCount.pop_back();
                    charCount.pop_back();
                }
            }
            while (size > 2);
            
            // traverse node tree
            traverseNodes(charCount[0], &codeTable);
            
            // create pattern
            createPattern(&codeTable);

            // write to file
#ifndef DEBUG
            inputFile.open((string)(argv[1]), ios::in | ios::binary);
#else
            inputFile.open("Faust.txt", ios::in | ios::binary);
#endif
            shiftToFile(&codeTable, &inputFile, &outputFile);
        }



        // close files
        inputFile.close();
        outputFile << EOF;
        outputFile.close();
    }
    
    return 0;
}
