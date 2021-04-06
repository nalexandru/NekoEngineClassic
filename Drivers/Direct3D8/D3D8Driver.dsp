# Microsoft Developer Studio Project File - Name="D3D8Driver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=D3D8Driver - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "D3D8Driver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "D3D8Driver.mak" CFG="D3D8Driver - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "D3D8Driver - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "D3D8Driver - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "D3D8Driver - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Bin\Release"
# PROP Intermediate_Dir "..\..\Build\Release\D3D8Driver"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D3D8DRIVER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\Include" /I "..\..\Deps" /I "..\..\Deps\Lua" /I "..\..\Compat/c89" /D "NDEBUG" /D "SNPRINTF_COMPAT" /D "STBI_NO_SIMD" /D "DISABLE_SIMD" /D "STRTOULL_COMPAT" /D inline=__inline /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D3D8DRIVER_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 NekoEngine.lib d3d8.lib /nologo /dll /machine:I386 /libpath:"..\..\Bin\Release"
# Begin Special Build Tool
ProjDir=.
SOURCE="$(InputPath)"
PostBuild_Desc=Compiling shaders...
PostBuild_Cmds=$(ProjDir)\Shaders\compile.cmd
# End Special Build Tool

!ELSEIF  "$(CFG)" == "D3D8Driver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Bin\Debug"
# PROP Intermediate_Dir "..\..\Build\Debug\D3D8Driver"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D3D8DRIVER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\Include" /I "..\..\Deps" /I "..\..\Deps\Lua" /I "..\..\Compat/c89" /D "_DEBUG" /D "SNPRINTF_COMPAT" /D "STBI_NO_SIMD" /D "DISABLE_SIMD" /D "STRTOULL_COMPAT" /D inline=__inline /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D3D8DRIVER_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 NekoEngine.lib d3d8.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\..\Bin\Debug"
# Begin Special Build Tool
ProjDir=.
SOURCE="$(InputPath)"
PostBuild_Desc=Compiling shaders...
PostBuild_Cmds=$(ProjDir)\Shaders\compile.cmd
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "D3D8Driver - Win32 Release"
# Name "D3D8Driver - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\D3D8Buffer.c
# End Source File
# Begin Source File

SOURCE=.\D3D8Device.c
# End Source File
# Begin Source File

SOURCE=.\D3D8Drv.c
# End Source File
# Begin Source File

SOURCE=.\D3D8Shader.c
# End Source File
# Begin Source File

SOURCE=.\D3D8Texture.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\D3D8Drv.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Shaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Shaders\compile.cmd
# End Source File
# Begin Source File

SOURCE=.\Shaders\Default_FS.psh
# End Source File
# Begin Source File

SOURCE=.\Shaders\Default_VS.vsh
# End Source File
# Begin Source File

SOURCE=.\Shaders\Text_FS.psh
# End Source File
# Begin Source File

SOURCE=.\Shaders\UI_VS.vsh
# End Source File
# Begin Source File

SOURCE=.\Shaders\Unlit_FS.psh
# End Source File
# Begin Source File

SOURCE=.\Shaders\Unlit_VS.vsh
# End Source File
# End Group
# End Target
# End Project