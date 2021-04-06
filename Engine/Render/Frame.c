#include <Render/Render.h>
#include <Render/Model.h>
#include <Render/Material.h>
#include <Engine/Engine.h>
#include <Engine/ECSystem.h>
#include <Scene/Scene.h>
#include <Scene/Camera.h>
#include <Math/Math.h>

void
Re_RenderScene(struct Scene *s, const struct Camera *cam)
{
	uint32_t i;
	
	struct mat4 mvp;
	
	struct Buffer *vb = NULL, *ib = NULL;
	const struct Shader *shader = NULL;
	
	static uint32_t maxDrawables = 100;
	struct CollectDrawablesArgs collectArgs;
	
	collectArgs.drawables = Sys_Alloc(sizeof(*collectArgs.drawables), maxDrawables, MH_Transient);
	collectArgs.maxDrawables = maxDrawables;
	collectArgs.requiredDrawables = collectArgs.drawableCount = 0;
	
	E_ExecuteSystemS(s, RE_COLLECT_DRAWABLES, &collectArgs);
	
	if (collectArgs.requiredDrawables > collectArgs.maxDrawables)
		maxDrawables = collectArgs.requiredDrawables;
	
	m4_mul(&mvp, &cam->projMatrix, &cam->viewMatrix);
	//m4_translate(&mvp, .5f, 0.f, 0.f);

	for (i = 0; i < collectArgs.drawableCount; ++i) {
		const struct Drawable *d = &collectArgs.drawables[i];
		if (shader != d->mat->shader) {
			Re_BindShader(d->mat->shader);
			shader = d->mat->shader;
		}
		
		if (vb != d->vb) {
			Re_BindVertexBuffer(d->vb);
			vb = d->vb;
		}
			
		if (ib != d->ib) {
			Re_BindIndexBuffer(d->ib);
			ib = d->ib;
		}
		
		Re_SetVertexConstants(0, mvp.m, 16);
		
		Re_SetMaterial(d->mat);
		Re_DrawIndexed(PT_TRIANGLES, d->vertexCount, d->firstIndex, d->indexCount);
	}
}
