echo off
set obj_diff="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/obj_diff.exe" 

::set path_blend_lap=E:/GDrive/Shared/Data/Rejuve/blending_lap/
::set path_original=E:/GDrive/Shared/Data/Rejuve/
::set path_out=E:/GDrive/Shared/Data/Rejuve/diff_lap/
::
::
::%obj_diff% %path_blend_lap%fig_2_3_rj_19000.obj  %path_original%fig02.obj  %path_out%fig_2_3_rj_diff_fig02.obj
::%obj_diff% %path_blend_lap%fig_2_4_rj_19000.obj  %path_original%fig02.obj  %path_out%fig_2_4_rj_diff_fig02.obj
::%obj_diff% %path_blend_lap%fig_2_5_rj_19000.obj  %path_original%fig02.obj  %path_out%fig_2_5_rj_diff_fig02.obj





set path_blend_lap=E:/GDrive/Shared/Data/Rejuve/blending_cdm/
set path_original=E:/GDrive/Shared/Data/Rejuve/
set path_out=E:/GDrive/Shared/Data/Rejuve/diff_cdm/



%obj_diff% %path_blend_lap%fig_2_3_cdm_4_000_100.obj  %path_original%fig02.obj  %path_out%fig_2_3_cdm_rj_diff_fig02.obj
%obj_diff% %path_blend_lap%fig_2_4_cdm_4_000_100.obj  %path_original%fig02.obj  %path_out%fig_2_4_cdm_rj_diff_fig02.obj
%obj_diff% %path_blend_lap%fig_2_5_cdm_4_000_100.obj  %path_original%fig02.obj  %path_out%fig_2_5_cdm_rj_diff_fig02.obj
