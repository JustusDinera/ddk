#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

#define DEBUG
#define DEBUG_INPUT "test.txt.huf"
#define DEBUG_OUTPUT "test_decode.txt"

using namespace std;

// datatype for character counter
struct Tcounter
{
    unsigned char character;
    int count;
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
void createPattern(vector<TsymTab> & table){
    // pattern variable
    static unsigned int pattern = 0;
    // sort table by pattern length
    sort(table.begin(), table.end(), sortTableLenAsc);
    
    for (int i = 0; i < table.size(); i++)
    {
        table[i].bitPattern.whole = pattern;

        if ((table[i].patternLen != table[i+1].patternLen) && (i < table.size()-1))
        {
            pattern <<= 1;
        }
        pattern++;
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
    int32_t tempPat = 0;
    uPattern bitStream = {0};
    int bitStreamLen = 0;

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
        if (!(inputFile.is_open()))
        {
            printf("Eingabefile konnte nicht geoeffnet werden\n");
        }
        else if (!(outputFile.is_open()))
        {
            printf("Ausgabedatei konnte nicht geoeffnet werden\n");
        }
        else
        {
            // read Inputfile
            currentChar = inputFile.get();
            if (currentChar == EOF)
            {
                printf("Leere Datei geoeffnet\n");
            }
            else {
                // get length of the pattern (read header)
                for (int i = 0; i < 256; i++)
                {
                    if (currentChar != 0)
                    {
                        tempSymb.character = i;
                        tempSymb.patternLen = currentChar;
                        codeTable.push_back(tempSymb);
                    }
                    currentChar = inputFile.get();
                }

                // creates the pattern for characters
                createPattern(codeTable);

                /*
                // set min pattern length
                minPatLen = codeTable[0].patternLen;

                // set max pattern length
                maxPatLen = codeTable[codeTable.size()-1].patternLen;
                */ 
                
                //currentChar = inputFile.get();
                while ((currentChar != EOF))
                {
                    while((bitStreamLen < 24) && (currentChar != EOF))
                    {
                        bitStreamLen += 8;
                        bitStream.whole <<= 8;
                        bitStream.whole |= currentChar;

                        currentChar = inputFile.get();
                    }
                    

                    for (int j = 0; j < codeTable.size(); j++)
                    {
                        tempPat = bitStream.whole >> (bitStreamLen - codeTable[j].patternLen);
                        tempPat &= (uint32_t)(0xFFFFFFFF) >> (32 - codeTable[j].patternLen);
                        if (codeTable[j].bitPattern.whole == tempPat)
                        {
                            outputFile.put((unsigned char)codeTable[j].character);
                            //bitStream.whole <<= codeTable[j].patternLen;
                            bitStreamLen -= codeTable[j].patternLen;
                            // loop exit criterias
                            j = 256;
                        }
                    }
                }
                while (bitStreamLen > 0)
                {
                    for (int j = 0; j < codeTable.size(); j++)
                    {
                        tempPat = bitStream.whole >> (bitStreamLen - codeTable[j].patternLen);
                        tempPat &= (uint32_t)(0xFFFFFFFF) >> (32 - codeTable[j].patternLen);
                        if (codeTable[j].bitPattern.whole == tempPat)
                        {
                            outputFile.put(codeTable[j].character);
                            //bitStream.whole <<= codeTable[j].patternLen;
                            bitStreamLen -= codeTable[j].patternLen;
                            // loop exit criterias
                            j = 256;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
