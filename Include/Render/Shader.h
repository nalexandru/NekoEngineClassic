#ifndef _RE_SHADER_H_
#define _RE_SHADER_H_

#include <Render/Types.h>
#include <Render/Device.h>

enum InputAttributeUsage
{
	IAU_POSITION = 0,
	IAU_BLENDWEIGHT = 1,
	IAU_BLENDINDICES = 2,
	IAU_NORMAL = 3,
	IAU_TEXCOORD = 5,
	IAU_TANGENT = 6,
	IAU_BINORMAL = 7,
	IAU_TESSFACTOR = 8,
	IAU_COLOR = 10
};

enum InputAttributeType
{
	IAT_FLOAT,
	IAT_COLOR,
	IAT_SHORT
};

struct InputAttribute
{
	uint32_t stream;
	uint32_t binding;
	enum InputAttributeType type;
	uint32_t count;
	char name[32];
	enum InputAttributeUsage usage;
};

struct InputLayoutDesc
{
	uint32_t attributeCount;
	const struct InputAttribute *attributes;
};

struct BindingDesc
{
	char name[32];
	uint32_t reg;
};

struct BindingLayoutDesc
{
	uint32_t vertexConstantCount;
	const struct BindingDesc *vertexConstants;

	uint32_t fragmentConstantCount;
	const struct BindingDesc *fragmentConstants;

	uint32_t textureCount;
	const struct BindingDesc *textures;
};

bool Re_LoadShaders(void);
struct Shader *Re_GetShader(const char *name);
void Re_UnloadShaders(void);

static inline struct InputLayout *Re_CreateInputLayout(const struct InputLayoutDesc *desc)
{ return Re_deviceProcs.CreateInputLayout(desc); }
static inline void Re_DestroyInputLayout(struct InputLayout *layout)
{ Re_deviceProcs.DestroyInputLayout(layout); }

static inline struct ShaderModule *Re_CreateVertexModule(const char *name, const struct InputLayout *layout)
{ return Re_deviceProcs.CreateVertexModule(name, layout); }
static inline struct ShaderModule *Re_CreateFragmentModule(const char *name)
{ return Re_deviceProcs.CreateFragmentModule(name); }
static inline void Re_DestroyShaderModule(struct ShaderModule *sm)
{ Re_deviceProcs.DestroyShaderModule(sm); }

static inline void Re_BindShader(const struct Shader *s) { Re_deviceProcs.BindShader(s); }
static inline struct Shader *Re_CreateShader(void) { return Re_deviceProcs.CreateShader(); }
static inline void Re_AddShaderModule(struct Shader *s, const struct ShaderModule *module)
{ Re_deviceProcs.AddShaderModule(s, module); }
static inline bool Re_LinkShader(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc)
{ return Re_deviceProcs.LinkShader(s, inputLayout, bindingLayoutDesc); }
static inline void Re_DestroyShader(struct Shader *s) { Re_deviceProcs.DestroyShader(s); }

static inline void Re_SetVertexConstants(uint32_t binding, const float *value, uint32_t count)
{ Re_deviceProcs.SetVertexConstants(binding, value, count); }
static inline void Re_SetFragmentConstants(uint32_t binding, const float *value, uint32_t count)
{ Re_deviceProcs.SetFragmentConstants(binding, value, count); }

#endif /* _RE_SHADER_H_ */
