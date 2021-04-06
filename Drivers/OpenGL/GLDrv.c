#include "GLDrv.h"

#include <System/Log.h>
#include <System/System.h>
#include <Render/Driver.h>
#include <Render/Device.h>

#define GL_DRV	"OpenGLDriver"

bool GL_useShaderObjectsARB = false;

static bool _Init(void);
static void _Term(void);
static bool _EnumerateDevices(uint32_t *count, struct RenderDeviceInfo *devices);

static struct RenderDriver _drv =
{
	NE_RENDER_DRIVER_ID,
	NE_RENDER_DRIVER_API,
	"OpenGL",

	_Init,
	_Term,

	_EnumerateDevices,
	GL_CreateDevice,
	GL_DestroyDevice
};

#ifdef RENDER_DRIVER_BUILTIN
const struct RenderDriver *Re_LoadBuiltinDriver(void) { return &_drv; }
#else
#ifdef _WIN32
#	define EXPORT __declspec(dllexport)
#else
#	define EXPORT
#endif

EXPORT const struct RenderDriver *Re_LoadDriver(void) { return &_drv; }
#endif

static bool
_Init(void)
{
	int major, minor;
	const GLubyte *glVersion;

	if (!GL_InitContext())
		return false;

	glGetError();
	
	glVersion = glGetString(GL_VERSION);
	sscanf((const char *)glVersion, "%d.%d", &major, &minor);

	if (major == 1 && !GL_CheckExtension("GL_ARB_shader_objects")) {
		Sys_LogEntry(GL_DRV, LOG_CRITICAL, "GL_ARB_shader_objects is required for OpenGL 1.x");
		return false;
	}
	
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	GL_SwapBuffers();
	
	glGetError();

#ifndef __APPLE__
	GL_useShaderObjectsARB = major == 1;
#endif

	return true;
}

static void
_Term(void)
{
	GL_TermContext();
}

static bool
_EnumerateDevices(uint32_t *count, struct RenderDeviceInfo *info)
{
	const GLubyte *str = NULL;
	GLint val;
	
	if (!*count || !info) {
		*count = 1;
		return true;
	}
	
	str = glGetString(GL_RENDERER);
	snprintf(info[0].deviceName, sizeof(info[0].deviceName), "%s", str);

	info[0].features.uint32IndexSupport = true;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
	info[0].limits.maxTextureHeight = info[0].limits.maxTextureWidth = (uint32_t)val;
	
	info[0].limits.maxAnisotropy = 16;
	
	GL_VRAMSize(&info[0].localMemorySize);
	
	return true;
}

bool
GL_CheckExtension(const char *name)
{
	static const char *glExtensions = NULL;

	if (!glExtensions)
		glExtensions = (const char *)glGetString(GL_EXTENSIONS);

	return strstr(glExtensions, name) != NULL;
}
