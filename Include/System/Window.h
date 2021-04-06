#ifndef _SYS_WINDOW_H_
#define _SYS_WINDOW_H_

#include <Engine/Types.h>

bool Sys_CreateWindow(void);
void Sys_SetWindowTitle(const char *title);
void Sys_DestroyWindow(void);

#endif /* _SYS_WINDOW__H_ */