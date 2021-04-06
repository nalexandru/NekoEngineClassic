#include <assert.h>

#include <Math/Math.h>
#include <System/Log.h>
#include <Engine/Engine.h>
#include <Engine/Config.h>
#include <Render/Device.h>
#include <Engine/Resource.h>

#include "D3D8Drv.h"

static D3DCOLOR _clearColor = D3DCOLOR_COLORVALUE(0.f, 0.f, 0.f, 1.f);
static D3DVIEWPORT8 _viewport = { 0, 0, 0, 0, 0.f, 1.f };

static const struct Shader *_boundShader;

static D3DCULL NeToD3D8CullMode[] =
{
	D3DCULL_NONE,
	D3DCULL_CW,
	D3DCULL_CCW
};

static D3DPRIMITIVETYPE NeToD3D8Topology[] = 
{
	D3DPT_TRIANGLELIST,
	D3DPT_POINTLIST,
	D3DPT_LINELIST
};

static D3DBLEND NeToD3D8BlendFactor[] =
{
	D3DBLEND_ZERO,
	D3DBLEND_ONE,
	D3DBLEND_SRCCOLOR,
	D3DBLEND_INVSRCCOLOR,
	D3DBLEND_DESTCOLOR,
	D3DBLEND_INVDESTCOLOR,
	D3DBLEND_SRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_DESTALPHA,
	D3DBLEND_INVDESTALPHA,
	-1,
	-1,
	-1,
	-1,
	D3DBLEND_SRCALPHASAT,
	-1,
	-1,
	-1,
	-1
};

static void
_CullMode(enum CullMode mode)
{
	IDirect3DDevice8_SetRenderState(D3D8_device, D3DRS_CULLMODE, NeToD3D8CullMode[mode]);
}

static void
_DepthTest(bool enable)
{
	IDirect3DDevice8_SetRenderState(D3D8_device, D3DRS_ZENABLE, enable ? D3DZB_TRUE : D3DZB_FALSE);
}

static void
_DepthWrite(bool enable)
{
	IDirect3DDevice8_SetRenderState(D3D8_device, D3DRS_ZWRITEENABLE, enable);
}

static void
_ClearColor(float r, float g, float b, float a)
{
	_clearColor = D3DCOLOR_COLORVALUE(r, g, b, a);
}

static void
_Blend(bool enable)
{
	IDirect3DDevice8_SetRenderState(D3D8_device, D3DRS_ALPHABLENDENABLE, enable);
}

static void
_BlendFunc(enum BlendFactor srcFactor, enum BlendFactor dstFactor)
{
	IDirect3DDevice8_SetRenderState(D3D8_device, D3DRS_SRCBLEND, NeToD3D8BlendFactor[srcFactor]);
	IDirect3DDevice8_SetRenderState(D3D8_device, D3DRS_DESTBLEND, NeToD3D8BlendFactor[dstFactor]);
}

static void
_BlendColor(float r, float g, float b, float a)
{
	(void)r; (void)g; (void)b; (void)a;
}

static void
_BeginFrame(void)
{
	IDirect3DDevice8_BeginScene(D3D8_device);
}

static void
_Clear(enum ClearType type)
{
	DWORD flags = 0;

	if ((type & CT_COLOR) == CT_COLOR)
		flags |= D3DCLEAR_TARGET;
	if ((type & CT_DEPTH) == CT_DEPTH)
		flags |= D3DCLEAR_ZBUFFER;
	if ((type & CT_STENCIL) == CT_STENCIL)
		flags |= D3DCLEAR_STENCIL;

	IDirect3DDevice8_Clear(D3D8_device, 0, NULL, flags, _clearColor, _viewport.MaxZ, 0);
}

static void
_Present(void)
{
	IDirect3DDevice8_Present(D3D8_device, NULL, NULL, NULL, NULL);
}

static void
_EndFrame(void)
{
	IDirect3DDevice8_EndScene(D3D8_device);
}

