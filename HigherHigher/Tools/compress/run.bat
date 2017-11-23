@echo off
call config.bat

mkdir %E_TEMP_DIR%
copy %E_DIR_SRC%\Compressor_Exporter.h %E_TEMP_DIR%
copy %E_DIR_SRC%\Compressor_Exporter.cpp %E_TEMP_DIR%

cd %E_DIR_RELEASE%
%E_JAVA_HOME%\java -classpath .;%E_JXL% Pine7zCompressor %E_DIR_DATA%\Compressor_Exporter.conf %E_TEMP_DIR%\Compressor_Exporter.h %E_TEMP_DIR%\Compressor_Exporter.cpp %FILE_1% %FILE_2% 
cd %E_PRJ_HOME%
copy %E_DIR_RELEASE%\Compressor_Exporter.h %E_TEMP_DIR%
copy %E_DIR_RELEASE%\Compressor_Exporter.cpp %E_TEMP_DIR%
copy %E_DIR_RELEASE%\Compressor.bat %E_TEMP_DIR%
