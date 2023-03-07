@echo off

set "isBuildFirst=y"
set "SCRIPTFILE=CreateShortcut.vbs"

:parseargs
IF "%~1" == "" GOTO endparseargs
IF "%~1" == "-c" SET "doCodeCompile=y"
IF "%~1" == "-e" SET "doEraseBuildDir=y"
IF "%~1" == "-nc" SET "doCodeCompile=n"
IF "%~1" == "-ne" SET "doEraseBuildDir=n"
SHIFT
GOTO parseargs
:endparseargs


IF NOT DEFINED doCodeCompile (
:confirmCodeCompile	
	set /p "doCodeCompile=Выполнить компиляцию кода? [Y/n]: "
	if /i "%doCodeCompile%"=="y" goto :confirmCodeCompileEnd
	if /i "%doCodeCompile%"=="n" goto :confirmCodeCompileEnd
	goto confirmCodeCompile
)
:confirmCodeCompileEnd


IF EXIST build (
	SET "isBuildFirst=n"
	IF NOT DEFINED doEraseBuildDir (
	:confirmBuildErase
		set /p "doEraseBuildDir=Выполнить чистую сборку? [Y/n]: "
		IF /i "%doEraseBuildDir%" == "y" (
			RMDIR /s /q build
			ECHO * build dir was removed!
			goto confirmBuildEraseEnd
		)
		if /i "%doEraseBuildDir%" == "n" goto confirmBuildEraseEnd
		goto confirmBuildErase
	)	
	IF DEFINED doEraseBuildDir (
		IF "%doEraseBuildDir%" == "y" (
			RMDIR /s /q build
			ECHO * build dir was removed!
		)
	)
)
:confirmBuildEraseEnd

cmake -S . -B build

IF "%doCodeCompile%" == "y" (
	cmake --build build
)

IF "y" == "y" (
	echo * Creating solution shortcut . . .
	
	echo Set oWS = WScript.CreateObject^("WScript.Shell"^) >> %SCRIPTFILE%
	echo sLinkFile = oWS.ExpandEnvironmentStrings^("2djourney.sln - Shortcut.lnk"^) >> %SCRIPTFILE%
	echo Set oLink = oWS.CreateShortcut^(sLinkFile^) >> %SCRIPTFILE% 
	echo oLink.TargetPath = oWS.ExpandEnvironmentStrings^("%~dp0\build\2djourney.sln"^) >> %SCRIPTFILE%
	echo oLink.Save >> %SCRIPTFILE%
	
	cscript /nologo %SCRIPTFILE%
	del /f /q %SCRIPTFILE%
)

SET "doEraseBuildDir="
SET "doCodeCompile="

if %0 == "%~0" pause
