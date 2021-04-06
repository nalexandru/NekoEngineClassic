#include <assert.h>

#include "GLDrv.h"

#include <Render/Device.h>
#include <Engine/Engine.h>
#include <Engine/Resource.h>

#ifndef GL_SRC1_COLOR
#	define GL_SRC1_COLOR	0
#endif

#ifndef GL_ONE_MINUS_SRC1_COLOR
#	define GL_ONE_MINUS_SRC1_COLOR	0
#endif

#ifndef GL_ONE_MINUS_SRC1_ALPHA
#	define GL_ONE_MINUS_SRC1_ALPHA	0
#endif

static GLenum NeToGLTopology[] =
{
	GL_TRIANGLES,
	GL_POINTS,
	GL_LINES
};

static GLenum NeToGLIndexType[] =
{
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT
};

static GLenum NeToGLBlendFactor[] =
{
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA,
	GL_SRC_ALPHA_SATURATE,
	GL_SRC1_COLOR,
	GL_ONE_MINUS_SRC1_COLOR,
	GL_SRC1_ALPHA,
	GL_ONE_MINUS_SRC1_ALPHA
};

static const struct Shader *_boundProgram;
static uint64_t _boundInputLayoutHash;
static GLenum _boundIndexType;

static void
_CullMode(enum CullMode mode)
{
	switch (mode) {
	case CM_NONE:
		glDisable(GL_CULL_FACE);
	break;
	case CM_FRONT:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	break;
	case CM_BACK:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	break;
	case CM_FRONT_AND_BACK:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	break;
	}
}

static void
_DepthTest(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

static void
_DepthWrite(bool enable)
{
	glDepthMask(enable);
}

static void _Blend(bool enable)
{
	if (enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

static void
_BlendFunc(enum BlendFactor srcFactor, enum BlendFactor dstFactor)
{
	glBlendFunc(NeToGLBlendFactor[srcFactor], NeToGLBlendFactor[dstFactor]);
}

static void
_BlendColor(float r, float g, float b, float a)
{
	glBlendColor(r, g, b, a);
}

static void
_ClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

static void
_BeginFrame(void)
{
}

static void
_Clear(enum ClearType type)
{
	GLbitfield mask = 0;
	
	if ((type & CT_COLOR) == CT_COLOR)
		mask |= GL_COLOR_BUFFER_BIT;
	
	if ((type & CT_DEPTH) == CT_DEPTH)
		mask |= GL_DEPTH_BUFFER_BIT;
	
	if ((type & CT_STENCIL) == CT_STENCIL)
		mask |= GL_STENCIL_BUFFER_BIT;
	
	glClear(mask);
}

static void
_EndFrame(void)
{
	_boundProgram = NULL;
}

static void
_Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

static void
_DepthRange(float zNear, float zFar)
{
	glDepthRange(zNear, zFar);
}

static void
_Scissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glScissor(x, y, width, height);
}

static void
_BindShader(const struct Shader *s)
{
	glUseProgram(s->program);
	_boundProgram = s;
}

static void
_BindIndexBuffer(const struct Buffer *buff)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff->id);
	_boundIndexType = NeToGLIndexType[buff->indexType];
}

static void
_BindVertexBuffer(const struct Buffer *buff)
{
	glBindBuffer(GL_ARRAY_BUFFER, buff->id);
	
	if (_boundInputLayoutHash != _boundProgram->inputLayout->hash) {
		uint32_t i;
		for (i = 0; i < _boundProgram->inputLayout->attributeCount; ++i) {
			const struct GLInputAttribute *a = &_boundProgram->inputLayout->attributes[i];
			
			glEnableVertexAttribArray(a->index);
			glVertexAttribPointer(a->index, a->size, a->type, GL_FALSE, _boundProgram->inputLayout->stride, a->ptr);
		}
		
		_boundInputLayoutHash = _boundProgram->inputLayout->hash;
	}
}

static void
_BindTexture(Handle texture, uint32_t binding)
{
	const struct Texture **t = E_ResourcePtr(texture);
	const struct TextureBinding *tb = &_boundProgram->textureBindings[binding];
	glActiveTexture(GL_TEXTURE0 + tb->unit);
	glBindTexture((*t)->target, (*t)->id);
	glUniform1i(tb->location, tb->unit);
}

