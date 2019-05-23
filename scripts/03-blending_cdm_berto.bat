echo off
set blending_cdm="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/blending_details_cdm.exe" 

set path_cdm=E:/GDrive/Shared/Data/Berto_Buraco/
set path_mba=E:/GDrive/Shared/Data/Berto_Buraco/
set path_out=E:/GDrive/Shared/Data/Berto_Buraco/



%blending_cdm% %path_mba%fig02_face4_2_10_mba_2.obj %path_cdm%fig02_face4_cdm_3.obj %path_mba%fig02_face4_2_10_mba_2.obj %path_mba%fig02_face4_2_10_mba_3.obj %path_cdm%fig02_face4_cdm_3.obj 0.00 1.00 %path_out%fig02_face4_2_2_cdm_3_000_100.obj
%blending_cdm% %path_mba%fig02_face4_2_10_mba_3.obj %path_cdm%fig02_face4_cdm_4.obj %path_mba%fig02_face4_2_10_mba_3.obj %path_mba%fig02_face4_2_10_mba_4.obj %path_cdm%fig02_face4_cdm_4.obj 0.00 1.00 %path_out%fig02_face4_2_2_cdm_4_000_100.obj
%blending_cdm% %path_mba%fig02_face4_2_10_mba_4.obj %path_cdm%fig02_face4_cdm_5.obj %path_mba%fig02_face4_2_10_mba_4.obj %path_mba%fig02_face4_2_10_mba_5.obj %path_cdm%fig02_face4_cdm_5.obj 0.00 1.00 %path_out%fig02_face4_2_2_cdm_5_000_100.obj

%blending_cdm% %path_mba%fig02_face4_separado_2_10_mba_2.obj %path_cdm%fig02_face4_separado_cdm_3.obj %path_mba%fig02_face4_separado_2_10_mba_2.obj %path_mba%fig02_face4_separado_2_10_mba_3.obj %path_cdm%fig02_face4_separado_cdm_3.obj 0.00 1.00 %path_out%fig02_face4_separado_2_2_cdm_3_000_100.obj
%blending_cdm% %path_mba%fig02_face4_separado_2_10_mba_3.obj %path_cdm%fig02_face4_separado_cdm_4.obj %path_mba%fig02_face4_separado_2_10_mba_3.obj %path_mba%fig02_face4_separado_2_10_mba_4.obj %path_cdm%fig02_face4_separado_cdm_4.obj 0.00 1.00 %path_out%fig02_face4_separado_2_2_cdm_4_000_100.obj
%blending_cdm% %path_mba%fig02_face4_separado_2_10_mba_4.obj %path_cdm%fig02_face4_separado_cdm_5.obj %path_mba%fig02_face4_separado_2_10_mba_4.obj %path_mba%fig02_face4_separado_2_10_mba_5.obj %path_cdm%fig02_face4_separado_cdm_5.obj 0.00 1.00 %path_out%fig02_face4_separado_2_2_cdm_5_000_100.obj

