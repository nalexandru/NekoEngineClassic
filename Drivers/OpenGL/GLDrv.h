#ifndef _OPENGL_DRIVER_H_
#define _OPENGL_DRIVER_H_

#ifdef __APPLE__
#	include <OpenGL/gl.h>
#else
#	include <glad/glad.h>
#endif

#include <Render/Render.h>

#define GLDRV_MOD	"OpenGLDriver"

struct Buffer
{
	GLuint id;
	GLenum indexType;
	GLenum target, usage;
	uint32_t size;
};

struct Texture
{
	GLuint id;
	GLenum target;
	uint32_t width, height;
	GLenum type, format, internalFormat;
	bool compressed;
};

struct TextureBinding
{
	uint8_t location;
	uint8_t unit;
};

struct Shader
{
	union {
		GLuint program;
		GLhandleARB programARB;
	};
	const struct InputLayout *inputLayout;
	
	uint32_t vertexUniformCount;
	uint8_t *vertexUniformLocations;
	
	uint32_t fragmentUniformCount;
	uint8_t *fragmentUniformLocations;
	
	uint32_t textureCount;
	struct TextureBinding *textureBindings;
};

struct GLInputAttribute
{
	GLuint index;
	GLint size;
	GLenum type;
	const void *ptr;
	char name[32];
};

struct InputLayout
{
	uint64_t hash;
	GLsizei stride;
	uint32_t attributeCount;
	struct GLInputAttribute *attributes;
};

extern bool GL_useShaderObjectsARB;

// Platform specific
bool GL_InitContext(void);
void GL_VRAMSize(uint64_t *vramSize);
void GL_SwapInterval(int interval);
void GL_SwapBuffers(void);
void GL_ScreenResized(void);
void GL_TermContext(void);

// Device
bool GL_CreateDevice(struct RenderDeviceInfo *info, struct RenderDeviceProcs *procs);
void GL_DestroyDevice(void);

// Texture
struct Texture *GL_CreateTexture(const struct TextureCreateInfo *tci);
void GL_UpdateTexture(struct Texture *tex, const void *data);
void GL_DestroyTexture(struct Texture *tex);

// Buffer
struct Buffer *GL_CreateBuffer(const struct BufferCreateInfo *bci);
void GL_UpdateBuffer(struct Buffer *buff, const void *data, uint32_t offset, uint32_t dataSize);
void *GL_LockBuffer(struct Buffer *buff);
void GL_UnlockBuffer(struct Buffer *buff);
void GL_DestroyBuffer(struct Buffer *buff);

// Shader
struct InputLayout *GL_CreateInputLayout(const struct InputLayoutDesc *desc);
void GL_DestroyInputLayout(struct InputLayout *layout);

struct ShaderModule *GL_CreateVertexModule_GL2(const char *name, const struct InputLayout *layout);
struct ShaderModule *GL_CreateFragmentModule_GL2(const char *name);
void GL_DestroyShaderModule_GL2(struct ShaderModule *sm);

struct Shader *GL_CreateShader_GL2(void);
void GL_AddShaderModule_GL2(struct Shader *s, const struct ShaderModule *module);
bool GL_LinkShader_GL2(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc);
void GL_DestroyShader_GL2(struct Shader *s);

struct ShaderModule *GL_CreateVertexModule_ARB(const char *name, const struct InputLayout *layout);
struct ShaderModule *GL_CreateFragmentModule_ARB(const char *name);
void GL_DestroyShaderModule_ARB(struct ShaderModule *sm);

struct Shader *GL_CreateShader_ARB(void);
void GL_AddShaderModule_ARB(struct Shader *s, const struct ShaderModule *module);
bool GL_LinkShader_ARB(struct Shader *s, const struct InputLayout *inputLayout, const struct BindingLayoutDesc *bindingLayoutDesc);
void GL_DestroyShader_ARB(struct Shader *s);

// Helper functions
bool GL_CheckExtension(const char *name);
	
#endif /* _OPENGL_DRIVER_H_ */
