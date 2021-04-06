#include <System/Window.h>
#include <Engine/Engine.h>

bool
Sys_CreateWindow(void)
{
	E_screen = NULL;
	return true;
}

void
Sys_SetWindowTitle(const char *name)
{
	(void)name;
}

void
Sys_DestroyWindow(void)
{
}
