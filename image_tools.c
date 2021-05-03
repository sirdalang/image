#include "image_tools.h"

#include <stdint.h>

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

typedef struct C_1555
{
    UBYTE red:5;
    UBYTE green:5;
    UBYTE blue:5;
    UBYTE alpha:1;
}  __attribute__((packed)) C_1555;

static int imagetools_rawconvert_RGBA_to_1555(const void *pSrc, unsigned int nSrcSize, 
    void *pDst, unsigned int nDstSize)
{
    const C_RGBA *p32Src = (const C_RGBA*) pSrc;
    C_1555 *p16Dst = (C_1555*) pDst;
    int n32SrcSize = nSrcSize / sizeof(C_RGBA);
    int n16DstSize = nDstSize / sizeof(C_1555);

    int i = 0;
    int j = 0;
    for (i = 0, j = 0; i < n32SrcSize && j < n16DstSize; ++i, ++j)
    {
        const C_RGBA *pCRGBA = p32Src + i;
        C_1555 *pC1555 = p16Dst + j;
        pC1555->red = (pCRGBA->red & 0xF8) >> 3;
        pC1555->green = (pCRGBA->green & 0xF8) >> 3;
        pC1555->blue = (pCRGBA->blue & 0xF8) >> 3;
        pC1555->alpha = (pCRGBA->alpha > 0x80 ? 1 : 0);
    }

    return 0;
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
        default:
        {
            _error ("unexpected type=%d\n", eConvType);
            break;
        }
    }
    return ret;
}