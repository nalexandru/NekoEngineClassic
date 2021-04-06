AC_INIT([NekoEngine], [0.7], [alexandru.naiman@protonmail.ch])
AM_INIT_AUTOMAKE([subdir-objects foreign -Wall -Werror dist-bzip2])

AC_PROG_CC
AC_PROG_RANLIB

AM_PROG_AR

LT_INIT

AC_CHECK_LIB([dl], [dlopen])
AC_CHECK_LIB([GL], [glClear])
AC_CHECK_LIB([GLX], [glXCreateContext])
AC_CHECK_LIB([X11], [XOpenDisplay])
AC_CHECK_LIB([m], [pow])
AC_CHECK_LIB([pthread], [pthread_create])

AC_CONFIG_FILES([Makefile Platform/Makefile Engine/Makefile Deps/Makefile Deps/Lua/Makefile Deps/PhysFS/Makefile Drivers/Makefile Drivers/OpenGL/Makefile])
AC_OUTPUT
