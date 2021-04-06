#include <assert.h>

#include <Engine/IO.h>
#include <Engine/Asset.h>
#include <Render/Device.h>
#include <Render/Shader.h>
#include <Runtime/Array.h>
#include <Runtime/Json.h>
#include <System/Log.h>

#define SHMOD					"Shader"

#define SHADER_META_ID			"NeShader"
#define SHADER_META_VER			1

#define SSTR_VERTEX				"vertex"
#define SSTR_FRAGMENT			"fragment"

struct StageInfo
{
	enum ShaderStage stage;
	const char *module;
};

struct ShaderInfo
{
	uint64_t hash;
	struct Shader *shader;
	struct StageInfo stages[2];
};

struct ShaderModuleInfo
{
	uint64_t hash;
	struct ShaderModule *module;
};

struct InputLayoutInfo
{
	uint64_t hash;
	struct InputLayout *layout;
};

static struct Array _modules, _shaders, _layouts;

static void _LoadShader(const char *path);
static inline uint32_t _ReadShaderStage(struct Metadata *meta, struct ShaderInfo *info, uint32_t count, uint32_t pos);
static inline uint32_t _ReadInputLayout(struct Metadata *meta, struct InputLayoutDesc *desc, uint32_t count, uint32_t pos);
static inline uint32_t _ReadBindings(struct Metadata *meta, struct BindingDesc *bindings, uint32_t count, uint32_t pos);

static uint64_t _HashInputLayoutDesc(const struct InputLayoutDesc *desc);

static int32_t _Sort(const struct ShaderModuleInfo *a, const struct ShaderModuleInfo *b);
static int32_t _Compare(const struct ShaderModuleInfo *item, const uint64_t *hash);

bool
Re_LoadShaders(void)
{
	if (!Rt_InitArray(&_shaders, 1, sizeof(struct ShaderInfo)))
		return false;
	
	if (!Rt_InitArray(&_modules, 2, sizeof(struct ShaderModuleInfo))) {
		Rt_TermArray(&_shaders);
		return false;
	}

	if (!Rt_InitArray(&_layouts, 2, sizeof(struct InputLayoutInfo))) {
		Rt_TermArray(&_modules);
		Rt_TermArray(&_shaders);
		return false;
	}
	
	E_ProcessFiles("/Shaders", "shader", true, _LoadShader);
	Rt_ArraySort(&_shaders, (RtSortFunc)&_Sort);
	
	return true;
}

void
Re_UnloadShaders(void)
{
	size_t i = 0;

	for (i = 0; i < _shaders.count; ++i) {
		struct ShaderInfo *si = Rt_ArrayGet(&_shaders, i);
		Re_DestroyShader(si->shader);
	}

	for (i = 0; i < _modules.count; ++i) {
		struct ShaderModuleInfo *smi = Rt_ArrayGet(&_modules, i);
		Re_DestroyShaderModule(smi->module);
	}

	for (i = 0; i < _layouts.count; ++i) {
		struct InputLayoutInfo *layout = Rt_ArrayGet(&_layouts, i);
		Re_DestroyInputLayout(layout->layout);
	}

	Rt_TermArray(&_shaders);
	Rt_TermArray(&_modules);
	Rt_TermArray(&_layouts);
}

struct Shader *
Re_GetShader(const char *name)
{
	struct ShaderInfo *info = NULL;
	uint64_t hash = Rt_HashString(name);
	info = Rt_ArrayBSearch(&_shaders, &hash, (RtCmpFunc)_Compare);
	return info ? info->shader : NULL;
}

