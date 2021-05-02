#include "image2rgba.h"

#include <string.h>

#include "image_bmp.h"
#include "image_jpeg.h"

IMAGE_NAME_IMPORT;

#define BMP_SUFFIX ".bmp"
#define JPEG_SUFFIX ".jpeg"
#define JPEG_SUFFIX_2 ".jpg"

HANDLE_IMAGE2RGBA       image2rgba_open(const char *szFile)
{
    ImageInterface *pObj = nullptr;

    do 
    {
        if (strstr(szFile, BMP_SUFFIX) != nullptr)
        {
            pObj = new ImageBMP();
        }
        else if (strstr (szFile, JPEG_SUFFIX) != nullptr
            || strstr (szFile, JPEG_SUFFIX_2) != nullptr)
        {
            pObj = new ImageJpeg();
        }
        else 
        {
            _error ("filetype not support [%s]\n", szFile);
            break;
        }

        pObj->LoadFile(szFile);
    }
    while (0);

    return pObj;
}

int                     image2rgba_close(HANDLE_IMAGE2RGBA handle)
{
    ImageInterface *pObj = static_cast<ImageInterface*> (handle);
    if (nullptr != pObj)
    {
        delete pObj;
        pObj = nullptr;
    }

    return 0;
}

int image2rgba_getsize(HANDLE_IMAGE2RGBA handle, int *pW, int *pH)
{
    ImageInterface *pObj = static_cast<ImageInterface*> (handle);

    *pW = pObj->Width();
    *pH = pObj->Height();
    return 0;
}

int image2rgba_getpixel(HANDLE_IMAGE2RGBA handle, int i, int j, Image2RGBA_Pixel *pPixel)
{
    ImageInterface *pObj = static_cast<ImageInterface*> (handle);

    bool ret = pObj->GetPixel (i,j,*pPixel);

    return ret ? 0 : -1;
}