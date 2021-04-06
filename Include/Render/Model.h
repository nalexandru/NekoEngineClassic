#ifndef _RE_MODEL_H_
#define _RE_MODEL_H_

#include <Engine/Types.h>
#include <Runtime/Runtime.h>
#include <Engine/Component.h>

#pragma pack(push,1)
struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
	float tx, ty, tz;
	float u, v;
};
#pragma pack(pop)

struct Mesh
{
	uint32_t vertexCount;
	uint32_t indexOffset;
	uint32_t indexCount;
	const char *materialName;
};

struct Model
{
	struct Mesh *meshes;
	uint32_t meshCount;
	
	struct Buffer *vertexBuffer;
	struct Buffer *indexBuffer;

	struct Vertex *vertices;
	uint32_t vertexCount;
	
	uint32_t *indices;
	uint32_t indexCount;
};

struct ModelCreateInfo
{
	struct Vertex *vertices;
	uint32_t vertexCount;
	
	union {
		uint32_t *indices32;
		uint16_t *indices16;
	};
	uint32_t indexCount;
	
	struct Mesh *meshes;
	uint32_t meshCount;
};

struct ModelRender
{
	COMPONENT_BASE;
	
	Handle model;
	Handle *materials;
};

bool Re_CreateModelResource(const char *name, const struct ModelCreateInfo *ci, struct Model *mdl, Handle h);
bool Re_LoadModelResource(struct ResourceLoadInfo *li, const char *args, struct Model *mdl, Handle h);
void Re_UnloadModelResource(struct Model *mdl, Handle h);

bool Re_InitModelRender(struct ModelRender *mr, const void **args);
void Re_TermModelRender(struct ModelRender *mr);

#endif /* _RE_MODEL_H_ */
