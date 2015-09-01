	@echo off

set ENTRY_POINT= c:\IMGL\code\win32\platform_layer.cpp

set WARNINGS=-WX -W4 -wd4505 -wd4201 -wd4100 -wd4189
set OPTIMSATIONS=-Od -Oi
set EXCEPTIONS=-EHsc- -EHa-
set DEBUGGING=-FC -Fmwin32_source_map.map -Zi
set LIBS=user32.lib Gdi32.lib Winmm.lib opengl32.lib
set SWITCHES=-DBUILD_INTERNAL=1 -DBUILD_SLOW=1 -DBUILD_WIN32=1I 

set COMPILE_FLAGS=-MT -nologo %WARNINGS% %OPTIMSATIONS% %DEBUGGING% -Gm -GR %SWITCHES% %EXCEPTIONS%
set LINK_FLAGS=-incremental:no -opt:ref %LIBS%

pushd ..\build

	REM 32bit build
	REM cl %COMPILE_FLAGS% %ENTRY_POINT% /link %LINK_FLAGS% -subsystem:windows,5.10

	REM 64bit build
	cl %COMPILE_FLAGS% %ENTRY_POINT% /link %LINK_FLAGS%

popd


REM BUILD_SLOW: 1 = asserts and such. 0 = no slow code allowed
REM BUILD_INTERNAL: 1 = base virtual memory etc
REM BUILD_WIN32: 1 = windows build
REM -nologo: gets rid of made by microsoft
REM -WX: warnings as errors
REM -W4: warning level 4
REM -wdXXXX turn off specific warning
REM -Oi: turns on CPU intrinsic optimisations
REM -O2 optimisation level
REM -fp:fast fast floating point (instead of accurate)
REM -MT include the C runtime library with executable (default on the command line)
REM -Gm turn off incremental builds
REM -GR Enables run-time type information (RTTI).
REM -EH catches exceptions
REM -wd4505 turns off warning for unsused functions
REM -FC Display full path of source code files passed to cl.exe in diagnostic text.
REM -Z7 Generates C 7.0–compatible debugging information.
REM -Zi Generates complete debugging information.
REM -Fm Creates a mapfile.
