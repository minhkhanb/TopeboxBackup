@echo off
call config.bat
cd %PRJ_DIR%
if exist %RES_DES% goto DEPLOY
echo have not build data
echo please wait to build data first
call build_data.bat
cd %PRJ_DIR%

:DEPLOY
mkdir %BIN%\Res_iOS
cd %RES_DES%
copy sfx* %BIN%\Res_iOS
copy bgm* %BIN%\Res_iOS
REM copy %BIN%\*.pbg %BIN%\Res_iOS
REM copy %BIN%\adv_mask* %BIN%\Res_iOS
cd %BIN%\FileVersion

for %%i in (*.*) do (
	REM if '%%i'=='Ver_960x640' mkdir %BIN%\Res_iOS\%%i
	REM if '%%i'=='Ver_1024x768' mkdir %BIN%\Res_iOS\%%i
	REM if '%%i'=='Ver_1136x640' mkdir %BIN%\Res_iOS\%%i
	REM echo "%%i"
	for /F %%a in (%%i) do (
		if '%%i'=='Ver_960x640' copy %RES_DES%\*%%a %BIN%\Res_iOS
		if '%%i'=='Ver_1024x768' copy %RES_DES%\*%%a %BIN%\Res_iOS
		if '%%i'=='Ver_1136x640' copy %RES_DES%\*%%a %BIN%\Res_iOS
	)
)

pause
