#include "image_interface.h"

#include "def_inner.h"

IMAGE_NAME_IMPORT;

ImageInterface::ImageInterface()
    : m_nWidth(0),m_nHeight(0),m_bFileLoaded(false)
{
    // blank
}

ImageInterface::~ImageInterface()
{
    // blank
}

bool ImageInterface::LoadFile (const char *szFileName)
{
    m_bFileLoaded = false;
    m_nWidth = 0;
    m_nHeight = 0;
    m_vectPixelArray.resize (0);
    m_strFileName = std::string(szFileName);

    bool ret = CheckOrLoadFile();
    return ret;
}

int ImageInterface::Width()
{
    CheckOrLoadFile ();

    return m_nWidth;
}

int ImageInterface::Height()
{
    CheckOrLoadFile ();

    return m_nHeight;
}

bool ImageInterface::GetPixel(int x, int y, Pixel& refPixel)
{
    if (! CheckOrLoadFile())
    {
        return false;
    }

    int nPixelPos = x + y * m_nWidth;
    if (nPixelPos >= static_cast<int>(m_vectPixelArray.size()))
    {
        _error ("[%d,%d] over [%d,%d]\n", x,y,m_nWidth,m_nHeight);
        return false;
    }

    refPixel = m_vectPixelArray[nPixelPos];
    return true;
}

bool ImageInterface::CheckOrLoadFile ()
{
    if (m_bFileLoaded)
    {
        return true;
    }

    if (LoadFile_override())
    {
        m_bFileLoaded = true;
        return true;
    }

    return false;
}