#ifndef _E_VERSION_H_
#define _E_VERSION_H_

#ifndef _NESTR
#define _NESTR_INTERNAL(x) #x
#define _NESTR(x) _NESTR_INTERNAL(x)
#endif

#define E_VER_MAJOR		0
#define E_VER_MINOR		7
#define E_VER_BUILD		72
#define E_VER_REVISION	0

#define E_PGM_NAME		"NekoEngine Classic"
#define E_CODENAME		"Evelyn"
#define E_CPY_STR		"2015-2021 Alexandru Naiman. All Rights Reserved."

#if E_VER_REVISION == 0
#   define E_VER_STR     _NESTR(E_VER_MAJOR) "." _NESTR(E_VER_MINOR) "." _NESTR(E_VER_BUILD)
#else
#   define E_VER_STR     _NESTR(E_VER_MAJOR) "." _NESTR(E_VER_MINOR) "." _NESTR(E_VER_BUILD) "." _NESTR(E_VER_REVISION)
#endif

#endif /* _E_VERSION_H_ */
