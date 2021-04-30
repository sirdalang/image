
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
    char bitmap[8] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};

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
    char src[256] = {};
    for (int i = 0; i < sizeof(src); ++i)
    {
        src[i] = i;
    }

    src[0] = 0xFF;
    src[1] = 0x00;
    src[2] = 0x00;

    bitprint (src, 3);

    char dst[256] = {};
    COMM_ImageByteConvert (src, 6, dst, 4, COMM_RGB_1555);

    bitprint (dst, 4);

    return 0;
}

libjpeg
bmp