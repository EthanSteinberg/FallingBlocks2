#include "renderClass.h"
#include <android/log.h>

//#include <GLES/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include <stdlib.h>

#include "png/png.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void checkGlError(const char *op)
{
   for (GLint error = glGetError(); error; error
         = glGetError())
   {
      LOGI("after %s() glError (0x%x)\n", op, error);
   }
}

void t_RenderClass::loadTexture(int target, const char *filename)
{
   FILE *theFile = fopen(filename,"rb");

   if (!theFile)
   {
      LOGI("Fopen failed");
      exit(1);
   }

   char header[8];
   fread(header,1,8,theFile);

   if (ferror(theFile))
   {
      LOGI("Fread failed");
      exit(1);
   }

   if (png_sig_cmp((png_byte *) header,0,8))
   {
      LOGI("The file is not a png\n");
      exit(1);
   }

   png_structp png_ptr = png_create_read_struct(
                            PNG_LIBPNG_VER_STRING,
                            NULL,NULL,NULL
                         );

   if (!png_ptr)
   {
      printf("Failed initialization of png_ptr\n");
      exit(1);
   }

   png_infop info_ptr = png_create_info_struct(png_ptr);

   if (!info_ptr)
   {
      png_destroy_read_struct(&png_ptr,
                              (png_infopp)NULL, (png_infopp)NULL);
      printf("Info ptr creation failed\n");
      exit(1);
   }

   png_infop end_info = png_create_info_struct(png_ptr);

   if (!end_info)
   {
      png_destroy_read_struct(&png_ptr, &info_ptr,
                              (png_infopp)NULL);
      printf("Info ptr creation failed\n");
      exit(1);
   }

   png_init_io(png_ptr,theFile);
   png_set_sig_bytes(png_ptr,8);

   png_read_info(png_ptr,info_ptr);
   int width = png_get_image_width(png_ptr,info_ptr);
   int height = png_get_image_width(png_ptr,info_ptr);
   int depth = png_get_bit_depth(png_ptr,info_ptr);
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

   png_byte* image_data = new png_byte[rowbytes * height];

   png_bytep *row_pointers = new png_bytep [height];

   for (int i = 0; i < height; ++i)
   {
      row_pointers[i] = image_data + i * rowbytes;
   }


   LOGI("I have read the image with width %d, height %d, depth %d, rowbytes %d\n",width,height,depth,rowbytes);

   png_read_image(png_ptr,row_pointers);

   png_read_end(png_ptr,NULL);
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);


   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image_data);
   checkGlError("glClear");

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   checkGlError("glClear");

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   checkGlError("glClear");

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   checkGlError("glClear");

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   checkGlError("glClear");
}
