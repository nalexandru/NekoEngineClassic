# Microsoft Developer Studio Project File - Name="NekoEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Xbox Application" 0x0b01

CFG=NekoEngine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NekoEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NekoEngine.mak" CFG="NekoEngine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NekoEngine - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NekoEngine - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "NekoEngine - Xbox Debug" (based on "Xbox Application")
!MESSAGE "NekoEngine - Xbox Release" (based on "Xbox Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Bin\Release"
# PROP Intermediate_Dir "Build\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "Include Deps Deps\Lua Deps\PhysFS" /I "Include" /I "Deps" /I "Deps\Lua" /I "Deps\PhysFS" /I "Compat/c89" /D "NDEBUG" /D "_ENGINE_INTERNAL_" /D "SNPRINTF_COMPAT" /D "STBI_NO_SIMD" /D "DISABLE_SIMD" /D "STRTOULL_COMPAT" /D inline=__inline /D "WIN32" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 user32.lib gdi32.lib advapi32.lib ole32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Bin\Debug"
# PROP Intermediate_Dir "Build\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "Include" /I "Deps" /I "Deps\Lua" /I "Deps\PhysFS" /I "Compat/c89" /D "_DEBUG" /D "_ENGINE_INTERNAL_" /D "SNPRINTF_COMPAT" /D "STBI_NO_SIMD" /D "DISABLE_SIMD" /D "STRTOULL_COMPAT" /D inline=__inline /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib gdi32.lib advapi32.lib ole32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "NekoEngine___Xbox_Debug"
# PROP BASE Intermediate_Dir "NekoEngine___Xbox_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Bin\XboxDebug"
# PROP Intermediate_Dir "Build\XboxDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "Include" /I "Deps" /I "Deps\Lua" /I "Deps\PhysFS" /I "Compat/c89" /D "WIN32" /D "_XBOX" /D "_DEBUG" /D "RENDER_DRIVER_BUILTIN" /D "_ENGINE_INTERNAL_" /D "SNPRINTF_COMPAT" /D "STBI_NO_SIMD" /D "DISABLE_SIMD" /D "STRTOULL_COMPAT" /D inline=__inline /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "Include" /I "Deps" /I "Deps\Lua" /I "Deps\PhysFS" /I "Compat/c89" /D "WIN32" /D "_XBOX" /D "_DEBUG" /D "RENDER_DRIVER_BUILTIN" /D "_ENGINE_INTERNAL_" /D "SNPRINTF_COMPAT" /D "STBI_NO_SIMD" /D "DISABLE_SIMD" /D "STRTOULL_COMPAT" /D inline=__inline /FR /YX /FD /GZ /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /debug /machine:IX86 /pdbtype:sept
# ADD LINK32 d3d8d.lib d3dx8d.lib xboxkrnl.lib xapilibd.lib /nologo /debug /machine:IX86 /pdbtype:sept /SUBSYSTEM:XBOX
# SUBTRACT LINK32 /pdb:none
XBE=imagebld.exe
# ADD BASE XBE /nologo /stack:0x10000 /debug
# ADD XBE /nologo /testname:"NekoEngine" /stack:0x10000 /debug /testratings:0xFFFFFFFF
XBCP=xbecopy.exe
# ADD BASE XBCP /NOLOGO
# ADD XBCP /NOLOGO

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "NekoEngine___Xbox_Release"
# PROP BASE Intermediate_Dir "NekoEngine___Xbox_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Bin\XboxRelease"
# PROP Intermediate_Dir "Build\XboxRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I "Include Deps Deps\Lua Deps\PhysFS" /I "Include" /I "Deps" /I "Deps\Lua" /I "Deps\PhysFS" /I "Compat/c89" /D "WIN32" /D "_XBOX" /D "NDEBUG" /D "RENDER_DRIVER_BUILTIN" /D "_ENGINE_INTERNAL_" /D "SNPRINTF_COMPAT" /D "STBI_NO_SIMD" /D "DISABLE_SIMD" /D "STRTOULL_COMPAT" /D inline=__inline /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "Include Deps Deps\Lua Deps\PhysFS" /I "Include" /I "Deps" /I "Deps\Lua" /I "Deps\PhysFS" /I "Compat/c89" /D "WIN32" /D "_XBOX" /D "NDEBUG" /D "RENDER_DRIVER_BUILTIN" /D "_ENGINE_INTERNAL_" /D "SNPRINTF_COMPAT" /D "STBI_NO_SIMD" /D "DISABLE_SIMD" /D "STRTOULL_COMPAT" /D inline=__inline /FR /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /machine:IX86
# ADD LINK32 d3d8.lib d3dx8.lib xboxkrnl.lib xapilib.lib libcmt.lib /nologo /machine:IX86 /SUBSYSTEM:XBOX
# SUBTRACT LINK32 /pdb:none
XBE=imagebld.exe
# ADD BASE XBE /nologo /stack:0x10000
# ADD XBE /nologo /testname:"NekoEngine" /stack:0x10000 /testratings:0xFFFFFFFF
XBCP=xbecopy.exe
# ADD BASE XBCP /NOLOGO
# ADD XBCP /NOLOGO

!ENDIF 

# Begin Target

# Name "NekoEngine - Win32 Release"
# Name "NekoEngine - Win32 Debug"
# Name "NekoEngine - Xbox Debug"
# Name "NekoEngine - Xbox Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Application"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Application\Application.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Application\TestApplication.h
# End Source File
# End Group
# Begin Group "Asset"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Asset\Font.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Asset\glTF.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Asset\Image.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Asset\Metadata.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Asset\NMesh.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Asset\Ogg.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Asset\TGA.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Asset\Wave.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Audio\Audio.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Audio\Clip.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Audio\Source.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Engine\Component.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\Config.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\ECS.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\ECSInit.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\ECSystem.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\Engine.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\Entity.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\Event.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\IO.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\Job.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Engine\Resource.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Input\Input.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Render"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Render\Frame.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Render\Material.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Render\Model.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Render\Render.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Render\Shader.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Render\Systems.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Render\Texture.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Scene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Scene\Camera.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Scene\Scene.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Scene\ScriptHost.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Scene\Transform.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Script\Script.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "System"

# PROP Default_Filter ""
# Begin Group "Compat"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\System\Compat\getopt.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\System\Compat\reallocarray.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\System\Compat\snprintf.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\Engine\System\AtomicLock.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\System\Log.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\System\Memory.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\System\System.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "UI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\UI\Text.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\UI\UI.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\UI\UIInternal.h
# End Source File
# End Group
# Begin Group "Platform"

# PROP Default_Filter ""
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Platform\Win32\Main.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Platform\Win32\Win32.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Platform\Win32\Win32Input.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Platform\Win32\Win32Platform.h
# End Source File
# Begin Source File

SOURCE=.\Platform\Win32\Window.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Xbox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Platform\Xbox\Xbox.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Platform\Xbox\XboxInput.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Platform\Xbox\XboxMain.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Platform\Xbox\XboxWindow.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Shared"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Platform\Win32\Thread.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Platform\Win32\Win32Compat.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Platform\Win32\Win32Compat.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "I_Audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Audio\Audio.h
# End Source File
# Begin Source File

SOURCE=.\Include\Audio\Clip.h
# End Source File
# Begin Source File

SOURCE=.\Include\Audio\Source.h
# End Source File
# End Group
# Begin Group "I_Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Engine\Application.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Asset.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Component.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Config.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\ECSystem.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Engine.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Entity.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Event.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Events.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\IO.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Job.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Types.h
# End Source File
# Begin Source File

SOURCE=.\Include\Engine\Version.h
# End Source File
# End Group
# Begin Group "I_Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Input\Codes.h
# End Source File
# Begin Source File

SOURCE=.\Include\Input\Input.h
# End Source File
# End Group
# Begin Group "I_Math"

# PROP Default_Filter ""
# Begin Group "sse"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Math\sse\mat4_sse.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\sse\vec4_sse.h
# End Source File
# End Group
# Begin Group "altivec"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Math\altivec\mat4_altivec.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\altivec\vec4_altivec.h
# End Source File
# End Group
# Begin Group "neon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Math\neon\mat4_neon.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\neon\vec4_neon.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Include\Math\aabb2.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\aabb3.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\debug.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\defs.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\func.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\mat3.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\mat4.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\Math.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\plane.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\quat.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\ray2.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\ray3.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\sanity.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\vec2.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\vec3.h
# End Source File
# Begin Source File

SOURCE=.\Include\Math\vec4.h
# End Source File
# End Group
# Begin Group "I_Render"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Render\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\Include\Render\Device.h
# End Source File
# Begin Source File

SOURCE=.\Include\Render\Driver.h
# End Source File
# Begin Source File

SOURCE=.\Include\Render\Material.h
# End Source File
# Begin Source File

SOURCE=.\Include\Render\Model.h
# End Source File
# Begin Source File

SOURCE=.\Include\Render\Render.h
# End Source File
# Begin Source File

SOURCE=.\Include\Render\Shader.h
# End Source File
# Begin Source File

SOURCE=.\Include\Render\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Include\Render\Types.h
# End Source File
# End Group
# Begin Group "I_Runtime"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Runtime\Array.h
# End Source File
# Begin Source File

SOURCE=.\Include\Runtime\Json.h
# End Source File
# Begin Source File

SOURCE=.\Include\Runtime\RtDefs.h
# End Source File
# Begin Source File

SOURCE=.\Include\Runtime\Runtime.h
# End Source File
# End Group
# Begin Group "I_Scene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Scene\Camera.h
# End Source File
# Begin Source File

SOURCE=.\Include\Scene\Components.h
# End Source File
# Begin Source File

SOURCE=.\Include\Scene\Scene.h
# End Source File
# Begin Source File

SOURCE=.\Include\Scene\ScriptHost.h
# End Source File
# Begin Source File

SOURCE=.\Include\Scene\Sky.h
# End Source File
# Begin Source File

SOURCE=.\Include\Scene\Systems.h
# End Source File
# Begin Source File

SOURCE=.\Include\Scene\Transform.h
# End Source File
# End Group
# Begin Group "I_Script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\Script\Script.h
# End Source File
# End Group
# Begin Group "I_System"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\System\AtomicLock.h
# End Source File
# Begin Source File

SOURCE=.\Include\System\Endian.h
# End Source File
# Begin Source File

SOURCE=.\Include\System\Log.h
# End Source File
# Begin Source File

SOURCE=.\Include\System\Memory.h
# End Source File
# Begin Source File

SOURCE=.\Include\System\System.h
# End Source File
# Begin Source File

SOURCE=.\Include\System\Thread.h
# End Source File
# Begin Source File

SOURCE=.\Include\System\Window.h
# End Source File
# End Group
# Begin Group "I_UI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include\UI\Font.h
# End Source File
# Begin Source File

SOURCE=.\Include\UI\Style.h
# End Source File
# Begin Source File

SOURCE=.\Include\UI\Text.h
# End Source File
# Begin Source File

SOURCE=.\Include\UI\UI.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Application\Application.rc

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Engine\Exports.def
# End Source File
# End Group
# Begin Group "Deps"

# PROP Default_Filter ""
# Begin Group "Lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Deps\Lua\lapi.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lapi.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lauxlib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lauxlib.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lbaselib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lcode.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lcode.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lcorolib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lctype.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lctype.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ldblib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ldebug.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ldebug.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ldo.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ldo.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ldump.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lfunc.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lfunc.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lgc.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lgc.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\linit.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\liolib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ljumptab.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\llex.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\llex.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\llimits.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lmathlib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lmem.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lmem.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\loadlib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lobject.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lobject.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lopcodes.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lopcodes.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lopnames.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\loslib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lparser.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lparser.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lprefix.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lstate.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lstate.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lstring.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lstring.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lstrlib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ltable.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ltable.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ltablib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ltm.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\ltm.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lua.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\luaconf.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lualib.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lundump.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lundump.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lutf8lib.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lvm.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lvm.h
# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lzio.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\Lua\lzio.h
# End Source File
# End Group
# Begin Group "PhysFS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs.h
# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_7z.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_dir.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_grp.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_hog.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_iso9660.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_mvl.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_qpak.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_slb.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_unpacked.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_vdf.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_wad.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_archiver_zip.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_byteorder.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_casefolding.h
# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_internal.h
# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_lzmasdk.h
# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_miniz.h
# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_platform_windows.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_platform_xbox.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_platforms.h
# End Source File
# Begin Source File

SOURCE=.\Deps\PhysFS\physfs_unicode.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "Drivers"

# PROP Default_Filter ""
# Begin Group "Direct3D8"

# PROP Default_Filter ""
# Begin Group "Shaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Drivers\Direct3D8\Shaders\Default_FS.psh
# End Source File
# Begin Source File

SOURCE=.\Drivers\Direct3D8\Shaders\Default_VS.vsh
# End Source File
# End Group
# Begin Source File

SOURCE=.\Drivers\Direct3D8\D3D8Buffer.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Drivers\Direct3D8\D3D8Device.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Drivers\Direct3D8\D3D8Drv.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Drivers\Direct3D8\D3D8Drv.h
# End Source File
# Begin Source File

SOURCE=.\Drivers\Direct3D8\D3D8Shader.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Drivers\Direct3D8\D3D8Texture.c

!IF  "$(CFG)" == "NekoEngine - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Debug"

!ELSEIF  "$(CFG)" == "NekoEngine - Xbox Release"

!ENDIF 

# End Source File
# End Group
# End Group
# End Target
# End Project
