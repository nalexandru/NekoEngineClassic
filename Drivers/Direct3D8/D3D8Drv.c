#include <System/System.h>
#include <Render/Driver.h>
#include <Render/Device.h>

#include "D3D8Drv.h"

IDirect3D8 *D3D8_d3d;
IDirect3DDevice8 *D3D8_device;
D3DCAPS8 D3D8_deviceCaps;
D3DPRESENT_PARAMETERS D3D8_presentParams;

static bool _Init(void);
static void _Term(void);
static bool _EnumerateDevices(uint32_t *count, struct RenderDeviceInfo *devices);

static struct RenderDriver _drv =
{
	NE_RENDER_DRIVER_ID,
	NE_RENDER_DRIVER_API,
	"Direct3D8",

	_Init,
	_Term,

	_EnumerateDevices,
	D3D8_CreateDevice,
	D3D8_DestroyDevice
};

#ifdef RENDER_DRIVER_BUILTIN
const struct RenderDriver *Re_LoadBuiltinDriver(void) { return &_drv; }
#else
__declspec (dllexport) const struct RenderDriver *Re_LoadDriver(void) { return &_drv; }
#endif

static bool
_Init(void)
{
	D3D8_d3d = Direct3DCreate8(D3D_SDK_VERSION);
	return D3D8_d3d != NULL;
}

static void
_Term(void)
{
	IDirect3D8_Release(D3D8_d3d);
}

static bool
_EnumerateDevices(uint32_t *count, struct RenderDeviceInfo *info)
{
	uint32_t i = 0;

	if (!*count || !info) {
		*count = IDirect3D8_GetAdapterCount(D3D8_d3d);
		return true;
	}

	for (i = 0; i < *count; ++i) {
		D3DCAPS8 caps;
		D3DDISPLAYMODE dm;
		D3DADAPTER_IDENTIFIER8 identifier;

		if (FAILED(IDirect3D8_GetDeviceCaps(D3D8_d3d, i, D3DDEVTYPE_HAL, &caps)))
			continue;

		if ((caps.DevCaps & D3DDEVCAPS_HWRASTERIZATION) != D3DDEVCAPS_HWRASTERIZATION)
			continue;

		if (FAILED(IDirect3D8_GetAdapterIdentifier(D3D8_d3d, i, 0, &identifier)))
			continue;

		if (FAILED(IDirect3D8_GetAdapterDisplayMode(D3D8_d3d, i, &dm)))
			continue;

#ifndef _XBOX
		if (FAILED(IDirect3D8_CheckDeviceFormat(D3D8_d3d, i, D3DDEVTYPE_HAL, dm.Format,
					D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)))
			continue;
	
		if (FAILED(IDirect3D8_CheckDepthStencilMatch(D3D8_d3d, i,
					D3DDEVTYPE_HAL, dm.Format, dm.Format, D3DFMT_D24S8)))
			continue;

		info[i].features.uint32IndexSupport = true;
#else
		info[i].features.uint32IndexSupport = false;
#endif

		snprintf(info[i].deviceName, sizeof(info[i].deviceName), "%s", identifier.Description);

		info[i].limits.maxTextureWidth = caps.MaxTextureWidth;
		info[i].limits.maxTextureHeight = caps.MaxTextureHeight;
		info[i].limits.maxAnisotropy = caps.MaxAnisotropy;

		info[i].localMemorySize = 0;

		info[i].private = (void *)i;
	}

	return true;
}
