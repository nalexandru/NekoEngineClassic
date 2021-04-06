#include <System/Log.h>
#include <Render/Render.h>
#include <Render/Material.h>
#include <Engine/Resource.h>
#include <Engine/Asset.h>
#include <Runtime/Json.h>

#define MAT_MOD	"Material"

static inline bool _InitMaterial(struct Material *mat, const struct MaterialCreateInfo *ci);

void
Re_InitMaterial(struct Material *mat, enum MaterialType type)
{
	memset(mat, 0x0, sizeof(*mat));
	
	mat->type = type;
	
	v4(&mat->diffuseColor, 1.f, 1.f, 1.f, 1.f);
	v4(&mat->specularColor, 1.f, 1.f, 1.f, 1.f);
	v4(&mat->emissionColor, 0.f, 0.f, 0.f, 0.f);
	
	mat->diffuseMap = mat->normalMap = E_INVALID_HANDLE;
}

void
Re_SetMaterial(const struct Material *mat)
{
	float properties[4] = { mat->specularPower, 0.f, 0.f, 0.f };

	Re_SetFragmentConstants(MAT_DIFFUSE_COLOR, &mat->diffuseColor.x, 4);
	
	if (mat->type != MAT_UNLIT) {
		Re_SetFragmentConstants(MAT_SPECULAR_COLOR, &mat->specularColor.x, 4);
		Re_SetFragmentConstants(MAT_EMISSION_COLOR, &mat->emissionColor.x, 4);

		Re_SetFragmentConstants(MAT_PROPERTIES, properties, 4);
		
	/*	if (mat->type == MAT_FRAGMENT_LIT)
			Re_SetFragmentConstants(MAT_LIGHTS, NULL, 0);
		else
			Re_SetVertexConstants(MAT_LIGHTS, NULL, 0);*/
	}

	if (mat->diffuseMap != E_INVALID_HANDLE)
		Re_BindTexture(mat->diffuseMap, MAT_DIFFUSE_MAP);

	if (mat->normalMap != E_INVALID_HANDLE)
		Re_BindTexture(mat->normalMap, MAT_NORMAL_MAP);
}

bool
Re_CreateMaterialResource(const char *name, const struct MaterialCreateInfo *ci, struct Material *mat, Handle h)
{
	return _InitMaterial(mat, ci);
}

bool
Re_LoadMaterialResource(struct ResourceLoadInfo *li, const char *args, struct Material *mat, Handle h)
{
	uint32_t i;
	struct Metadata meta = { NULL, NULL, 0, 0, MATERIAL_META_VER, MATERIAL_META_ID };
	struct MaterialCreateInfo ci;
	
	if (!E_LoadMetadataFromStream(&meta, &li->stm))
		return false;

	memset(&ci, 0x0, sizeof(ci));

	for (i = 0; i < meta.tokenCount; ++i) {
		jsmntok_t key = meta.tokens[i];
		jsmntok_t val = meta.tokens[++i];

		meta.json[val.end] = 0x0;

		if (JSON_STRING("type", key, meta.json)) {
			if (JSON_STRING("unlit", val, meta.json))
				ci.type = MAT_UNLIT;
			else if (JSON_STRING("vertexLit", val, meta.json))
				ci.type = MAT_VERTEX_LIT;
			else if (JSON_STRING("fragmentLit", val, meta.json))
				ci.type = MAT_FRAGMENT_LIT;
		} else if (JSON_STRING("shader", key, meta.json)) {
			ci.shader = meta.json + val.start;
		} else if (JSON_STRING("diffuseMap", key, meta.json)) {
			ci.diffuseMap = meta.json + val.start;
		} else if (JSON_STRING("normalMap", key, meta.json)) {
			ci.normalMap = meta.json + val.start;
		} else if (JSON_STRING("emissionMap", key, meta.json)) {
			ci.emissionMap = meta.json + val.start;
		} else if (JSON_STRING("diffuseColor", key, meta.json)) {
			E_LoadMetadataFloatVector(&meta, &val, &ci.diffuseColor.x, 4);
		} else if (JSON_STRING("specularColor", key, meta.json)) {
			E_LoadMetadataFloatVector(&meta, &val, &ci.specularColor.x, 4);
		} else if (JSON_STRING("emissionColor", key, meta.json)) {
			E_LoadMetadataFloatVector(&meta, &val, &ci.emissionColor.x, 4);
		} else if (JSON_STRING("specularPower", key, meta.json)) {
			E_LoadMetadataFloatVector(&meta, &val, &ci.specularPower, 1);
		} else if (JSON_STRING("transparent", key, meta.json)) {
			ci.transparent = strstr(meta.json + val.start, "true") != NULL;
		}
	}

	return _InitMaterial(mat, &ci);
}

void
Re_UnloadMaterialResource(struct Material *mat, Handle h)
{
	if (mat->diffuseMap != E_INVALID_HANDLE)
		E_UnloadResource(mat->diffuseMap);

	if (mat->normalMap != E_INVALID_HANDLE)
		E_UnloadResource(mat->normalMap);
}

static inline bool
_InitMaterial(struct Material *mat, const struct MaterialCreateInfo *ci)
{
	memset(mat, 0x0, sizeof(*mat));
	mat->diffuseMap = E_INVALID_HANDLE;
	mat->normalMap = E_INVALID_HANDLE;
	
	mat->type = ci->type;
	
	v4_copy(&mat->diffuseColor, &ci->diffuseColor);
	v4_copy(&mat->specularColor, &ci->specularColor);
	v4_copy(&mat->emissionColor, &ci->emissionColor);

	mat->transparent = ci->transparent;
	mat->specularPower = ci->specularPower;

	if (ci->shader && !(mat->shader = Re_GetShader(ci->shader))) {
		Sys_LogEntry(MAT_MOD, LOG_WARNING, "Failed to load shader [%s]", ci->shader);
		goto error;
	}

	if (ci->diffuseMap && (mat->diffuseMap = E_LoadResource(ci->diffuseMap, RES_TEXTURE)) == E_INVALID_HANDLE) {
		Sys_LogEntry(MAT_MOD, LOG_WARNING, "Failed to load diffuse map [%s]", ci->diffuseMap);
		goto error;
	}

	if (ci->normalMap && (mat->normalMap = E_LoadResource(ci->normalMap, RES_TEXTURE)) == E_INVALID_HANDLE) {
		Sys_LogEntry(MAT_MOD, LOG_WARNING, "Failed to load normal map [%s]", ci->normalMap);
		goto error;
	}

	return true;

error:
	if (mat->diffuseMap != E_INVALID_HANDLE)
		E_UnloadResource(mat->diffuseMap);

	if (mat->normalMap != E_INVALID_HANDLE)
		E_UnloadResource(mat->normalMap);

	return false;
}
