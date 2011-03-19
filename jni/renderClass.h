#ifndef RENDER_CLASS_H_INCLUDED
#define RENDER_CLASS_H_INCLUDED

#include <string>
#include <map>

class t_RenderClass
{
public:

t_RenderClass()
{
   gTextureVertices[0]  = 0 *(1<<16);
   gTextureVertices[1]  = 1 *(1<<16);

   gTextureVertices[2]  = 0 *(1<<16);
   gTextureVertices[3]  = 0 *(1<<16);
   
   gTextureVertices[4]  = 1 *(1<<16);
   gTextureVertices[5]  = 1 *(1<<16);
   
   gTextureVertices[6]  = 1 *(1<<16);
   gTextureVertices[7]  = 0 *(1<<16);


   gMoveVertices[0] = -.5 * (1<<16); 
   gMoveVertices[1] = -.5 * (1<<16); 

   gMoveVertices[2] = -.5 * (1<<16); 
   gMoveVertices[3] = .5 * (1<<16); 
   
   gMoveVertices[4] = .5 * (1<<16); 
   gMoveVertices[5] = -.5 * (1<<16); 
   
   gMoveVertices[6] = .5 * (1<<16); 
   gMoveVertices[7] = .5 * (1<<16); 
   

   gElements[0] = 0;
   gElements[1] = 1;
   gElements[2] = 2;
   gElements[3] = 1;
   gElements[4] = 2;
   gElements[5] = 3;

   memset(&trisArray[0][0],0,10 * 21 * sizeof(int));

   corex = 4;
   corey = 18;

   xpieceArray[0] = -1;
   xpieceArray[1] = -1;
   xpieceArray[2] = 0;
   xpieceArray[3] = 0;

   ypieceArray[0] = 1;
   ypieceArray[1] = 0;
   ypieceArray[2] = 0;
   ypieceArray[3] = -1;

   pieceColor = 4;

   score = 0;

}

   void move(float w, float h);
   void renderFrame();
   bool setupGraphics(int w, int h);
   void loadTextures();
   void loadText(int size);
   void drawText(std::string, float x, float y);
   void loadTexture(int target, const char *filename);

   void drawArray();
   void drawPiece();

   bool checkValid();
   void makeStatic();
   
   void clearRows();

   void moveDown();
   void moveLeft();
   void moveRight();

   void rotateLeft();
   void rotateRight();

private:
   std::map<int,int> yeah;


   int gTextureVertices[8] ;
   int gMoveVertices[8] ;

   char gElements[6];

   int gTriangleVertices[12];

   unsigned int textures[2];
   
   double lasttime;

   double targettime;

   int theWidth;
   int theHeight;

   time_t secondOffset;

   int trisArray[21][10];

   int score;

   //piece data
   int corex;
   int corey;
   int xpieceArray[4];
   int ypieceArray[4];
   int pieceColor;


};

#endif
