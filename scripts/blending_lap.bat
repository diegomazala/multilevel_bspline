echo off
set blending_detail="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/blending_details_lap.exe" 


set path_d=E:/GDrive/Shared/Data/FigsHigh/
set path_t=E:/GDrive/Shared/Data/FigsHigh/
set path_df=E:/GDrive/Shared/Data/FigsHigh/smooth/
set path_tf=E:/GDrive/Shared/Data/FigsHigh/smooth/
set path_o=E:/GDrive/Shared/Data/FigsHigh/blending_lap/

%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_50.obj    --tf %path_tf%fig03_lap_50.obj    --o %path_o%fig_2_3_50.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_100.obj   --tf %path_tf%fig03_lap_100.obj   --o %path_o%fig_2_3_100.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_500.obj   --tf %path_tf%fig03_lap_500.obj   --o %path_o%fig_2_3_500.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_1000.obj  --tf %path_tf%fig03_lap_1000.obj  --o %path_o%fig_2_3_1000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_2500.obj  --tf %path_tf%fig03_lap_2500.obj  --o %path_o%fig_2_3_2500.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_5000.obj  --tf %path_tf%fig03_lap_5000.obj  --o %path_o%fig_2_3_5000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_7500.obj  --tf %path_tf%fig03_lap_7500.obj  --o %path_o%fig_2_3_7500.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_10000.obj --tf %path_tf%fig03_lap_10000.obj --o %path_o%fig_2_3_10000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_12000.obj --tf %path_tf%fig03_lap_12000.obj --o %path_o%fig_2_3_12000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_15000.obj --tf %path_tf%fig03_lap_15000.obj --o %path_o%fig_2_3_15000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_17000.obj --tf %path_tf%fig03_lap_17000.obj --o %path_o%fig_2_3_17000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_19000.obj --tf %path_tf%fig03_lap_19000.obj --o %path_o%fig_2_3_19000.obj

%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_50.obj    --tf %path_tf%fig04_lap_50.obj    --o %path_o%fig_2_4_50.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_100.obj   --tf %path_tf%fig04_lap_100.obj   --o %path_o%fig_2_4_100.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_500.obj   --tf %path_tf%fig04_lap_500.obj   --o %path_o%fig_2_4_500.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_1000.obj  --tf %path_tf%fig04_lap_1000.obj  --o %path_o%fig_2_4_1000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_2500.obj  --tf %path_tf%fig04_lap_2500.obj  --o %path_o%fig_2_4_2500.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_5000.obj  --tf %path_tf%fig04_lap_5000.obj  --o %path_o%fig_2_4_5000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_7500.obj  --tf %path_tf%fig04_lap_7500.obj  --o %path_o%fig_2_4_7500.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_10000.obj --tf %path_tf%fig04_lap_10000.obj --o %path_o%fig_2_4_10000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_12000.obj --tf %path_tf%fig04_lap_12000.obj --o %path_o%fig_2_4_12000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_15000.obj --tf %path_tf%fig04_lap_15000.obj --o %path_o%fig_2_4_15000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_17000.obj --tf %path_tf%fig04_lap_17000.obj --o %path_o%fig_2_4_17000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_19000.obj --tf %path_tf%fig04_lap_19000.obj --o %path_o%fig_2_4_19000.obj

%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_50.obj    --tf %path_tf%fig05_lap_50.obj    --o %path_o%fig_2_5_50.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_100.obj   --tf %path_tf%fig05_lap_100.obj   --o %path_o%fig_2_5_100.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_500.obj   --tf %path_tf%fig05_lap_500.obj   --o %path_o%fig_2_5_500.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_1000.obj  --tf %path_tf%fig05_lap_1000.obj  --o %path_o%fig_2_5_1000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_2500.obj  --tf %path_tf%fig05_lap_2500.obj  --o %path_o%fig_2_5_2500.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_5000.obj  --tf %path_tf%fig05_lap_5000.obj  --o %path_o%fig_2_5_5000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_7500.obj  --tf %path_tf%fig05_lap_7500.obj  --o %path_o%fig_2_5_7500.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_10000.obj --tf %path_tf%fig05_lap_10000.obj --o %path_o%fig_2_5_10000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_12000.obj --tf %path_tf%fig05_lap_12000.obj --o %path_o%fig_2_5_12000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_15000.obj --tf %path_tf%fig05_lap_15000.obj --o %path_o%fig_2_5_15000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_17000.obj --tf %path_tf%fig05_lap_17000.obj --o %path_o%fig_2_5_17000.obj
%blending_detail% --d %path_d%fig02_face.obj --df %path_df%fig02_lap_19000.obj --tf %path_tf%fig05_lap_19000.obj --o %path_o%fig_2_5_19000.obj


