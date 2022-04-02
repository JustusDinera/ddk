#include <string>
#include <stdio.h>
#include <fstream>

#define DEBUG

using namespace std;

/* 
    Variablen Deklaration
*/
char currentChar;
char lastChar = '\377';
int charCounter = 0;



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
        inputFile.open((string)(argv[1]));
        outputFile.open((string)(argv[2]));
#else
        inputFile.open("WHZ_uncompressed.bmp");
        outputFile.open("WHZ_bmp.rle");
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
                currentChar = inputFile.get();

                // chaeck last and current char
                if ((currentChar != lastChar) && (lastChar != EOF))
                {
                    // check times the char was found
                    if (charCounter < 3)
                    {
                        // found max 2 times
                        for (int i = 0; i < charCounter; i++)
                        {
                            outputFile << lastChar;
                        }
                    }
                    // found more than 2 times
                    else {
                        outputFile << currentChar << currentChar << currentChar << (charCounter-3);
                    }
                    // set memorize found char
                    lastChar = currentChar;
                    charCounter = 1;
                }
                // if last char is identical to current
                else
                {
                    charCounter++;
                }
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
