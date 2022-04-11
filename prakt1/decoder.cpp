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
int charCounter = 1;


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
        inputFile.open("WHZ_bmp.rle", ios::in | ios::binary);
        outputFile.open("WHZ_decompressed.bmp", ios::out | ios::binary);
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
                // check if last char is identical to current
                if (currentChar == lastChar) 
                {
                    charCounter++;
                }
                // Current char differs from the last char
                else
                {
                    charCounter = 1;
                }

                // put the current char to output file
                outputFile << (uint8_t)currentChar;

                // if more then 3 same char in a row put the amount after the 3 chars 
                if (charCounter > 2)
                {
                    for (int i = inputFile.get(); i > 0; i--)
                    {
                        outputFile << (uint8_t)currentChar;
                    }
                    // reset char counter for next same char
                    charCounter = 0;
                }

                // get next char
                lastChar = currentChar;
                currentChar = inputFile.get();
            } while (currentChar != EOF);
        }

        // close files
        inputFile.close();
        outputFile << EOF;
        outputFile.close();
    }

    return 0;
}