@echo off

cd Shaders

FOR /F %%f IN ('DIR /b *.vsh') DO (
	xsasm %%f ..\..\..\Bin\Data\Shaders\D3D8\%%~nf.xvu
)

FOR /F %%f IN ('DIR /b *.psh') DO (
	xsasm %%f ..\..\..\Bin\Data\Shaders\D3D8\%%~nf.xpu
)