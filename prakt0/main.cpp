
#include <stdio.h>
#include <fstream>
#include <string>
#define MAX_ZEICHEN 127
#define DEBUG


using namespace std;

//int zeichenCount[MAX_ZEICHEN];

enum eZeichensatz {A,A_,B,B_,C,C_,D,D_,E,E_,F,F_,G,G_,H,H_,I,I_,J,J_,K,K_,L,L_,M,M_,N,N_,O,O_,P,P_,Q,Q_,R,R_,S,S_,T,T_,U,U_,V,V_,W,W_,X,X_,Y,Y_,Z,Z_,LEER,ENTER,PUNKT,KOMMA,ETC};
string zeichensatz ={"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ \n.,"};

int main(int argc, char const *argv[])
{
    int *zeichenCount = (int*)malloc(zeichensatz.length());
    char currentChar;
    ofstream outFile;
    ifstream inFile;
    int found = 0;

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


        while (!inFile.eof())
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
                zeichenCount[ETC]++;
            }
            
            
            /*
            switch (currentChar)
            {
            case 'a':
                zeichenCount[A]++;
                break;
            case 'A':
                zeichenCount[A_]++;
                break;
            case 'b':
                zeichenCount[B]++;
                break;
            case 'B':
                zeichenCount[B_]++;
                break;
            case 'c':
                zeichenCount[C]++;
                break;
            case 'C':
                zeichenCount[C_]++;
                break;
            case 'd':
                zeichenCount[D]++;
                break;
            case 'D':
                zeichenCount[D_]++;
                break;
            case 'e':
                zeichenCount[E]++;
                break;
            case 'E':
                zeichenCount[E_]++;
                break;
            case 'f':
                zeichenCount[F]++;
                break;
            case 'F':
                zeichenCount[F_]++;
                break;
            case 'g':
                zeichenCount[G]++;
                break;
            case 'G':
                zeichenCount[G_]++;
                break;
            case 'h':
                zeichenCount[H]++;
                break;
            case 'H':
                zeichenCount[H_]++;
                break;
            case 'i':
                zeichenCount[I]++;
                break;
            case 'I':
                zeichenCount[I_]++;
                break;
            case 'j':
                zeichenCount[J]++;
                break;
            case 'J':
                zeichenCount[J_]++;
                break;
            case 'k':
                zeichenCount[K]++;
                break;
            case 'K':
                zeichenCount[K_]++;
                break;
            case 'l':
                zeichenCount[L]++;
                break;
            case 'L':
                zeichenCount[L_]++;
                break;
            case 'm':
                zeichenCount[M]++;
                break;
            case 'M':
                zeichenCount[M_]++;
                break;
            case 'n':
                zeichenCount[N]++;
                break;
            case 'N':
                zeichenCount[N_]++;
                break;
            case 'o':
                zeichenCount[O]++;
                break;
            case 'O':
                zeichenCount[O_]++;
                break;
            case 'p':
                zeichenCount[P]++;
                break;
            case 'P':
                zeichenCount[P_]++;
                break;
            case 'q':
                zeichenCount[Q]++;
                break;
            case 'Q':
                zeichenCount[Q_]++;
                break;
            case 'r':
                zeichenCount[R]++;
                break;
            case 'R':
                zeichenCount[R_]++;
                break;
            case 's':
                zeichenCount[S]++;
                break;
            case 'S':
                zeichenCount[S_]++;
                break;
            case 't':
                zeichenCount[T]++;
                break;
            case 'T':
                zeichenCount[T_]++;
                break;
            case 'u':
                zeichenCount[U]++;
                break;
            case 'U':
                zeichenCount[U_]++;
                break;
            case 'v':
                zeichenCount[V]++;
                break;
            case 'V':
                zeichenCount[V_]++;
                break;
            case 'w':
                zeichenCount[W]++;
                break;
            case 'W':
                zeichenCount[W_]++;
                break;
            case 'x':
                zeichenCount[X]++;
                break;
            case 'X':
                zeichenCount[X_]++;
                break;
            case 'y':
                zeichenCount[Y]++;
                break;
            case 'Y':
                zeichenCount[Y_]++;
                break;
            case 'z':
                zeichenCount[Z]++;
                break;
            case 'Z':
                zeichenCount[Z_]++;
                break;
            case ' ':
                zeichenCount[LEER]++;
                break;
            case ',':
                zeichenCount[KOMMA]++;
                break;
            case '.':
                zeichenCount[PUNKT]++;
                break;
            case '\n':
                zeichenCount[ENTER]++;
                break;
            default:
                zeichenCount[ETC]++;
                break;
            }
            */
        } 
        inFile.close();

        for (int i = 0; i < MAX_ZEICHEN; i++)
        {
            outFile << zeichensatz[i] << "\t" << zeichenCount[i] << endl;
        }
        outFile.close();
    }

    return 0;
}

