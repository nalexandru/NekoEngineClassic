#ifndef _RE_RENDER_H_
#define _RE_RENDER_H_

#include <Render/Types.h>

#include <Render/Model.h>
#include <Render/Driver.h>
#include <Render/Device.h>
#include <Render/Buffer.h>
#include <Render/Shader.h>
#include <Render/Texture.h>
#include <Render/Material.h>

// Systems
#define RE_COLLECT_DRAWABLES	"ReCollectDrawables"

struct CollectDrawablesArgs
{
	struct Drawable *drawables;
	uint32_t maxDrawables, drawableCount, requiredDrawables;
};

struct Drawable
{
	struct Buffer *vb, *ib;
	struct Material *mat;
	uint32_t firstIndex, indexCount;
	uint32_t vertexCount;
	float distance;
	bool transparent;
};

bool Re_Init(void);

void Re_RenderScene(struct Scene *s, const struct Camera *cam);

void Re_Term(void);

// Systems
void Re_CollectDrawables(void **comp, struct CollectDrawablesArgs *args);

#endif /* _RE_RENDER_H_ */