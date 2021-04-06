#include <Engine/IO.h>
#include <Engine/Asset.h>
#include <Engine/Config.h>
#include <Engine/Resource.h>
#include <Runtime/Runtime.h>

#include "UIInternal.h"

static int _compare(const void *key, const void *elem);

void
UI_DrawText(struct UIContext *ctx, const char *text, float px, float py, float size, struct Font *font)
{
	float sizeFactor;
	uint16_t vtxOffset;
	struct UIVertex v;
	struct UIDrawCmd drawCmd;

	if (!text[0])
		return;

	if (!font)
		font = &UI_sysFont;

	drawCmd.idxCount = drawCmd.vtxCount = 0;
	drawCmd.vtxOffset = (uint16_t)ctx->vertices.count;
	drawCmd.idxOffset = (uint16_t)ctx->indices.count;
	drawCmd.texture = font->texture;

	v.color[0] = v.color[1] = v.color[2] = v.color[3] = 1.f;

	sizeFactor = size / 10.f;
	py += size;

	vtxOffset = drawCmd.vtxOffset;
	for (text; *text; ++text) {
		const struct Glyph *g = NULL;
		float x, y, w, h;

	/*	g = bsearch(text, _glyphs, _glyphCount, sizoef(*_glyphs), _compre)

		if (!g)
			g = &_glyphs[*text - 0x20]; */

		if ((uint8_t)*text > 127)
			g = &font->glyphs[95];
		else
			g = &font->glyphs[*text - 0x20];

		x = px + (g->bearing.x * sizeFactor);
		y = py - (g->bearing.y * sizeFactor);
		w = (float)g->size.w * sizeFactor;
		h = (float)g->size.h * sizeFactor;
		px += g->adv * sizeFactor;

		v.posUv[0] = x;
		v.posUv[1] = y;
		v.posUv[2] = g->u;
		v.posUv[3] = g->v;
		Rt_ArrayAdd(&ctx->vertices, &v);

		v.posUv[0] = x;
		v.posUv[1] = y + h;
		v.posUv[2] = g->u;
		v.posUv[3] = g->v + g->th;
		Rt_ArrayAdd(&ctx->vertices, &v);

		v.posUv[0] = x + w;
		v.posUv[1] = y + h;
		v.posUv[2] = g->u + g->tw;
		v.posUv[3] = g->v + g->th;
		Rt_ArrayAdd(&ctx->vertices, &v);

		v.posUv[0] = x + w;
		v.posUv[1] = y;
		v.posUv[2] = g->u + g->tw;
		v.posUv[3] = g->v;
		Rt_ArrayAdd(&ctx->vertices, &v);

		*(uint16_t *)Rt_ArrayAllocate(&ctx->indices) = vtxOffset;
		*(uint16_t *)Rt_ArrayAllocate(&ctx->indices) = vtxOffset + 1;
		*(uint16_t *)Rt_ArrayAllocate(&ctx->indices) = vtxOffset + 2;

		*(uint16_t *)Rt_ArrayAllocate(&ctx->indices) = vtxOffset;
		*(uint16_t *)Rt_ArrayAllocate(&ctx->indices) = vtxOffset + 2;
		*(uint16_t *)Rt_ArrayAllocate(&ctx->indices) = vtxOffset + 3;

		vtxOffset += 4;
		drawCmd.vtxCount += 4;
		drawCmd.idxCount += 6;
	}

	Rt_ArrayAdd(&ctx->draws, &drawCmd);
}

int
_compare(const void *key, const void *elem)
{
return 0;
//	return *(uint8_t *)key - 
}

