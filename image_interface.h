
#pragma once

#include "def_inner.h"

#include <string>
#include <vector>

IMAGE_NAME_START

class ImageInterface
{
public:
    ImageInterface();
    virtual ~ImageInterface();
    bool LoadFile(const char *szFileName);
    int Width();
    int Height();
    bool GetPixel(int x, int y, Pixel& refPixel);
protected:
    ImageInterface(const ImageInterface& ref)=delete;
    ImageInterface& operator=(const ImageInterface& ref)=delete;
    virtual bool LoadFile_override() = 0;
protected:
    std::string m_strFileName;
    std::vector<Pixel> m_vectPixelArray;
    int m_nWidth;
    int m_nHeight;
private:
    bool CheckOrLoadFile();
private:
    bool m_bFileLoaded;
};

IMAGE_NAME_END