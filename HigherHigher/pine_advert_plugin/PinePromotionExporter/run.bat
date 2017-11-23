@echo off
call config.bat
rd /S /Q ##temp_res
mkdir ##temp_res
call copy_files.bat ..\gfx ##temp_res
cd ##temp_res
for %%i in (*.pine) do (
	..\tools\PineSpriteEditor\PineSpriteEditor.exe 	-i %%i		-o %%~ni.bpine  QUALITY_TRUE_COLOR	EXPORT_AS_3D_TEXTURE	EXPORT_FMODULE_OPACITY USE_MULTI_MODULE_TYPE EXPORT_PALETTE_COLOR
)
copy *.bpine ..\release

cd %E_DIR_RELEASE%
REM %JAVA_HOME%\bin\java -classpath .;%JXL% ExportDefines Defines.xls
copy ..\..\PineGamePromotion.xls %E_DIR_DATA%\Objects.xls
%E_JAVA_HOME%\java -classpath .;%E_JXL% ExportPineGamePromotion %E_DIR_DATA%\Objects.xls
copy pinegame_promotion_info.pine ..\..\gfx\%APPLE_ID%
cd %E_PRJ_HOME%