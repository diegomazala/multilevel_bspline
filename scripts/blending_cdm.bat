echo off
set blending_cdm="E:/Projects/DSc/multilevel_bspline/msvc/bin/Release/blending_details_cdm.exe" 

set path_cdm=E:/GDrive/Shared/Data/FigsHigh/cdm/
set path_mba=E:/GDrive/Shared/Data/FigsHigh/mba_2_10/
set path_out=E:/GDrive/Shared/Data/FigsHigh/blending_cdm/


:: -- Base: Fig_03  Details: Fig_02  Sd: 02  Cdm: 03
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.00 1.00 %path_out%fig_3_2_cdm_3_000_100.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.08 0.91 %path_out%fig_3_2_cdm_3_008_092.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.16 0.84 %path_out%fig_3_2_cdm_3_016_084.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.25 0.75 %path_out%fig_3_2_cdm_3_025_075.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.33 0.67 %path_out%fig_3_2_cdm_3_033_067.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.42 0.58 %path_out%fig_3_2_cdm_3_042_058.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.50 0.50 %path_out%fig_3_2_cdm_3_050_050.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.58 0.42 %path_out%fig_3_2_cdm_3_058_042.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.67 0.33 %path_out%fig_3_2_cdm_3_067_033.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.75 0.25 %path_out%fig_3_2_cdm_3_075_025.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.84 0.16 %path_out%fig_3_2_cdm_3_084_016.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.92 0.08 %path_out%fig_3_2_cdm_3_092_008.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_2.obj %path_cdm%fig03_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 1.00 0.00 %path_out%fig_3_2_cdm_3_100_000.obj

:: -- Base: Fig_04  Details: Fig_02  Sd: 02  Cdm: 03
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.00 1.00 %path_out%fig_4_2_cdm_3_000_100.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.08 0.91 %path_out%fig_4_2_cdm_3_008_092.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.16 0.84 %path_out%fig_4_2_cdm_3_016_084.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.25 0.75 %path_out%fig_4_2_cdm_3_025_075.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.33 0.67 %path_out%fig_4_2_cdm_3_033_067.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.42 0.58 %path_out%fig_4_2_cdm_3_042_058.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.50 0.50 %path_out%fig_4_2_cdm_3_050_050.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.58 0.42 %path_out%fig_4_2_cdm_3_058_042.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.67 0.33 %path_out%fig_4_2_cdm_3_067_033.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.75 0.25 %path_out%fig_4_2_cdm_3_075_025.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.84 0.16 %path_out%fig_4_2_cdm_3_084_016.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.92 0.08 %path_out%fig_4_2_cdm_3_092_008.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_2.obj %path_cdm%fig04_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 1.00 0.00 %path_out%fig_4_2_cdm_3_100_000.obj
::
:::: -- Base: Fig_05  Details: Fig_02  Sd: 02  Cdm: 03
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.00 1.00 %path_out%fig_5_2_cdm_3_000_100.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.08 0.91 %path_out%fig_5_2_cdm_3_008_092.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.16 0.84 %path_out%fig_5_2_cdm_3_016_084.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.25 0.75 %path_out%fig_5_2_cdm_3_025_075.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.33 0.67 %path_out%fig_5_2_cdm_3_033_067.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.42 0.58 %path_out%fig_5_2_cdm_3_042_058.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.50 0.50 %path_out%fig_5_2_cdm_3_050_050.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.58 0.42 %path_out%fig_5_2_cdm_3_058_042.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.67 0.33 %path_out%fig_5_2_cdm_3_067_033.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.75 0.25 %path_out%fig_5_2_cdm_3_075_025.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.84 0.16 %path_out%fig_5_2_cdm_3_084_016.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 0.92 0.08 %path_out%fig_5_2_cdm_3_092_008.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_2.obj %path_cdm%fig05_cdm_3.obj %path_mba%fig02_2_10_mba_2.obj %path_mba%fig02_2_10_mba_3.obj %path_cdm%fig02_cdm_3.obj 1.00 0.00 %path_out%fig_5_2_cdm_3_100_000.obj




