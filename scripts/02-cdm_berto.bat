echo off
set cdm="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/cdm.exe" 

set path_src=E:/GDrive/Shared/Data/Berto_Buraco/
set path_mba=E:/GDrive/Shared/Data/Berto_Buraco/
set path_cdm=E:/GDrive/Shared/Data/Berto_Buraco/

%cdm% %path_src%fig02_face4.obj %path_mba%fig02_face4_2_10_mba_3.obj  %path_cdm%fig02_face4_cdm_3.obj 
%cdm% %path_src%fig02_face4.obj %path_mba%fig02_face4_2_10_mba_4.obj  %path_cdm%fig02_face4_cdm_4.obj 
%cdm% %path_src%fig02_face4.obj %path_mba%fig02_face4_2_10_mba_5.obj  %path_cdm%fig02_face4_cdm_5.obj 

%cdm% %path_src%fig02_face4_separado.obj %path_mba%fig02_face4_separado_2_10_mba_3.obj  %path_cdm%fig02_face4_separado_cdm_3.obj 
%cdm% %path_src%fig02_face4_separado.obj %path_mba%fig02_face4_separado_2_10_mba_4.obj  %path_cdm%fig02_face4_separado_cdm_4.obj 
%cdm% %path_src%fig02_face4_separado.obj %path_mba%fig02_face4_separado_2_10_mba_5.obj  %path_cdm%fig02_face4_separado_cdm_5.obj 

