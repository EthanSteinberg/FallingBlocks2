#include "renderClass.h"
#include <algorithm>
#include <cstdlib>


void t_RenderClass::moveRight()
{
   corex++;

   if (!checkValid())
   {
      corex--;
   }
}

void t_RenderClass::moveLeft()
{
   corex--;

   if (!checkValid())
   {
      corex++;
   }
}

bool t_RenderClass::checkValid()
{
   for (int i = 0; i <4; i++)
   {
      int tempx = corex + xpieceArray[i];
      int tempy = corey + ypieceArray[i];

      if (tempx < 0 || tempx > 9 || tempy < 0 || tempy > 19 || trisArray[tempy][tempx])
      {
         return 0;
      }
   }
}

void t_RenderClass::rotateLeft()
{
   for (int i = 0; i <4; i++)
   {
      std::swap(xpieceArray[i],ypieceArray[i]);
      ypieceArray[i] *= -1;
   }

   if (!checkValid())
      for (int i = 0; i <4; i++)
      {
         std::swap(xpieceArray[i],ypieceArray[i]);
         xpieceArray[i] *= -1;
      }
}

void t_RenderClass::rotateRight()
{
   for (int i = 0; i <4; i++)
   {
      std::swap(xpieceArray[i],ypieceArray[i]);
      xpieceArray[i] *= -1;
   }

   if (!checkValid())
      for (int i = 0; i <4; i++)
      {
         std::swap(xpieceArray[i],ypieceArray[i]);
         ypieceArray[i] *= -1;
      }
}


void t_RenderClass::moveDown()
{
   corey--;
   
   if (!checkValid())
   {
      corey++;
      makeStatic();
      clearRows();
   }
}

void t_RenderClass::clearRows()
{
   int bonus = 1;

   for (int y = 0; y< 20;y++)
   {
      bool rowComplete = true;

      for (int x = 0; x<10;x++)
      {
         if (!trisArray[y][x])
         {
            rowComplete = false;
            break;
         }
      }

      if (rowComplete)
      {
         score += 100 * bonus;
         bonus*=2;
         
         for (int ty = y; ty < 20;ty++)
         {
            memcpy(trisArray[ty],trisArray[ty +1],10 * sizeof(trisArray[0][0]));
         }
      }
   }
}


void t_RenderClass::makeStatic()
{
   for (int i = 0; i <4; i++)
   {
      int tempx = corex + xpieceArray[i];
      int tempy = corey + ypieceArray[i];

      trisArray[tempy][tempx] = pieceColor;
   }

   pieceColor = rand() %7 + 1;
   int shape = rand() %7;
   corex = 4;
   corey = 18;

   switch (shape)
   {
      case 0:
         xpieceArray[0] = -1;
         xpieceArray[1] = -1;
         xpieceArray[3] =  0;

         ypieceArray[0] =  1;
         ypieceArray[1] =  0;
         ypieceArray[3] = -1;
         break;
      
      case 1:
         xpieceArray[0] =  1;
         xpieceArray[1] =  1;
         xpieceArray[3] =  0;

         ypieceArray[0] =  1;
         ypieceArray[1] =  0;
         ypieceArray[3] = -1;
         break;
      
      case 2:
         xpieceArray[0] = -1;
         xpieceArray[1] =  0;
         xpieceArray[3] =  1;

         ypieceArray[0] =  0;
         ypieceArray[1] =  1;
         ypieceArray[3] =  0;
         break;

      case 3:
         xpieceArray[0] =  0;
         xpieceArray[1] =  0;
         xpieceArray[3] =  1;

         ypieceArray[0] = -1;
         ypieceArray[1] = -2;
         ypieceArray[3] =  0;
         break;
      
      case 4:
         xpieceArray[0] =  0;
         xpieceArray[1] =  0;
         xpieceArray[3] = -1;

         ypieceArray[0] = -1;
         ypieceArray[1] = -2;
         ypieceArray[3] =  0;
         break;
      
      case 5:
         xpieceArray[0] =  1;
         xpieceArray[1] =  1;
         xpieceArray[3] =  0;

         ypieceArray[0] =  0;
         ypieceArray[1] = -1;
         ypieceArray[3] = -1;
         break;

      case 6:
         xpieceArray[0] = -2;
         xpieceArray[1] = -1;
         xpieceArray[3] =  1;

         ypieceArray[0] =  0;
         ypieceArray[1] =  0;
         ypieceArray[3] =  0;
         break;
   }

}
