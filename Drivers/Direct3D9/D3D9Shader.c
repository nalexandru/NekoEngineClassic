#include <stdlib.h>

#include <Engine/IO.h>
#include <System/Log.h>
#include <Render/Shader.h>
#include <Runtime/Runtime.h>

#include "D3D9Drv.h"

#define SHADER_PATH_SIZE	1024
#define SHADER_BASE_PATH	"/Shaders/D3D9"

#ifdef _XBOX
#	define VS_EXT	"xvu"
#	define PS_EXT	"xpu"
#else
#	define VS_EXT	"vso"
#	define PS_EXT	"pso"
#endif

struct ShaderModule
{
	IDirect3DVertexShader9 *vertex;
	IDirect3DPixelShader9 *pixel;
	const struct InputLayout *layout;
};

static inline bool _LoadShader(const char *name, const char *ext, void **code);

struct InputLayout *
D3D9_CreateInputLayout(const struct InputLayoutDesc *desc)
{
	HRESULT hr;
	uint32_t i = 0;
	D3DVERTEXELEMENT9 *elements;
	struct InputLayout *il = calloc(1, sizeof(*il));
	if (!il)
		return NULL;

	elements = Sys_Alloc(sizeof(*elements), desc->attributeCount + 1, MH_Transient);
	if (!elements) {
		free(il);
		return NULL;
	}

	for (i = 0; i < desc->attributeCount; ++i) {
		DWORD type = 0;
		const struct InputAttribute *a = &desc->attributes[i];

		//elements[i].Usage = a->binding;
		elements[i].UsageIndex = 0;
		elements[i].Stream = 0;
		elements[i].Method = D3DDECLMETHOD_DEFAULT;
		elements[i].Offset = il->stride;

		// see d3d9types.h
		switch (a->type) {
		case IAT_FLOAT:
			elements[i].Type = a->count - 1;
			il->stride += sizeof(float) * a->count;
		break;
		case IAT_COLOR:
			type = D3DDECLTYPE_D3DCOLOR;
			il->stride += sizeof(UINT);
		break;
		case IAT_SHORT:
			type = a->count + 5;
			il->stride += sizeof(short) * a->count;
		break;
		}

		switch (a->usage) {
		case IAU_POSITION: elements[i].Usage = D3DDECLUSAGE_POSITION; break;
		case IAU_BLENDWEIGHT: elements[i].Usage = D3DDECLUSAGE_BLENDWEIGHT; break;
		case IAU_BLENDINDICES: elements[i].Usage = D3DDECLUSAGE_BLENDINDICES; break;
		case IAU_NORMAL: elements[i].Usage = D3DDECLUSAGE_NORMAL; break;
		case IAU_TEXCOORD: elements[i].Usage = D3DDECLUSAGE_TEXCOORD; break;
		case IAU_TANGENT: elements[i].Usage = D3DDECLUSAGE_TANGENT; break;
		case IAU_BINORMAL: elements[i].Usage = D3DDECLUSAGE_BINORMAL; break;
		case IAU_TESSFACTOR: elements[i].Usage = D3DDECLUSAGE_TESSFACTOR; break;
		case IAU_COLOR: elements[i].Usage = D3DDECLUSAGE_COLOR; break;
		}
	}

	elements[desc->attributeCount].Stream = 0xFF;
	elements[desc->attributeCount].Type = D3DDECLTYPE_UNUSED;

	hr = IDirect3DDevice9_CreateVertexDeclaration(D3D9_device, elements, &il->vertexDecl);
	if (FAILED(IDirect3DDevice9_CreateVertexDeclaration(D3D9_device, elements, &il->vertexDecl))) {
		free(il);
		return NULL;
	}

	return il;
}

void
D3D9_DestroyInputLayout(struct InputLayout *layout)
{
	IDirect3DVertexDeclaration9_Release(layout->vertexDecl);
	free(layout);
}

struct ShaderModule *
D3D9_CreateVertexModule(const char *name, const struct InputLayout *layout)
{
	HRESULT hr = S_FALSE;
	DWORD *code = NULL;
	
	struct ShaderModule *sm = malloc(sizeof(*sm));
	if (!sm)
		return NULL;

	sm->layout = layout;
	sm->pixel = NULL;

	if (!_LoadShader(name, VS_EXT, &code)) {
		free(sm);
		return NULL;
	}

	hr = IDirect3DDevice9_CreateVertexShader(D3D9_device, code, &sm->vertex);
	if (FAILED(hr)) {
		Sys_LogEntry(D3D9DRV_MOD, LOG_CRITICAL, "Failed to create vertex shader %s: 0x%x", name, hr);
		free(sm);
		return NULL;
	}

	return sm;
}

struct ShaderModule *
D3D9_CreateFragmentModule(const char *name)
{
	HRESULT hr = S_FALSE;
	DWORD *code = NULL;
	
	struct ShaderModule *sm = malloc(sizeof(*sm));
	if (!sm)
		return NULL;

	sm->vertex = NULL;

	if (!_LoadShader(name, PS_EXT, &code)) {
		free(sm);
		return NULL;
	}

	hr = IDirect3DDevice9_CreatePixelShader(D3D9_device, code, &sm->pixel);
	if (FAILED(hr)) {
		Sys_LogEntry(D3D9DRV_MOD, LOG_CRITICAL, "Failed to create pixel shader %s: 0x%x", name, hr);
		free(sm);
		return NULL;
	}

	return sm;
}

void
D3D9_DestroyShaderModule(struct ShaderModule *sm)
{
	if (sm->vertex)
		IDirect3DVertexShader9_Release(sm->vertex);

	if (sm->pixel)
		IDirect3DPixelShader9_Release(sm->pixel);

	free(sm);
}

struct Shader *
D3D9_CreateShader(void)
{
	struct Shader *s = calloc(1, sizeof(*s));
	return s;
}

void
D3D9_AddShaderModule(struct Shader *s, const struct ShaderModule *module)
{
	if (module->vertex) {
		s->vertex = module->vertex;
		s->layout = module->layout;
	} else if (module->pixel) {
		s->pixel = module->pixel;
	}
}

bool
D3D9_LinkShader(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc)
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
D3D9_DestroyShader(struct Shader *s)
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

