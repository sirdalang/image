
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>

#include <string.h>
#include <stdlib.h>

#include "image2rgba.h"

#define DEBUG

#ifdef DEBUG
#define _debug(x...) do {printf("[debug][%s %d %s]", \
	__FILE__,__LINE__,__FUNCTION__);printf(x);} while (0)
#define _info(x...) do {printf("[info][%s %d %s]", \
	__FILE__,__LINE__,__FUNCTION__);printf(x);} while (0)
#define _error(x...) do {printf("[error][%s %d %s]", \
	__FILE__,__LINE__,__FUNCTION__);printf(x);} while (0)
#else 
#define _debug(x...) do {;} while (0)
#define _info(x...) do {printf("[info][%s %d %s]", \
	__FILE__,__LINE__,__FUNCTION__);printf(x);} while (0)
#define _error(x...) do {printf("[error][%s %d %s]", \
	__FILE__,__LINE__,__FUNCTION__);printf(x);} while (0)
#endif 

typedef enum 
{
    COMM_RGB_1555,
} COMM_CONVERT_TYPE_E;

static int COMM_ImageByteConvert (const void *pSrc, unsigned int nSrcSize, 
    void *pDst, unsigned int nDstSize, COMM_CONVERT_TYPE_E eConvType)
{
    int bError = 0;
    if (COMM_RGB_1555 == eConvType)
    {
        int i = 0;
        int j = 0;
        for (int i = 0; i < (int)nSrcSize; i += 3)
        {
            unsigned char R = ((const char *)pSrc)[i];
            unsigned char G = ((const char *)pSrc)[i+1];
            unsigned char B = ((const char *)pSrc)[i+2];

            char dc1555[2] = {};
            dc1555[0] = (R & 0xF8) >> 3;        // 1-5 bit
            dc1555[0] = dc1555[0] ^ (G & 0xF8 >> 3);       // 6-8 bit
            dc1555[1] = G & 0xC0 >> 3;          // 9-10 bit
            dc1555[1] = dc1555[1] ^ B & 0xF8;   // 11 - 15 bit
            dc1555[1] = dc1555[1] | 0x80;       // 16 bit

            if (j + 1 < nDstSize)
            {
                ((char *)pDst)[j + 1] = dc1555[0];
                ((char *)pDst)[j + 2] = dc1555[1];
            }
        }

        if (i + 1 != nSrcSize)
        {
            _error ("[%s %d %s]: dst buffer small [src=%d,dst=%d]\n", 
                __FILE__,__LINE__,__FUNCTION__,
                nSrcSize, nDstSize);
            bError = 1;
        }
    }
    else 
    {
        _error ("[%s %d %s]: not support type[%d]\n", 
                __FILE__,__LINE__,__FUNCTION__,
                eConvType);
    }

    return bError;
}

static void bitprint(const void *pData, int nSize)
{
    unsigned char bitmap[8] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};

    for (int i = 0; i < nSize; ++i)
    {
        if ((i) % 4 == 0)
            printf ("0x%04x: ", i);
        for (int j = sizeof(bitmap) - 1; j >= 0; --j)
        {
            if (((char*)pData)[i] & bitmap[j])
            {
                printf ("1");
            }
            else 
            {
                printf ("0");
            }
        }
        printf (" ");
        
        if ((i+1) % 4 == 0)
            printf ("\n");
    }

    printf ("\n");
}

static void RGBAprint(const Image2RGBA_Pixel *pPixel, int nPixelCount, int nWidth)
{
    for (int i = 0; i < nPixelCount; ++i)
    {
        printf ("[%02x %02x %02x %02x] ", 
            pPixel[i].R, 
            pPixel[i].G,
            pPixel[i].B,
            pPixel[i].A);
        if (nWidth <= 0)
        {
            printf ("\n");
        }
        else if ((i + 1) % nWidth == 0)
        {
            printf ("\n");
        }
    }
    printf ("\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        _error ("usage: %s [filename]\n", argv[0]);
        return -1;   
    }

    const char *szFile = argv[1];

    HANDLE_IMAGE2RGBA handle = NULL;

    _debug ("open file %s\n", szFile);

    handle = image2rgba_open (szFile);
    if (handle == NULL)
    {
        _error ("open failed\n");
        return -1;
    }

    int nW = 0;
    int nH = 0;
    image2rgba_getsize (handle, &nW, &nH);

    _debug ("[%d,%d]\n", nW, nH);

    int nPixelSize = nW * nH;
    int nByteSize = nPixelSize * sizeof(Image2RGBA_Pixel);
    Image2RGBA_Pixel *pPixel = (Image2RGBA_Pixel*)malloc (nByteSize);
    for (int y = 0; y < nH; ++y)
    {
        for (int x = 0; x < nW; ++x)
        {
            image2rgba_getpixel (handle, x, y, & pPixel[x + y * nH]);
        }
    }
    RGBAprint (pPixel, nPixelSize, nW);

    image2rgba_close (handle);

    return 0;
}