#include <System/System.h>
#include <Render/Driver.h>
#include <Render/Device.h>

#include "D3D9Drv.h"

IDirect3D9 *D3D9_d3d;
IDirect3DDevice9 *D3D9_device;

static bool _Init(void);
static void _Term(void);
static bool _EnumerateDevices(uint32_t *count, struct RenderDeviceInfo *devices);

static struct RenderDriver _drv =
{
	NE_RENDER_DRIVER_ID,
	NE_RENDER_DRIVER_API,
	"Direct3D9",

	_Init,
	_Term,

	_EnumerateDevices,
	D3D9_CreateDevice,
	D3D9_DestroyDevice
};

#ifdef RENDER_DRIVER_BUILTIN
const struct RenderDriver *Re_LoadBuiltinDriver(void) { return &_drv; }
#else
__declspec (dllexport) const struct RenderDriver *Re_LoadDriver(void) { return &_drv; }
#endif

static bool
_Init(void)
{
	D3D9_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	return D3D9_d3d != NULL;
}

static void
_Term(void)
{
	IDirect3D9_Release(D3D9_d3d);
}

static bool
_EnumerateDevices(uint32_t *count, struct RenderDeviceInfo *info)
{
	uint32_t i = 0;

	if (!*count || !info) {
		*count = IDirect3D9_GetAdapterCount(D3D9_d3d);
		return true;
	}

	for (i = 0; i < *count; ++i) {
		D3DCAPS9 caps;
		D3DDISPLAYMODE dm;
		D3DADAPTER_IDENTIFIER9 identifier;

		if (FAILED(IDirect3D9_GetDeviceCaps(D3D9_d3d, i, D3DDEVTYPE_HAL, &caps)))
			continue;
		
		if ((caps.DevCaps & D3DDEVCAPS_HWRASTERIZATION) != D3DDEVCAPS_HWRASTERIZATION)
			continue;

		if (FAILED(IDirect3D9_GetAdapterIdentifier(D3D9_d3d, i, 0, &identifier)))
			continue;

		if (FAILED(IDirect3D9_GetAdapterDisplayMode(D3D9_d3d, i, &dm)))
			continue;

		if (FAILED(IDirect3D9_CheckDeviceFormat(D3D9_d3d, i, D3DDEVTYPE_HAL, dm.Format,
					D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)))
			continue;
		
		if (FAILED(IDirect3D9_CheckDepthStencilMatch(D3D9_d3d, i,
					D3DDEVTYPE_HAL, dm.Format, dm.Format, D3DFMT_D24S8)))
			continue;

		snprintf(info[i].deviceName, sizeof(info[i].deviceName), "%s", identifier.Description);


		info[i].features.uint32IndexSupport = true;
		info[i].limits.maxTextureWidth = caps.MaxTextureWidth;
		info[i].limits.maxTextureHeight = caps.MaxTextureHeight;
		info[i].limits.maxAnisotropy = caps.MaxAnisotropy;

		info[i].localMemorySize = 0;

		info[i].private = (void *)i;
	}

	return true;
}
