//
//  EngineAppDelegate.m
//  NekoEngine
//
//  Created by Alexandru Naiman on 9/28/20.
//  Copyright 2020 __MyCompanyName__. All rights reserved.
//

#import "EngineAppDelegate.h"

void E_Term(void);	// don't include Engine.h

@implementation EngineAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)n
{
	//
}

- (void)applicationWillTerminate:(NSNotification *)n
{
	E_Term();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app
{
	return YES;
}

- (void)applicationDidChangeOcclusionState:(NSNotification *)n
{
}

@end
