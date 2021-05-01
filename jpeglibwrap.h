/**
 * Author: x
 * Date: 20210501
 * Description: Wrapper of libjpeg
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif 

typedef void * HANDLE_JPEGWRAP;

typedef struct JpegWrap_Pixel
{
    char R;
    char G;
    char B;
    char A;
} JpegWrap_Pixel;

HANDLE_JPEGWRAP     jpegwrap_create(const char *szFile);
int                 jpegwrap_destroy(HANDLE_JPEGWRAP handle);

int jpegwrap_size(HANDLE_JPEGWRAP handle, int *pW, int *pH);

int jpegwrap_get(HANDLE_JPEGWRAP handle, JpegWrap_Pixel *pPixel, int i, int j);

#ifdef __cplusplus
}
#endif