LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# compile in ARM mode, since the glyph loader/renderer is a hotspot
# when loading complex pages in the browser
#
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES:= \
	freetype/src/base/ftbbox.c \
	freetype/src/base/ftbitmap.c \
	freetype/src/base/ftglyph.c \
	freetype/src/base/ftstroke.c \
	freetype/src/base/ftxf86.c \
	freetype/src/base/ftbase.c \
	freetype/src/base/ftsystem.c \
	freetype/src/base/ftinit.c \
	freetype/src/base/ftgasp.c \
	freetype/src/raster/raster.c \
	freetype/src/sfnt/sfnt.c \
	freetype/src/smooth/smooth.c \
	freetype/src/autofit/autofit.c \
	freetype/src/truetype/truetype.c \
	freetype/src/cff/cff.c \
	freetype/src/psnames/psnames.c \
	freetype/src/pshinter/pshinter.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/freetype/builds \
	$(LOCAL_PATH)/freetype/include

LOCAL_CFLAGS += -W -Wall
LOCAL_CFLAGS += -fPIC -DPIC
LOCAL_CFLAGS += "-DDARWIN_NO_CARBON"
LOCAL_CFLAGS += "-DFT2_BUILD_LIBRARY"

LOCAL_CFLAGS += -O2

LOCAL_MODULE:= libft2

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libpng
LOCAL_SRC_FILES := \
	png/png.c\
	png/pngerror.c \
	png/pngget.c \
	png/pngmem.c \
	png/pngpread.c \
	png/pngread.c \
	png/pngrio.c \
	png/pngrtran.c \
	png/pngrutil.c \
	png/pngset.c \
	png/pngtrans.c \
	png/pngwio.c \
	png/pngwrite.c \
	png/pngwtran.c \
	png/pngwutil.c 

LOCAL_LDLIBS := -lz

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := glStuff3
LOCAL_SRC_FILES := \
	yeah.cpp\
  	loadTextures.cpp\
  	loadText.cpp\
  	myVector2.cpp\
  	algoMaxRects.cpp\
  	myBox.cpp\
	drawArray.cpp\
	renderClass.cpp\
	tetrisMath.cpp
	
LOCAL_STATIC_LIBRARIES := libpng libft2
LOCAL_LDLIBS := -lz -llog -lGLESv1_CM
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/freetype/builds \
	$(LOCAL_PATH)/freetype/include

LOCAL_CFLAGS += -std=gnu++0x

include $(BUILD_SHARED_LIBRARY)
