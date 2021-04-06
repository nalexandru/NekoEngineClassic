#define Handle __EngineHandle

#include <Engine/IO.h>
#include <Engine/Engine.h>
#include <Engine/Config.h>
#include "GLDrv.h"

#undef Handle

#import <Cocoa/Cocoa.h>
#import <IOKit/IOKitLib.h>

static NSOpenGLContext *_ctx;
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
};

bool
GL_InitContext(void)
{
	NSOpenGLPixelFormatAttribute *aptr = &_pfAttribs[5];
	
#if defined(MAC_OS_X_VERSION_10_7) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
	*aptr++ = NSOpenGLPFAOpenGLProfile;
	*aptr++ = NSOpenGLProfileVersionLegacy;
#endif
	
	if (CVAR_BOOL("Render_Multisampling")) {
		*aptr++ = NSOpenGLPFAMultisample;
		*aptr++ = NSOpenGLPFASampleBuffers;
		*aptr++ = 1;
		*aptr++ = NSOpenGLPFASamples;
		*aptr++ = E_GetCVarI32("Render_Samples", 4)->i32;
	}
	
	_pf = [[NSOpenGLPixelFormat alloc] initWithAttributes: _pfAttribs];
	if (!_pf)
		return false;
	
	_ctx = [[NSOpenGLContext alloc] initWithFormat: _pf shareContext: nil];
	if (!_ctx)
		return false;
	
	[_ctx setView: [(NSWindow *)E_screen contentView]];
	[_ctx makeCurrentContext];
	
	NSString *path = [[NSBundle mainBundle] resourcePath];
	
	E_Mount([path UTF8String], "/Shaders/OpenGL");
	
	return true;
}

void
GL_VRAMSize(uint64_t *vramSize)
{
	CFTypeRef typeCode;
	long size = 0;
	io_registry_entry_t port;
	
	port = CGDisplayIOServicePort(kCGDirectMainDisplay);
	
	typeCode = IORegistryEntrySearchCFProperty(port, kIOServicePlane, CFSTR(kIOFBMemorySizeKey),
											   kCFAllocatorDefault,kIORegistryIterateRecursively | kIORegistryIterateParents);
	
	if (typeCode && CFGetTypeID(typeCode) == CFNumberGetTypeID()) {
		CFNumberGetValue(typeCode, kCFNumberSInt32Type, &size);
		CFRelease(typeCode);
	}
	
	*vramSize = size;
}

void
GL_SwapInterval(int interval)
{
	[_ctx setValues: (long *)&interval forParameter: NSOpenGLCPSwapInterval];
}

void
GL_SwapBuffers(void)
{
	[_ctx flushBuffer];
}

void
GL_ScreenResized(void)
{
	[_ctx update];
}

void
GL_TermContext(void)
{
	[NSOpenGLContext clearCurrentContext];
	[_ctx release];
}
