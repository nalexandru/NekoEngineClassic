#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#define Handle __EngineHandle

#include <System/Window.h>
#include <System/Memory.h>
#include <Engine/Engine.h>
#include <Render/Render.h>

#include "MacXPlatform.h"

#undef Handle

#import <Cocoa/Cocoa.h>
#import "EngineView.h"

@interface EngineWindowDelegate : NSObject
@end

@implementation EngineWindowDelegate

- (void)windowDidResize:(NSNotification*)notification
{
	Re_ScreenResized();
}

@end

bool
Sys_CreateWindow(void)
{
#if defined(MAC_OS_X_VERSION_10_12) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_12
	NSUInteger mask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
		NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;
#else
	unsigned long mask = NSTitledWindowMask | NSClosableWindowMask |
		NSMiniaturizableWindowMask | NSResizableWindowMask;
#endif

	NSWindow *w = [[NSWindow alloc]
		initWithContentRect:NSMakeRect(0, 0, *E_screenWidth, *E_screenHeight)
		styleMask: mask
		backing: NSBackingStoreBuffered
		defer: YES];
		
	[w cascadeTopLeftFromPoint: NSMakePoint(20, 20)];
	[w setTitle: @"NekoEngine"];

	NSView *v = (NSView *)[[EngineView alloc] initWithFrame: [(NSView *)[w contentView] frame]];

	[w setContentView: v];
	[w setInitialFirstResponder: v];
	[w makeFirstResponder: v];
	[w setAcceptsMouseMovedEvents: true];
	
	[w makeKeyAndOrderFront: NSApp];
	[w center];
	
	[w setDelegate: [[EngineWindowDelegate alloc] init]];
	
	[NSApp activateIgnoringOtherApps: YES];

	E_screen = (void *)w;

	return true;
}

void
Sys_SetWindowTitle(const char *name)
{
	[(NSWindow *)E_screen setTitle: [NSString stringWithFormat:@"%s", name]];
}

void
Sys_DestroyWindow(void)
{
}
