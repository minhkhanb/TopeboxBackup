echo off
rem *************************************
rem * Tool: Resize Sprite				*
rem * Author: GinHa.Do					*
rem * Contact: Nghia.do@colorbox.com.vn *
rem * Thanks for your feedback			*
rem *************************************


call config.bat


rem delete out dir if exist.
rd %OUTPUT_DIR% \s\y
md %OUTPUT_DIR%
copy %RESIZE_TOOLS% %INPUT_DIR%
pause
cd %INPUT_DIR%
for %%i in (*.pine) do (
	 java ResizePineAnim  -input %%i -suffix %SUFFIX% -scalex %SCALE_X% -scaley %SCALE_Y% -scalew %SCALE_W% -scaleh %SCALE_H%
	 dir *%SUFFIX%.*
	 copy  *%SUFFIX%.* %OUTPUT_DIR% 
	 del *%SUFFIX%.*
)
for %%i in (*.*) do (
del %%i
)
cd %ROOT_DIR%
pause
	