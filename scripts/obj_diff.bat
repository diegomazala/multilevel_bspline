echo off
set obj_diff="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/obj_diff.exe" 

set path_src=E:/GDrive/Shared/Data/FigsHigh/


::%obj_diff% %path_src%fig02.obj %path_src%fig03.obj %path_src%fig23_diff.obj 
::%obj_diff% %path_src%fig02.obj %path_src%fig04.obj %path_src%fig24_diff.obj 
::%obj_diff% %path_src%fig02.obj %path_src%fig05.obj %path_src%fig25_diff.obj 

%obj_diff% %path_src%fig03.obj %path_src%fig02.obj %path_src%fig32_diff.obj 
%obj_diff% %path_src%fig04.obj %path_src%fig02.obj %path_src%fig42_diff.obj 
%obj_diff% %path_src%fig05.obj %path_src%fig02.obj %path_src%fig52_diff.obj 

