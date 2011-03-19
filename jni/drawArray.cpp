#include "renderClass.h"
#include "fixedPoint.h"
#include <string>
#include <ctime>

#include <map>

#include <vector>

//#include <GLES/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include "loadTextures.h"
#include "loadText.h"
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s)
{
   const char *v = (const char *) glGetString(s);
   LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char *op)
{
   for (GLint error = glGetError(); error; error
         = glGetError())
   {
      LOGI("after %s() glError (0x%x)\n", op, error);
   }
}

void t_RenderClass::drawPiece()
{
   char *elements = new char[4 * 6];

   for (int i = 0; i<4; i++)
   {
      elements[0 + i * 6] = i*4 + 0;
      elements[1 + i * 6] = i*4 + 1;
      elements[2 + i * 6] = i*4 + 2;
      elements[3 + i * 6] = i*4 + 1;
      elements[4 + i * 6] = i*4 + 2;
      elements[5 + i * 6] = i*4 + 3;
   }

   t_fixed *vertArray = new t_fixed[4 *8];

   for (int i = 0; i<4; i++)
   {
      int tempX = corex + xpieceArray[i];
      int tempY = corey + ypieceArray[i];

      vertArray[0 + i * 8] = (tempX * 5 + -0);
      vertArray[1 + i * 8] = (tempY * 5 + -0) ;

      vertArray[2 + i * 8] = (tempX * 5 + -0);
      vertArray[3 + i * 8] = ((tempY+1) * 5 + -0) ;

      vertArray[4 + i * 8] = ((tempX+1) * 5 + -0) ;
      vertArray[5 + i * 8] = (tempY * 5 + -0) ;

      vertArray[6 + i * 8] = ((tempX+1) * 5 + -0);
      vertArray[7 + i * 8] = ((tempY+1) * 5 + -0);
   }

   unsigned char *colorArray = new unsigned char[4 * 16];

   for (int i = 0; i<4*4; i++)
   {
      colorArray[3 + i * 4] = 255;

      switch (pieceColor)
      {
      case 1:
         colorArray[0 + i * 4] = 0;
         colorArray[1 + i * 4] = 255;
         colorArray[2 + i * 4] = 255;
         break;

      case 2:
         colorArray[0 + i * 4] = 0;
         colorArray[1 + i * 4] = 0;
         colorArray[2 + i * 4] = 255;
         break;

      case 3:
         colorArray[0 + i * 4] = 255;
         colorArray[1 + i * 4] = 165;
         colorArray[2 + i * 4] = 0;
         break;
      
      case 4:
         colorArray[0 + i * 4] = 255;
         colorArray[1 + i * 4] = 255;
         colorArray[2 + i * 4] = 0;
         break;
      
      case 5:
         colorArray[0 + i * 4] = 0;
         colorArray[1 + i * 4] = 255;
         colorArray[2 + i * 4] = 0;
         break;
      
      case 6:
         colorArray[0 + i * 4] = 160;
         colorArray[1 + i * 4] = 32;
         colorArray[2 + i * 4] = 240;
         break;

      case 7:
         colorArray[0 + i * 4] = 255;
         colorArray[1 + i * 4] = 0;
         colorArray[2 + i * 4] = 0;
         break;

      }
   }

   glEnableClientState(GL_COLOR_ARRAY);
   glDisable(GL_TEXTURE_2D);

   glVertexPointer(2, GL_FIXED, 0, vertArray);
   glColorPointer(4,GL_UNSIGNED_BYTE,0,colorArray);
   glDrawElements(GL_TRIANGLES, 6 * 4,GL_UNSIGNED_BYTE,elements);
   checkGlError("glVertexAttribPointer");

   glEnable(GL_TEXTURE_2D);
   glDisableClientState(GL_COLOR_ARRAY);
}

