echo off
set blending_detail="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/blending_details_lap.exe" 


::("d",  0, "Detail mesh file", true);
::("b",  0, "Base mesh file", false);
::("ds", 0, "Smooth detail mesh file", true);
::("bs", 0, "Smooth target mesh file", true);
::("o",  0, "Output mesh result filename", true, "output.obj");


set path_d=E:/GDrive/Shared/Data/Rejuve/
set path_b=E:/GDrive/Shared/Data/Rejuve/
set path_ds=E:/GDrive/Shared/Data/Rejuve/smooth/
set path_bs=E:/GDrive/Shared/Data/Rejuve/smooth/
set path_o=E:/GDrive/Shared/Data/Rejuve/blending_lap/

%blending_detail% --bs %path_bs%fig_2_3_lap_50.obj    --d %path_d%fig02.obj --ds %path_ds%fig02_lap_50.obj    --o %path_o%fig_2_3_rj_50.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_100.obj   --d %path_d%fig02.obj --ds %path_ds%fig02_lap_100.obj   --o %path_o%fig_2_3_rj_100.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_500.obj   --d %path_d%fig02.obj --ds %path_ds%fig02_lap_500.obj   --o %path_o%fig_2_3_rj_500.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_1000.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_1000.obj  --o %path_o%fig_2_3_rj_1000.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_2500.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_2500.obj  --o %path_o%fig_2_3_rj_2500.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_5000.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_5000.obj  --o %path_o%fig_2_3_rj_5000.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_7500.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_7500.obj  --o %path_o%fig_2_3_rj_7500.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_10000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_10000.obj --o %path_o%fig_2_3_rj_10000.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_12000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_12000.obj --o %path_o%fig_2_3_rj_12000.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_15000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_15000.obj --o %path_o%fig_2_3_rj_15000.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_17000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_17000.obj --o %path_o%fig_2_3_rj_17000.obj
%blending_detail% --bs %path_bs%fig_2_3_lap_19000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_19000.obj --o %path_o%fig_2_3_rj_19000.obj

%blending_detail% --bs %path_bs%fig_2_4_lap_50.obj    --d %path_d%fig02.obj --ds %path_ds%fig02_lap_50.obj    --o %path_o%fig_2_4_rj_50.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_100.obj   --d %path_d%fig02.obj --ds %path_ds%fig02_lap_100.obj   --o %path_o%fig_2_4_rj_100.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_500.obj   --d %path_d%fig02.obj --ds %path_ds%fig02_lap_500.obj   --o %path_o%fig_2_4_rj_500.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_1000.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_1000.obj  --o %path_o%fig_2_4_rj_1000.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_2500.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_2500.obj  --o %path_o%fig_2_4_rj_2500.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_5000.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_5000.obj  --o %path_o%fig_2_4_rj_5000.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_7500.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_7500.obj  --o %path_o%fig_2_4_rj_7500.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_10000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_10000.obj --o %path_o%fig_2_4_rj_10000.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_12000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_12000.obj --o %path_o%fig_2_4_rj_12000.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_15000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_15000.obj --o %path_o%fig_2_4_rj_15000.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_17000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_17000.obj --o %path_o%fig_2_4_rj_17000.obj
%blending_detail% --bs %path_bs%fig_2_4_lap_19000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_19000.obj --o %path_o%fig_2_4_rj_19000.obj

%blending_detail% --bs %path_bs%fig_2_5_lap_50.obj    --d %path_d%fig02.obj --ds %path_ds%fig02_lap_50.obj    --o %path_o%fig_2_5_rj_50.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_100.obj   --d %path_d%fig02.obj --ds %path_ds%fig02_lap_100.obj   --o %path_o%fig_2_5_rj_100.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_500.obj   --d %path_d%fig02.obj --ds %path_ds%fig02_lap_500.obj   --o %path_o%fig_2_5_rj_500.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_1000.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_1000.obj  --o %path_o%fig_2_5_rj_1000.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_2500.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_2500.obj  --o %path_o%fig_2_5_rj_2500.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_5000.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_5000.obj  --o %path_o%fig_2_5_rj_5000.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_7500.obj  --d %path_d%fig02.obj --ds %path_ds%fig02_lap_7500.obj  --o %path_o%fig_2_5_rj_7500.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_10000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_10000.obj --o %path_o%fig_2_5_rj_10000.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_12000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_12000.obj --o %path_o%fig_2_5_rj_12000.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_15000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_15000.obj --o %path_o%fig_2_5_rj_15000.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_17000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_17000.obj --o %path_o%fig_2_5_rj_17000.obj
%blending_detail% --bs %path_bs%fig_2_5_lap_19000.obj --d %path_d%fig02.obj --ds %path_ds%fig02_lap_19000.obj --o %path_o%fig_2_5_rj_19000.obj