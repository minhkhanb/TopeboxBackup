call config.bat
@echo on
set OUT_DIR=%ROOT_DIR%\out
rd %OUT_DIR% \s\y\q
MKDIR %OUT_DIR%
set path=%path%;%ROOT_DIR%;

cd %OUTPUT_DIR%
for %%i in (*.*) do (
	resize_utils rename -input=%%i -output_dir=%OUT_DIR% -reg_find="([\w\d_]+)(_100)([\w\d_\.]+)" -reg_replace="$1$3"
)
pause
cd %OUT_DIR%
for %%i in (*.pine) do (
	resize_utils modify -input=%%i -output=%%i -reg_find="([\w\d_]+)(_@\!\!)(\.png)" -reg_replace="$1$3"
)
pause
cd %ROOT_DIR%