void t_RenderClass::drawArray()
{
   t_fixed testElem[8];

   testElem[0] = 0;
   testElem[1] = 0;

   testElem[2] = 0;
   testElem[3] = 20 * 5;

   testElem[4] = 10 * 5;
   testElem[5] = 20 * 5;

   testElem[6] = 10 * 5;
   testElem[7] = 0 * 5;


   glDisable(GL_TEXTURE_2D);

   glVertexPointer(2, GL_FIXED, 0, testElem);
   glDrawArrays(GL_LINE_LOOP,0,4);
   checkGlError("glVertexAttribPointer");

   glEnable(GL_TEXTURE_2D);


   std::vector<int> xcords;
   std::vector<int> ycords;
   std::vector<int> colors;

   for (int y = 0; y<20; y++)
   {
      for (int x= 0; x<10; x++)
      {
         if (trisArray[y][x])
         {
            xcords.push_back(x);
            ycords.push_back(y);
            colors.push_back(trisArray[y][x]);
         }
      }
   }

   char *elements = new char[xcords.size() * 6];

   for (int i = 0; i<xcords.size(); i++)
   {
      elements[0 + i * 6] = i*4 + 0;
      elements[1 + i * 6] = i*4 + 1;
      elements[2 + i * 6] = i*4 + 2;
      elements[3 + i * 6] = i*4 + 1;
      elements[4 + i * 6] = i*4 + 2;
      elements[5 + i * 6] = i*4 + 3;
   }

   t_fixed *vertArray = new t_fixed[xcords.size() *8];

   for (int i = 0; i<xcords.size(); i++)
   {
      vertArray[0 + i * 8] = (xcords[i] * 5 + -0);
      vertArray[1 + i * 8] = (ycords[i] * 5 + -0) ;

      vertArray[2 + i * 8] = (xcords[i] * 5 + -0);
      vertArray[3 + i * 8] = ((ycords[i]+1) * 5 + -0) ;

      vertArray[4 + i * 8] = ((xcords[i]+1) * 5 + -0) ;
      vertArray[5 + i * 8] = (ycords[i] * 5 + -0) ;

      vertArray[6 + i * 8] = ((xcords[i]+1) * 5 + -0);
      vertArray[7 + i * 8] = ((ycords[i]+1) * 5 + -0);
   }

   unsigned char *colorArray = new unsigned char[xcords.size() * 16];

   for (int i = 0; i<xcords.size()*4; i++)
   {
      colorArray[3 + i * 4] = 255;

      switch (colors[i/4])
      {
      case 1:
         colorArray[0 + i * 4] = 0;
         colorArray[1 + i * 4] = 255;
         colorArray[2 + i * 4] = 255;
         break;

      case 2:
         colorArray[0 + i * 4] = 0;
         colorArray[1 + i * 4] = 0;
         colorArray[2 + i * 4] = 255;
         break;

      case 3:
         colorArray[0 + i * 4] = 255;
         colorArray[1 + i * 4] = 165;
         colorArray[2 + i * 4] = 0;
         break;
      
      case 4:
         colorArray[0 + i * 4] = 255;
         colorArray[1 + i * 4] = 255;
         colorArray[2 + i * 4] = 0;
         break;
      
      case 5:
         colorArray[0 + i * 4] = 0;
         colorArray[1 + i * 4] = 255;
         colorArray[2 + i * 4] = 0;
         break;
      
      case 6:
         colorArray[0 + i * 4] = 160;
         colorArray[1 + i * 4] = 32;
         colorArray[2 + i * 4] = 240;
         break;

      case 7:
         colorArray[0 + i * 4] = 255;
         colorArray[1 + i * 4] = 0;
         colorArray[2 + i * 4] = 0;
         break;

      }
   }

      glEnableClientState(GL_COLOR_ARRAY);
      glDisable(GL_TEXTURE_2D);

      glVertexPointer(2, GL_FIXED, 0, vertArray);
      glColorPointer(4,GL_UNSIGNED_BYTE,0,colorArray);
      glDrawElements(GL_TRIANGLES, 6 * xcords.size(),GL_UNSIGNED_BYTE,elements);
      checkGlError("glVertexAttribPointer");

      glEnable(GL_TEXTURE_2D);
      glDisableClientState(GL_COLOR_ARRAY);
   }

