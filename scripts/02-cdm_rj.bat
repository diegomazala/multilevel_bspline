echo off
set cdm="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/cdm.exe" 

set path_src=E:/GDrive/Shared/Data/Rejuve/
set path_mba=E:/GDrive/Shared/Data/Rejuve/mba_2_10/
set path_cdm=E:/GDrive/Shared/Data/Rejuve/cdm/

%cdm% %path_src%fig_2_3.obj %path_mba%fig_2_3_2_10_mba_4.obj  %path_cdm%fig_2_3_cdm_4.obj 
%cdm% %path_src%fig_2_4.obj %path_mba%fig_2_4_2_10_mba_4.obj  %path_cdm%fig_2_4_cdm_4.obj 
%cdm% %path_src%fig_2_5.obj %path_mba%fig_2_5_2_10_mba_4.obj  %path_cdm%fig_2_5_cdm_4.obj 
