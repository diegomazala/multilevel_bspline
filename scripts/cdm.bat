echo off
set cdm="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/cdm.exe" 

set path_src=E:/GDrive/Shared/Data/FigsHigh/
set path_mba=E:/GDrive/Shared/Data/FigsHigh/mba_2_10/
set path_cdm=E:/GDrive/Shared/Data/FigsHigh/cdm/

%cdm% %path_src%fig02.obj %path_mba%fig02_2_10_mba_3.obj  %path_cdm%fig02_cdm_3.obj 
%cdm% %path_src%fig02.obj %path_mba%fig02_2_10_mba_4.obj  %path_cdm%fig02_cdm_4.obj 
%cdm% %path_src%fig02.obj %path_mba%fig02_2_10_mba_5.obj  %path_cdm%fig02_cdm_5.obj 

%cdm% %path_src%fig03.obj %path_mba%fig03_2_10_mba_3.obj  %path_cdm%fig03_cdm_3.obj 
%cdm% %path_src%fig03.obj %path_mba%fig03_2_10_mba_4.obj  %path_cdm%fig03_cdm_4.obj 
%cdm% %path_src%fig03.obj %path_mba%fig03_2_10_mba_5.obj  %path_cdm%fig03_cdm_5.obj 

%cdm% %path_src%fig04.obj %path_mba%fig04_2_10_mba_3.obj  %path_cdm%fig04_cdm_3.obj 
%cdm% %path_src%fig04.obj %path_mba%fig04_2_10_mba_4.obj  %path_cdm%fig04_cdm_4.obj 
%cdm% %path_src%fig04.obj %path_mba%fig04_2_10_mba_5.obj  %path_cdm%fig04_cdm_5.obj 

%cdm% %path_src%fig05.obj %path_mba%fig05_2_10_mba_3.obj  %path_cdm%fig05_cdm_3.obj 
%cdm% %path_src%fig05.obj %path_mba%fig05_2_10_mba_4.obj  %path_cdm%fig05_cdm_4.obj 
%cdm% %path_src%fig05.obj %path_mba%fig05_2_10_mba_5.obj  %path_cdm%fig05_cdm_5.obj 