:: -- Base: Fig_03  Details: Fig_02  Sd: 03  Cdm: 04
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.00 1.00 %path_out%fig_3_2_cdm_4_000_100.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.08 0.91 %path_out%fig_3_2_cdm_4_008_092.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.16 0.84 %path_out%fig_3_2_cdm_4_016_084.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.25 0.75 %path_out%fig_3_2_cdm_4_025_075.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.33 0.67 %path_out%fig_3_2_cdm_4_033_067.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.42 0.58 %path_out%fig_3_2_cdm_4_042_058.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.50 0.50 %path_out%fig_3_2_cdm_4_050_050.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.58 0.42 %path_out%fig_3_2_cdm_4_058_042.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.67 0.33 %path_out%fig_3_2_cdm_4_067_033.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.75 0.25 %path_out%fig_3_2_cdm_4_075_025.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.84 0.16 %path_out%fig_3_2_cdm_4_084_016.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.92 0.08 %path_out%fig_3_2_cdm_4_092_008.obj
%blending_cdm% %path_mba%fig03_2_10_mba_3.obj %path_cdm%fig03_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 1.00 0.00 %path_out%fig_3_2_cdm_4_100_000.obj

:: -- Base: Fig_04  Details: Fig_02  Sd: 03  Cdm: 04
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.00 1.00 %path_out%fig_4_2_cdm_4_000_100.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.08 0.91 %path_out%fig_4_2_cdm_4_008_092.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.16 0.84 %path_out%fig_4_2_cdm_4_016_084.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.25 0.75 %path_out%fig_4_2_cdm_4_025_075.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.33 0.67 %path_out%fig_4_2_cdm_4_033_067.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.42 0.58 %path_out%fig_4_2_cdm_4_042_058.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.50 0.50 %path_out%fig_4_2_cdm_4_050_050.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.58 0.42 %path_out%fig_4_2_cdm_4_058_042.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.67 0.33 %path_out%fig_4_2_cdm_4_067_033.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.75 0.25 %path_out%fig_4_2_cdm_4_075_025.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.84 0.16 %path_out%fig_4_2_cdm_4_084_016.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.92 0.08 %path_out%fig_4_2_cdm_4_092_008.obj
%blending_cdm% %path_mba%fig04_2_10_mba_3.obj %path_cdm%fig04_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 1.00 0.00 %path_out%fig_4_2_cdm_4_100_000.obj

:: -- Base: Fig_05  Details: Fig_02  Sd: 03  Cdm: 04
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.00 1.00 %path_out%fig_5_2_cdm_4_000_100.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.08 0.91 %path_out%fig_5_2_cdm_4_008_092.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.16 0.84 %path_out%fig_5_2_cdm_4_016_084.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.25 0.75 %path_out%fig_5_2_cdm_4_025_075.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.33 0.67 %path_out%fig_5_2_cdm_4_033_067.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.42 0.58 %path_out%fig_5_2_cdm_4_042_058.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.50 0.50 %path_out%fig_5_2_cdm_4_050_050.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.58 0.42 %path_out%fig_5_2_cdm_4_058_042.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.67 0.33 %path_out%fig_5_2_cdm_4_067_033.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.75 0.25 %path_out%fig_5_2_cdm_4_075_025.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.84 0.16 %path_out%fig_5_2_cdm_4_084_016.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 0.92 0.08 %path_out%fig_5_2_cdm_4_092_008.obj
%blending_cdm% %path_mba%fig05_2_10_mba_3.obj %path_cdm%fig05_cdm_4.obj %path_mba%fig02_2_10_mba_3.obj %path_mba%fig02_2_10_mba_4.obj %path_cdm%fig02_cdm_4.obj 1.00 0.00 %path_out%fig_5_2_cdm_4_100_000.obj




