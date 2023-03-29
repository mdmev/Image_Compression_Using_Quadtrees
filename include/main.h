#ifndef __Image_comp__
#define __Image_comp__

//Quadtree Struct
typedef struct node
{
    int X;
    int Y;
    int Altura;
    int Width;
    int color;
    struct node* nodos[4];
} Quadtree;

int potencia_2(int N);

//--------------------------------------------------------------------------------------------------------------
//Set Nodes
void SetNode(Quadtree* newNode, int X, int Y, int Width, int color, int Altura);

//--------------------------------------------------------------------------------------------------------------
//Checks the colour of the array
int CheckColour (int X, int Y, int Width, int ** Array); 

//--------------------------------------------------------------------------------------------------------------
//Change the colour of the array
void Pintar(Quadtree * Node, int ** image);

//--------------------------------------------------------------------------------------------------------------
//Builds the Quadtree
void BuildQuadTree (Quadtree * Node, int ** Array, int Height);


#endif //__Image_comp__
