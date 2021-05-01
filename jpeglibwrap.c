
#include "jpeglibwrap.h"

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

#include "jpeglib.h"

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

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

typedef struct jpegwrap_obj
{
    int nW;
    int nH;
    JpegWrap_Pixel *pPixels; // count = width * height
};

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

HANDLE_JPEGWRAP jpegwrap_create(const char *szFile)
{
    struct jpegwrap_obj *pObj = NULL;

    struct jpeg_decompress_struct cinfo = {};
    struct my_error_mgr cmyerr = {};
    FILE* fp = NULL;

    do 
    {
        /* malloc */
        pObj = (struct jpegwrap_obj*)malloc (sizeof(struct jpegwrap_obj));

        if (NULL == pObj)
        {
            _error ("malloc failed\n");
            break;
        }

        memset (pObj, 0, sizeof(struct jpegwrap_obj));

        /* open file */
        if ((fp = fopen (szFile, "rb")) == NULL)
        {
            _error ("open file %s failed\n", szFile);
            free (pObj);
            pObj = NULL;
            break;
        }

        /** 
         * error handling 
         * all possible error should happen in this function
         **/
        cinfo.err = jpeg_std_error (& cmyerr.pub);
        cmyerr.pub.error_exit = my_error_exit;

        if (setjmp(cmyerr.setjmp_buffer))
        {
            /* if error happens in libjpeg, jump to this line */
            jpeg_destroy_decompress (& cinfo);

            fclose (fp);
            fp = NULL;

            if (pObj->pPixels)
            {
                free (pObj->pPixels);
                pObj->pPixels = NULL;
            }

            free (pObj);
            pObj = NULL;
            break;
        }

        /* create decompressor */
        jpeg_create_decompress (& cinfo);

        /* specify data source */
        jpeg_stdio_src (&cinfo, fp);

        /* read header */
        jpeg_read_header (&cinfo, TRUE);

        /* start decompressor */
        jpeg_start_decompress (& cinfo);

        /* create data buffer */
        pObj->pPixels = malloc(cinfo.output_width * cinfo.output_height * sizeof(*pObj->pPixels));

        /* copy data */
        JSAMPARRAY buffer = NULL;
        int row_stride = cinfo.output_width * cinfo.output_components;
        buffer = (cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, 
            JPOOL_IMAGE, row_stride, 1);
        
        if (3 != cinfo.output_components)
        {
            _error ("component != 3, [%d]\n", cinfo.output_components);
            longjmp (cmyerr.setjmp_buffer, 1);
        }

        /* read data */
        int j = 0;
        while (cinfo.output_scanline < cinfo.output_height)
        {
            jpeg_read_scanlines (&cinfo, buffer, 1);

            /* copy pixels of a row */
            for (int i = 0; i < cinfo.output_width; ++i)
            {
                int nPixelPos = i + j * cinfo.output_width;
                int nByteRed = i * cinfo.output_components + 0;
                int nByteGreen = i * cinfo.output_components + 1;
                int nByteBlue = i * cinfo.output_components + 2;
                
                pObj->pPixels[nPixelPos].R = buffer[nByteRed];
                pObj->pPixels[nPixelPos].G = buffer[nByteGreen];
                pObj->pPixels[nPixelPos].B = buffer[nByteBlue];
                pObj->pPixels[nPixelPos].A = 0;
            }

            ++j;
        }
        pObj->nW = cinfo.output_width;
        pObj->nH = cinfo.output_height;

        /* finish decompressor */
        jpeg_finish_decompress (&cinfo);

        /* release object */
        jpeg_destroy_decompress (&cinfo);

        fclose (fp);
        fp = NULL;
    }
    while (0);

    return pObj;
}

int jpegwrap_destroy(HANDLE_JPEGWRAP handle)
{
    struct jpegwrap_obj *pObj = (struct jpegwrap_obj *)handle;

    if (NULL != pObj->pPixels)
    {
        free (pObj->pPixels);
        pObj->pPixels = NULL;
    }

    free (pObj);
    pObj = NULL;

    return 0;
}

int jpegwrap_size(HANDLE_JPEGWRAP handle, int *pW, int *pH)
{
    struct jpegwrap_obj *pObj = (struct jpegwrap_obj *)handle;

    *pW = pObj->nW;
    *pH = pObj->nH;

    return 0;
}

int jpegwrap_get(HANDLE_JPEGWRAP handle, JpegWrap_Pixel *pPixel, int i, int j)
{
    struct jpegwrap_obj *pObj = (struct jpegwrap_obj *)handle;

    if (i >= pObj->nW || j >= pObj->nH || i < 0 || j < 0)
    {
        _error ("invalid arg[%d,%d]\n", i,j);
        return -1;
    }

    int nPixelPos = j * pObj->nW + i;
    *pPixel = pObj->pPixels[nPixelPos];
    
    return 0;
}