static void
_Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	_viewport.X = x;
	_viewport.Y = y;
	_viewport.Width = width;
	_viewport.Height = height;
	IDirect3DDevice8_SetViewport(D3D8_device, &_viewport);
}

static void
_DepthRange(float zNear, float zFar)
{
	_viewport.MinZ = zNear;
	_viewport.MaxZ = zFar;
	IDirect3DDevice8_SetViewport(D3D8_device, &_viewport);
}

static void
_Scissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
}

static void
_BindShader(const struct Shader *s)
{
	IDirect3DDevice8_SetVertexShader(D3D8_device, s->vertex);
	IDirect3DDevice8_SetPixelShader(D3D8_device, s->pixel);

	_boundShader = s;
}

static void
_BindIndexBuffer(const struct Buffer *buff)
{
	IDirect3DDevice8_SetIndices(D3D8_device, buff->idxBuff, 0);
}

static void
_BindVertexBuffer(const struct Buffer *buff)
{
	assert(_boundShader);
	IDirect3DDevice8_SetStreamSource(D3D8_device, 0, buff->vtxBuff, _boundShader->layout->stride);
}

static void
_BindTexture(Handle texture, uint32_t unit)
{
	struct Texture **tex = E_ResourcePtr(texture);
	IDirect3DDevice8_SetTexture(D3D8_device, unit, (IDirect3DBaseTexture8 *)(*tex)->tex);
}

static void
_SetVertexConstants(uint32_t binding, const float *value, uint32_t count)
{
	if (count == 16)
		m4_transpose((struct mat4 *)value, (struct mat4 *)value);

	IDirect3DDevice8_SetVertexShaderConstant(D3D8_device, _boundShader->vertexConstantRegisters[binding], value, count / 4);
}

static void
_SetFragmentConstants(uint32_t binding, const float *value, uint32_t count)
{
	IDirect3DDevice8_SetPixelShaderConstant(D3D8_device, _boundShader->pixelConstantRegisters[binding], value, count / 4);
}

static void
_Draw(enum PrimitiveTopology topology, uint32_t first, uint32_t count)
{
	IDirect3DDevice8_DrawPrimitive(D3D8_device, NeToD3D8Topology[topology], first, count / 3);
}

static void
_DrawIndexed(enum PrimitiveTopology topology, uint32_t vertexCount, uint32_t firstIndex, uint32_t indexCount)
{
	IDirect3DDevice8_DrawIndexedPrimitive(D3D8_device, NeToD3D8Topology[topology], 0,
											vertexCount, firstIndex, indexCount / 3);
}

