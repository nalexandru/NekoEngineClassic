#include <Windows.h>

#include <Engine/IO.h>
#include <Engine/Engine.h>
#include <Engine/Config.h>
#include "GLDrv.h"

#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042

typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALEXTPROC)(int interval);
typedef BOOL (APIENTRY *PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC,
			const int *, const FLOAT *, UINT, int *, UINT *);

static HDC _dc;
static HGLRC _ctx;
static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
static PIXELFORMATDESCRIPTOR _pfd =
{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	PFD_MAIN_PLANE, 0, 0, 0, 0
};

static int _attr[20] =
{
	WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
	WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
	WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
	0
};

/*static NSOpenGLContext *_ctx;
static NSOpenGLPixelFormat *_pf;
static NSOpenGLPixelFormatAttribute _pfAttribs[] =
{
	NSOpenGLPFADoubleBuffer,
	NSOpenGLPFAAccelerated,
	NSOpenGLPFANoRecovery,
	NSOpenGLPFADepthSize, 24,
	0, 0, 0, 0, 0,
	0, 0,
	0
};*/

bool
GL_InitContext(void)
{
	int fmt = 0;
	UINT count = 0;
	int *aptr = &_attr[8];
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;

	if (CVAR_BOOL("Render_Multisampling")) {
		*aptr++ = WGL_SAMPLE_BUFFERS_ARB;
		*aptr++ = 1;
		*aptr++ = WGL_SAMPLES_ARB;
		*aptr++ = E_GetCVarI32("Render_Samples", 4)->i32;
	}

	_dc = GetDC((HWND)E_screen);

	fmt = ChoosePixelFormat(_dc, &_pfd);
	SetPixelFormat(_dc, fmt, &_pfd);
	
	_ctx = wglCreateContext(_dc);
	if (!_ctx)
		return false;

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (wglChoosePixelFormatARB) {
		wglChoosePixelFormatARB(_dc, _attr, NULL, 1, &fmt, &count);
		SetPixelFormat(_dc, fmt, &_pfd);
		
		wglDeleteContext(_ctx);

		_ctx = wglCreateContext(_dc);
		if (!_ctx)
			return false;
	}

	if (!wglMakeCurrent(_dc, _ctx)) {
		wglDeleteContext(_ctx);
		return false;
	}

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	
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
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(interval);
}

void
GL_SwapBuffers(void)
{
	SwapBuffers(_dc);
}

void
GL_ScreenResized(void)
{
}

void
GL_TermContext(void)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(_ctx);
}
