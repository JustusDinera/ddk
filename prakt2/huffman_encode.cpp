#include <string>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>

#define DEBUG

using namespace std;

typedef struct Tcounter
{
    unsigned char carakter;
    int count;
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
bool sortVector(Tcounter i, Tcounter j){
    return (i.count > j.count);
}



int main(int argc, char const *argv[])
{
    ifstream inputFile;
    ofstream outputFile;
    vector<unsigned char> inFile;
    vector<Tcounter> charCount;
    Tcounter tempCount;

    // inital fill charCounter
    tempCount.count = 0;
    for (int i = 0; i < 256; i++)
    {
        tempCount.carakter = i;
        charCount.push_back(tempCount);
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
                charCount[(unsigned char)currentChar].count++;
                currentChar = inputFile.get();
            }
            while (currentChar != EOF);

            sort(charCount.begin(), charCount.end(), sortVector);
        }

        

        // close files
        inputFile.close();
        outputFile << EOF;
        outputFile.close();
    }
    



    return 0;
}
