
#include <stdio.h>
#include <fstream>
#include <string>

#define DEBUG


using namespace std;

//int zeichenCount[MAX_ZEICHEN];

//enum eZeichensatz {A,A_,B,B_,C,C_,D,D_,E,E_,F,F_,G,G_,H,H_,I,I_,J,J_,K,K_,L,L_,M,M_,N,N_,O,O_,P,P_,Q,Q_,R,R_,S,S_,T,T_,U,U_,V,V_,W,W_,X,X_,Y,Y_,Z,Z_,LEER,ENTER,PUNKT,KOMMA,ETC};
string zeichensatz ={"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ \n.,"};

int main(int argc, char const *argv[])
{
    char currentChar;
    ofstream outFile;
    ifstream inFile;
    int found = 0;
    int *zeichenCount = new int[zeichensatz.length()+1];

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
        inFile.open("Faust.txt");
        outFile.open("faust.csv");
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

        do
        {
            currentChar = inFile.get();
            for (int i = 0; i < zeichensatz.size(); i++)
            {
                if (currentChar == (char)zeichensatz[i])
                {
                    zeichenCount[i]++;
                    found = 1;
                    i = zeichensatz.size();
                }
            }
            if (!found)
            {
                zeichenCount[zeichensatz.size()+1]++;
            }
            
        } 
        while (currentChar != EOF);
        inFile.close();

        for (int i = 0; i < zeichensatz.length()+1; i++)
        {
            if (zeichensatz[i] == '\n') {
                outFile << "CR\t" << zeichenCount[i] << endl;
            }
            else if (zeichensatz[i] == ' ')
            {
                outFile << "\' \'\t" << zeichenCount[i] << endl;
            }
            else if (zeichensatz[i] == '\0')
            {
                outFile << "etc\t" << zeichenCount[i] << endl;
            }
            else             
            {
                outFile << zeichensatz[i] << "\t" << zeichenCount[i] << endl;
            }
        }
        outFile.close();
    }

    return 0;
}

