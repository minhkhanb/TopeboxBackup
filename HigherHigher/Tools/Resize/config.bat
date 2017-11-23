echo off
rem *************************************
rem * Tool: Resize 						*
rem * Author: GinHa.Do					*
rem * Contact: 	do.ginha@yahoo.com		*
rem * Thanks for your feedback			*
rem *************************************



rem config build system.
set SIGN=port
set ROOT_DIR=%cd%
set INPUT_DIR=%ROOT_DIR%\input
set OUTPUT_DIR=%ROOT_DIR%\%SIGN%_output
set RESIZE_TOOLS=%ROOT_DIR%\ResizePineAnim.class

set SUFFIX=_200
set SCALE_X=0.9
set SCALE_Y=0.9
set SCALE_W=0.9
set SCALE_H=0.9

rem ipad
REM set SCALE_X=1.07
REM set SCALE_Y=1.2

REM set SCALE_X=2.0
REM set SCALE_Y=2.0

REM set SCALE_X=1.0
REM set SCALE_Y=1.0
