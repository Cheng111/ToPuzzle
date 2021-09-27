//Cheng Chen
//University of Tennessee
//CS594 lab1 jgraph 2021 Fall
#include <iostream>
#include <cstdio>
#include "ToPuzzle.h"
#include <cstring>
using namespace std;

void printhelp();
int main(int argc, char **argv)
{
    char * imageName;
    char * o = NULL;
    char dif = 'e';
    vector<float> * color = NULL;//the puzzle lines' color
    float fcolor;
    bool inputfile = false;
    int i, r, c, j;
    i = 1;
    r = -1;//the number of the puzzle rows
    c = -1;//the number of the puzzle columns
    if(argc <= 1)
    {
        printhelp();
        return -1;
    }
    //read paramters in
    while(i < argc)
    {
        if( (strcmp(argv[i], "-i") == 0) & ((i + 1) < argc) )
        {
            inputfile = true;
            imageName = argv[i + 1];
            i = i + 2;
        }
        else if( (strcmp(argv[i], "-o") == 0) & ((i + 1) < argc) )
        {
            o = argv[i + 1];
            i = i + 2;
        }
        else if( (strcmp(argv[i], "-r") == 0) & ((i + 1) < argc) )
        {
            r = stoi(argv[i + 1]);
            i = i + 2;
        }
        else if( (strcmp(argv[i], "-c") == 0) & ((i + 1) < argc) )
        {
            c = stoi(argv[i + 1]);
            i = i + 2;
        }
        else if( (strcmp(argv[i], "-d") == 0) & ((i + 1) < argc) )
        {
            dif = *argv[i + 1];
            i = i + 2;
        }
        else if( (strcmp(argv[i], "-w") == 0) & ((i + 3) < argc) )
        {
            color = new vector<float>;
            for(j = 1; j <= 3; j++)
            {
                fcolor = stof(argv[i + j]);
                if( (fcolor <= 1) & (fcolor >= 0) )
                {color->push_back(fcolor);}
                else
                {
                    fprintf(stderr, "Wrong color number %.2f. The color number should no greater than 1 and no smaller than 0.\n", fcolor);
                    return -1;
                }
            }

            i = i + 4;
        }
        else if( (strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == true) )
        {
            printhelp();
            return 0;
        }
        else
        {
            printhelp();
            return -1;
        }

    }
    if(inputfile != true)
    {
        printhelp();
        return -1;
    }
    //initial a images class
    images ig(imageName, r, c, o, dif, color);
    ig.printpixels();
    ig.draw_puzzle();
    ig.runjgraph();
    return 0;
}

//print the help page
void printhelp()
{
    printf("Usage: ToPuzzle -i file -o output -r number -c number -d e/h\n");
    printf("    -i*  input file\n");
    printf("    -o  output file\n");
    printf("    -r  the row number of puzzle\n");
    printf("    -c  the col number of puzzle\n");
    printf("    -w  the color of the puzzle lines. Should be three fload between 0 and 1. e.g. 0.5 0.1 0.4\n");
    printf("    -d  the complexity of the puzzle. There are two opitions. 'e' is easy, 'h' is hard\n");
    printf("    -h  print the help page\n");
    printf("    --help  print the help page\n");
    printf("The paramters with '*' is necessary\n");
}