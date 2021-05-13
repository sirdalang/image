#include "image_tools.h"

#include <stdint.h>
#include <string.h>

#include "def_inner.h"

typedef uint8_t UBYTE;
typedef uint32_t U32_RGBA;
typedef uint16_t U16_1555;

typedef struct C_RGBA
{
    UBYTE red;
    UBYTE green;
    UBYTE blue;
    UBYTE alpha;
} __attribute__((packed)) C_RGBA;

typedef struct C_1555_SmallEndian
{
    UBYTE blue:5;
    UBYTE green:5;
    UBYTE red:5;
    UBYTE alpha:1;
}  __attribute__((packed)) C_1555_SmallEndian;

typedef struct C_1555_BigEndian
{
    UBYTE alpha:1;
    UBYTE red:5;
    UBYTE green:5;
    UBYTE blue:5;
}  __attribute__((packed)) C_1555_BigEndian;

static int imagetools_rawconvert_RGBA_to_1555(const void *pSrc, unsigned int nSrcSize, 
    void *pDst, unsigned int nDstSize)
{
    const U32_RGBA *p32Src = (const U32_RGBA*) pSrc;
    U16_1555 *p16Dst = (U16_1555*) pDst;
    int n32SrcSize = nSrcSize / sizeof(U32_RGBA);
    int n16DstSize = nDstSize / sizeof(U16_1555);

    int bSmallEndian = imagetools_smallendian();

    int i = 0;
    int j = 0;
    for (i = 0, j = 0; i < n32SrcSize && j < n16DstSize; ++i, ++j)
    {
        const C_RGBA *pCRGBA = (const C_RGBA *)(p32Src + i);
        if (bSmallEndian)
        {
            C_1555_SmallEndian *pC1555 = (C_1555_SmallEndian *)(p16Dst + j);
            pC1555->red = (pCRGBA->red & 0xF8) >> 3;
            pC1555->green = (pCRGBA->green & 0xF8) >> 3;
            pC1555->blue = (pCRGBA->blue & 0xF8) >> 3;
            pC1555->alpha = (pCRGBA->alpha > 0x80 ? 1 : 0);
        }
        else 
        {
            C_1555_BigEndian *pC1555 = (C_1555_BigEndian *)(p16Dst + j);
            pC1555->red = (pCRGBA->red & 0xF8) >> 3;
            pC1555->green = (pCRGBA->green & 0xF8) >> 3;
            pC1555->blue = (pCRGBA->blue & 0xF8) >> 3;
            pC1555->alpha = (pCRGBA->alpha > 0x80 ? 1 : 0);
        }
    }

    return 0;
}

int imagetools_rawconvert_HBIT_to_1555(const void *pSrc, unsigned int nSrcSize, 
    void *pDst, unsigned int nDstSize, const void *pPixel1555_0, const void *pPixel1555_1)
{
    const unsigned char bitmap[8] = {0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};

    UBYTE *p8Src = (UBYTE *)pSrc;
    U16_1555 *p16Dst = (U16_1555 *)pDst;
    U16_1555 *p16Pixel1555_0 = (U16_1555*)pPixel1555_0;
    U16_1555 *p16Pixel1555_1 = (U16_1555*)pPixel1555_1;
    unsigned int n8SrcSize = nSrcSize;
    unsigned int n16DstSize = nDstSize / 2;

    unsigned int i = 0;     
    unsigned int j = 0;
    for (i = 0,j = 0; i < n8SrcSize && j < n16DstSize; ++i)
    {
        for (int k = 0; k < 8 && j < n16DstSize; ++k, ++j)
        {
            if (p8Src[i] & bitmap[k])
            {
                p16Dst[j] = *(p16Pixel1555_1);
            }
            else 
            {
                p16Dst[j] = *(p16Pixel1555_0);
            }
        }
    }

    return 0;
}

