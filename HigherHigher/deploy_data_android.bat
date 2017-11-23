@echo off
call config.bat
cd %PRJ_DIR%
if exist %RES_DES% goto DEPLOY
echo have not build data
echo please wait to build data first
call build_data.bat
cd %PRJ_DIR%

:DEPLOY
echo *** Deploy to %ANDROID_RES% ***	
copy/b %RES_DES%\..\res_android\*.* %ANDROID_RES%\*.pack
copy/b %RES_DES%\..\res\GameDesign.bdsg %ANDROID_RES%\GameDesign.bdsg.pack
cd %PRJ_DIR%
rem rd %TMPRES%

echo *** Deploy Complete ***

pause