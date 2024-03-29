#ifndef _E_APPLICATION_H_
#define _E_APPLICATION_H_

#include <Engine/Types.h>

struct ApplicationInfo
{
	const char name[64];
	const char copyright[64];
	struct Version version;
};

ENGINE_API extern struct ApplicationInfo App_applicationInfo;

bool App_InitApplication(int argc, char *argv[]);
void App_TermApplication(void);

#endif /* _E_APPLICATION_H_ */
