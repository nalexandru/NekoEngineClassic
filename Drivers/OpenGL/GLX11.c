#include <assert.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <Engine/IO.h>
#include <Engine/Engine.h>
#include <Engine/Config.h>
#include "GLDrv.h"

#include <GL/glx.h>

extern Display *X11_display;
extern XVisualInfo X11_visualInfo;

static GLXContext _ctx;

static PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = NULL;

static int _attr[] =
{
	GLX_X_RENDERABLE, True,
	GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
	GLX_RENDER_TYPE, GLX_RGBA_BIT,
	GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
	GLX_RED_SIZE, 8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE, 8,
	GLX_ALPHA_SIZE, 0,
	GLX_DEPTH_SIZE, 24,
	GLX_STENCIL_SIZE, 0,
	0, 0,
	0, 0,
	0
};

bool
GL_InitContext(void)
{
	int fbCount = 0;
	int *aptr = &_attr[8];
	GLXFBConfig *fbc = NULL;
	PFNGLXCHOOSEFBCONFIGPROC glXChooseFBConfig = NULL;
	PFNGLXGETVISUALFROMFBCONFIGPROC glXGetVisualFromFBConfig = NULL;
	PFNGLXGETFBCONFIGATTRIBPROC glXGetFBConfigAttrib = NULL;
	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = NULL;
	
	/*if (CVAR_BOOL("Render_Multisampling")) {
		*aptr++ = WGL_SAMPLE_BUFFERS_ARB;
		*aptr++ = 1;
		*aptr++ = WGL_SAMPLES_ARB;
		*aptr++ = E_GetCVarI32("Render_Samples", 4)->i32;
	}*/

	glXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC)glXGetProcAddress((const GLubyte*)"glXChooseFBConfig");
	assert(glXChooseFBConfig);
	
	glXGetVisualFromFBConfig = (PFNGLXGETVISUALFROMFBCONFIGPROC)glXGetProcAddress((const GLubyte*)"glXGetVisualFromFBConfig");
	assert(glXGetVisualFromFBConfig);
	
	glXGetFBConfigAttrib = (PFNGLXGETFBCONFIGATTRIBPROC)glXGetProcAddress((const GLubyte*)"glXGetFBConfigAttrib");
	assert(glXGetFBConfigAttrib);
	
	glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((const GLubyte *)"glXCreateContextAttribsARB");
	
	if (glXCreateContextAttribsARB) {
		/*fbc = glXChooseFBConfig(X11_display, DefaultScreen(X11_display), _attr, &fbCount);
		if (!fbc)
			return false;
		
		_fbConfig = fbc[0];
		XFree(fbc);*/
		_ctx = glXCreateContext(X11_display, &X11_visualInfo, NULL, true);
	} else {
		_ctx = glXCreateContext(X11_display, &X11_visualInfo, NULL, true);
	}
	
	if (!_ctx)
		return false;
	
	XSync(X11_display, False);

	if (!glXMakeCurrent(X11_display, (GLXDrawable)E_screen, _ctx)) {
		glXDestroyContext(X11_display, _ctx);
		return false;
	}

	glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const GLubyte *)"glXSwapIntervalEXT");
	
	gladLoadGL();

//	E_Mount([path UTF8String], "/Shaders/OpenGL");
	
	return true;
}

void
GL_VRAMSize(uint64_t *vramSize)
{
/*	CFTypeRef typeCode;
	long size = 0;
	io_registry_entry_t port;
	
	port = CGDisplayIOServicePort(kCGDirectMainDisplay);
	
	typeCode = IORegistryEntrySearchCFProperty(port, kIOServicePlane, CFSTR(kIOFBMemorySizeKey),
											   kCFAllocatorDefault,kIORegistryIterateRecursively | kIORegistryIterateParents);
	
	if (typeCode && CFGetTypeID(typeCode) == CFNumberGetTypeID()) {
		CFNumberGetValue(typeCode, kCFNumberSInt32Type, &size);
		CFRelease(typeCode);
	}
	
	*vramSize = size;*/

	*vramSize = 0;
}

void
GL_SwapInterval(int interval)
{
	if (glXSwapIntervalEXT)
		glXSwapIntervalEXT(X11_display, (GLXDrawable)E_screen, interval);
}

void
GL_SwapBuffers(void)
{
	glXSwapBuffers(X11_display, (GLXDrawable)E_screen);
}

void
GL_ScreenResized(void)
{
}

void
GL_TermContext(void)
{
	glXMakeCurrent(X11_display, (GLXDrawable)E_screen, NULL);
	glXDestroyContext(X11_display, _ctx);
}
