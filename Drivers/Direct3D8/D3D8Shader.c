#include <stdlib.h>

#include <Engine/IO.h>
#include <System/Log.h>
#include <Render/Shader.h>
#include <Runtime/Runtime.h>

#include "D3D8Drv.h"

#define SHADER_PATH_SIZE	1024
#define SHADER_BASE_PATH	"/Shaders/D3D8"

#ifdef _XBOX
#	define VS_EXT	"xvu"
#	define PS_EXT	"xpu"
#else
#	define VS_EXT	"vso"
#	define PS_EXT	"pso"
#endif

struct ShaderModule
{
	enum ShaderStage stage;
	DWORD handle;
	const struct InputLayout *layout;
};

static inline bool _LoadShader(const char *name, const char *ext, void **code);

struct InputLayout *
D3D8_CreateInputLayout(const struct InputLayoutDesc *desc)
{
	uint32_t i = 0;
	struct InputLayout *il = NULL;
	const size_t size = sizeof(*il) + (2 + desc->attributeCount) * sizeof(DWORD);

	il = calloc(1, size);
	if (!il)
		return NULL;

	il->vertexDecl = (DWORD *)(((uint8_t *)il) + sizeof(*il));
	il->vertexDecl[0] = D3DVSD_STREAM(0);

	for (i = 1; i <= desc->attributeCount; ++i) {
		DWORD type = 0;
		const struct InputAttribute *a = &desc->attributes[i - 1];

		// see d3d8types.h
		switch (a->type) {
		case IAT_FLOAT:
			type = a->count - 1;
			il->stride += sizeof(float) * a->count;
		break;
		case IAT_COLOR:
			type = D3DVSDT_D3DCOLOR;
			il->stride += sizeof(UINT);
		break;
		case IAT_SHORT:
			type = a->count + 5;
			il->stride += sizeof(short) * a->count;
		break;
		}

		il->vertexDecl[i] = D3DVSD_REG(a->binding, type);
	}

	il->vertexDecl[desc->attributeCount + 1] = D3DVSD_END();

	return il;
}

void
D3D8_DestroyInputLayout(struct InputLayout *layout)
{
	free(layout);
}

struct ShaderModule *
D3D8_CreateVertexModule(const char *name, const struct InputLayout *layout)
{
	DWORD usage = 0;
	HRESULT hr = S_FALSE;
	DWORD *code = NULL;
	
	struct ShaderModule *sm = malloc(sizeof(*sm));
	if (!sm)
		return NULL;

	sm->stage = SS_VERTEX;
	sm->layout = layout;

	if (!_LoadShader(name, VS_EXT, &code)) {
		free(sm);
		return NULL;
	}

	hr = IDirect3DDevice8_CreateVertexShader(D3D8_device, layout->vertexDecl, code, &sm->handle, usage);
	if (FAILED(hr)) {
		Sys_LogEntry(D3D8DRV_MOD, LOG_CRITICAL, "Failed to create vertex shader %s: 0x%x", name, hr);
		free(sm);
		return NULL;
	}

	return sm;
}

struct ShaderModule *
D3D8_CreateFragmentModule(const char *name)
{
	DWORD usage = 0;
	HRESULT hr = S_FALSE;
	DWORD *code = NULL;
#ifdef _XBOX
	D3DPIXELSHADERDEF_FILE psFile = { 0 };
#endif

	struct ShaderModule *sm = malloc(sizeof(*sm));
	if (!sm)
		return NULL;

	sm->stage = SS_FRAGMENT;

	if (!_LoadShader(name, PS_EXT, &code)) {
		free(sm);
		return NULL;
	}

#ifndef _XBOX
	hr = IDirect3DDevice8_CreatePixelShader(D3D8_device, code, &sm->handle);
	if (FAILED(hr)) {
		Sys_LogEntry(D3D8DRV_MOD, LOG_CRITICAL, "Failed to create pixel shader %s: 0x%x", name, hr);
		free(sm);
		return NULL;
	}
#else
	memcpy(&psFile, code, sizeof(psFile));
	if (psFile.FileID != D3DPIXELSHADERDEF_FILE_ID) {
		Sys_LogEntry(D3D8DRV_MOD, LOG_CRITICAL, "%s is not a valid pixel shader");
		free(sm);
		return NULL;
	}

	hr = IDirect3DDevice8_CreatePixelShader(D3D8_device, &psFile.Psd, &sm->handle);
	if (FAILED(hr)) {
		Sys_LogEntry(D3D8DRV_MOD, LOG_CRITICAL, "Failed to create pixel shader %s: 0x%x", name, hr);
		free(sm);
		return NULL;
	}
#endif

	return sm;
}

void
D3D8_DestroyShaderModule(struct ShaderModule *sm)
{
	switch (sm->stage) {
	case SS_VERTEX: IDirect3DDevice8_DeleteVertexShader(D3D8_device, sm->handle); break;
	case SS_FRAGMENT: IDirect3DDevice8_DeletePixelShader(D3D8_device, sm->handle); break;
	}

	free(sm);
}

struct Shader *
D3D8_CreateShader(void)
{
	struct Shader *s = calloc(1, sizeof(*s));
	return s;
}

void
D3D8_AddShaderModule(struct Shader *s, const struct ShaderModule *module)
{
	if (module->stage == SS_VERTEX) {
		s->vertex = module->handle;
		s->layout = module->layout;
	} else if (module->stage == SS_FRAGMENT) {
		s->pixel = module->handle;
	}
}

bool
D3D8_LinkShader(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc)
{
	uint32_t i = 0;

	s->vertexConstantCount = bindingLayoutDesc->vertexConstantCount;
	s->vertexConstantRegisters = calloc(s->vertexConstantCount, sizeof(*s->vertexConstantRegisters));

	for (i = 0; i < s->vertexConstantCount; ++i)
		s->vertexConstantRegisters[i] = bindingLayoutDesc->vertexConstants[i].reg;

	s->pixelConstantCount = bindingLayoutDesc->fragmentConstantCount;
	s->pixelConstantRegisters = calloc(s->pixelConstantCount, sizeof(*s->pixelConstantRegisters));

	for (i = 0; i < s->pixelConstantCount; ++i)
		s->pixelConstantRegisters[i] = bindingLayoutDesc->fragmentConstants[i].reg;

	return true;
}

void
D3D8_DestroyShader(struct Shader *s)
{
	free(s);
}

static inline bool
_LoadShader(const char *name, const char *ext, void **code)
{
	File f;
	int64_t codeSize;

	char *path = Sys_Alloc(sizeof(*path), SHADER_PATH_SIZE, MH_Transient);
	if (!path)
		return false;

	snprintf(path, SHADER_PATH_SIZE, "%s/%s.%s", SHADER_BASE_PATH, name, ext); 

	f = E_OpenFile(path, IO_READ);
	if (!f)
		return false;

	*code = E_ReadFileBlob(f, &codeSize, true);
	E_CloseFile(f);

	return *code != NULL;
}
