#include <Engine/IO.h>
#include <Engine/Asset.h>
#include <Engine/Resource.h>
#include <Render/Model.h>
#include <Render/Render.h>

bool _CreateBuffers(struct Model *mdl);

bool
Re_CreateModelResource(const char *name, const struct ModelCreateInfo *ci, struct Model *mdl, Handle h)
{
	const size_t vertexSize = sizeof(*mdl->vertices) * ci->vertexCount;
	const size_t indexSize = sizeof(*mdl->indices) * ci->indexCount;
	const size_t meshSize = sizeof(*mdl->meshes) * ci->meshCount;
	
	if (!(mdl->vertices = calloc(1, vertexSize)))
		goto error;
	memcpy(mdl->vertices, ci->vertices, vertexSize);
	mdl->vertexCount = ci->vertexCount;
	
	if (!(mdl->indices = calloc(1, indexSize)))
		goto error;
	memcpy(mdl->indices, ci->indices16, indexSize);
	mdl->indexCount = ci->indexCount;
	
	if (!(mdl->meshes = calloc(1, meshSize)))
		goto error;
	memcpy(mdl->meshes, ci->meshes, meshSize);
	mdl->meshCount = ci->meshCount;
	
	// TODO: materials

	return _CreateBuffers(mdl);
	
error:
	free(mdl->vertices);
	free(mdl->indices);
	free(mdl->meshes);
	
	return false;
}

bool
Re_LoadModelResource(struct ResourceLoadInfo *li, const char *args, struct Model *mdl, Handle h)
{
	bool rc = false;
	
	if (strstr(li->path, ".glb") || strstr(li->path, ".gltf"))
		rc = E_LoadglTFAsset(li->path, &li->stm, mdl);
	else
		rc = E_LoadNMeshAsset(&li->stm, mdl);
	
	if (!rc)
		return false;
		
	return _CreateBuffers(mdl);		
}

void
Re_UnloadModelResource(struct Model *mdl, Handle h)
{
	uint32_t i;
	for (i = 0; i < mdl->meshCount; ++i) {
		// destroy material
	}
	
	free(mdl->vertices);
	free(mdl->indices);
	free(mdl->meshes);
}

bool
Re_InitModelRender(struct ModelRender *mr, const void **args)
{
	for (; args && *args; ++args) {
		const char *arg = *args;
		size_t len = strlen(arg);

		if (!strncmp(arg, "Model", len)) {
			mr->model = E_LoadResource(*(++args), RES_MODEL);
		} else if (!strncmp(arg, "Material", len)) {
			
		}
	}

	return true;
}

void
Re_TermModelRender(struct ModelRender *mr)
{
	uint32_t i;
	struct Model *m = E_ResourcePtr(mr->model);

	for (i = 0; i < m->meshCount; ++i)
		E_UnloadResource(mr->materials[i]);

	E_UnloadResource(mr->model);
	
	free(mr->materials);
}

bool
_CreateBuffers(struct Model *mdl)
{
	struct BufferCreateInfo vertexInfo =
	{
		sizeof(*mdl->vertices) * mdl->vertexCount,
		BT_VERTEX_BUFFER,
		BU_STATIC,
		0,
		mdl->vertices,
		0, sizeof(*mdl->vertices) * mdl->vertexCount,
		true
	};
	struct BufferCreateInfo indexInfo =
	{
		sizeof(uint16_t) * mdl->indexCount,
		BT_INDEX_BUFFER,
		BU_STATIC,
		IT_UINT_16,
		mdl->indices,
		0, sizeof(uint16_t) * mdl->indexCount,
		true
	};

	mdl->vertexBuffer = Re_CreateBuffer(&vertexInfo);
	mdl->indexBuffer = Re_CreateBuffer(&indexInfo);

	return true;
}