%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_50.obj    --tf %path_tf%fig02_lap_50.obj    --o %path_o%fig_3_2_50.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_100.obj   --tf %path_tf%fig02_lap_100.obj   --o %path_o%fig_3_2_100.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_500.obj   --tf %path_tf%fig02_lap_500.obj   --o %path_o%fig_3_2_500.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_1000.obj  --tf %path_tf%fig02_lap_1000.obj  --o %path_o%fig_3_2_1000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_2500.obj  --tf %path_tf%fig02_lap_2500.obj  --o %path_o%fig_3_2_2500.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_5000.obj  --tf %path_tf%fig02_lap_5000.obj  --o %path_o%fig_3_2_5000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_7500.obj  --tf %path_tf%fig02_lap_7500.obj  --o %path_o%fig_3_2_7500.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_10000.obj --tf %path_tf%fig02_lap_10000.obj --o %path_o%fig_3_2_10000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_12000.obj --tf %path_tf%fig02_lap_12000.obj --o %path_o%fig_3_2_12000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_15000.obj --tf %path_tf%fig02_lap_15000.obj --o %path_o%fig_3_2_15000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_17000.obj --tf %path_tf%fig02_lap_17000.obj --o %path_o%fig_3_2_17000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_19000.obj --tf %path_tf%fig02_lap_19000.obj --o %path_o%fig_3_2_19000.obj

%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_50.obj    --tf %path_tf%fig04_lap_50.obj    --o %path_o%fig_3_4_50.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_100.obj   --tf %path_tf%fig04_lap_100.obj   --o %path_o%fig_3_4_100.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_500.obj   --tf %path_tf%fig04_lap_500.obj   --o %path_o%fig_3_4_500.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_1000.obj  --tf %path_tf%fig04_lap_1000.obj  --o %path_o%fig_3_4_1000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_2500.obj  --tf %path_tf%fig04_lap_2500.obj  --o %path_o%fig_3_4_2500.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_5000.obj  --tf %path_tf%fig04_lap_5000.obj  --o %path_o%fig_3_4_5000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_7500.obj  --tf %path_tf%fig04_lap_7500.obj  --o %path_o%fig_3_4_7500.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_10000.obj --tf %path_tf%fig04_lap_10000.obj --o %path_o%fig_3_4_10000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_12000.obj --tf %path_tf%fig04_lap_12000.obj --o %path_o%fig_3_4_12000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_15000.obj --tf %path_tf%fig04_lap_15000.obj --o %path_o%fig_3_4_15000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_17000.obj --tf %path_tf%fig04_lap_17000.obj --o %path_o%fig_3_4_17000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_19000.obj --tf %path_tf%fig04_lap_19000.obj --o %path_o%fig_3_4_19000.obj

%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_50.obj    --tf %path_tf%fig05_lap_50.obj    --o %path_o%fig_3_5_50.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_100.obj   --tf %path_tf%fig05_lap_100.obj   --o %path_o%fig_3_5_100.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_500.obj   --tf %path_tf%fig05_lap_500.obj   --o %path_o%fig_3_5_500.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_1000.obj  --tf %path_tf%fig05_lap_1000.obj  --o %path_o%fig_3_5_1000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_2500.obj  --tf %path_tf%fig05_lap_2500.obj  --o %path_o%fig_3_5_2500.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_5000.obj  --tf %path_tf%fig05_lap_5000.obj  --o %path_o%fig_3_5_5000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_7500.obj  --tf %path_tf%fig05_lap_7500.obj  --o %path_o%fig_3_5_7500.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_10000.obj --tf %path_tf%fig05_lap_10000.obj --o %path_o%fig_3_5_10000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_12000.obj --tf %path_tf%fig05_lap_12000.obj --o %path_o%fig_3_5_12000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_15000.obj --tf %path_tf%fig05_lap_15000.obj --o %path_o%fig_3_5_15000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_17000.obj --tf %path_tf%fig05_lap_17000.obj --o %path_o%fig_3_5_17000.obj
%blending_detail% --d %path_d%fig03_face.obj --df %path_df%fig03_lap_19000.obj --tf %path_tf%fig05_lap_19000.obj --o %path_o%fig_3_5_19000.obj


