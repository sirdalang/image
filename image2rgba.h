/**
 * Author: x
 * Date: 20210501
 * Description: image to rgba
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct Pixel
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
} Image2RGBA_Pixel;

typedef void* HANDLE_IMAGE2RGBA;

HANDLE_IMAGE2RGBA       image2rgba_open(const char *szFile);
int                     image2rgba_close(HANDLE_IMAGE2RGBA handle);

int image2rgba_getsize(HANDLE_IMAGE2RGBA handle, int *pW, int *pH);
int image2rgba_getpixel(HANDLE_IMAGE2RGBA handle, int i, int j, Image2RGBA_Pixel *pPixel);

int image2rgba_replaceall(HANDLE_IMAGE2RGBA handle, const Image2RGBA_Pixel *cpOldPixel, const Image2RGBA_Pixel *cpNewPixel);

#ifdef __cplusplus
}
#endif