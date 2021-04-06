bin_PROGRAMS = NekoEngineTestApplication
NekoEngineTestApplication_SOURCES = Application/Application.c

SUBDIRS = Deps Platform Engine Drivers

NekoEngineTestApplication_LDADD = \
	Engine/libEngine.a \
	Drivers/libDrivers.la \
	Deps/libDeps.la \
	Platform/libPlatform.a

NekoEngineTestApplication_CPPFLAGS = -I$(srcdir)/Include -I$(srcdir)/Deps -I$(srcdir)/Deps/Lua -I$(srcdir)/Deps/PhysFS