%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_50.obj    --tf %path_tf%fig02_lap_50.obj    --o %path_o%fig_4_2_50.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_100.obj   --tf %path_tf%fig02_lap_100.obj   --o %path_o%fig_4_2_100.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_500.obj   --tf %path_tf%fig02_lap_500.obj   --o %path_o%fig_4_2_500.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_1000.obj  --tf %path_tf%fig02_lap_1000.obj  --o %path_o%fig_4_2_1000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_2500.obj  --tf %path_tf%fig02_lap_2500.obj  --o %path_o%fig_4_2_2500.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_5000.obj  --tf %path_tf%fig02_lap_5000.obj  --o %path_o%fig_4_2_5000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_7500.obj  --tf %path_tf%fig02_lap_7500.obj  --o %path_o%fig_4_2_7500.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_10000.obj --tf %path_tf%fig02_lap_10000.obj --o %path_o%fig_4_2_10000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_12000.obj --tf %path_tf%fig02_lap_12000.obj --o %path_o%fig_4_2_12000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_15000.obj --tf %path_tf%fig02_lap_15000.obj --o %path_o%fig_4_2_15000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_17000.obj --tf %path_tf%fig02_lap_17000.obj --o %path_o%fig_4_2_17000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_19000.obj --tf %path_tf%fig02_lap_19000.obj --o %path_o%fig_4_2_19000.obj

%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_50.obj    --tf %path_tf%fig03_lap_50.obj    --o %path_o%fig_4_3_50.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_100.obj   --tf %path_tf%fig03_lap_100.obj   --o %path_o%fig_4_3_100.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_500.obj   --tf %path_tf%fig03_lap_500.obj   --o %path_o%fig_4_3_500.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_1000.obj  --tf %path_tf%fig03_lap_1000.obj  --o %path_o%fig_4_3_1000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_2500.obj  --tf %path_tf%fig03_lap_2500.obj  --o %path_o%fig_4_3_2500.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_5000.obj  --tf %path_tf%fig03_lap_5000.obj  --o %path_o%fig_4_3_5000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_7500.obj  --tf %path_tf%fig03_lap_7500.obj  --o %path_o%fig_4_3_7500.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_10000.obj --tf %path_tf%fig03_lap_10000.obj --o %path_o%fig_4_3_10000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_12000.obj --tf %path_tf%fig03_lap_12000.obj --o %path_o%fig_4_3_12000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_15000.obj --tf %path_tf%fig03_lap_15000.obj --o %path_o%fig_4_3_15000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_17000.obj --tf %path_tf%fig03_lap_17000.obj --o %path_o%fig_4_3_17000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_19000.obj --tf %path_tf%fig03_lap_19000.obj --o %path_o%fig_4_3_19000.obj

%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_50.obj    --tf %path_tf%fig05_lap_50.obj    --o %path_o%fig_4_5_50.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_100.obj   --tf %path_tf%fig05_lap_100.obj   --o %path_o%fig_4_5_100.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_500.obj   --tf %path_tf%fig05_lap_500.obj   --o %path_o%fig_4_5_500.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_1000.obj  --tf %path_tf%fig05_lap_1000.obj  --o %path_o%fig_4_5_1000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_2500.obj  --tf %path_tf%fig05_lap_2500.obj  --o %path_o%fig_4_5_2500.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_5000.obj  --tf %path_tf%fig05_lap_5000.obj  --o %path_o%fig_4_5_5000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_7500.obj  --tf %path_tf%fig05_lap_7500.obj  --o %path_o%fig_4_5_7500.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_10000.obj --tf %path_tf%fig05_lap_10000.obj --o %path_o%fig_4_5_10000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_12000.obj --tf %path_tf%fig05_lap_12000.obj --o %path_o%fig_4_5_12000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_15000.obj --tf %path_tf%fig05_lap_15000.obj --o %path_o%fig_4_5_15000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_17000.obj --tf %path_tf%fig05_lap_17000.obj --o %path_o%fig_4_5_17000.obj
%blending_detail% --d %path_d%fig04_face.obj --df %path_df%fig04_lap_19000.obj --tf %path_tf%fig05_lap_19000.obj --o %path_o%fig_4_5_19000.obj


