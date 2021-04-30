
/**
 * Author: x
 * Date: 20210501
 * Description: C Wrapper of EasyBMP
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif 

typedef void * HANDLE_EASYBMP;

HANDLE_EASYBMP easybmpc_create();
int easybmpc_destroy(HANDLE_EASYBMP hBMP);

int easybmpc_open(HANDLE_EASYBMP hBMP, const char *szFile);
int easybmpc_size(HANDLE_EASYBMP hBMP, int *pW, int *pH);

#ifdef __cplusplus
}
#endif