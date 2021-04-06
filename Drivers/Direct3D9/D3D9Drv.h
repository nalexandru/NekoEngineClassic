#ifndef _DIRECT3D9_DRIVER_H_
#define _DIRECT3D9_DRIVER_H_

#ifdef _XBOX
#include <xtl.h>
#endif

#include <d3d9.h>

#include <Render/Types.h>

#define D3D9DRV_MOD	"Direct3D9Driver"

struct Buffer
{
	union {
		IDirect3DVertexBuffer9 *vtxBuff;
		IDirect3DIndexBuffer9 *idxBuff;
	};
	uint32_t size;
	enum BufferType type;
};

struct Texture
{
	IDirect3DTexture9 *tex;
	uint32_t width, height, levels;
	enum TextureFormat format;
	UINT formatSize;
};

struct Shader
{
	IDirect3DVertexShader9 *vertex;
	IDirect3DPixelShader9 *pixel;
	const struct InputLayout *layout;

	uint8_t vertexConstantCount;
	uint8_t *vertexConstantRegisters;

	uint8_t pixelConstantCount;
	uint8_t *pixelConstantRegisters;

	uint8_t textureCount;
	uint8_t *textureRegisters;
};

struct InputLayout
{
	UINT stride;
	IDirect3DVertexDeclaration9 *vertexDecl;
};

extern IDirect3D9 *D3D9_d3d;
extern IDirect3DDevice9 *D3D9_device;
extern D3DCAPS9 D3D9_deviceCaps;
extern D3DPRESENT_PARAMETERS D3D9_presentParams;

// Device
bool D3D9_CreateDevice(struct RenderDeviceInfo *info, struct RenderDeviceProcs *procs);
void D3D9_DestroyDevice(void);

// Texture
struct Texture *D3D9_CreateTexture(const struct TextureCreateInfo *tci);
void D3D9_UpdateTexture(struct Texture *tex, const void *data);
void D3D9_DestroyTexture(struct Texture *tex);

// Buffer
struct Buffer *D3D9_CreateBuffer(const struct BufferCreateInfo *bci);
void D3D9_UpdateBuffer(struct Buffer *buff, const void *data, uint32_t offset, uint32_t dataSize);
void *D3D9_LockBuffer(struct Buffer *buff);
void D3D9_UnlockBuffer(struct Buffer *buff);
void D3D9_DestroyBuffer(struct Buffer *buff);

// Shader
struct InputLayout *D3D9_CreateInputLayout(const struct InputLayoutDesc *desc);
void D3D9_DestroyInputLayout(struct InputLayout *layout);

struct ShaderModule *D3D9_CreateVertexModule(const char *name, const struct InputLayout *layout);
struct ShaderModule *D3D9_CreateFragmentModule(const char *name);
void D3D9_DestroyShaderModule(struct ShaderModule *sm);

struct Shader *D3D9_CreateShader(void);
void D3D9_AddShaderModule(struct Shader *s, const void *module);
bool D3D9_LinkShader(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc);
void D3D9_DestroyShader(struct Shader *s);

#endif /* _DIRECT3D9_DRIVER_H_ */