static int imagetools_rawreplace_RGBA(void *pPixels, unsigned int nPixelCount, 
    const void *pOldPixel, const void *pNewPixel)
{
    U32_RGBA *p32Src = (U32_RGBA *)pPixels;

    unsigned int i = 0;
    unsigned int nCount = 0;
    for (i = 0; i < nPixelCount; ++i)
    {
        if (memcmp (& p32Src[i], pOldPixel, sizeof(U32_RGBA)) == 0)
        {
            memcpy (& p32Src[i], pNewPixel, sizeof(U32_RGBA));
            ++nCount;
        }
    }

    return nCount;
}

static int imagetools_rawreplace_1555(void *pPixels, unsigned int nPixelCount, 
    const void *pOldPixel, const void *pNewPixel)
{
    U16_1555 *p16Src = (U16_1555 *)pPixels;

    unsigned int i = 0;
    unsigned int nCount = 0;
    for (i = 0; i < nPixelCount; ++i)
    {
        if (memcmp (& p16Src[i], pOldPixel, sizeof(U16_1555)) == 0)
        {
            memcpy (& p16Src[i], pNewPixel, sizeof(U16_1555));
            ++nCount;
        }
    }

    return nCount;
}

int imagetools_smallendian()
{
    unsigned int u32 = 0x1;
    char *pu8 = (char*)&u32;
    if (*pu8)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int imagetools_rawconvert (const void *pSrc, unsigned int nSrcSize, 
    void *pDst, unsigned int nDstSize, IMAGE_RAW_CONVERT_TYPE_E eConvType)
{
    int ret = -1;
    switch (eConvType)
    {
        case IMAGE_RAW_RGBA_1555:
        {
            ret = imagetools_rawconvert_RGBA_to_1555 (pSrc, nSrcSize, pDst, nDstSize);
            break;
        }
        case IMAGE_RAW_HBIT_1555_1BLACK_1ALPHA:
        {
            U16_1555 pixel_1 = 0x8000;
            U16_1555 pixel_0 = 0xFFFF;
            ret = imagetools_rawconvert_HBIT_to_1555 (pSrc, nSrcSize, pDst, nDstSize, &pixel_0, &pixel_1);
            break;
        }
        case IMAGE_RAW_HBIT_1555_1BLACK_0ALPHA:
        {
            U16_1555 pixel_1 = 0x0000;
            U16_1555 pixel_0 = 0x7FFF;
            ret = imagetools_rawconvert_HBIT_to_1555 (pSrc, nSrcSize, pDst, nDstSize, &pixel_0, &pixel_1);
            break;
        }
        case IMAGE_RAW_HBIT_1555_1WHITE_1ALPHA:
        {
            U16_1555 pixel_1 = 0xFFFF;
            U16_1555 pixel_0 = 0x8000;
            ret = imagetools_rawconvert_HBIT_to_1555 (pSrc, nSrcSize, pDst, nDstSize, &pixel_0, &pixel_1);
            break;
        }
        case IMAGE_RAW_HBIT_1555_1WHITE_0ALPHA:
        {
            U16_1555 pixel_1 = 0x7FFF;
            U16_1555 pixel_0 = 0x0000;
            ret = imagetools_rawconvert_HBIT_to_1555 (pSrc, nSrcSize, pDst, nDstSize, &pixel_0, &pixel_1);
            break;
        }
        default:
        {
            _error ("unexpected type=%d\n", eConvType);
            break;
        }
    }
    return ret;
}

int imagetools_replaceall (void *pPixels, unsigned int nPixelCount, 
    const void *pOldPixel, const void *pNewPixel, IMAGE_RAW_TYPE_E eImageRawType)
{
    int ret = -1;
    switch (eImageRawType)
    {
        case IMAGE_RAW_1555:
        {
            ret = imagetools_rawreplace_1555 (pPixels, nPixelCount, pOldPixel, pNewPixel);
            break;   
        }
        case IMAGE_RAW_RGBA:
        {
            ret = imagetools_rawreplace_RGBA (pPixels, nPixelCount, pOldPixel, pNewPixel);
            break;  
        }
        default:
        {
            _error ("unexpected type=%d\n", eImageRawType);
            break;
        }
    }
    return ret;
}
