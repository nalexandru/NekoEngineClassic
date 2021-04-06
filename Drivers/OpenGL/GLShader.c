#include <stdlib.h>

#include <Engine/IO.h>
#include <System/Log.h>
#include <System/Memory.h>
#include <Runtime/Runtime.h>

#include "GLDrv.h"

#define PATH_BUFF_SIZE		1024
#define SHADER_BASE_PATH	"/Shaders/OpenGL"

struct ShaderModule
{
	union {
		GLuint shader;
		GLhandleARB shaderARB;
	};
	const struct InputLayout *layout;
};

static inline bool _LoadShader_GL2(GLuint s, const char *path);
static inline bool _LoadShader_ARB(GLhandleARB s, const char *path);

static inline void _CreateBindingLayout(struct Shader *s, const struct BindingLayoutDesc *desc);

struct InputLayout *
GL_CreateInputLayout(const struct InputLayoutDesc *desc)
{
	uint32_t i;
	struct InputLayout *il = malloc(sizeof(*il));
	
	if (!il)
		return NULL;
		
	il->hash = 0;
	il->attributeCount = desc->attributeCount;
	il->attributes = calloc(desc->attributeCount, sizeof(*il->attributes));
	
	if (!il->attributes) {
		free(il);
		return NULL;
	}
	
	il->hash = 0;
	il->stride = 0;
	
#define HASH_VALUE(x)		\
	il->hash += x;				\
	il->hash += (il->hash << 10);	\
	il->hash ^= (il->hash >> 6)
	
	for (i = 0; i < il->attributeCount; ++i) {
		GLsizei size = 0;
		
		il->attributes[i].ptr = (const void *)il->stride;
		il->attributes[i].size = desc->attributes[i].count;
		il->attributes[i].index = desc->attributes[i].binding;
		
		strncpy(il->attributes[i].name, desc->attributes[i].name, sizeof(il->attributes[i].name));
		
		switch (desc->attributes[i].type) {
		case IAT_FLOAT:
			size = sizeof(float) * desc->attributes[i].count;
			il->attributes[i].type = GL_FLOAT;
		break;
		case IAT_SHORT:
			size = sizeof(uint16_t) * desc->attributes[i].count;
			il->attributes[i].type = GL_UNSIGNED_SHORT;
		break;
		case IAT_COLOR:
			size = sizeof(uint32_t) * desc->attributes[i].count;
			il->attributes[i].type = GL_UNSIGNED_INT;
		break;
		}

		il->stride += size;
		
		HASH_VALUE((uintptr_t)il->attributes[i].ptr);
		HASH_VALUE(il->attributes[i].size);
		HASH_VALUE(il->attributes[i].index);
		HASH_VALUE(il->attributes[i].type);
		HASH_VALUE(Rt_HashString(il->attributes[i].name));
	}
	
	il->hash += (il->hash << 3);
	il->hash ^= (il->hash >> 11);
	il->hash += (il->hash << 15);
	
	return il;
}

void
GL_DestroyInputLayout(struct InputLayout *layout)
{
	free(layout->attributes);
	free(layout);
}

// OpenGL 2.0

struct ShaderModule *
GL_CreateVertexModule_GL2(const char *name, const struct InputLayout *layout)
{
	char *path = NULL;
	struct ShaderModule *sm = malloc(sizeof(*sm));
	if (!sm)
		return NULL;
	
	sm->layout = layout;
	sm->shader = glCreateShader(GL_VERTEX_SHADER);
	
	path = Sys_Alloc(sizeof(*path), PATH_BUFF_SIZE, MH_Transient);
	snprintf(path, PATH_BUFF_SIZE, "%s/%s.vert", SHADER_BASE_PATH, name);
	
	if (!_LoadShader_GL2(sm->shader, path)) {
		free(sm);
		return NULL;
	}

	return sm;
}

struct ShaderModule *
GL_CreateFragmentModule_GL2(const char *name)
{
	char *path;
	struct ShaderModule *sm = malloc(sizeof(*sm));
	if (!sm)
		return NULL;
	
	sm->layout = NULL;
	sm->shader = glCreateShader(GL_FRAGMENT_SHADER);
	
	path = Sys_Alloc(sizeof(*path), PATH_BUFF_SIZE, MH_Transient);
	snprintf(path, PATH_BUFF_SIZE, "%s/%s.frag", SHADER_BASE_PATH, name);
	
	if (!_LoadShader_GL2(sm->shader, path)) {
		free(sm);
		return NULL;
	}
	
	return sm;
}

