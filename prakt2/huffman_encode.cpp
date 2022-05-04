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

// datatype for encode symbole table 
typedef struct TsymTab
{
    unsigned char character;
    unsigned char patternLen = 0;
    long bitPatern;
};

/* 
    variable declaration
*/
int currentChar;
int lastChar = '\377';

unsigned char traverseNodes(node * nodeTranvers, vector<TsymTab> * table){
    unsigned char retVal = 0;
    if (nodeTranvers->right != nullptr)
    {
        // get char from lower right node
        retVal = traverseNodes(nodeTranvers->right, table);
        // increase length
        (table->begin()+retVal)->patternLen++;
        // shift "1" in on left
        ((table->begin()+retVal)->bitPatern) >>= 1;
        ((table->begin()+retVal)->bitPatern) |= 1;
    }
    else if (nodeTranvers->left != nullptr)
    {
        // get char from lower left node
        retVal = traverseNodes(nodeTranvers->left, table);
        // increase lenght 
        (table->begin()+retVal)->patternLen++;
        // shift "0" in on left        
        ((table->begin()+retVal)->bitPatern) >>= 1;
    }
    else 
    {
        // return char if the node has no childe
        retVal = nodeTranvers->character;
    }

    return retVal;
}

bool sortVectorDec(node * i, node * j){
    return (i->count > j->count);
}

bool sortVectorAsc(Tcounter i, Tcounter j){
    return (i.count < j.count);
}

node * createNode(unsigned char character, int count) {
    struct node *node = (struct node *)malloc(sizeof(struct node));
    node->character = character;
    node->count = count;
    node->left = nullptr;
    node->right = nullptr;
    return node;
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

    // inital fill charCounter
    for (int i = 0; i < 256; i++)
    {
        tempCharacter = (unsigned char)i;
        charCount.push_back(createNode(tempCharacter, 0));
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

            // sort the vector -> most common first
            sort(charCount.begin(), charCount.end(), sortVectorDec);

            do
            {
                size = charCount.size();
                // fill code table with zero length characters
                if (charCount[size-1]->count == 0)
                {
                    // build table item
                    tempTabItem.character = charCount[size-1]->character;
                    tempTabItem.patternLen = 0;
                    tempTabItem.bitPatern = 0;
                    // push table item
                    codeTable.push_back(tempTabItem);
                    // delete last entry of counter
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
            
            // write to file
            /*
                TODO !!! !!! !!!
            */
        }



        // close files
        inputFile.close();
        outputFile << EOF;
        outputFile.close();
    }
    
    return 0;
}
