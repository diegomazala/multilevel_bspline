echo off
set obj_diff="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/obj_diff.exe" 

set path_blend_lap=E:/GDrive/Shared/Data/FigsHigh/blending_lap/
set path_blend_others=E:/GDrive/Shared/Data/FigsHigh/blending_cdm/
set path_out=E:/GDrive/Shared/Data/FigsHigh/diff_blending_lap_others/



%obj_diff% %path_blend_lap%fig_3_2_50.obj     %path_blend_others%fig_3_2_cdm_4_100_000.obj  %path_out%fig_3_2_l1_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_100.obj    %path_blend_others%fig_3_2_cdm_4_092_008.obj  %path_out%fig_3_2_l2_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_500.obj    %path_blend_others%fig_3_2_cdm_4_084_016.obj  %path_out%fig_3_2_l3_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_1000.obj   %path_blend_others%fig_3_2_cdm_4_075_025.obj  %path_out%fig_3_2_l4_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_2500.obj   %path_blend_others%fig_3_2_cdm_4_067_033.obj  %path_out%fig_3_2_l5_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_5000.obj   %path_blend_others%fig_3_2_cdm_4_058_042.obj  %path_out%fig_3_2_l6_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_7500.obj   %path_blend_others%fig_3_2_cdm_4_050_050.obj  %path_out%fig_3_2_l7_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_10000.obj  %path_blend_others%fig_3_2_cdm_4_042_058.obj  %path_out%fig_3_2_l8_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_12000.obj  %path_blend_others%fig_3_2_cdm_4_033_067.obj  %path_out%fig_3_2_l9_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_15000.obj  %path_blend_others%fig_3_2_cdm_4_025_075.obj  %path_out%fig_3_2_l10_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_17000.obj  %path_blend_others%fig_3_2_cdm_4_016_084.obj  %path_out%fig_3_2_l11_diff.obj
%obj_diff% %path_blend_lap%fig_3_2_19000.obj  %path_blend_others%fig_3_2_cdm_4_008_092.obj  %path_out%fig_3_2_l12_diff.obj
 
%obj_diff% %path_blend_lap%fig_4_2_50.obj     %path_blend_others%fig_4_2_cdm_4_100_000.obj  %path_out%fig_4_2_l1_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_100.obj    %path_blend_others%fig_4_2_cdm_4_092_008.obj  %path_out%fig_4_2_l2_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_500.obj    %path_blend_others%fig_4_2_cdm_4_084_016.obj  %path_out%fig_4_2_l3_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_1000.obj   %path_blend_others%fig_4_2_cdm_4_075_025.obj  %path_out%fig_4_2_l4_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_2500.obj   %path_blend_others%fig_4_2_cdm_4_067_033.obj  %path_out%fig_4_2_l5_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_5000.obj   %path_blend_others%fig_4_2_cdm_4_058_042.obj  %path_out%fig_4_2_l6_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_7500.obj   %path_blend_others%fig_4_2_cdm_4_050_050.obj  %path_out%fig_4_2_l7_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_10000.obj  %path_blend_others%fig_4_2_cdm_4_042_058.obj  %path_out%fig_4_2_l8_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_12000.obj  %path_blend_others%fig_4_2_cdm_4_033_067.obj  %path_out%fig_4_2_l9_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_15000.obj  %path_blend_others%fig_4_2_cdm_4_025_075.obj  %path_out%fig_4_2_l10_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_17000.obj  %path_blend_others%fig_4_2_cdm_4_016_084.obj  %path_out%fig_4_2_l11_diff.obj
%obj_diff% %path_blend_lap%fig_4_2_19000.obj  %path_blend_others%fig_4_2_cdm_4_008_092.obj  %path_out%fig_4_2_l12_diff.obj

%obj_diff% %path_blend_lap%fig_5_2_50.obj     %path_blend_others%fig_5_2_cdm_4_100_000.obj  %path_out%fig_5_2_l1_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_100.obj    %path_blend_others%fig_5_2_cdm_4_092_008.obj  %path_out%fig_5_2_l2_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_500.obj    %path_blend_others%fig_5_2_cdm_4_084_016.obj  %path_out%fig_5_2_l3_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_1000.obj   %path_blend_others%fig_5_2_cdm_4_075_025.obj  %path_out%fig_5_2_l4_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_2500.obj   %path_blend_others%fig_5_2_cdm_4_067_033.obj  %path_out%fig_5_2_l5_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_5000.obj   %path_blend_others%fig_5_2_cdm_4_058_042.obj  %path_out%fig_5_2_l6_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_7500.obj   %path_blend_others%fig_5_2_cdm_4_050_050.obj  %path_out%fig_5_2_l7_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_10000.obj  %path_blend_others%fig_5_2_cdm_4_042_058.obj  %path_out%fig_5_2_l8_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_12000.obj  %path_blend_others%fig_5_2_cdm_4_033_067.obj  %path_out%fig_5_2_l9_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_15000.obj  %path_blend_others%fig_5_2_cdm_4_025_075.obj  %path_out%fig_5_2_l10_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_17000.obj  %path_blend_others%fig_5_2_cdm_4_016_084.obj  %path_out%fig_5_2_l11_diff.obj
%obj_diff% %path_blend_lap%fig_5_2_19000.obj  %path_blend_others%fig_5_2_cdm_4_008_092.obj  %path_out%fig_5_2_l12_diff.obj