static void
_LoadShader(const char *path)
{
	uint32_t i;
	struct Metadata meta = { NULL, NULL, 0, 0, SHADER_META_VER, SHADER_META_ID };
	struct ShaderInfo info = { 0 };
	struct InputLayout *layout = NULL;
	struct InputLayoutDesc layoutDesc = { 0 };
	struct BindingLayoutDesc bindingLayoutDesc = { 0 };

	if (!E_LoadMetadata(&meta, path))
		return;
	
	for (i = 0; i < meta.tokenCount; ++i) {
		jsmntok_t key = meta.tokens[i];
		jsmntok_t val = meta.tokens[++i];

		if (JSON_STRING("name", key, meta.json)) {
			char *tmp = meta.json + val.start;
			meta.json[val.end] = 0x0;
			
			info.hash = Rt_HashString(tmp);
		} else if (JSON_STRING("modules", key, meta.json)) {
			if (val.type != JSMN_ARRAY)
				continue;

			if (val.size > 2) {
				Sys_LogEntry(SHMOD, LOG_WARNING, "Too many shader stages");
				return;
			}

			i = _ReadShaderStage(&meta, &info, val.size, i);
		} else if (JSON_STRING("layout", key, meta.json)) {
			uint64_t hash;
			struct InputLayoutInfo *ili = NULL;

			if (val.type != JSMN_ARRAY)
				continue;

			i = _ReadInputLayout(&meta, &layoutDesc, val.size, i);

			hash = _HashInputLayoutDesc(&layoutDesc);
			ili = Rt_ArrayBSearch(&_layouts, &hash, (RtCmpFunc)_Compare);
			if (!ili) {
				struct InputLayoutInfo newIli = { hash, NULL };

				newIli.layout = Re_CreateInputLayout(&layoutDesc);
				if (!newIli.layout) {
					Sys_LogEntry(SHMOD, LOG_CRITICAL, "Failed to create input layout for shader %s", path);
					goto error;
				}

				ili = &newIli;

				Rt_ArrayAdd(&_layouts, &newIli);
				Rt_ArraySort(&_layouts, (RtSortFunc)_Sort);
			}
			
			layout = ili->layout;
		} else if (JSON_STRING("vertexConstants", key, meta.json)) {
			if (val.type != JSMN_ARRAY)
				continue;

			bindingLayoutDesc.vertexConstantCount = val.size;
			bindingLayoutDesc.vertexConstants = Sys_Alloc(sizeof(*bindingLayoutDesc.vertexConstants), val.size, MH_Transient);

			i = _ReadBindings(&meta, (struct BindingDesc *)bindingLayoutDesc.vertexConstants, val.size, i);
		} else if (JSON_STRING("fragmentConstants", key, meta.json)) {
			if (val.type != JSMN_ARRAY)
				continue;

			bindingLayoutDesc.fragmentConstantCount = val.size;
			bindingLayoutDesc.fragmentConstants = Sys_Alloc(sizeof(*bindingLayoutDesc.fragmentConstants), val.size, MH_Transient);

			i = _ReadBindings(&meta, (struct BindingDesc *)bindingLayoutDesc.fragmentConstants, val.size, i);
		} else if (JSON_STRING("textures", key, meta.json)) {
			if (val.type != JSMN_ARRAY)
				continue;

			bindingLayoutDesc.textureCount = val.size;
			bindingLayoutDesc.textures = Sys_Alloc(sizeof(*bindingLayoutDesc.textures), val.size, MH_Transient);

			i = _ReadBindings(&meta, (struct BindingDesc *)bindingLayoutDesc.textures, val.size, i);
		}
	}

	// build
	info.shader = Re_CreateShader();
	if (!info.shader) {
		Sys_LogEntry(SHMOD, LOG_CRITICAL, "Failed to create shader");
		return;
	}

	for (i = 0; i < sizeof(info.stages) / sizeof(info.stages[0]); ++i) {
		uint64_t hash = Rt_HashString(info.stages[i].module);
		struct ShaderModuleInfo *smi = Rt_ArrayBSearch(&_modules, &hash, (RtCmpFunc)_Compare);
		
		if (!smi) {
			struct ShaderModuleInfo newSmi = { hash, NULL };

			switch (info.stages[i].stage) {
			case SS_VERTEX: newSmi.module = Re_CreateVertexModule(info.stages[i].module, layout); break;
			case SS_FRAGMENT: newSmi.module = Re_CreateFragmentModule(info.stages[i].module); break;
			}

			if (!newSmi.module) {
				Sys_LogEntry(SHMOD, LOG_CRITICAL, "Failed to load %s module %s for shader %s",
					info.stages[i].stage == SS_VERTEX ? "vertex" : "fragment",
					info.stages[i].module, path);
				goto error;
			}

			smi = &newSmi;

			Rt_ArrayAdd(&_modules, &newSmi);
			Rt_ArraySort(&_modules, (RtSortFunc)_Sort);
		}
		
		Re_AddShaderModule(info.shader, smi->module);
	}

	if (!Re_LinkShader(info.shader, layout, &bindingLayoutDesc))
		goto error;
	
	Rt_ArrayAdd(&_shaders, &info);
	return;
	
error:
	if (info.shader)
		Re_DestroyShader(info.shader);
}

static inline uint32_t
_ReadShaderStage(struct Metadata *meta, struct ShaderInfo *info, uint32_t count, uint32_t pos)
{
	uint32_t i;
	for (i = 0; i < count; ++i) {
		int k, size;

		while (meta->tokens[++pos].type != JSMN_OBJECT)	;

		size = meta->tokens[pos].size;

		for (k = 0; k < size; ++k) {
			const jsmntok_t key = meta->tokens[++pos];
			const jsmntok_t val = meta->tokens[++pos];

			if (JSON_STRING("name", key, meta->json)) {
				info->stages[i].module = meta->json + val.start;
				meta->json[val.end] = 0x0;
			} else if (JSON_STRING("stage", key, meta->json)) {
				if (JSON_STRING(SSTR_VERTEX, val, meta->json))
					info->stages[i].stage = SS_VERTEX;
				else if (JSON_STRING(SSTR_FRAGMENT, val, meta->json))
					info->stages[i].stage = SS_FRAGMENT;
			}
		}
	}

	return pos;
}

