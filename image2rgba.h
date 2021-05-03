/**
 * Author: x
 * Date: 20210501
 * Description: image to rgba
 */

#pragma once

#include "def.h"

#ifdef __cplusplus
extern "C" {
#endif 

HANDLE_IMAGE2RGBA       image2rgba_open(const char *szFile);
int                     image2rgba_close(HANDLE_IMAGE2RGBA handle);

int image2rgba_getsize(HANDLE_IMAGE2RGBA handle, int *pW, int *pH);
int image2rgba_getpixel(HANDLE_IMAGE2RGBA handle, int i, int j, Image2RGBA_Pixel *pPixel);

#ifdef __cplusplus
}
#endif