#include <xtl.h>

#include <Engine/Engine.h>
#include <System/System.h>

VOID __cdecl
main(void)
{
	char *argv[] =
	{
		"Default.xbe",
		"-d",
		"D:\\Data"
	};

	if (!E_Init(sizeof(argv) / sizeof(argv[0]), argv))
		return;

	E_Run();
}
