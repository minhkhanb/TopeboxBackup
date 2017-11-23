@echo off
call config.bat

mkdir %E_TEMP_DIR%
copy %E_DIR_SRC%\GDS_Exporter.h %E_TEMP_DIR%
copy %E_DIR_SRC%\GDS_Exporter.cpp %E_TEMP_DIR%

cd %E_DIR_RELEASE%
%E_JAVA_HOME%\java -classpath .;%E_JXL% GDS_Exporter %E_DIR_DATA%\GDS_Exporter.conf %E_TEMP_DIR%\GDS_Exporter.h %E_TEMP_DIR%\GDS_Exporter.cpp %FILE_1% %FILE_2% 
cd %E_PRJ_HOME%
copy %E_DIR_RELEASE%\GDS_Exporter.h %E_TEMP_DIR%
copy %E_DIR_RELEASE%\GDS_Exporter.cpp %E_TEMP_DIR%
