//Cheng Chen
//University of Tennessee
//CS594 lab1 jgraph 2021 Fall
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <math.h>
#include "ToPuzzle.h"
#include <stdio.h>
using namespace std;

//class constructor
images::images(char * imageName, int r, int c, char * o, char dif, vector<float> * color)
{
    img = stbi_load(imageName, &width, &height, &channels, 0);
    if(img == NULL) 
    {
        fprintf(stderr, "Error in loading the image\n");
        exit(1);
    }
    img_size = width * height * channels;
    //initialize paramters. If the user does not input paramter, set default value
    if(set_values(r, c, o, dif, color) < 0)
    {
        perror("Fail to initial\n");
        exit(1);
    }
    else
    {
        //if successfully set the paramters, read the input file in
        loadpixel();
        string fjgraph = string(output) + string(".jgr");
        pFile = fopen (fjgraph.c_str(),"w");
    }
}

//read the image in
void images::loadpixel()
{
    unsigned char *p;
    size_t i = 0;
    vector<pixel> * tmpv;
    tmpv = new vector<pixel>;
    //read each pixel one by one. 
    for(i = 0; i < img_size; i += channels)
    {
        p = img + i;
        pixel pix;
        //not matter the original image has how many channels, we only store the first three color channels
        pix.set(*p, *(p + 1), *(p + 2), floor( (i / channels) / width), (i / channels) % width);
        (*tmpv).push_back(pix);
        if( (int)(i / channels) % width == (width - 1) )
        {
            image.push_back(tmpv);
            tmpv = new vector<pixel>;
        }
    }

}

//change the input image to a format that jgraph can read
void images::printpixels()
{
    int w, h;
    pixel pix;
    pixel prepix = (*image[0])[0];
    vector<pixel> * tmpv;
    fprintf(pFile, "newgraph\n");
    fprintf(pFile, "newcurve pts ");
    for(h = 0; h < height; h++)
    {
        tmpv = image[h];
        for(w = 0; w < width; w++)
        {
            pix = (*tmpv)[w];
            fprintf(pFile, "%d %d   ", width - w, height - h);
            if(samecolor(pix, prepix) == false)
            {
                fprintf(pFile, "color %.2f %.2f %.2f\n", prepix.red, prepix.green, prepix.blue);
                prepix = pix;
                fprintf(pFile, "newcurve pts ");
            }
        }
    }
    
}

//compare whether two pixels have the same color
bool images::samecolor(pixel x, pixel y)
{
    if( (x.red == y.red) & (x.blue == y.blue) & (x.green == y.green) )
    {return true;}
    return false;
}

//set paramaters
int images::set_values(int r, int c, char * o, char dif, vector<float> * color)
{
    if(width <= 10 || height <= 10)
    {
        fprintf(stderr, "The image does not have enough pixels\n");
        return -1;
    }
    if(r == -1)//if user doesn't input row number of the puzzles, set default value
    {
        puzzle_r = floor(height / 30);
    }
    else
    {
        if( (height / r) < 10 || r <= 0)
        {
            fprintf(stderr, "The puzzle row number is wrong. The recommand value should less than %d and bigger than 0\n", (int)floor( height / 10));
            return -1;
        }
        puzzle_r = r;
    }

    if(c == -1)//if user doesn't input colmun number of the puzzles, set default value
    {
        puzzle_c = floor(width / 30);
    }
    else
    {
        if( (width / c) < 10 || c <= 0)
        {
            fprintf(stderr, "The puzzle column number is wrong. The recommand value should less than %d and bigger than 0\n", (int)floor( width / 10));
            return -1;
        }
        puzzle_c = c;
    }
    if(o == NULL)//if user doesn't input the output file name, set default output as puzzle
    {
        output = (char *)malloc(sizeof("puzzle") + 1);
        strcpy(output, "puzzle");
    }
    else
    {output = o;}

    if(dif == 'e' || dif == 'h')//can print two different puzzles: easy or hard
    {d = dif;}
    else
    {fprintf(stderr, "wrong difficulty paramter %c, should be 'e' or 'h'\n", dif);}

    if(color != NULL)
    {puzzle_color = color;}
    else//if user doesn't input the color of the puzzle lines, set the default as red
    {
        puzzle_color = new vector<float>;
        puzzle_color->push_back(1.0);
        puzzle_color->push_back(0);
        puzzle_color->push_back(0);
    }

    return 0;
}
//draw the half cycle
void images::halfcycle()
{
    int puzzle_width = floor(width / puzzle_c);
    int puzzle_height = floor(height / puzzle_r);
    int i;
    int rad_x, rad_y;
    rad_x = floor(puzzle_width / 4);
    rad_y = floor(puzzle_height / 4);
    //the half cycle that from degree 0 to degree 180
    for(i = 0; i <= 180; i++)
    {
        crosswise.push_back( make_pair( rad_x * cos(i * M_PI / 180), rad_x * sin(i * M_PI / 180) ) );
    }
    //the half cycle that from degree 90 to degree 270
    for(i = 90; i <= 270; i++)
    {
        lengthways.push_back( make_pair( rad_y * cos(i * M_PI / 180), rad_y * sin(i * M_PI / 180) ) );
    }
}

//build the easy puzzle which the cut lines are just straight lines
void images::draw_easy()
{
    int puzzle_width = floor(width / puzzle_c);
    int puzzle_height = floor(height / puzzle_r);
    int i;
    for(i = puzzle_width; i < width; i = i + puzzle_width)
    {
        fprintf(pFile, "newline color %.2f %.2f %.2f   linethickness 0.1   pts %d 0   %d %d   linetype solid\n", \
        (*puzzle_color)[0], (*puzzle_color)[1], (*puzzle_color)[2], i, i, height);
    }
    for(i = puzzle_height; i < height; i = i + puzzle_height)
    {
        fprintf(pFile, "newline color %.2f %.2f %.2f   linethickness 0.1   pts 0 %d   %d %d   linetype solid\n", \
        (*puzzle_color)[0], (*puzzle_color)[1], (*puzzle_color)[2], i, width, i);
    }
}

