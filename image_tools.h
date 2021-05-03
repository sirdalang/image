/**
 * Author: x
 * Date: 20210503
 * Description: rgba --> 1555,...
 */

#pragma once

typedef enum 
{
    IMAGE_RAW_RGBA_1555 = 0x1,
} IMAGE_RAW_CONVERT_TYPE_E;

#ifdef __cplusplus
extern "C" {
#endif 

int imagetools_rawconvert (const void *pSrc, unsigned int nSrcSize, 
    void *pDst, unsigned int nDstSize, IMAGE_RAW_CONVERT_TYPE_E eConvType);

int imagetools_smallendian();

#ifdef __cplusplus
}
#endif