/**
 * Author: x
 * Date: 20210503
 * Description: rgba --> 1555,...
 * 
 * 说明：
 * 1555 格式的字节排列按照书写的顺序排列，或者说是大端字节序（即地址从低到高依次是alpha,red,green,blue）。
 * 0x0000 表示黑色
 * 0x8C00 表示红色
 */

#pragma once

typedef enum 
{
    IMAGE_RAW_RGBA_1555 = 0x1,
    IMAGE_RAW_HBIT_1555_1BLACK_1ALPHA,        // HBIT: BYTE & 0x80 表示第一个像素，1 表示 黑色，alpha 为 1
    IMAGE_RAW_HBIT_1555_1BLACK_0ALPHA,        // HBIT: BYTE & 0x80 表示第一个像素，1 表示 黑色，alpha 为 0
    IMAGE_RAW_HBIT_1555_1WHITE_1ALPHA,        // HBIT: BYTE & 0x80 表示第一个像素，1 表示 白色，alpha 为 1
    IMAGE_RAW_HBIT_1555_1WHITE_0ALPHA,        // HBIT: BYTE & 0x80 表示第一个像素，1 表示 白色，alpha 为 0
} IMAGE_RAW_CONVERT_TYPE_E;

typedef enum
{
    IMAGE_RAW_RGBA = 0x1,
    IMAGE_RAW_1555,
} IMAGE_RAW_TYPE_E;

#ifdef __cplusplus
extern "C" {
#endif 

int imagetools_rawconvert (const void *pSrc, unsigned int nSrcSize, 
    void *pDst, unsigned int nDstSize, IMAGE_RAW_CONVERT_TYPE_E eConvType);

int imagetools_rawconvert_HBIT_to_1555(const void *pSrc, unsigned int nSrcSize, 
    void *pDst, unsigned int nDstSize, const void *pPixel1555_0, const void *pPixel1555_1);

int imagetools_rawconvert_HBIT_to_RGBA(const void *pSrc, unsigned int nSrcSize, 
    void *pDst, unsigned int nDstSize, const void *pPixelRGBA_0, const void *pPixelRGBA_1);

int imagetools_scale(const void *pSrc, unsigned int nSrcW, unsigned int nSrcH, 
    void *pDst, unsigned int nDstW, unsigned int nDstH, IMAGE_RAW_TYPE_E eImageRawType);

int imagetools_smallendian();

int imagetools_replaceall (void *pPixels, unsigned int nPixelCount, 
    const void *pOldPixel, const void *pNewPixel, IMAGE_RAW_TYPE_E eImageRawType);

int imagetools_settwocolor (void *pPixels, unsigned int nPixelCount, 
    const void *pBackColor, const void *pBackToColor, const void* pFrontToColor, IMAGE_RAW_TYPE_E eImageRawType);

#ifdef __cplusplus
}
#endif