bool
D3D8_CreateDevice(struct RenderDeviceInfo *info, struct RenderDeviceProcs *procs)
{
	HRESULT hr;
	DWORD flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	UINT adapter = (UINT)info->private;

	ZeroMemory(&D3D8_presentParams, sizeof(D3D8_presentParams));

#ifndef _XBOX
	{
		D3DDISPLAYMODE dm;
		hr = IDirect3D8_GetAdapterDisplayMode(D3D8_d3d, adapter, &dm);
		if (FAILED(hr))
			return false;

		D3D8_presentParams.BackBufferFormat = dm.Format;
	}

	D3D8_presentParams.Windowed = TRUE;

	if (E_GetCVarBln("Render_Multisampling", true)->bln) {
		D3D8_presentParams.MultiSampleType = (D3DMULTISAMPLE_TYPE)E_GetCVarI32("Render_Samples", 4)->i32;

		if (FAILED(IDirect3D8_CheckDeviceMultiSampleType(D3D8_d3d, adapter,
				D3DDEVTYPE_HAL, D3D8_presentParams.BackBufferFormat, D3D8_presentParams.Windowed,
				D3D8_presentParams.MultiSampleType))) {
			Sys_LogEntry(D3D8DRV_MOD, LOG_WARNING, "The requested multisample settings are not supported");
			D3D8_presentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
		}
	}
#else
	D3D8_presentParams.BackBufferFormat = D3DFMT_X8R8G8B8;
	D3D8_presentParams.Windowed = FALSE;
#endif

	D3D8_presentParams.AutoDepthStencilFormat = D3DFMT_D24S8;
	D3D8_presentParams.BackBufferWidth = *E_screenWidth;
	D3D8_presentParams.BackBufferHeight = *E_screenHeight;
	D3D8_presentParams.BackBufferCount = 2;
	D3D8_presentParams.hDeviceWindow = (HWND)E_screen;
	D3D8_presentParams.EnableAutoDepthStencil = TRUE;	
	D3D8_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

	hr = IDirect3D8_CreateDevice(D3D8_d3d, adapter, D3DDEVTYPE_HAL, (HWND)E_screen,
									flags, &D3D8_presentParams, &D3D8_device);
	if (FAILED(hr))
		return false;

#ifndef _XBOX
	if (E_GetCVarBln("Render_Multisampling", true)->bln)
		IDirect3DDevice8_SetRenderState(D3D8_device, D3DRS_MULTISAMPLEANTIALIAS, TRUE);

	info->localMemorySize = IDirect3DDevice8_GetAvailableTextureMem(D3D8_device);
#endif

	IDirect3DDevice8_SetRenderState(D3D8_device, D3DRS_LIGHTING, FALSE);
	IDirect3DDevice8_SetRenderState(D3D8_device, D3DRS_CULLMODE, D3DCULL_NONE);

	procs->BeginFrame = _BeginFrame;
	procs->Clear = _Clear;
	procs->Present = _Present;
	procs->EndFrame = _EndFrame;

	procs->CullMode = _CullMode;
	procs->DepthTest = _DepthTest;
	procs->DepthWrite = _DepthWrite;
	procs->ClearColor = _ClearColor;

	procs->Blend = _Blend;
	procs->BlendFunc = _BlendFunc;
	procs->BlendColor = _BlendColor;

	procs->Viewport = _Viewport;
	procs->Scissor = _Scissor;
	procs->DepthRange = _DepthRange;

	procs->BindVertexBuffer = _BindVertexBuffer;
	procs->BindIndexBuffer = _BindIndexBuffer;
	procs->BindTexture = _BindTexture;
	procs->BindShader = _BindShader;
	procs->SetVertexConstants = _SetVertexConstants;
	procs->SetFragmentConstants = _SetFragmentConstants;

	procs->Draw = _Draw;
	procs->DrawIndexed = _DrawIndexed;

	procs->CreateTexture = D3D8_CreateTexture;
	procs->UpdateTexture = D3D8_UpdateTexture;
	procs->DestroyTexture = D3D8_DestroyTexture;

	procs->CreateBuffer = D3D8_CreateBuffer;
	procs->UpdateBuffer = D3D8_UpdateBuffer;
	procs->LockBuffer = D3D8_LockBuffer;
	procs->UnlockBuffer = D3D8_UnlockBuffer;
	procs->DestroyBuffer = D3D8_DestroyBuffer;

	procs->CreateInputLayout = D3D8_CreateInputLayout;
	procs->DestroyInputLayout = D3D8_DestroyInputLayout;

	procs->CreateVertexModule = D3D8_CreateVertexModule;
	procs->CreateFragmentModule = D3D8_CreateFragmentModule;
	procs->DestroyShaderModule = D3D8_DestroyShaderModule;

	procs->CreateShader = D3D8_CreateShader;
	procs->AddShaderModule = D3D8_AddShaderModule;
	procs->LinkShader = D3D8_LinkShader;
	procs->DestroyShader = D3D8_DestroyShader;

	return true;
}

void
D3D8_DestroyDevice(void)
{
	if (D3D8_device)
		IDirect3DDevice8_Release(D3D8_device);
}