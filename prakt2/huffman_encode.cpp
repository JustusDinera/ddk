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
  string character;
  int count;
  struct node *left = nullptr;
  struct node *right = nullptr;
};

// datatype for encode symbole table 
typedef struct TsymTab
{
    unsigned char character;
    unsigned char patternLen;
    unsigned char bitPatern;
};


/* 
    Variablen Deklaration
*/
int currentChar;
int lastChar = '\377';


/*
void putMany(ofstream * output, char character, int characterCount){
    int counter = characterCount;
    do
    {
        // check times the char was found
        if (counter < 3)
        {
            // found max 2 times
            for (int i = counter; i > 0; i--)
            {
                (*output) << character;
                counter--;
            }
        }
        // found more than one byte long sequence
        else if (counter > 258){
            (*output) << character << character << character << (uint8_t)(255);
            counter -= 258;
        }
        // found a sequence that fits in a byte
        else {
            (*output) << character << character << character << (uint8_t)(counter-3);      
            counter = 0;
        }
    } while (counter > 0);
}                  
*/ 
bool sortVectorDec(node * i, node * j){
    return (i->count > j->count);
}

bool sortVectorAsc(Tcounter i, Tcounter j){
    return (i.count < j.count);
}

node * createNode(string character, int count) {
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
    string tempCharacter;
    int size;


    vector<TsymTab> codeTable;
    TsymTab tempTabItem;
    // inital fill charCounter
    tempCount.count = 0;
    for (int i = 0; i < 256; i++)
    {
        tempCount.character = i;
        charCount.push_back(createNode(std::string(1, (unsigned char)i), 0));
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
                    tempTabItem.character = charCount[size-1]->character[0];
                    tempTabItem.patternLen = 0;
                    tempTabItem.bitPatern = 0;
                    // push table item
                    codeTable.push_back(tempTabItem);
                    // delete last entry of counter
                    charCount.pop_back();
                }
                else {
                    // build characer sting 
                    tempCharacter.assign(charCount[size-1]->character);
                    tempCharacter.append(charCount[size-2]->character); 
                    // set count with sum of two least elements
                    charCount.push_back(createNode(tempCharacter, charCount[size-1]->count + charCount[size-2]->count));
                    // set childe nodes
                    charCount[size-1]->left = charCount[size-3];
                    charCount[size-1]->right = charCount[size-2];
                    // rank new entry 
                    sort(charCount.begin(), charCount.end(), sortVectorDec);
                    // pop childe nodes
                    charCount.pop_back();
                    charCount.pop_back();
                }
            }
            while (size > 1);
            

            for (int i = size-1; i >= 0; i--)
            {
                
            }
            

              

            

        }



        // close files
        inputFile.close();
        outputFile << EOF;
        outputFile.close();
    }
    



    return 0;
}
