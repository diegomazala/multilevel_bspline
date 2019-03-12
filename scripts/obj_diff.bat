echo off
set obj_diff="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/obj_diff.exe" 

set path_base=E:/GDrive/Shared/Data/FigsHigh/
set path_blend=E:/GDrive/Shared/Data/FigsHigh/blending_lap/
set path_out=E:/GDrive/Shared/Data/FigsHigh/blending_lap_diff/


::%obj_diff% %path_src%fig02_face.obj %path_src%fig03.obj %path_src%fig23_diff.obj 
::%obj_diff% %path_src%fig02_face.obj %path_src%fig04.obj %path_src%fig24_diff.obj 
::%obj_diff% %path_src%fig02_face.obj %path_src%fig05.obj %path_src%fig25_diff.obj 

::%obj_diff% %path_src%fig03.obj %path_src%fig02_face.obj %path_src%fig32_diff.obj 
::%obj_diff% %path_src%fig04.obj %path_src%fig02_face.obj %path_src%fig42_diff.obj 
::%obj_diff% %path_src%fig05.obj %path_src%fig02_face.obj %path_src%fig52_diff.obj 

%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_50.obj     %path_out%fig23_50_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_100.obj    %path_out%fig23_100_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_500.obj    %path_out%fig23_500_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_1000.obj   %path_out%fig23_1000_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_2500.obj   %path_out%fig23_2500_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_5000.obj   %path_out%fig23_5000_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_7500.obj   %path_out%fig23_7500_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_10000.obj  %path_out%fig23_10000_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_12000.obj  %path_out%fig23_12000_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_15000.obj  %path_out%fig23_15000_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_17000.obj  %path_out%fig23_17000_diff.obj
%obj_diff% %path_base%fig03_face.obj %path_blend%fig_2_3_19000.obj  %path_out%fig23_19000_diff.obj


%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_50.obj     %path_out%fig24_50_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_100.obj    %path_out%fig24_100_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_500.obj    %path_out%fig24_500_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_1000.obj   %path_out%fig24_1000_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_2500.obj   %path_out%fig24_2500_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_5000.obj   %path_out%fig24_5000_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_7500.obj   %path_out%fig24_7500_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_10000.obj  %path_out%fig24_10000_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_12000.obj  %path_out%fig24_12000_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_15000.obj  %path_out%fig24_15000_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_17000.obj  %path_out%fig24_17000_diff.obj
%obj_diff% %path_base%fig04_face.obj %path_blend%fig_2_4_19000.obj  %path_out%fig24_19000_diff.obj

%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_50.obj     %path_out%fig25_50_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_100.obj    %path_out%fig25_100_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_500.obj    %path_out%fig25_500_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_1000.obj   %path_out%fig25_1000_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_2500.obj   %path_out%fig25_2500_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_5000.obj   %path_out%fig25_5000_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_7500.obj   %path_out%fig25_7500_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_10000.obj  %path_out%fig25_10000_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_12000.obj  %path_out%fig25_12000_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_15000.obj  %path_out%fig25_15000_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_17000.obj  %path_out%fig25_17000_diff.obj
%obj_diff% %path_base%fig05_face.obj %path_blend%fig_2_5_19000.obj  %path_out%fig25_19000_diff.obj