#include <string>
#include <stdio.h>
#include <fstream>

//#define DEBUG

using namespace std;

/* 
    Variablen Deklaration
*/
int currentChar;
int lastChar = '\377';
int charCounter = 0;

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

int main(int argc, char const *argv[])
{

    ifstream inputFile;
    ofstream outputFile;

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
        inputFile.open("WHZ_uncompressed.bmp", ios::in | ios::binary);
        outputFile.open("WHZ_bmp.rle", ios::out | ios::binary);
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
            do
            {
                // chaeck last and current char
                if ((currentChar != lastChar) && (currentChar != EOF))
                {
                    putMany(&outputFile, lastChar, charCounter);
                    // set memorize found char
                    lastChar = currentChar;
                    charCounter = 1;
                }
                // if last char is identical to current
                else
                {
                    charCounter++;
                }
                currentChar = inputFile.get();
            }
            while (currentChar != EOF);
        }

        // close files
        inputFile.close();
        outputFile << EOF;
        outputFile.close();
    }
    



    return 0;
}
