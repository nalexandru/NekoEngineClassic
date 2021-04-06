#ifndef _RE_MATERIAL_H_
#define _RE_MATERIAL_H_

#include <Math/Math.h>
#include <Render/Shader.h>


#define MATERIAL_META_ID			"NeMaterial"
#define MATERIAL_META_VER			1

#define MAT_DIFFUSE_COLOR	0
#define MAT_SPECULAR_COLOR	1
#define MAT_EMISSION_COLOR	2
#define MAT_PROPERTIES		3

#define MAT_DIFFUSE_MAP		0
#define MAT_NORMAL_MAP		1

enum MaterialType
{
	MAT_UNLIT,
	MAT_VERTEX_LIT,
	MAT_FRAGMENT_LIT
};

struct Material
{
	enum MaterialType type;
	const struct Shader *shader;
	
	bool transparent;
	
	struct vec4 diffuseColor;
	struct vec4 specularColor;
	struct vec4 emissionColor;
	
	float specularPower;
	
	Handle diffuseMap;
	Handle normalMap;
};

struct MaterialCreateInfo
{
	const char *shader;
	const char *diffuseMap;
	const char *normalMap;
	const char *emissionMap;

	enum MaterialType type;
	bool transparent;
	float specularPower;
	struct vec4 diffuseColor;
	struct vec4 specularColor;
	struct vec4 emissionColor;
};

void Re_InitMaterial(struct Material *mat, enum MaterialType type);
void Re_SetMaterial(const struct Material *mat);

bool Re_CreateMaterialResource(const char *name, const struct MaterialCreateInfo *ci, struct Material *mat, Handle h);
bool Re_LoadMaterialResource(struct ResourceLoadInfo *li, const char *args, struct Material *mat, Handle h);
void Re_UnloadMaterialResource(struct Material *mat, Handle h);

#endif /* _RE_MATERIAL_H_ */