#ifndef _TEST_APPLICATION_H_
#define _TEST_APPLICATION_H_

#ifndef _NESTR
#define _NESTR_INTERNAL(x) #x
#define _NESTR(x) _NESTR_INTERNAL(x)
#endif

#define A_VER_MAJOR		0
#define A_VER_MINOR		7
#define A_VER_BUILD		22
#define A_VER_REVISION	0

#define A_PGM_NAME		"NekoEngine Test Application"
#define A_CODENAME		"Evelyn"
#define A_CPY_STR		"2020-2021 Alexandru Naiman. All Rights Reserved."
#define A_COMPANY		"Alexandru Naiman"

#if A_VER_REVISION == 0
#	define A_VER_STR		_NESTR(A_VER_MAJOR) "." _NESTR(A_VER_MINOR) "." _NESTR(A_VER_BUILD)
#else
#	define A_VER_STR		_NESTR(A_VER_MAJOR) "." _NESTR(A_VER_MINOR) "." _NESTR(A_VER_BUILD) "." _NESTR(A_VER_REVISION)
#endif

#endif /* _TEST_APPLICATION_H_ */
