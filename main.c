
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

#include "easybmpwrap.h"

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
        for (int i = 0; i < nSrcSize; i += 3)
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
            printf ("[%s %d %s]: dst buffer small [src=%d,dst=%d]\n", 
                __FILE__,__LINE__,__FUNCTION__,
                nSrcSize, nDstSize);
            bError = 1;
        }
    }
    else 
    {
        printf ("[%s %d %s]: not support type[%d]\n", 
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

int main(int argc, char *argv[])
{
    int ret = -1;
    HANDLE_EASYBMP hBMP = NULL;

    do 
    {
        if (argc != 2)
        {
            printf ("usage: %s [filename]\n", argv[0]);
            break;
        }
        const char *szFile = argv[1];

        printf ("file=%s\n", szFile);

        hBMP = easybmpc_create();
        if (hBMP == NULL)
        {
            printf ("create failed\n");
            break;
        }

        ret = easybmpc_open (hBMP, szFile);
        if (ret < 0)
        {
            printf ("open file %s failed\n", szFile);
            break;
        }

        int nW = 0;
        int nH = 0;
        ret = easybmpc_size (hBMP, & nW, & nH);
        printf ("w=%d,h=%d\n", nW, nH);

        int nSize = nW * nH * sizeof(EasyBmp_Pixel);
        EasyBmp_Pixel *pPixelData = (EasyBmp_Pixel *)malloc (nSize);
        for (int i = 0; i < nW; ++i)
        {
            for (int j = 0; j < nH; ++j)
            {
                easybmpc_get (hBMP, & pPixelData[i + j * nW], i, j);
            }
        }

        bitprint (pPixelData, nSize);

        free (pPixelData);

        ret = 0;
    }
    while (0);

    if (hBMP != NULL)
    {
        easybmpc_destroy (hBMP);
        hBMP = NULL;
    }

    return ret;
}