void
GL_DestroyShaderModule_GL2(struct ShaderModule *sm)
{
	glDeleteShader(sm->shader);
	free(sm);
}

struct Shader *
GL_CreateShader_GL2(void)
{
	struct Shader *s = malloc(sizeof(*s));
	
	s->program = glCreateProgram();
	
	return s;
}

void
GL_AddShaderModule_GL2(struct Shader *s, const struct ShaderModule *module)
{
	if (module->layout)
		s->inputLayout = module->layout;
	
	glAttachShader(s->program, module->shader);
}

bool
GL_LinkShader_GL2(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc)
{
	GLint rc;
	uint32_t i;
	
	if (s->inputLayout && (!inputLayout || inputLayout->hash != s->inputLayout->hash))
		return false;
	
	glLinkProgram(s->program);
	glGetProgramiv(s->program, GL_LINK_STATUS, &rc);
	
	if (!rc)
		goto error;
		
	for (i = 0; i < inputLayout->attributeCount; ++i)
		glBindAttribLocation(s->program, inputLayout->attributes[i].index, inputLayout->attributes[i].name);
	
	glLinkProgram(s->program);
	glGetProgramiv(s->program, GL_LINK_STATUS, &rc);
	
	_CreateBindingLayout(s, bindingLayoutDesc);
	
	return true;
	
error:
	glGetProgramiv(s->program, GL_INFO_LOG_LENGTH, &rc);
	
	{
		char *data = Sys_Alloc(sizeof(*data), rc, MH_Transient);
		glGetProgramInfoLog(s->program, rc, NULL, data);
		
		Sys_LogEntry(GLDRV_MOD, LOG_CRITICAL, "Failed to link program: %s", data);
		
		glDeleteProgram(s->program);
	}

	return false;
}

void
GL_DestroyShader_GL2(struct Shader *s)
{
	glDeleteProgram(s->program);
	free(s);
}

static inline bool
_LoadShader_GL2(GLuint s, const char *path)
{
	char *source;
	int64_t sourceSize;
	GLint len = 0;
	File f;

	f = E_OpenFile(path, IO_READ);
	if (!f)
		return false;

	source = E_ReadFileText(f, &sourceSize, true);
	E_CloseFile(f);

	len = (GLint)sourceSize;
	glShaderSource(s, 1, (const GLchar **)&source, NULL);
	glCompileShader(s);
	glGetShaderiv(s, GL_COMPILE_STATUS, &len);
	
	if (!len) {
		char *msg = NULL;

		glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);

		msg = Sys_Alloc(sizeof(char), len, MH_Transient);
		glGetShaderInfoLog(s, len, NULL, msg);

		Sys_LogEntry(GLDRV_MOD, LOG_CRITICAL, "Failed to compile shader %s: %s", path, msg);

		glDeleteShader(s);
	
		return false;
	}
	
	return true;
}

// ARB shader objects

struct ShaderModule *
GL_CreateVertexModule_ARB(const char *name, const struct InputLayout *layout)
{
	char *path = NULL;
	struct ShaderModule *sm = malloc(sizeof(*sm));
	if (!sm)
		return NULL;
	
	sm->layout = layout;
	sm->shaderARB = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	
	path = Sys_Alloc(sizeof(*path), PATH_BUFF_SIZE, MH_Transient);
	snprintf(path, PATH_BUFF_SIZE, "%s/%s.vert", SHADER_BASE_PATH, name);
	
	if (!_LoadShader_ARB(sm->shaderARB, path)) {
		free(sm);
		return NULL;
	}
	
	return sm;
}

struct ShaderModule *
GL_CreateFragmentModule_ARB(const char *name)
{
	char *path;
	struct ShaderModule *sm = malloc(sizeof(*sm));
	if (!sm)
		return NULL;
	
	sm->layout = NULL;
	sm->shaderARB = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	
	path = Sys_Alloc(sizeof(*path), PATH_BUFF_SIZE, MH_Transient);
	snprintf(path, PATH_BUFF_SIZE, "%s/%s.frag", SHADER_BASE_PATH, name);
	
	if (!_LoadShader_ARB(sm->shaderARB, path)) {
		free(sm);
		return NULL;
	}
	
	return sm;
}

