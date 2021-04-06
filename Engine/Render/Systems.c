#include <Render/Render.h>
#include <Render/Model.h>
#include <Render/Material.h>
#include <Engine/Resource.h>

void
Re_CollectDrawables(void **comp, struct CollectDrawablesArgs *args)
{
	uint32_t i;
	struct Transform *xform = comp[0];
	struct ModelRender *mr = comp[1];
	struct Model *mdl = NULL;

	mdl = E_ResourcePtr(mr->model);
	if (!mdl)
		return;

	if (args->drawableCount >= args->maxDrawables) {
		args->requiredDrawables += mdl->meshCount;
		return;
	}

	for (i = 0; i < mdl->meshCount; ++i) {
		struct Drawable d;

		// TODO: visibility test

		d.vb = mdl->vertexBuffer;
		d.ib = mdl->indexBuffer;
		d.vertexCount = mdl->meshes[i].vertexCount;
		d.firstIndex = mdl->meshes[i].indexOffset;
		d.indexCount = mdl->meshes[i].indexCount;
		d.mat = E_ResourcePtr(mr->materials[i]);

		args->drawables[args->drawableCount++] = d;

		if (args->drawableCount == args->maxDrawables) {
			args->requiredDrawables = args->drawableCount;
			break;
		}
	}
}
