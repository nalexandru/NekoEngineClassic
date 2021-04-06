#include "Win32Platform.h"
#include <crtdbg.h>

#include <Engine/Engine.h>
#include <System/System.h>

// NVIDIA Optimus
// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
_declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;

HINSTANCE Win32_instance;

int APIENTRY
WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
#if 1
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
//	_crtBreakAlloc = 290;
	flag |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#endif

	Win32_instance = hInst;

	if (!E_Init(__argc, __argv))
		return -1;

	return E_Run();
}
