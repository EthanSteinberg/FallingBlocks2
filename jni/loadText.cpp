#include "renderClass.h"
#include <map>
#include <string>

#include <cmath>
#include "fixedPoint.h"

#include "myVector2.h"
#include "myBox.h"

#include "algoMaxRects.h"

#include <cstdio>

#include <GLES/gl.h>
#include <GLES/glext.h>

#include <android/log.h>
#include <cstdlib>

#include <freetype/include/ft2build.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#include FT_FREETYPE_H
#include FT_GLYPH_H


const int buf = 1;

void checkFaceFlags(long faceFlags);

template <class T>
T nexthigher(T k)
{
   k--;

   for (unsigned int i=1; i<sizeof(T)*8; i<<=1)
   {
      k = k | k >> i;
   }

   return k+1;
}

FT_Face face;

char *buffer;
int sidew;
int sideh;
std::map<int,t_myBox> charMap;
std::map<int,int> advancex;
std::map<int,int> minYs;
std::map<int,int> bearingXs;

void t_RenderClass::drawText(std::string text,float startX, float startY)
{

   int number = text.length();
   //LOGI("The number was %d",number);

   glBindTexture(GL_TEXTURE_2D,textures[1]);
   char *gElements = new char[number * 6];

   for (int i =0 ; i < number; i ++)
   {
      gElements[i * 6 + 0] = 0 + i * 4;
      gElements[i * 6 + 1] = 1 + i * 4;
      gElements[i * 6 + 2] = 2 + i * 4;
      gElements[i * 6 + 3] = 1 + i * 4;
      gElements[i * 6 + 4] = 2 + i * 4;
      gElements[i * 6 + 5] = 3 + i * 4;
   }

   t_fixed *textcords = new t_fixed[number * 8];
   t_fixed *drawcords = new t_fixed[number * 8];
   float advx = 0;

   for (int i = 0 ; i < number; i++)
   {

      FT_Vector kerning;
      kerning.x = 0;
      kerning.y = 0;

      if (i +1 < number)
      {
         int left = FT_Get_Char_Index(face,text[i]);
         int right = FT_Get_Char_Index(face,text[i+1]);
         int error = FT_Get_Kerning(face,left,right,FT_KERNING_DEFAULT,&kerning);

         if (error)
         {
            LOGE("Kerning failed");
            exit(1);
         }
      }


      t_myBox box= charMap[text[i]];
      float yMin = minYs[text[i]];
      float x = box.pos.x + buf;
      float y = box.pos.y + buf;
      float sizex = box.size.x - 2*buf;
      float sizey = box.size.y - 2*buf;

      advx += (float)bearingXs[text[i]]/(1<<6);

      drawcords[ i * 8 + 0] = (startX  + 100* (advx)/(sidew/1));
      drawcords[ i * 8 + 1] = (startY  + 100* yMin/(sideh/1));

      drawcords[ i * 8 + 2] = (startX  + 100* (advx)/(sidew/1));
      drawcords[ i * 8 + 3] = (startY  + 100* (sizey +yMin)/(sideh/1));

      advx += sizex;

      drawcords[ i * 8 + 4] = (startX  + 100* (advx)/(sidew/1));
      drawcords[ i * 8 + 5] = (startY  + 100* yMin/(sideh/1));

      drawcords[ i * 8 + 6] = (startX  + 100* (advx)/(sidew/1));
      drawcords[ i * 8 + 7] = (startY  + 100* (sizey +yMin)/(sideh/1));

      advx += (float)advancex[text[i]]/(1<<16) - sizex - (float) bearingXs[text[i]]/(1<<6) + (float)kerning.x/(1<<6);

      textcords[i * 8 +0] = x/sidew;
      textcords[i * 8 +1] = (y + sizey)/sideh;

      textcords[i * 8 +2] = x/sidew;
      textcords[i * 8 +3] = y/sideh;

      textcords[i * 8 +4] = (x + sizex)/sidew;
      textcords[i * 8 +5] = (y + sizey)/sideh;

      textcords[i * 8 +6] = (x + sizex)/sidew;
      textcords[i * 8 +7] = y/sideh;
   }

   glVertexPointer(2, GL_FIXED,  0, drawcords);
   glTexCoordPointer(2, GL_FIXED,  0, textcords);

   glDrawElements(GL_TRIANGLES, 6 * number,GL_UNSIGNED_BYTE,gElements);

   glBindTexture(GL_TEXTURE_2D,textures[0]);
}


