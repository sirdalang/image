
/**
 * Author: x
 * Date: 20210501
 * Description: C Wrapper of EasyBMP
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif 

typedef void * HANDLE_EASYBMP;

typedef struct EasyBmp_Pixel
{
    char R;
    char G;
    char B;
    char A;
} EasyBmp_Pixel;

HANDLE_EASYBMP  easybmpc_create();
int             easybmpc_destroy(HANDLE_EASYBMP hBMP);

int easybmpc_open(HANDLE_EASYBMP hBMP, const char *szFile);
int easybmpc_size(HANDLE_EASYBMP hBMP, int *pW, int *pH);

int easybmpc_get(HANDLE_EASYBMP hBMP, EasyBmp_Pixel *pPixel, int i, int j);

#ifdef __cplusplus
}
#endif