void move(float width,float height);
#include <jni.h>
#include "renderClass.h"

#include <GLES/gl.h>
#include <GLES/glext.h>

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


extern "C" {

   JNIEXPORT void JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_init
   (JNIEnv *, jclass, jint, jint,jlong);

   JNIEXPORT void JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_step
   (JNIEnv *, jclass,jlong);

   JNIEXPORT void JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_pushed
   (JNIEnv *s , jclass ss, jfloat width, jfloat height,jlong);

   JNIEXPORT void JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_createTexts
   (JNIEnv *s , jclass ss,jlong);

   JNIEXPORT jlong JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_create
   (JNIEnv *s , jclass ss);
};

JNIEXPORT void JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_init
(JNIEnv *s , jclass ss, jint width, jint height, jlong thandle)
{
   t_RenderClass *handle = (t_RenderClass *) thandle;
   handle->setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_step
(JNIEnv *s, jclass ss, jlong thandle)
{
   t_RenderClass *handle = (t_RenderClass *) thandle;
   handle->renderFrame();
}

JNIEXPORT void JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_pushed
(JNIEnv *s , jclass ss, jfloat width, jfloat height, jlong thandle)
{

   t_RenderClass *handle = (t_RenderClass *) thandle;
   handle->move(width,height);
}

JNIEXPORT void JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_createTexts
(JNIEnv *s , jclass ss, jlong thandle)
{

   t_RenderClass *handle = (t_RenderClass *) thandle;
   handle->loadTextures();

}
JNIEXPORT jlong JNICALL Java_com_steinberg_ethan_FallingBlocks_OpenGLStuff_create
(JNIEnv *s , jclass ss)
{
   jlong handle =(jlong) new t_RenderClass;
   return handle;
}



