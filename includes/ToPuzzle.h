//Cheng Chen
//University of Tennessee
//CS594 lab1 jgraph 2021 Fall
#include <vector>
#include <math.h>
#include <cstdio>
using namespace std;
struct pixel
{
    /* Common 3-digit 0-255 color representation */
    float red, green, blue;
    int i, j;

    /* Update the values of the pixels */
    void set(int r, int g, int b, int row, int col) 
    {
        int precision = 100;
        red = round(r * precision / 255) / precision; 
        green = round(g  * precision / 255) / precision;
        blue = round (b * precision / 255) / precision; 
        i = row; j = col;}
};
class images
{
    int set_values(int r, int c, char * o, char dif, vector<float> * color);
    int width, height, channels, puzzle_r, puzzle_c;
    //char * output = "puzzle";
    char * output;
    char d;
    size_t img_size;
    unsigned char *img;
    void loadpixel();
    vector< vector<pixel>* > image;
    bool samecolor(pixel x, pixel y);
    void halfcycle();
    vector< pair<float, float> > crosswise;
    vector< pair<float, float> > lengthways;
    void draw_easy();
    void draw_hard();
    void draw_cycle(int center, int direction, int cell_size, int rad);
    vector<float> * puzzle_color;
    FILE * pFile;

    public:
    images(char * imageName, int r, int c, char * o, char dif, vector<float> * color);
    void printpixels();
    int draw_puzzle();
    void runjgraph();
};

