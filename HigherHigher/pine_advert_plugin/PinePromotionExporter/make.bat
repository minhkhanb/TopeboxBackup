@echo off
cls
color 07
call config.bat


:MAKE_DIR
echo.
echo Deleted old file...
del /q %E_DIR_RELEASE%\*.class >nul 2>nul


:COMPLIE
echo.
echo Complie...
cd %E_DIR_SRC%
%E_JAVAC% -classpath %E_LIBRARY% -d %E_DIR_RELEASE% *.java
if ERRORLEVEL 1 goto ERROR	


goto END
:ERROR
	color 4E
	echo *************** HAVE ERROR ***************
	pause
	color 07


:END
	cd %E_PRJ_HOME%
