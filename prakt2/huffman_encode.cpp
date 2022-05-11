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

bool sortTableAsc(TsymTab i, TsymTab j){
    bool retVal = false;

    if (i.patternLen == j.patternLen)
        retVal = (i.character < j.character);
    else
        retVal = (i.patternLen < j.patternLen);
    
    return retVal;
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
    // sort table by pattern length
    sort(table[0].begin(), table[0].end(), sortTableAsc);
    
    /*
    // PATTERN ERSTELLEN
    */

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
        tempTabItem.bitPatern = 0;
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
