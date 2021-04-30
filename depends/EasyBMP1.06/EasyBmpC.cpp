
#include "EasyBmpC.h"

#include "EasyBMP.h"

HANDLE_EASYBMP easybmpc_create()
{
    return new BMP();
}

int easybmpc_destroy(HANDLE_EASYBMP hBMP)
{
    BMP* pBMP = static_cast<BMP*>(hBMP);
    if (pBMP != nullptr)
    {
        delete pBMP;
        pBMP = nullptr;
    }
    return 0;
}

int easybmpc_open(HANDLE_EASYBMP hBMP, const char *szFile)
{
    int ret = -1;
    BMP* pBMP = static_cast<BMP*>(hBMP);
    if (pBMP->ReadFromFile (szFile))
    {
        ret = 0;
    }
    return ret;
}

int easybmpc_size(HANDLE_EASYBMP hBMP, int *pW, int *pH)
{
    int ret = -1;
    BMP* pBMP = static_cast<BMP*>(hBMP);
    *pW = pBMP->TellWidth();
    *pH = pBMP->TellHeight();
    ret = 0;
    return ret;
}