:: -- Base: Fig_03  Details: Fig_02  Sd: 04  Cdm: 05
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.00 1.00 %path_out%fig_3_2_cdm_5_000_100.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.08 0.91 %path_out%fig_3_2_cdm_5_008_092.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.16 0.84 %path_out%fig_3_2_cdm_5_016_084.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.25 0.75 %path_out%fig_3_2_cdm_5_025_075.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.33 0.67 %path_out%fig_3_2_cdm_5_033_067.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.42 0.58 %path_out%fig_3_2_cdm_5_042_058.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.50 0.50 %path_out%fig_3_2_cdm_5_050_050.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.58 0.42 %path_out%fig_3_2_cdm_5_058_042.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.67 0.33 %path_out%fig_3_2_cdm_5_067_033.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.75 0.25 %path_out%fig_3_2_cdm_5_075_025.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.84 0.16 %path_out%fig_3_2_cdm_5_084_016.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.92 0.08 %path_out%fig_3_2_cdm_5_092_008.obj
::%blending_cdm% %path_mba%fig03_2_10_mba_4.obj %path_cdm%fig03_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 1.00 0.00 %path_out%fig_3_2_cdm_5_100_000.obj
::
:::: -- Base: Fig_04  Details: Fig_02  Sd:04  Cdm: 05
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.00 1.00 %path_out%fig_4_2_cdm_5_000_100.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.08 0.91 %path_out%fig_4_2_cdm_5_008_092.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.16 0.84 %path_out%fig_4_2_cdm_5_016_084.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.25 0.75 %path_out%fig_4_2_cdm_5_025_075.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.33 0.67 %path_out%fig_4_2_cdm_5_033_067.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.42 0.58 %path_out%fig_4_2_cdm_5_042_058.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.50 0.50 %path_out%fig_4_2_cdm_5_050_050.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.58 0.42 %path_out%fig_4_2_cdm_5_058_042.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.67 0.33 %path_out%fig_4_2_cdm_5_067_033.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.75 0.25 %path_out%fig_4_2_cdm_5_075_025.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.84 0.16 %path_out%fig_4_2_cdm_5_084_016.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.92 0.08 %path_out%fig_4_2_cdm_5_092_008.obj
::%blending_cdm% %path_mba%fig04_2_10_mba_4.obj %path_cdm%fig04_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 1.00 0.00 %path_out%fig_4_2_cdm_5_100_000.obj
::
:::: -- Base: Fig_05  Details: Fig_02  Sd:04  Cdm: 05
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.00 1.00 %path_out%fig_5_2_cdm_5_000_100.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.08 0.91 %path_out%fig_5_2_cdm_5_008_092.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.16 0.84 %path_out%fig_5_2_cdm_5_016_084.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.25 0.75 %path_out%fig_5_2_cdm_5_025_075.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.33 0.67 %path_out%fig_5_2_cdm_5_033_067.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.42 0.58 %path_out%fig_5_2_cdm_5_042_058.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.50 0.50 %path_out%fig_5_2_cdm_5_050_050.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.58 0.42 %path_out%fig_5_2_cdm_5_058_042.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.67 0.33 %path_out%fig_5_2_cdm_5_067_033.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.75 0.25 %path_out%fig_5_2_cdm_5_075_025.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.84 0.16 %path_out%fig_5_2_cdm_5_084_016.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 0.92 0.08 %path_out%fig_5_2_cdm_5_092_008.obj
::%blending_cdm% %path_mba%fig05_2_10_mba_4.obj %path_cdm%fig05_cdm_5.obj %path_mba%fig02_2_10_mba_4.obj %path_mba%fig02_2_10_mba_5.obj %path_cdm%fig02_cdm_5.obj 1.00 0.00 %path_out%fig_5_2_cdm_5_100_000.obj