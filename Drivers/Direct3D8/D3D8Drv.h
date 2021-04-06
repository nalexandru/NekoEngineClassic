#ifndef _DIRECT3D8_DRIVER_H_
#define _DIRECT3D8_DRIVER_H_

#ifdef _XBOX
#include <xtl.h>
#endif

#include <d3d8.h>

#include <Render/Types.h>

#define D3D8DRV_MOD	"Direct3D8Driver"

struct Buffer
{
	union {
		IDirect3DVertexBuffer8 *vtxBuff;
		IDirect3DIndexBuffer8 *idxBuff;
	};
	uint32_t size;
	enum BufferType type;
};

struct Texture
{
	IDirect3DTexture8 *tex;
	uint32_t width, height, levels;
	enum TextureFormat format;
	UINT formatSize;
};

struct D3D8ConstantBinding
{
	uint32_t startRegister;
};

struct Shader
{
	DWORD vertex;
	DWORD pixel;
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
	DWORD *vertexDecl;
};

extern IDirect3D8 *D3D8_d3d;
extern IDirect3DDevice8 *D3D8_device;
extern D3DCAPS8 D3D8_deviceCaps;
extern D3DPRESENT_PARAMETERS D3D8_presentParams;

// Device
bool D3D8_CreateDevice(struct RenderDeviceInfo *info, struct RenderDeviceProcs *procs);
void D3D8_DestroyDevice(void);

// Texture
struct Texture *D3D8_CreateTexture(const struct TextureCreateInfo *tci);
void D3D8_UpdateTexture(struct Texture *tex, const void *data);
void D3D8_DestroyTexture(struct Texture *tex);

// Buffer
struct Buffer *D3D8_CreateBuffer(const struct BufferCreateInfo *bci);
void D3D8_UpdateBuffer(struct Buffer *buff, const void *data, uint32_t offset, uint32_t dataSize);
void *D3D8_LockBuffer(struct Buffer *buff);
void D3D8_UnlockBuffer(struct Buffer *buff);
void D3D8_DestroyBuffer(struct Buffer *buff);

// Shader
struct InputLayout *D3D8_CreateInputLayout(const struct InputLayoutDesc *desc);
void D3D8_DestroyInputLayout(struct InputLayout *layout);

struct ShaderModule *D3D8_CreateVertexModule(const char *name, const struct InputLayout *layout);
struct ShaderModule *D3D8_CreateFragmentModule(const char *name);
void D3D8_DestroyShaderModule(struct ShaderModule *sm);

struct Shader *D3D8_CreateShader(void);
void D3D8_AddShaderModule(struct Shader *s, const void *module);
bool D3D8_LinkShader(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc);
void D3D8_DestroyShader(struct Shader *s);

#endif /* _DIRECT3D8_DRIVER_H_ */