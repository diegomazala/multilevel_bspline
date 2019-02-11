echo off
set cdm="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/cdm.exe" 

set path_src=E:/GDrive/Shared/Data/Figs/
set path_mba=E:/GDrive/Shared/Data/Figs/mba_2_10/
set path_cdm=E:/GDrive/Shared/Data/Figs/cdm/

%cdm% %path_src%fig05.obj %path_mba%fig05_2_10_mba_3.obj  %path_cdm%fig05_cdm_3.obj 
%cdm% %path_src%fig05.obj %path_mba%fig05_2_10_mba_4.obj  %path_cdm%fig05_cdm_4.obj 
%cdm% %path_src%fig05.obj %path_mba%fig05_2_10_mba_5.obj  %path_cdm%fig05_cdm_5.obj 