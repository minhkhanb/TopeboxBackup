@echo off
call config.bat


echo *** Creating directories ***
mkdir %TMPRES%
mkdir %BIN%
cd %DATA_EXPORT_TOOL_DIR%

echo *** Build Data ***
REM cd %DESIGN_EXPORT_DIR%
copy  %DESIGN_FILE% %DATA_EXPORT_TOOL_DIR%\data\Objects.xls
REM cd %DESIGN_EXPORT_DIR%
REM echo %cd%
call make.bat
call run.bat
cd %DATA_EXPORT_TOOL_DIR%\release

copy SpriteList.h %OBJECT_RELEASE_DIR%
copy lib_SpriteList.cpp %OBJECT_RELEASE_DIR%
copy *.h %DESIGN_RELEASE_DIR%
copy *.manifest %RES%
REM cd %TOOLS_PATH%

REM call copy_files.bat %RES% %TMPRES%
REM copy %BPINE_TAG%\*.* %TMPRES%
cd %TMPRES%
copy /Y %TOOLS_PATH%\SmartCopy.class
dir /B/S/A-D %BPINE_TAG% | findstr /V .svn >file_copy.txt 
REM java 
for /f %%i in (file_copy.txt) do (
	java SmartCopy -i %%i
)

dir /B/S/A-D %RES% | findstr /V .svn >file_copy.txt 
REM java 
for /f %%i in (file_copy.txt) do (
	java SmartCopy -i %%i
)
rem pack text
for %%i in (*.text) do (
	%ANIM_EXPORT% 	-packtext 	%%i		%%i.bin 	UNICODE 	%SRC_DIR_H%\%%i.h
)

rem pack animation
for %%i in (*.pine) do (
	REM dir %%i >%%i.log
	java %THREAD_TOOL% %ANIM_EXPORT% 	-i %%i		-o %%~ni.bpine  QUALITY_TRUE_COLOR	EXPORT_AS_3D_TEXTURE	EXPORT_FMODULE_OPACITY USE_MULTI_MODULE_TYPE EXPORT_PALETTE_COLOR
	REM java %THREAD_TOOL% %ANIM_EXPORT% 	-i %%i		-o %%~ni.bpine  QUALITY_TRUE_COLOR	EXPORT_AS_3D_TEXTURE	EXPORT_FMODULE_OPACITY USE_MULTI_MODULE_TYPE EXPORT_PALETTE_COLOR
)

REM java %THREAD_TOOL%  %ANIM_EXPORT% 	-i dragon.pine		-o dragon.bpine			QUALITY_TRUE_COLOR	EXPORT_AS_3D_TEXTURE	PIXEL_16BITS EXPORT_FMODULE_OPACITY USE_MULTI_MODULE_TYPE


%ANIM_EXPORT% 	-packlib %TMPRES%\lib.manifest 	%BIN%	CPP		%SRC_DIR_H%\ResourceManifest.h	%SRC_DIR_CPP%\ResourceManifest.cpp		
copy bg_01_i5.png %BIN%\bg_01_i5.pbg
copy splashscreen_ip5.png %BIN%\splashscreen_ip5.pbg
copy *.bpine*	%BPINE_TAG%
copy bg_hunting_grass_ip5.png %BIN%\bg_hunting_grass_ip5.pbg
copy *.h %SRC_DIR_H%
rem del /Q *.*
REM cd %PRJ_DIR%
REM mkdir ##advert_release

REM cd %PRJ_DIR%\pine_advert_plugin\PinePromotionExporter
REM call make.bat
REM call run.bat
REM copy release\*.bpine %PRJ_DIR%\##advert_release
REM copy release\pinegame_promotion_info.pine %PRJ_DIR%\##advert_release

REM copy release\*.bpine %BIN%
REM copy release\pinegame_promotion_info.pine %BIN%
pause
echo *************Build Design************************
copy %GAMEDESIGN_FILE% %GAMEDESIGN_TOOL%\data\Objects.xls
REM pause
copy %GAMEDESIGN_CONFIG_FILE% %GAMEDESIGN_TOOL%\data
cd %GAMEDESIGN_TOOL%
call make.bat
call run.bat
copy release\*.h %OBJECT_RELEASE_DIR%
copy release\*.cpp %OBJECT_RELEASE_DIR%
echo *************************************************
cd %PRJ_DIR%
rem rd %TMPRES%

echo *** Building Data Complete ***

pause