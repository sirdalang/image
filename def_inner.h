/**
 * Author: x
 * Date: 20210503
 * Description: public names
 */

#pragma once

#include <stdio.h>

#include "def.h"

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


#ifndef IMAGE_NAMESPACE_NAME
#define IMAGE_NAMESPACE_NAME \
    NAMESPACE_IMAGE
#endif 

#ifndef IMAGE_NAME_START
#define IMAGE_NAME_START \
    namespace IMAGE_NAMESPACE_NAME {
#endif 

#ifndef IMAGE_NAME_END
#define IMAGE_NAME_END \
    }
#endif 

#ifndef IMAGE_NAME_IMPORT
#define IMAGE_NAME_IMPORT \
    using namespace IMAGE_NAMESPACE_NAME
#endif 