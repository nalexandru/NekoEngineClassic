@echo off

cd Shaders

FOR /F %%f IN ('DIR /b *.vsh') DO (
	vsa -0 %%f
	move %%~nf.vso ..\..\..\Bin\Data\Shaders\D3D8
)

FOR /F %%f IN ('DIR /b *.psh') DO (
	psa -0 %%f
	move %%~nf.pso ..\..\..\Bin\Data\Shaders\D3D8
)