static inline uint32_t
_ReadInputLayout(struct Metadata *meta, struct InputLayoutDesc *desc, uint32_t count, uint32_t pos)
{
	uint32_t i;
	struct InputAttribute *attributes;

	attributes = Sys_Alloc(sizeof(*desc->attributes), count, MH_Transient);
	assert(attributes);

	desc->attributeCount = count;
	desc->attributes = attributes;

	for (i = 0; i < count; ++i) {
		int k, size;
		struct InputAttribute attrib = { UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX };

		while (meta->tokens[++pos].type != JSMN_OBJECT)	;

		size = meta->tokens[pos].size;
		
		for (k = 0; k < size; ++k) {
			const jsmntok_t key = meta->tokens[++pos];
			const jsmntok_t val = meta->tokens[++pos];

			if (JSON_STRING("name", key, meta->json)) {
				char *tmp = meta->json + val.start;		
				meta->json[val.end] = 0x0;

				snprintf(attrib.name, sizeof(attrib.name), "%s", tmp);
			} else if (JSON_STRING("stream", key, meta->json)) {
				attrib.stream = atoi(&meta->json[val.start]);
			} else if (JSON_STRING("binding", key, meta->json)) {
				attrib.binding = atoi(&meta->json[val.start]);
			} else if (JSON_STRING("type", key, meta->json)) {
				if (JSON_STRING("float", val, meta->json))
					attrib.type = IAT_FLOAT;
				else if (JSON_STRING("color", val, meta->json))
					attrib.type = IAT_COLOR;
				else if (JSON_STRING("short", val, meta->json))
					attrib.type = IAT_SHORT;
			} else if (JSON_STRING("count", key, meta->json)) {
				attrib.count = atoi(&meta->json[val.start]);
			} else if (JSON_STRING("usage", key, meta->json)) {
				if (JSON_STRING("POSITION", val, meta->json))
					attrib.usage = IAU_POSITION;
				else if (JSON_STRING("BLENDWEIGHT", val, meta->json))
					attrib.usage = IAU_BLENDWEIGHT;
				else if (JSON_STRING("BLENDINDICES", val, meta->json))
					attrib.usage = IAU_BLENDINDICES;
				else if (JSON_STRING("NORMAL", val, meta->json))
					attrib.usage = IAU_NORMAL;
				else if (JSON_STRING("TEXCOORD", val, meta->json))
					attrib.usage = IAU_TEXCOORD;
				else if (JSON_STRING("TANGENT", val, meta->json))
					attrib.usage = IAU_TANGENT;
				else if (JSON_STRING("BINORMAL", val, meta->json))
					attrib.usage = IAU_BINORMAL;
				else if (JSON_STRING("TESSFACTOR", val, meta->json))
					attrib.usage = IAU_TESSFACTOR;
				else if (JSON_STRING("COLOR", val, meta->json))
					attrib.usage = IAU_COLOR;
			}
		}

		if (attrib.stream == UINT32_MAX || attrib.binding == UINT32_MAX ||
				attrib.type == UINT32_MAX || attrib.count == UINT32_MAX) {
			Sys_LogEntry(SHMOD, LOG_WARNING, "Invalid input attribute declaration");
			continue;
		}

		attributes[i] = attrib;
	}

	return pos;
}

static inline uint32_t
_ReadBindings(struct Metadata *meta, struct BindingDesc *bindings, uint32_t count, uint32_t pos)
{
	uint32_t i;
	for (i = 0; i < count; ++i) {
		int k, size;

		while (meta->tokens[++pos].type != JSMN_OBJECT)	;

		size = meta->tokens[pos].size;

		for (k = 0; k < size; ++k) {
			const jsmntok_t key = meta->tokens[++pos];
			const jsmntok_t val = meta->tokens[++pos];

			if (JSON_STRING("name", key, meta->json)) {
				char *tmp = meta->json + val.start;		
				meta->json[val.end] = 0x0;

				snprintf(bindings[i].name, sizeof(bindings[i].name), "%s", tmp);
			} else if (JSON_STRING("register", key, meta->json)) {
				bindings[i].reg = atoi(&meta->json[val.start]);
			}
		}
	}

	return pos;
}

uint64_t
_HashInputLayoutDesc(const struct InputLayoutDesc *desc)
{
	uint32_t i;
	uint64_t hash = desc->attributeCount;

	#define HASH_VALUE(x)		\
		hash += x;				\
		hash += (hash << 10);	\
		hash ^= (hash >> 6)

	for (i = 0; i < desc->attributeCount; ++i) {
		HASH_VALUE(desc->attributes[i].stream);
		HASH_VALUE(desc->attributes[i].binding);
		HASH_VALUE(desc->attributes[i].type);
		HASH_VALUE(desc->attributes[i].count);
		HASH_VALUE(Rt_HashString(desc->attributes[i].name));
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

static int32_t
_Sort(const struct ShaderModuleInfo *a, const struct ShaderModuleInfo *b)
{
	if (a->hash == b->hash)
		return 0;
	else if (a->hash > b->hash)
		return -1;
	else
		return 1;
}

static int32_t
_Compare(const struct ShaderModuleInfo *item, const uint64_t *hash)
{
	if (item->hash == *hash)
		return 0;
	else if (item->hash > *hash)
		return -1;
	else
		return 1;
}
