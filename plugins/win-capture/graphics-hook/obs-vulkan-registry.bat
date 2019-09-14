@echo off

if not exist %~dp0graphics-hook64.dll goto dllnotfound

:: BatchGotAdmin
:-------------------------------------
REM  --> Check for permissions
    IF "%PROCESSOR_ARCHITECTURE%" EQU "amd64" (
>nul 2>&1 "%SYSTEMROOT%\SysWOW64\cacls.exe" "%SYSTEMROOT%\SysWOW64\config\system"
) ELSE (
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
)

REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    set params= %*
    echo UAC.ShellExecute "cmd.exe", "/c ""%~s0"" %params:"=""%", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    pushd "%CD%"
    CD /D "%~dp0"
:-------------------------------------- 


REG QUERY HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\Vulkan\ImplicitLayers /v %~dp0obs-vulkan64.json
if '%errorlevel%' NEQ '0' (
    echo no key found, adding...
	goto enable-obs-vulkan
) else (
    echo Found existing key, removing...
    goto disable-obs-vulkan
)

:enable-obs-vulkan
	REG ADD HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\Vulkan\ImplicitLayers /v %~dp0obs-vulkan64.json /t REG_DWORD /d 00000000
	REG ADD HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Khronos\Vulkan\ImplicitLayers /v %~dp0obs-vulkan.json /t REG_DWORD /d 00000000
	goto end

:disable-obs-vulkan
	REG DELETE HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\Vulkan\ImplicitLayers /v %~dp0obs-vulkan64.json /f
	REG DELETE HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Khronos\Vulkan\ImplicitLayers /v %~dp0obs-vulkan.json /f
	goto end

:dllnotfound	
	echo dll not found, you should run this from build output folder
:end
pause