#include <stdio.h>
#include <stdbool.h>

#include <Engine/IO.h>
#include <Engine/Job.h>
#include <Audio/Audio.h>
#include <Input/Input.h>
#include <Engine/Engine.h>
#include <Engine/Config.h>
#include <System/Log.h>
#include <System/Endian.h>
#include <System/Memory.h>
#include <System/System.h>
#include <System/Window.h>
#include <Scene/Scene.h>
#include <Scene/Camera.h>
#include <Render/Render.h>
#include <Engine/Event.h>
#include <Engine/Entity.h>
#include <Engine/Version.h>
#include <Engine/Resource.h>
#include <Engine/ECSystem.h>
#include <Engine/Component.h>
#include <Engine/Application.h>
#include <Script/Script.h>
#include <UI/UI.h>

#include <Math/sanity.h>

#include "ECS.h"

#define EMOD	"Engine"

#define E_CONFIG_FILE	"Data/Config/Engine.ini"

struct EngineSubsystem
{
	char name[256];
	bool (*init)(void);
	void (*term)(void);
};

void *E_screen = NULL;
uint32_t *E_screenWidth = NULL;
uint32_t *E_screenHeight = NULL;
double E_deltaTime = 0.0;

static bool _shutdown;
static double _startTime, _prevTime;
static struct EngineSubsystem _subsystems[] =
{
	{ "Window", Sys_CreateWindow, Sys_DestroyWindow },
	{ "Job System", E_InitJobSystem, E_TermJobSystem },
	{ "I/O System", E_InitIOSystem, E_TermIOSystem },
	{ "Event System", E_InitEventSystem, E_TermEventSystem },
	{ "Components", E_InitComponents, E_TermComponents },
	{ "Entities", E_InitEntities, E_TermEntities },
	{ "ECSystems", E_InitECSystems, E_TermECSystems },
	{ "Resource System", E_InitResourceSystem, E_TermResourceSystem },
	{ "Render System", Re_Init, Re_Term },
//	{ "Audio System", Au_Init, Au_Term },
	{ "Resource Purge", NULL, E_PurgeResources },
	{ "Input", In_InitInput, In_TermInput },
	{ "UI", UI_InitUI, UI_TermUI },
	{ "Scripting", E_InitScriptSystem, E_TermScriptSystem }
};
static const int32_t _subsystemCount = sizeof(_subsystems) / sizeof(_subsystems[0]);

