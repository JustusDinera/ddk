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
    uPattern bitPattern;
};

// sort table entries by length first and char value secound
bool sortTableLenAsc(TsymTab i, TsymTab j){
    bool retVal = false;
    if (i.patternLen == j.patternLen)
        retVal = (i.character < j.character);
    else
        retVal = (i.patternLen < j.patternLen);
    return retVal;
}

// create the pattern of the characters
void createPattern(vector<TsymTab> * table){
    // pattern variable
    static unsigned int pattern = 0;
    // sort table by pattern length
    sort(table[0].begin(), table[0].end(), sortTableLenAsc);
    
    for (int i = 0; i < table[0].size(); i++)
    {
        if (0 != table[0][i].patternLen)
        {
            table[0][i].bitPattern.whole = pattern;

            if ((table[0][i].patternLen != table[0][i+1].patternLen) && (i < table[0].size()-1))
            {
                pattern <<= 1;
            }
            pattern++;
        }
    }
}

int main(int argc, char const *argv[])
{
    ifstream inputFile;
    ofstream outputFile;
    int currentChar;
    vector<TsymTab> codeTable;
    TsymTab tempSymb;
    int minPatLen = 0;
    int maxPatLen = 0;

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
        inputFile.open("Faust.txt.huf", ios::out | ios::binary);
        outputFile.open("Faust_decode.txt", ios::in | ios::binary);
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
            // get length of the pattern (read header)
            for (char i = 0; i < 256; i++)
            {
                tempSymb.character = i;
                tempSymb.patternLen = currentChar;
                codeTable.push_back(tempSymb);
            }

            // creates the pattern for characters
            createPattern(&codeTable);

            // set min pattern length
            for (int i = 0; i < codeTable.size(); i++)
            {
                if (codeTable[i].patternLen != 0)
                {
                    minPatLen = codeTable[i].patternLen;
                    i = codeTable.size();
                }
            }
            // set max pattern length
            maxPatLen = codeTable[codeTable.size()-1].patternLen;

            while (currentChar != EOF)
            {
                for (int i = minPatLen; i < maxPatLen; i++)
                {
                    
                }
                
            }
            
            
        }
    }
    return 0;
}
