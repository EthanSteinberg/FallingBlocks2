#include "renderClass.h"
#include "fixedPoint.h"
#include <string>
#include <ctime>

#include <map>

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

bool t_RenderClass::setupGraphics(int w, int h)
{
   move(0, 0);

   glEnable(GL_TEXTURE_2D);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   printGLString("Version", GL_VERSION);
   printGLString("Vendor", GL_VENDOR);
   printGLString("Renderer", GL_RENDERER);
   printGLString("Extensions", GL_EXTENSIONS);

   LOGI("setupGraphics(%d, %d)", w, h);

   glViewport(0, 0, w, h);
   theWidth = w;
   theHeight = h;

   glOrthox((t_fixed) 0.0,(t_fixed)(100.0) ,(t_fixed) -10.0, (t_fixed)((100.0 *((float)h)/w) -10.0) ,(t_fixed) -5.0, (t_fixed)5.0);
   checkGlError("glViewport");

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);


   return true;

   timespec ti;

   if (clock_gettime(CLOCK_MONOTONIC, &ti) == -1)
   {
      LOGE("The gettime failed");
      exit(1);
   }

   microSeconds = ti.tv_nsec;
   seconds = ti.tv_sec;

}

void t_RenderClass::renderFrame()
{
   static int frameNum = 0;
   static char buffer[100];

   glClearColorx((t_fixed) 0.5, (t_fixed)  0.5 , (t_fixed)  0.5 , (t_fixed)  1.0);
   checkGlError("glClearColor");
   glClear(GL_COLOR_BUFFER_BIT);
   checkGlError("glClear");


   drawArray();
   drawPiece();


   const int TIMES = 10;

   if (frameNum++ %TIMES == 0)
   {
      timespec ti;

      if (clock_gettime(CLOCK_MONOTONIC, &ti) == -1)
      {
         LOGE("The gettime failed");
         exit(1);
      }

      float fps = TIMES / (((signed long)ti.tv_nsec - (signed long)microSeconds) * 1.0e-9
                           + (ti.tv_sec - seconds));
         //);
      microSeconds = ti.tv_nsec;
      seconds = ti.tv_sec;

      snprintf(buffer, sizeof(buffer), "FPS: %d", (int) fps);

   }

   drawText(buffer,0,-10);
   checkGlError("glDrawArrays");
}

void t_RenderClass::move(float w, float h)
{
   
   if (w > theWidth/2)
      moveRight();

   else
      moveLeft();
}

void t_RenderClass::loadTextures()
{
   glGenTextures(2, textures);

   glBindTexture(GL_TEXTURE_2D, textures[0]);
   glActiveTexture(GL_TEXTURE0);

   loadTexture(0, "/mnt/sdcard/try6.png");
   glBindTexture(GL_TEXTURE_2D, textures[1]);

   loadText(40);
   glBindTexture(GL_TEXTURE_2D, textures[0]);
}
