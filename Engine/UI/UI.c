#include <assert.h>

#include <Math/Math.h>
#include <Engine/IO.h>
#include <Engine/Asset.h>
#include <Engine/Engine.h>
#include <Engine/ECSystem.h>
#include <Engine/Resource.h>
#include <Render/Render.h>
#include <Runtime/Runtime.h>

#include "UIInternal.h"

struct UpdateBufferArgs
{
	struct UIVertex *vertices;
	uint16_t *indices;
	uint32_t vertexCount, indexCount;
};

struct Font UI_sysFont;

static struct mat4 _projMatrix;
static struct Buffer *_vertexBuffer, *_indexBuffer;
static struct Shader *_textShader;

bool
UI_InitUI(void)
{
	struct Stream stm;

	struct BufferCreateInfo vertexInfo =
	{
		1000 * sizeof(struct UIVertex),
		BT_VERTEX_BUFFER,
		BU_DYNAMIC
	};
	struct BufferCreateInfo indexInfo =
	{
		4000 * sizeof(uint16_t),
		BT_INDEX_BUFFER,
		BU_DYNAMIC,
		IT_UINT_16
	};

	E_FileStream("/System/System.fnt", IO_READ, &stm);
	E_LoadFontAsset(&stm, &UI_sysFont);
	E_CloseStream(&stm);

	_vertexBuffer = Re_CreateBuffer(&vertexInfo);
	_indexBuffer = Re_CreateBuffer(&indexInfo);

	_textShader = Re_GetShader("Text");

	return _vertexBuffer && _indexBuffer && _textShader;
}

void
UI_TermUI(void)
{
	Re_DestroyBuffer(_indexBuffer);
	Re_DestroyBuffer(_vertexBuffer);

	E_UnloadResource(UI_sysFont.texture);
	free(UI_sysFont.glyphs);
}

void
UI_Render(struct Scene *s)
{
	struct UpdateBufferArgs updateArgs = { 0 };

	m4_ortho(&_projMatrix, 0.f, (float)*E_screenWidth, (float)*E_screenHeight, 0.f, 0.f, 1.f);

	updateArgs.vertices = Re_LockBuffer(_vertexBuffer);
	updateArgs.indices = Re_LockBuffer(_indexBuffer);

	E_ExecuteSystemS(s, UI_UPDATE_BUFFERS, &updateArgs);

	Re_UnlockBuffer(_vertexBuffer);
	Re_UnlockBuffer(_indexBuffer);

	Re_BindShader(_textShader);
	Re_BindVertexBuffer(_vertexBuffer);
	Re_BindIndexBuffer(_indexBuffer);

	Re_SetVertexConstants(0, _projMatrix.m, 16);
	
	Re_Blend(true);
	Re_BlendFunc(BF_SRC_ALPHA, BF_ONE_MINUS_SRC_ALPHA);

	E_ExecuteSystemS(s, UI_DRAW_CONTEXT, NULL);
	
	Re_Blend(false);
}

void
UI_SetFont(struct UIContext *ctx, struct Font *font)
{
}

void
UI_SetStyle(struct UIContext *ctx, struct UIStyle *style)
{
}

bool
UI_InitContext(struct UIContext *ctx, const void **args)
{
	uint32_t vertexCount = 64, indexCount = 100, drawCallCount = 10;

	for (; args && *args; ++args) {
		const char *arg = *args;
		size_t len = strlen(arg);

		if (!strncmp(arg, "VertexCount", len))
			vertexCount = atoi(*(++args));
		else if (!strncmp(arg, "IndexCount", len))
			indexCount = atoi(*(++args));
		else if (!strncmp(arg, "DrawCallCount", len))
			drawCallCount = atoi(*(++args));
	}
	
	if (!Rt_InitArray(&ctx->vertices, vertexCount, sizeof(struct UIVertex)))
		return false;

	if (!Rt_InitArray(&ctx->indices, indexCount, sizeof(uint16_t)))
		return false;

	if (!Rt_InitArray(&ctx->draws, drawCallCount, sizeof(struct UIDrawCmd)))
		return false;

	return true;
}

void
UI_TermContext(struct UIContext *ctx)
{
	Rt_TermArray(&ctx->vertices);
	Rt_TermArray(&ctx->indices);
	Rt_TermArray(&ctx->draws);
}

void
UI_UpdateBuffers(void **comp, void *a)
{
	size_t i = 0;
	struct UIContext *ctx = comp[0];
	struct UpdateBufferArgs *args = a;
	
	for (i = 0; i < ctx->draws.count; ++i) {
		struct UIDrawCmd dc = *(struct UIDrawCmd *)Rt_ArrayGet(&ctx->draws, i);

		assert(args->vertexCount + dc.vtxCount < 1000);
		assert(args->indexCount + dc.idxCount < 4000);

		memcpy(args->vertices, Rt_ArrayGet(&ctx->vertices, dc.vtxOffset), sizeof(*args->vertices) * dc.vtxCount);
		args->vertices += dc.vtxCount;

		memcpy(args->indices, Rt_ArrayGet(&ctx->indices, dc.idxOffset), sizeof(*args->indices) * dc.idxCount);
		args->indices += dc.idxCount;

		args->vertexCount += dc.vtxCount;

		dc.idxOffset = args->indexCount;
		args->indexCount += dc.idxCount;

		*(struct UIDrawCmd *)Rt_ArrayGet(&ctx->draws, i) = dc;
	}
}

void
UI_DrawContext(void **comp, void *args)
{
	size_t i;
	struct UIContext *ctx = comp[0];

	for (i = 0; i < ctx->draws.count; ++i) {
		const struct UIDrawCmd dc = *(const struct UIDrawCmd *)Rt_ArrayGet(&ctx->draws, i);
		
		Re_BindTexture(dc.texture, 0);
		Re_DrawIndexed(PT_TRIANGLES, dc.vtxCount, dc.idxOffset, dc.idxCount);
	}

	Rt_ClearArray(&ctx->vertices, false);
	Rt_ClearArray(&ctx->indices, false);
	Rt_ClearArray(&ctx->draws, false);
}