void t_RenderClass::loadText(int fontSize)
{
   FT_Library lib;
   int error = FT_Init_FreeType(&lib);

   if (error)
   {
      LOGI("An error in loading the library\n");
      exit(0);
   }

   error = FT_New_Face(lib,"/mnt/sdcard/DejaVuSans.ttf",0,&face);

   if (error)
   {
      LOGI("An error in loading the face\n");
      exit(0);
   }


   long faceFlags = face->face_flags;
   checkFaceFlags(faceFlags);

   int emUnits = face->units_per_EM;
   int numSizes = face->num_fixed_sizes;
   //LOGI("The units are %d with %d sizes\n",emUnits,numSizes);

   error = FT_Set_Char_Size(face,0,fontSize* 64, 120,120);

   if (error)
   {
      LOGI("An error in loading the face\n");
      exit(0);
   }


   const int start = ' ';

   const int end = '~';

   FT_Glyph characters[end - start + 1];

   std::vector<t_myVector2> sizes(end - start + 1);

   int area = 0;

   for (int character = start; character <= end; character++)
   {
      int glyph = FT_Get_Char_Index(face, character);

      //LOGI("The piece of %c has an index %d \n",character,glyph);

      error = FT_Load_Glyph(face,glyph, FT_LOAD_RENDER | FT_LOAD_TARGET_LCD);

      if (error)
      {
         LOGI("An error in loading the face\n");
         exit(0);
      }

      error = FT_Get_Glyph(face->glyph,&characters[character - start]);

      if (error)
      {
         LOGI("An error in loading the face\n");
         exit(0);
      }

      //LOGI("The size was %d %d\n\n",face->glyph->bitmap.width, face->glyph->bitmap.rows);
      sizes[character - start] = t_myVector2(face->glyph->bitmap.width/3 + 2*buf, face->glyph->bitmap.rows + 2*buf);

      bearingXs.insert(std::make_pair(character,face->glyph->metrics.horiBearingX));

      area += (face->glyph->bitmap.width/3 + 2*buf) * (face->glyph->bitmap.rows +2*buf);
   }


   int side = nexthigher((int) sqrt((float) area));
   //sidew = side/2;
   sidew = side;
   sideh = side;
   t_myVector2 size(sidew,sideh);

   bool done = 0;

   t_algoMaxRects algo;
   std::multimap<t_myVector2,t_myVector2> theMap = algo.pack(sizes,size,done);

   if (!done)
   {

      sidew*=2;
      t_myVector2 size(sidew,sideh);
      theMap = algo.pack(sizes,size,done);

   }

   LOGI("\nThe side is %d %dand %d\n",sidew,sideh,done);


   buffer = (char *)calloc((sidew*4) * sideh ,1);

   if (!buffer)
   {
      LOGI("No memory\n");
      exit(1);
   }

   for (int ch = start ; ch <= end; ch++)
   {
      t_myVector2 theSize = sizes[ch - start];
      FT_BitmapGlyph image = (FT_BitmapGlyph) characters[ch - start];
      std::multimap<t_myVector2, t_myVector2>::iterator  iter = theMap.find(theSize);

      if (iter == theMap.end())
      {
         LOGI("Not an iter?\n");
         continue;
      }

      t_myVector2 theLocation = iter->second;
      theMap.erase(iter);

      charMap.insert(std::make_pair(ch,t_myBox(theLocation,theSize)));
      advancex.insert(std::make_pair(ch,characters[ch-start]->advance.x));

      FT_BBox bbox;
      FT_Glyph_Get_CBox(characters[ch -start], FT_GLYPH_BBOX_PIXELS,&bbox);
      minYs.insert(std::make_pair(ch,bbox.yMin));




      for (int y = buf; y< (theSize.y-2*buf); y++)
      {


         for (int x = buf; x < (theSize.x-2*buf); x++)
         {
            int r  =  image->bitmap.buffer[(y-buf) * (image->bitmap.pitch) + (x-buf)*3+0];
            int g  =  image->bitmap.buffer[(y-buf) * (image->bitmap.pitch) + (x-buf)*3+1];
            int b  =  image->bitmap.buffer[(y) * (image->bitmap.pitch) + (x-buf)*3+2];
            buffer[theLocation.x*4 + ((theLocation.y + y)) * sidew * 4 +x * 4 + 0] =  r;
            buffer[theLocation.x*4 + ((theLocation.y + y)) * sidew * 4 +x * 4 + 1] =  g;
            buffer[theLocation.x*4 + ((theLocation.y + y)) * sidew * 4 +x * 4 + 2] =  b;
            buffer[theLocation.x*4 + ((theLocation.y + y)) * sidew * 4 +x * 4 + 3] = (r+ g +b)/3 ;
         }

      }


   }


   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,sidew,sideh,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
//checkGlError("glClear");

   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//checkGlError("glClear");

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//checkGlError("glClear");

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//checkGlError("glClear");

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//checkGlError("glClear");
}

void checkFaceFlags(long faceFlags)
{
   if (faceFlags & FT_FACE_FLAG_SCALABLE)
   {
      LOGI("It is scalable\n");
   }

   if (faceFlags & FT_FACE_FLAG_FIXED_SIZES)
   {
      LOGI("It is fixed sizes\n");
   }

   if (faceFlags & FT_FACE_FLAG_FIXED_WIDTH)
   {
      LOGI("It is fixed width\n");
   }

   if (faceFlags & FT_FACE_FLAG_SFNT)
   {
      LOGI("It is sfnt?\n");
   }

   if (faceFlags & FT_FACE_FLAG_HORIZONTAL)
   {
      LOGI("It is horizontal\n");
   }

   if (faceFlags & FT_FACE_FLAG_VERTICAL)
   {
      LOGI("It is vertical\n");
   }

   if (faceFlags & FT_FACE_FLAG_KERNING)
   {
      LOGI("It is kerning\n");
   }

   if (faceFlags & FT_FACE_FLAG_FAST_GLYPHS)
   {
      LOGI("It is fast glyphs\n");
   }

   if (faceFlags & FT_FACE_FLAG_MULTIPLE_MASTERS)
   {
      LOGI("It is multiple masters\n");
   }

   if (faceFlags & FT_FACE_FLAG_GLYPH_NAMES)
   {
      LOGI("It is glyph names\n");
   }

   if (faceFlags & FT_FACE_FLAG_EXTERNAL_STREAM)
   {
      LOGI("It is external stream\n");
   }

   if (faceFlags & FT_FACE_FLAG_HINTER)
   {
      LOGI("It is hinter\n");
   }

   if (faceFlags & FT_FACE_FLAG_CID_KEYED)
   {
      LOGI("It is cid keyed\n");
   }

   if (faceFlags & FT_FACE_FLAG_TRICKY)
   {
      LOGI("It is tricky\n");
   }
}
