copy %1\*.* %2
for /D %%i in (%1\*.*) do (
	call copy_files.bat %%i %2
)