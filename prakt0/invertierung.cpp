
#include <stdio.h>
#include <fstream>
#include <string>

#define DEBUG

using namespace std;


int main(int argc, char const *argv[])
{
    char currentChar;
    ofstream outFile;
    ifstream inFile;
    char invert_byte = 0;
 
    #ifndef DEBUG
    if (argv[1] == nullptr)
    {
        printf("Keine Dateien angegebn\n");
    }
    else if (argv[2] == nullptr)
    {
        printf("Keine Ausgabe Datei angegeben");
    }
    else
    #endif
    {
    #ifndef DEBUG
        inFile.open((string)(argv[1]));
        outFile.open((string)(argv[2]));
    #else
        inFile.open("WHZ_RLE8.bmp");
        outFile.open("WHZ_RLE8_invert.bmp");
    #endif

        if (!(outFile.is_open()))
        {
            printf("Ausgabedatei konnte nicht geoeffnet werden");
        }

        else if (!(inFile.is_open()))
        {
            printf("Eingabefile konnte nicht geoeffnet werden");
        }
        // read Inputfile

        currentChar = inFile.get();

        while (currentChar != EOF)
        {
            invert_byte = ~currentChar;
            outFile.put(invert_byte);

            currentChar = inFile.get();
        } 

        inFile.close();
        outFile.close();
    }

    return 0;
}