void
GL_DestroyShaderModule_ARB(struct ShaderModule *sm)
{
	glDeleteObjectARB(sm->shaderARB);
	free(sm);
}

struct Shader *
GL_CreateShader_ARB(void)
{
	struct Shader *s = malloc(sizeof(*s));
	
	s->programARB = glCreateProgramObjectARB();
	
	return s;
}

void
GL_AddShaderModule_ARB(struct Shader *s, const struct ShaderModule *module)
{
	if (module->layout)
		s->inputLayout = module->layout;
	
	glAttachObjectARB(s->programARB, module->shaderARB);
}

bool
GL_LinkShader_ARB(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc)
{
	GLint rc;
	
	if (s->inputLayout && (!inputLayout || inputLayout->hash != s->inputLayout->hash))
		return false;
	
	glLinkProgramARB(s->programARB);
	glGetObjectParameterivARB(s->programARB, GL_OBJECT_LINK_STATUS_ARB, &rc);
	
	if (!rc)
		goto error;
		
/*	for (i = 0; i < inputLayout->attributeCount; ++i)
		glBindAttribLocation(s->program, inputLayout->attributes[i].index, inputLayout->attributes[i].name);*/
	
	glLinkProgramARB(s->programARB);
	glGetObjectParameterivARB(s->programARB, GL_OBJECT_LINK_STATUS_ARB, &rc);
	
	_CreateBindingLayout(s, bindingLayoutDesc);
	
	return true;
	
error:
	/*glGetProgramiv(s->program, GL_INFO_LOG_LENGTH, &rc);
	
	{
		char *data = Sys_Alloc(sizeof(*data), rc, MH_Transient);
		glGetProgramInfoLog(s->program, rc, NULL, data);
		
		Sys_LogEntry(GLDRV_MOD, LOG_CRITICAL, "Failed to link program: %s", data);
		
		glDeleteProgram(s->program);
	}*/
	
	return false;
}

void
GL_DestroyShader_ARB(struct Shader *s)
{
	glDeleteObjectARB(s->programARB);
	free(s);
}

static inline bool
_LoadShader_ARB(GLhandleARB s, const char *path)
{
	char *source;
	int64_t sourceSize;
	GLint len = 0;
	File f;
	
	f = E_OpenFile(path, IO_READ);
	if (!f)
		return false;
	
	source = E_ReadFileText(f, &sourceSize, true);
	E_CloseFile(f);
	
	len = (GLint)sourceSize;
	glShaderSourceARB(s, 1, (const GLchar **)&source, NULL);
	glCompileShaderARB(s);
	glGetObjectParameterivARB(s, GL_COMPILE_STATUS, &len);
	
	if (!len) {
	/*	glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
		
		char *msg = Sys_Alloc(sizeof(char), len, MH_Transient);
		glGetShaderInfoLog(s, len, NULL, msg);
		
		Sys_LogEntry(GLDRV_MOD, LOG_CRITICAL, "Failed to compile shader %s: %s", path, msg);
		
		glDeleteShader(s);*/
		
		return false;
	}
	
	return true;
}

// Shared

static inline void
_CreateBindingLayout(struct Shader *s, const struct BindingLayoutDesc *desc)
{
	uint32_t i;
	
	s->vertexUniformCount = desc->vertexConstantCount;
	s->fragmentUniformCount = desc->fragmentConstantCount;
	s->textureCount = desc->textureCount;
	
	if (s->vertexUniformCount) {
		s->vertexUniformLocations = calloc(sizeof(*s->vertexUniformLocations), s->vertexUniformCount);
		for (i = 0; i < s->vertexUniformCount; ++i)
			s->vertexUniformLocations[i] = glGetUniformLocation(s->program, desc->vertexConstants[i].name);
	}
	
	if (s->fragmentUniformCount) {
		s->fragmentUniformLocations = calloc(sizeof(*s->fragmentUniformLocations), s->fragmentUniformCount);
		for (i = 0; i < s->fragmentUniformCount; ++i)
			s->fragmentUniformLocations[i] = glGetUniformLocation(s->program, desc->fragmentConstants[i].name);
	}
	
	if (s->textureCount) {
		s->textureBindings = calloc(sizeof(*s->textureBindings), s->textureCount);
		for (i = 0; i < s->textureCount; ++i) {
			s->textureBindings[i].location = glGetUniformLocation(s->program, desc->textures[i].name);
			s->textureBindings[i].unit = desc->textures[i].reg;
		}
	}
}
