#ifndef _RE_DEVICE_H_
#define _RE_DEVICE_H_

#include <Render/Types.h>

struct RenderDeviceInfo
{
	char deviceName[256];
	uint64_t localMemorySize;
	
	struct {
		bool uint32IndexSupport;
	} features;
	
	struct {
		uint32_t maxTextureWidth, maxTextureHeight;
		uint32_t maxAnisotropy;
	} limits;
		
	void *private;
};

// FrontFace

struct RenderDeviceProcs
{
	void (*BindTexture)(Handle texture, uint32_t binding);
	void (*SetVertexConstants)(uint32_t binding, const float *value, uint32_t count);
	void (*SetFragmentConstants)(uint32_t binding, const float *value, uint32_t count);

	void (*Draw)(enum PrimitiveTopology topology, uint32_t first, uint32_t count);
	void (*DrawIndexed)(enum PrimitiveTopology topology, uint32_t vertexCount, uint32_t firstIndex, uint32_t indexCount);

	void (*BindIndexBuffer)(const struct Buffer *buff);
	void (*BindVertexBuffer)(const struct Buffer *buff);

	void (*CullMode)(enum CullMode mode);
	void (*DepthTest)(bool enable);
	void (*DepthWrite)(bool enable);
	
	void (*Blend)(bool enable);
	void (*BlendFunc)(enum BlendFactor srcFactor, enum BlendFactor dstFactor);
	void (*BlendColor)(float r, float g, float b, float a);

	void *(*LockBuffer)(struct Buffer *buff);
	void (*UnlockBuffer)(struct Buffer *buff);

	void (*BeginFrame)(void);
	void (*Clear)(enum ClearType type);
	void (*Present)(void);
	void (*EndFrame)(void);

	void (*Viewport)(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	void (*DepthRange)(float zNear, float zFar);
	void (*Scissor)(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	void (*BindShader)(const struct Shader *s);
	struct Buffer *(*CreateBuffer)(const struct BufferCreateInfo *bci);
	void (*UpdateBuffer)(struct Buffer *buff, const void *data, uint32_t offset, uint32_t size);
	void (*DestroyBuffer)(struct Buffer *buff);

	struct Texture *(*CreateTexture)(const struct TextureCreateInfo *tci);
	void (*UpdateTexture)(struct Texture *tex, const void *data);
	void (*DestroyTexture)(struct Texture *tex);

	struct InputLayout *(*CreateInputLayout)(const struct InputLayoutDesc *desc);
	void (*DestroyInputLayout)(struct InputLayout *layout);

	struct ShaderModule *(*CreateVertexModule)(const char *name, const struct InputLayout *layout);
	struct ShaderModule *(*CreateFragmentModule)(const char *name);
	void (*DestroyShaderModule)(struct ShaderModule *sm);

	struct Shader *(*CreateShader)(void);
	void (*AddShaderModule)(struct Shader *s, const struct ShaderModule *module);
	bool (*LinkShader)(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc);
	void (*DestroyShader)(struct Shader *s);

	void (*ScreenResized)(void);
	void (*EnableVerticalSync)(bool enable);
	void (*ClearColor)(float r, float g, float b, float a);
};

extern struct RenderDeviceInfo Re_deviceInfo;
extern struct RenderDeviceProcs Re_deviceProcs;

static inline void Re_Draw(enum PrimitiveTopology topology, uint32_t first, uint32_t count) { Re_deviceProcs.Draw(topology, first, count); }
static inline void Re_DrawIndexed(enum PrimitiveTopology topology, uint32_t vertexCount, uint32_t firstIndex, uint32_t indexCount)
{ Re_deviceProcs.DrawIndexed(topology, vertexCount, firstIndex, indexCount); }

static inline void Re_CullMode(enum CullMode mode) { Re_deviceProcs.CullMode(mode); }
static inline void Re_DepthTest(bool enable) { Re_deviceProcs.DepthTest(enable); }
static inline void Re_DepthWrite(bool enable) { Re_deviceProcs.DepthWrite(enable); }

static inline void Re_Blend(bool enable) { Re_deviceProcs.Blend(enable); }
static inline void Re_BlendFunc(enum BlendFactor src, enum BlendFactor dst) { Re_deviceProcs.BlendFunc(src, dst); }
static inline void Re_BlendColor(float r, float g, float b, float a) { Re_deviceProcs.BlendColor(r, g, b, a); }

static inline void Re_BeginFrame(void) { Re_deviceProcs.BeginFrame(); }
static inline void Re_Clear(enum ClearType type) { Re_deviceProcs.Clear(type); }
static inline void Re_Present(void) { Re_deviceProcs.Present(); }
static inline void Re_EndFrame(void) { Re_deviceProcs.EndFrame(); }

static inline void Re_Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { Re_deviceProcs.Viewport(x, y, width, height); }
static inline void Re_DepthRange(float zNear, float zFar) { Re_deviceProcs.DepthRange(zNear, zFar); }
static inline void Re_Scissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { Re_deviceProcs.Scissor(x, y, width, height); }

static inline void Re_ScreenResized(void) { Re_deviceProcs.ScreenResized(); }
static inline void Re_EnableVerticalSync(bool enable) { Re_deviceProcs.EnableVerticalSync(enable); }
static inline void Re_ClearColor(float r, float g, float b, float a) { Re_deviceProcs.ClearColor(r, g, b, a); }

#endif /* _RE_DEVICE_H_ */
