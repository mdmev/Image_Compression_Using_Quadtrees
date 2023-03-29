//----------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
//----------------------------------------------------------------------
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"
//--------------------------------------------------------------------------------------------------------------
// 1 for grayscale
#define CHANNEL_NUM 1
//--------------------------------------------------------------------------------------------------------------
// Include custom header file for functions used in this program
#include "../include/main.h"

//--------------------------------------------------------------------------------------------------------------
//This function does the same as pow()
int potencia_2(int N){
    for(int i = 1;i<= 12;i++){
        if( N > pow(2,i-1) && N < pow(2,i)) return pow(2,i);
        else if( (N== pow(2,i-1)) || (N == pow(2,i)) ) return N;
    }}
//--------------------------------------------------------------------------------------------------------------
//Create Nodes
void SetNode(Quadtree* newNode, int X, int Y, int Width, int color, int Altura)
{
    int k; 
    newNode->Y=Y;
    newNode->X=X; 
    newNode->color= color;   
    newNode->Width= Width;
    newNode->Altura = Altura; 
    //Memory space for each node
    for (k = 0; k < 4; k++) 
        newNode->nodos[k] = (Quadtree*) malloc(sizeof(Quadtree));
}
//--------------------------------------------------------------------------------------------------------------
//CheckColour function
int CheckColour (int X, int Y, int Width, int ** Array)
{
    int i,j,blanco = 0,negro = 0; 
    uint8_t sumArray  = 0, cont = 0; 
    int last_pixel_x = X + (Width-1);
    int last_pixel_y = Y + (Width-1);
    //Search for leaves
    for ( i = X; i <= last_pixel_x; j++ )
    {
        for ( j = Y; j <= last_pixel_y; j++ )
        {
            if (Array[j][i]==255) blanco++;
            else if (Array[j][i]==0) negro ++;
        }
    }
    if (blanco==0) return 0;
    if (negro==0) return 1;
    return 2;
}
//--------------------------------------------------------------------------------------------------------------
//Changes the colour of the array
void Pintar(Quadtree * Node, int ** image)
{
    int i,j;
    int X = Node->X, Y = Node->Y, color = Node->color,
    Width = Node->Width, last_pixel_x = X + (Width-1),
    last_pixel_y = Y + (Width-1);
    
    if (color == 0) color = 0; //black
    else if (color == 1) color = 255; //white
    else if (color == 2) color = 128; //gray

   
    for ( i = X; i <= last_pixel_x; i++ ) 
    {
        for ( j = Y; j <= last_pixel_y; j++ )
        {
            image[j][i] = color;
        }
    }
}
//--------------------------------------------------------------------------------------------------------------
//Create QuadTree
void BuildQuadTree (Quadtree * Node, int ** Array, int Height)
{
    int X_act, Y_act, X, Y,
        Width, Width_act,
        Altura, Altura_act,
        color, color_act, 
        i; 
    
    X_act = Node->X;
    Y_act = Node->Y;
    Width_act = Node->Width;
    Altura_act = Node->Altura;
    color_act = Node->color;
    
    Width = (Width_act/2);
    Altura = Altura_act + 1;

    if (Height == Altura_act)  Pintar(Node, Array);
    
    if (Altura_act > Height) exit(1);
    //Case for each children
    for (i=0; i<4; i++)
    {
        switch(i)
        {
            case 0: 
                    X = X_act; 
                    Y = Y_act; 
                    color = CheckColour(X, Y, Width, Array);
                    SetNode(Node->nodos[0], X, Y, Width, color, Altura);
                    if (color == 2) BuildQuadTree(Node->nodos[0], Array, Altura);                        
                    break; 
            case 1: 
                    X = X_act + Width;
                    Y = Y_act;
                    color = CheckColour(X, Y, Width, Array);
                    SetNode(Node->nodos[1], X, Y, Width, color, Altura);
                    if (color == 2) BuildQuadTree(Node->nodos[1], Array, Altura);                           
                    break;
            case 2: 
                    X = X_act;
                    Y = Y_act + Width;
                    color = CheckColour(X, Y, Width, Array);
                    SetNode(Node->nodos[2], X, Y, Width, color, Altura);
                    if (color == 2) BuildQuadTree(Node->nodos[2], Array, Altura);                          
                    break;
            case 3: 
                    X = X_act + Width;
                    Y = Y_act + Width;
                    color = CheckColour(X, Y, Width, Array);
                    SetNode(Node->nodos[3], X, Y, Width, color, Altura); 
                    if (color == 2) BuildQuadTree(Node->nodos[3], Array, Altura);                                                  
                    break;
        }
    }
}
//--------------------------------------------------------------------------------------------------------------
//Main
int main(int argc, char **argv)
{
    int width, height,t_width, bpp;
    char *imgname = argv[1];
    int niveles = atoi(argv[2]);

    // Load the image
    uint8_t* img = stbi_load(imgname, &width, &height, &bpp, 1);
    printf("Imagen %s cargada! width %d, height %d, bpp %d\n", imgname, width,height, bpp);
    // make the image square
    if (width >= height) t_width = potencia_2(width);
    else t_width = potencia_2(height);

    int ** imagen; 
    int i,j, c = 0; 
    uint8_t * temp = img;
    imagen = (int**) malloc(t_width*sizeof(int*)); 
    for(i = 0; i < t_width; i++)
    {
        imagen[i] = (int*) malloc(t_width*sizeof(int)); 
        for ( j = 0; j < t_width; j++ )
        {
            if( i >= height || j <= width)imagen[i][j]=255;
            imagen[i][j] = *(temp++);
            c++;
        }
    }
    
    Quadtree RootNode; 
    SetNode(&RootNode, 0, 0, t_width, 2, 1); 
    BuildQuadTree(&RootNode, imagen, niveles); 
    
    temp = temp - c; 
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            *temp = imagen[i][j];
            temp++;
        }
    }

    //--------------------------------------------------------------------------------------------------------------
    // Save the image in out.jpg
    stbi_write_jpg("bin/out.jpg", width, height, CHANNEL_NUM, img, 100);
    //--------------------------------------------------------------------------------------------------------------
    // Free
    stbi_image_free(img);
    //Free
    for ( i=0; i < t_width; i++)
    {
        free(imagen[i]);
    }
    free(imagen);
    return 0;
}