%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_50.obj    --tf %path_tf%fig02_lap_50.obj    --o %path_o%fig_5_2_50.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_100.obj   --tf %path_tf%fig02_lap_100.obj   --o %path_o%fig_5_2_100.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_500.obj   --tf %path_tf%fig02_lap_500.obj   --o %path_o%fig_5_2_500.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_1000.obj  --tf %path_tf%fig02_lap_1000.obj  --o %path_o%fig_5_2_1000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_2500.obj  --tf %path_tf%fig02_lap_2500.obj  --o %path_o%fig_5_2_2500.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_5000.obj  --tf %path_tf%fig02_lap_5000.obj  --o %path_o%fig_5_2_5000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_7500.obj  --tf %path_tf%fig02_lap_7500.obj  --o %path_o%fig_5_2_7500.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_10000.obj --tf %path_tf%fig02_lap_10000.obj --o %path_o%fig_5_2_10000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_12000.obj --tf %path_tf%fig02_lap_12000.obj --o %path_o%fig_5_2_12000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_15000.obj --tf %path_tf%fig02_lap_15000.obj --o %path_o%fig_5_2_15000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_17000.obj --tf %path_tf%fig02_lap_17000.obj --o %path_o%fig_5_2_17000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_19000.obj --tf %path_tf%fig02_lap_19000.obj --o %path_o%fig_5_2_19000.obj

%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_50.obj    --tf %path_tf%fig03_lap_50.obj    --o %path_o%fig_5_3_50.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_100.obj   --tf %path_tf%fig03_lap_100.obj   --o %path_o%fig_5_3_100.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_500.obj   --tf %path_tf%fig03_lap_500.obj   --o %path_o%fig_5_3_500.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_1000.obj  --tf %path_tf%fig03_lap_1000.obj  --o %path_o%fig_5_3_1000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_2500.obj  --tf %path_tf%fig03_lap_2500.obj  --o %path_o%fig_5_3_2500.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_5000.obj  --tf %path_tf%fig03_lap_5000.obj  --o %path_o%fig_5_3_5000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_7500.obj  --tf %path_tf%fig03_lap_7500.obj  --o %path_o%fig_5_3_7500.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_10000.obj --tf %path_tf%fig03_lap_10000.obj --o %path_o%fig_5_3_10000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_12000.obj --tf %path_tf%fig03_lap_12000.obj --o %path_o%fig_5_3_12000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_15000.obj --tf %path_tf%fig03_lap_15000.obj --o %path_o%fig_5_3_15000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_17000.obj --tf %path_tf%fig03_lap_17000.obj --o %path_o%fig_5_3_17000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_19000.obj --tf %path_tf%fig03_lap_19000.obj --o %path_o%fig_5_3_19000.obj

%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_50.obj    --tf %path_tf%fig04_lap_50.obj    --o %path_o%fig_5_4_50.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_100.obj   --tf %path_tf%fig04_lap_100.obj   --o %path_o%fig_5_4_100.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_500.obj   --tf %path_tf%fig04_lap_500.obj   --o %path_o%fig_5_4_500.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_1000.obj  --tf %path_tf%fig04_lap_1000.obj  --o %path_o%fig_5_4_1000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_2500.obj  --tf %path_tf%fig04_lap_2500.obj  --o %path_o%fig_5_4_2500.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_5000.obj  --tf %path_tf%fig04_lap_5000.obj  --o %path_o%fig_5_4_5000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_7500.obj  --tf %path_tf%fig04_lap_7500.obj  --o %path_o%fig_5_4_7500.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_10000.obj --tf %path_tf%fig04_lap_10000.obj --o %path_o%fig_5_4_10000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_12000.obj --tf %path_tf%fig04_lap_12000.obj --o %path_o%fig_5_4_12000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_15000.obj --tf %path_tf%fig04_lap_15000.obj --o %path_o%fig_5_4_15000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_17000.obj --tf %path_tf%fig04_lap_17000.obj --o %path_o%fig_5_4_17000.obj
%blending_detail% --d %path_d%fig05_face.obj --df %path_df%fig05_lap_19000.obj --tf %path_tf%fig04_lap_19000.obj --o %path_o%fig_5_4_19000.obj