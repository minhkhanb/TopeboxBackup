@echo off
call config.bat
cd %PRJ_DIR%
if exist %RES_DES% goto DEPLOY
echo have not build data
echo please wait to build data first
call build_data.bat
cd %PRJ_DIR%

:DEPLOY
mkdir %BIN%\Res_Win8
cd %RES_DES%
REM copy %BIN%\*.pbg %BIN%\Res_Win8
REM copy %BIN%\adv_mask* %BIN%\Res_Win8
copy sfx* %BIN%\Res_Win8
copy bgm* %BIN%\Res_Win8
cd %BIN%\FileVersion

for %%i in (*.*) do (
	REM if '%%i'=='Ver_1366x768' mkdir %BIN%\Res_Win8\%%i
	REM echo "%%i"
	for /F %%a in (%%i) do (
		if '%%i'=='Ver_1366x768' copy %RES_DES%\*%%a %BIN%\Res_Win8
	)
)

pause