//build the hard puzzle which the cut lines are wavy lines
void images::draw_hard()
{
    int i, j;
    int puzzle_width = floor(width / puzzle_c);
    int puzzle_height = floor(height / puzzle_r);
    int rad_x, rad_y;
    rad_x = floor(puzzle_width / 4);
    rad_y = floor(puzzle_height / 4);
    int a_x, a_y, b_x, b_y;
    for(i = 1; i <= puzzle_c; i = i + 1)
    {
        fprintf(pFile, "newline color %.2f %.2f %.2f      linethickness 0.1   pts %d 0  %d %d   linetype solid\n", (*puzzle_color)[0], \
        (*puzzle_color)[1],(*puzzle_color)[2], i * puzzle_width, i * puzzle_width, rad_y);
        for(j = 1; j < puzzle_r; j = j + 1)
        {
            a_x = i * puzzle_width;
            b_x = a_x;
            a_y = (j - 1) * puzzle_height + 3 * rad_y;
            b_y = j * puzzle_height + rad_y;
            fprintf(pFile, "newline color %.2f %.2f %.2f   linethickness 0.1   pts %d %d  %d %d   linetype solid\n", (*puzzle_color)[0], \
            (*puzzle_color)[1],(*puzzle_color)[2], a_x, a_y, b_x, b_y);
        }
        a_x = i * puzzle_width;
        b_x = a_x;
        a_y = (j - 1) * puzzle_height + 3 * rad_y;
        b_y = height;
        fprintf(pFile, "newline color %.2f %.2f %.2f   linethickness 0.1   pts %d %d  %d %d   linetype solid\n", (*puzzle_color)[0], \
        (*puzzle_color)[1],(*puzzle_color)[2], a_x, a_y, b_x, b_y);
        draw_cycle(a_x, 1, puzzle_height, rad_y);
    }

    for(i = 1; i <= puzzle_r; i = i + 1)
    {
        fprintf(pFile, "newline color %.2f %.2f %.2f      linethickness 0.1   pts 0 %d  %d %d   linetype solid\n", (*puzzle_color)[0], \
        (*puzzle_color)[1],(*puzzle_color)[2], i * puzzle_height, rad_x, i * puzzle_height);
        for(j = 1; j < puzzle_c; j = j + 1)
        {
            a_x = (j - 1) * puzzle_width + 3 * rad_x;
            b_x = j * puzzle_width + rad_x;
            a_y = i * puzzle_height;
            b_y = a_y;
            fprintf(pFile, "newline color %.2f %.2f %.2f   linethickness 0.1   pts %d %d  %d %d   linetype solid\n", (*puzzle_color)[0], \
            (*puzzle_color)[1],(*puzzle_color)[2], a_x, a_y, b_x, b_y);
        }
        a_x = (j - 1) * puzzle_width + 3 * rad_x;
        b_x = width;
        a_y = i * puzzle_height;
        b_y = a_y;
        fprintf(pFile, "newline color %.2f %.2f %.2f   linethickness 0.1   pts %d %d  %d %d   linetype solid\n", (*puzzle_color)[0], \
        (*puzzle_color)[1],(*puzzle_color)[2], a_x, a_y, b_x, b_y);
        draw_cycle(a_y, 0, puzzle_width, rad_x);
    }

}

//draw the puzzle depend on the user input
int images::draw_puzzle()
{
    if(d == 'e')
    {draw_easy();}
    else if(d == 'h')
    {
        halfcycle();
        draw_hard();
    }
    fclose(pFile);
    return 0;
}

//draw the cycle of the puzzle lines
void images::draw_cycle(int center, int direction, int cell_size, int rad)
{
    vector< pair<float, float> >::iterator vit;
    pair<float, float> node;
    int j;
    float x, y;
    int flag = 1;
    if(direction ==1)
    {
        for(j = 0; j <= puzzle_r - 1; j++)
        {
            fprintf(pFile, "newline color %.2f %.2f %.2f   linethickness 0.1   pts ",(*puzzle_color)[0], (*puzzle_color)[1],(*puzzle_color)[2]);
            for(vit = lengthways.begin(); vit != lengthways.end(); vit++)
            {
                x = center + flag * (vit->first);
                y = j * cell_size + 2 * rad + vit->second;
                fprintf(pFile, "%.2f %.2f   ", x, y);
            }
            fprintf(pFile, "linetype solid\n");
            flag = flag * -1;
        }
    }

    if(direction == 0)
    {
        for(j = 0; j <= puzzle_c - 1; j++)
        {
            fprintf(pFile, "newline color %.2f %.2f %.2f   linethickness 0.1   pts ",(*puzzle_color)[0], (*puzzle_color)[1],(*puzzle_color)[2]);
            for(vit = crosswise.begin(); vit != crosswise.end(); vit++)
            {
                x = j * cell_size + 2 * rad + vit->first;
                y = center + flag * (vit->second);
                fprintf(pFile, "%.2f %.2f   ", x, y);
            }
            fprintf(pFile, "linetype solid\n");
            flag = flag * -1;
        }
    }
}

//use system call to run the jgraph
void images::runjgraph()
{
    string command;
    command = string("jgraph -P ") + string(output) +string(".jgr | ps2pdf - | convert -density 300 - -quality 100 ")\
    + string(output) + string(".jpg");
    system(&command[0]);
}