bool
E_Init(int argc, char *argv[])
{
	int i = 0;
	const char *configFile = E_CONFIG_FILE;
	const char *logFile = NULL;
	const char *dataDir = NULL;

	while ((i = getopt(argc, argv, "c:d:l:n")) != -1) {
		switch (i) {
		case 'c':
			configFile = optarg;
			break;
		case 'd':
			dataDir = optarg;
			break;
		case 'l':
			logFile = optarg;
			break;
		}
	}

	E_InitConfig(configFile);

	Sys_Init();
	Sys_InitMemory();

//	__MathDbg_SanityTest();

	if (logFile)
		E_SetCVarStr("Engine_LogFile", logFile);
	
	if (dataDir)
		E_SetCVarStr("Engine_DataDir", dataDir);
	
	E_screenWidth = &E_GetCVarU32("Engine_ScreenWidth", 800)->u32;
	E_screenHeight = &E_GetCVarU32("Engine_ScreenHeight", 600)->u32;
	
	if (App_applicationInfo.version.revision)
		Sys_LogEntry(EMOD, LOG_INFORMATION, "%s v%d.%d.%d.%d", App_applicationInfo.name, App_applicationInfo.version.major,
			App_applicationInfo.version.minor, App_applicationInfo.version.build, App_applicationInfo.version.revision);
	else
		Sys_LogEntry(EMOD, LOG_INFORMATION, "%s v%d.%d.%d", App_applicationInfo.name, App_applicationInfo.version.major,
			App_applicationInfo.version.minor, App_applicationInfo.version.build);
	Sys_LogEntry(EMOD, LOG_INFORMATION, "Copyright (c) %s", App_applicationInfo.copyright);

	Sys_LogEntry(EMOD, LOG_INFORMATION, "%s \"%s\" v%s", E_PGM_NAME, E_CODENAME, E_VER_STR);
	Sys_LogEntry(EMOD, LOG_INFORMATION, "Copyright (C) %s", E_CPY_STR);
	Sys_LogEntry(EMOD, LOG_INFORMATION, "Starting up...");

	Sys_LogEntry(EMOD, LOG_INFORMATION, "Host: %s", Sys_Hostname());
	Sys_LogEntry(EMOD, LOG_INFORMATION, "Platform: %s %s", Sys_OperatingSystem(), Sys_OperatingSystemVersion());
	Sys_LogEntry(EMOD, LOG_INFORMATION, "CPU: %s", Sys_CpuName());
	Sys_LogEntry(EMOD, LOG_INFORMATION, "\tFrequency: %d MHz", Sys_CpuFreq());
	Sys_LogEntry(EMOD, LOG_INFORMATION, "\tCount: %d", Sys_NumCpus());
	Sys_LogEntry(EMOD, LOG_INFORMATION, "\tArchitecture: %s", Sys_Machine());
	Sys_LogEntry(EMOD, LOG_INFORMATION, "\tBig Endian: %s", Sys_BigEndian() ? "yes" : "no");
	Sys_LogEntry(EMOD, LOG_INFORMATION, "RAM: %llu MB", Sys_TotalMemory() / 1024 / 1024);

	for (i = 0; i < _subsystemCount; ++i) {
		if (!_subsystems[i].init)
			continue;
			
		if (!_subsystems[i].init()) {
			char msg[256];
			
			snprintf(msg, sizeof(msg), "Failed to initialize %s. The program will now exit.", _subsystems[i].name);
			Sys_MessageBox("Fatal Error", msg, MSG_ICON_ERROR);

			Sys_LogEntry(EMOD, LOG_CRITICAL, "Failed to initialize %s", _subsystems[i].name);
			
			return false;
		}
	}

#ifdef _DEBUG
	{
		char titleBuff[256];

		snprintf(titleBuff, 256, "%s v%u.%u.%u", App_applicationInfo.name,
			App_applicationInfo.version.major, App_applicationInfo.version.minor, App_applicationInfo.version.build);

		if (App_applicationInfo.version.revision)
			snprintf(titleBuff + strlen(titleBuff), sizeof(titleBuff) - strlen(titleBuff),
				".%u", App_applicationInfo.version.revision);
	
		snprintf(titleBuff + strlen(titleBuff), sizeof(titleBuff) - strlen(titleBuff),
			" - %s v%u.%u.%u", E_PGM_NAME, E_VER_MAJOR, E_VER_MINOR, E_VER_BUILD);
	
		if (E_VER_REVISION)
			snprintf(titleBuff + strlen(titleBuff), sizeof(titleBuff) - strlen(titleBuff),
				".%u", E_VER_REVISION);
	
		snprintf(titleBuff + strlen(titleBuff), sizeof(titleBuff) - strlen(titleBuff),
			" - GPU: %s (%s)", Re_deviceInfo.deviceName, Re_driver->driverName);
	
		Sys_SetWindowTitle(titleBuff);
	}
#else
	Sys_SetWindowTitle(App_applicationInfo.name);
#endif

	_startTime = (double)Sys_Time();

	Sys_LogEntry(EMOD, LOG_INFORMATION, "Engine start up complete.");
	Sys_LogEntry(EMOD, LOG_INFORMATION, "Available memory: %llu MB", Sys_FreeMemory() / 1024 / 1024);

	if (!App_InitApplication(argc, argv))
		return false;
	
	Sys_LogEntry(EMOD, LOG_INFORMATION, "Application started.");

	Sys_ResetHeap(MH_Transient);

	return true;
}

void
E_Term(void)
{
	int i;

	Sys_LogEntry(EMOD, LOG_INFORMATION, "Shutting down...");

	App_TermApplication();

	if (Scn_activeScene)
		Scn_UnloadScene(Scn_activeScene);

	for (i = _subsystemCount - 1; i >= 0; --i) {
		if (!_subsystems[i].term)
			continue;
		_subsystems[i].term();
	}

	Sys_LogEntry(EMOD, LOG_INFORMATION, "Shut down complete.");

	Sys_TermMemory();
	Sys_Term();

	E_TermConfig();
}

int
E_Run(void)
{
	while (!_shutdown) {
		if (!Sys_ProcessEvents()) {
			_shutdown = true;
			break;
		}

		if (Sys_ScreenVisible())
			E_Frame();
	}

	E_Term();

	return 0;
}

void
E_Frame(void)
{
	double now = E_Time();
	E_deltaTime = now - _prevTime;
	_prevTime = now;

	if (!Scn_activeScene /*|| !Scn_activeCamera*/) {
		E_ProcessEvents();
		Sys_ResetHeap(MH_Transient);
		return;
	}

	{ // Update
		E_ProcessEvents();
		In_Update();
	
		E_ExecuteSystemGroupS(Scn_activeScene, ECSYS_GROUP_LOGIC);
		E_ExecuteSystemGroupS(Scn_activeScene, ECSYS_GROUP_POST_LOGIC);
	}
	
	{ // Render
		Re_BeginFrame();
		Re_Clear(CT_COLOR | CT_DEPTH);
		Re_Viewport(0, 0, *E_screenWidth, *E_screenHeight);
		
		E_ExecuteSystemGroupS(Scn_activeScene, ECSYS_GROUP_PRE_RENDER);
		Re_RenderScene(Scn_activeScene, Scn_activeCamera);
		E_ExecuteSystemGroupS(Scn_activeScene, ECSYS_GROUP_POST_RENDER);
		
		UI_Render(Scn_activeScene);
		
		Re_EndFrame();
		Re_Present();
	}

	Sys_ResetHeap(MH_Transient);
}

double
E_Time(void)
{
	return ((double)Sys_Time() - _startTime) * 0.000000001;
}

void
E_Shutdown(void)
{
	_shutdown = true;
}