static inline void
_SetConstants(const uint8_t *locations, uint32_t binding, const float *value, uint32_t count)
{
	switch (count) {
	case 1: glUniform1fv(locations[binding], 1, value); break;
	case 2: glUniform2fv(locations[binding], 1, value); break;
	case 3: glUniform3fv(locations[binding], 1, value); break;
	case 4: glUniform4fv(locations[binding], 1, value); break;
	case 12: glUniformMatrix3fv(locations[binding], 1, GL_FALSE, value); break;
	case 16: glUniformMatrix4fv(locations[binding], 1, GL_FALSE, value); break;
	default: assert(!"Invalid uniform value count"); break;
	}
}

static void
_SetVertexConstants(uint32_t binding, const float *value, uint32_t count)
{
	_SetConstants(_boundProgram->vertexUniformLocations, binding, value, count);
}

static void
_SetFragmentConstants(uint32_t binding, const float *value, uint32_t count)
{
	_SetConstants(_boundProgram->fragmentUniformLocations, binding, value, count);
}

static void
_Draw(enum PrimitiveTopology topology, uint32_t first, uint32_t count)
{
	glDrawArrays(NeToGLTopology[topology], first, count);
}

static void
_DrawIndexed(enum PrimitiveTopology topology, uint32_t vertexCount, uint32_t firstIndex, uint32_t indexCount)
{
	glDrawElements(NeToGLTopology[topology], indexCount, _boundIndexType, (void *)firstIndex);
}

bool
GL_CreateDevice(struct RenderDeviceInfo *info, struct RenderDeviceProcs *procs)
{
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);
	
	procs->BeginFrame = _BeginFrame;
	procs->Clear = _Clear;
	procs->Present = GL_SwapBuffers;
	procs->EndFrame = _EndFrame;

	procs->CullMode = _CullMode;
	procs->DepthTest = _DepthTest;
	procs->DepthWrite = _DepthWrite;
	procs->ClearColor = _ClearColor;
	
	procs->Blend = _Blend;
	procs->BlendFunc = _BlendFunc;
	procs->BlendColor = _BlendColor;
	
	procs->ScreenResized = GL_ScreenResized;
	procs->EnableVerticalSync = NULL;

	procs->Viewport = _Viewport;
	procs->DepthRange = _DepthRange;
	procs->Scissor = _Scissor;

	procs->BindVertexBuffer = _BindVertexBuffer;
	procs->BindIndexBuffer = _BindIndexBuffer;
	procs->BindTexture = _BindTexture;
	procs->BindShader = _BindShader;
	procs->SetVertexConstants = _SetVertexConstants;
	procs->SetFragmentConstants = _SetFragmentConstants;

	procs->Draw = _Draw;
	procs->DrawIndexed = _DrawIndexed;

	procs->CreateTexture = GL_CreateTexture;
	procs->UpdateTexture = GL_UpdateTexture;
	procs->DestroyTexture = GL_DestroyTexture;

	procs->CreateBuffer = GL_CreateBuffer;
	procs->UpdateBuffer = GL_UpdateBuffer;
	procs->LockBuffer = GL_LockBuffer;
	procs->UnlockBuffer = GL_UnlockBuffer;
	procs->DestroyBuffer = GL_DestroyBuffer;
	
	procs->CreateInputLayout = GL_CreateInputLayout;
	procs->DestroyInputLayout = GL_DestroyInputLayout;

	if (GL_useShaderObjectsARB) {
		procs->CreateVertexModule = GL_CreateVertexModule_ARB;
		procs->CreateFragmentModule = GL_CreateFragmentModule_ARB;
		procs->DestroyShaderModule = GL_DestroyShaderModule_ARB;
	
		procs->CreateShader = GL_CreateShader_ARB;
		procs->AddShaderModule = GL_AddShaderModule_ARB;
		procs->LinkShader = GL_LinkShader_ARB;
		procs->DestroyShader = GL_DestroyShader_ARB;
	} else {
		procs->CreateVertexModule = GL_CreateVertexModule_GL2;
		procs->CreateFragmentModule = GL_CreateFragmentModule_GL2;
		procs->DestroyShaderModule = GL_DestroyShaderModule_GL2;

		procs->CreateShader = GL_CreateShader_GL2;
		procs->AddShaderModule = GL_AddShaderModule_GL2;
		procs->LinkShader = GL_LinkShader_GL2;
		procs->DestroyShader = GL_DestroyShader_GL2;
	}

	return true;
}

void
GL_DestroyDevice(void)
{
}
