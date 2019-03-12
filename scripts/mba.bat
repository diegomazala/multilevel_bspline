echo off
set mba="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/mba_mesh_app.exe" 

set path=E:/GDrive/Shared/Data/FigsHigh/mba_2_10/

%mba% %path%fig02.obj 2 10
%mba% %path%fig03.obj 2 10
%mba% %path%fig04.obj 2 10
%mba% %path%fig05.obj 2 10