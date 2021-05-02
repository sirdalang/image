#include "image_bmp.h"

#include "def.h"
#include "def_inner.h"

#include "EasyBMP.h"

IMAGE_NAME_IMPORT;

bool ImageBMP::LoadFile_override()
{
    bool ret = false;

    do 
    {
        BMP bmp;

        if (! bmp.ReadFromFile (m_strFileName.c_str()))
        {
            _error ("read file %s failed\n", m_strFileName.c_str());
            break;
        }

        m_nHeight = bmp.TellWidth();
        m_nWidth = bmp.TellHeight();

        int nPixelCount = m_nHeight * m_nWidth;
        m_vectPixelArray.resize (nPixelCount);

        for (int x = 0; x < m_nWidth; ++x)
        {
            for (int y = 0; y < m_nHeight; ++y)
            {
                int nPixelPos = x + y * m_nWidth;
                m_vectPixelArray[nPixelPos].R = bmp.GetPixel(x, y).Red;
                m_vectPixelArray[nPixelPos].G = bmp.GetPixel(x, y).Green;
                m_vectPixelArray[nPixelPos].B = bmp.GetPixel(x, y).Blue;
                m_vectPixelArray[nPixelPos].A = bmp.GetPixel(x, y).Alpha;
            }
        }
        
        ret = true;
    }
    while (0);

    return ret;
}

