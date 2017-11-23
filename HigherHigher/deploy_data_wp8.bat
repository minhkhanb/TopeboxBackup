@echo off
call config.bat
cd %PRJ_DIR%
if exist %RES_DES% goto DEPLOY
echo have not build data
echo please wait to build data first
call build_data.bat
cd %PRJ_DIR%

:DEPLOY
mkdir %BIN%\Res_WP8
cd %RES_DES%
copy sfx* %BIN%\Res_WP8
copy bgm* %BIN%\Res_WP8
REM copy %BIN%\*.pbg %BIN%\Res_WP8
REM copy %BIN%\adv_mask* %BIN%\Res_WP8
cd %BIN%\FileVersion

for %%i in (*.*) do (
	REM if '%%i'=='Ver_800x480' mkdir %BIN%\Res_WP8\%%i
	REM if '%%i'=='Ver_1280x768' mkdir %BIN%\Res_WP8\%%i
	REM echo "%%i"
	for /F %%a in (%%i) do (
		if '%%i'=='Ver_800x480' copy %RES_DES%\*%%a %BIN%\Res_WP8
		if '%%i'=='Ver_1280x768' copy %RES_DES%\*%%a %BIN%\Res_WP8
	)
)

pause