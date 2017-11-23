if EXIST "%J2SE_DIR%\javac.exe" (
	set E_JAVA_HOME=%JAVA_SDK%
	echo abc %JAVA_SDK%
) else (
	set E_JAVA_HOME=%JAVA_SDK%
)

set E_PRJ_HOME=%cd%
set E_JAVAC=%E_JAVA_HOME%\javac
set E_DIR_RELEASE=%E_PRJ_HOME%\Release
set E_DIR_SRC=%E_PRJ_HOME%\Src
set E_DIR_DATA=%E_PRJ_HOME%\data
set E_JXL=%E_PRJ_HOME%\libs\jxl.jar;
set E_LIBRARY=%E_JAVA_HOME%\..\jre\lib\rt.jar;%E_JXL%
set E_TEMP_DIR=%E_PRJ_HOME%\#temp

set FILE_1=%E_DIR_DATA%\overall_design.xls 
set FILE_2=%E_DIR_DATA%\overall_design.xls 


