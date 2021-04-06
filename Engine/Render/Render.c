#include <System/Log.h>
#include <System/System.h>
#include <System/Memory.h>
#include <Engine/Config.h>
#include <Render/Render.h>
#include <Render/Driver.h>
#include <Render/Device.h>
#include <Render/Shader.h>
#include <Render/Model.h>
#include <Render/Texture.h>
#include <Render/Material.h>
#include <Engine/Resource.h>

#define RE_MOD "Render"
#define CHK_FAIL(x, y) if (!x) { Sys_LogEntry(RE_MOD, LOG_CRITICAL, y); return false; }

struct RenderDeviceInfo Re_deviceInfo = { 0 };
struct RenderDeviceProcs Re_deviceProcs = { 0 };

const struct RenderDriver *Re_driver = NULL;

#ifndef RENDER_DRIVER_BUILTIN
static void *_drvModule = NULL;
#endif

bool
Re_Init(void)
{
	uint32_t devCount = 0;
	struct RenderDeviceInfo *info = NULL, *selected = NULL;

#ifdef RENDER_DRIVER_BUILTIN
	Re_driver = Re_LoadBuiltinDriver();
#else
	ReLoadDriverProc loadDriver;
	const char *drvPath = E_GetCVarStr("Render_Driver", "D3D8Driver")->str;
	_drvModule = Sys_LoadLibrary(drvPath);
	CHK_FAIL(_drvModule, "Failed to load driver module");

	loadDriver = (ReLoadDriverProc)Sys_GetProcAddress(_drvModule, "Re_LoadDriver");
	CHK_FAIL(loadDriver, "The library is not a valid driver");

	Re_driver = loadDriver();
#endif

	CHK_FAIL(Re_driver, "Failed to load driver");
	CHK_FAIL((Re_driver->identifier == NE_RENDER_DRIVER_ID), "The library is not a valid driver");
	CHK_FAIL((Re_driver->apiVersion == NE_RENDER_DRIVER_API), "Driver version mismatch");
	CHK_FAIL(Re_driver->Init(), "Failed to initialize driver");
	CHK_FAIL(Re_driver->EnumerateDevices(&devCount, NULL), "Failed to enumerate devices");

	info = Sys_Alloc(sizeof(*info), devCount, MH_Transient);
	CHK_FAIL(info, "Failed to enumerate devices");
	CHK_FAIL(Re_driver->EnumerateDevices(&devCount, info), "Failed to enumerate devices");
	
	selected = &info[0];

	if (!selected) {
		Sys_MessageBox("Fatal", "No graphics devices found !", MSG_ICON_ERROR);
		return false;
	}

	memcpy(&Re_deviceInfo, selected, sizeof(Re_deviceInfo));

	CHK_FAIL(Re_driver->CreateDevice(&Re_deviceInfo, &Re_deviceProcs), "Failed to create device");

	Sys_LogEntry(RE_MOD, LOG_INFORMATION, "GPU: %s (%s)", Re_deviceInfo.deviceName, Re_driver->driverName);
	Sys_LogEntry(RE_MOD, LOG_INFORMATION, "\tMemory: %llu MB", Re_deviceInfo.localMemorySize / 1024 / 1024);

	CHK_FAIL(Re_LoadShaders(), "Failed to load shaders");

	E_RegisterResourceType(RES_MODEL, sizeof(struct Model), (ResourceCreateProc)Re_CreateModelResource,
		(ResourceLoadProc)Re_LoadModelResource, (ResourceUnloadProc)Re_UnloadModelResource);
	E_RegisterResourceType(RES_MATERIAL, sizeof(struct Material), (ResourceCreateProc)Re_CreateMaterialResource,
		(ResourceLoadProc)Re_LoadMaterialResource, (ResourceUnloadProc)Re_UnloadMaterialResource);
	E_RegisterResourceType(RES_TEXTURE, sizeof(struct Texture *), (ResourceCreateProc)Re_CreateTextureResource,
		(ResourceLoadProc)Re_LoadTextureResource, (ResourceUnloadProc)Re_UnloadTextureResource);

	Re_ClearColor(.3f, 0.f, .8f, 1.f);
	
	return true;
}

void
Re_Term(void)
{
	Re_UnloadShaders();

	Re_driver->DestroyDevice();
	Re_driver->Term();

#ifndef RENDER_DRIVER_BUILTIN
	Sys_UnloadLibrary(_drvModule);
#endif
}

