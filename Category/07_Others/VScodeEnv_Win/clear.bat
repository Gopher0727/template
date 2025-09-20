REM 注意文件换行 CRLF

@echo off
setlocal enabledelayedexpansion

REM 设置文件夹路径数组，使用分号分隔不同的路径
set "folders=.cph; build"
for %%i in (%folders%) do (
    if exist "%%i\" (
        pushd "%%i"
        del /q *
        popd
        echo Dir %%i has been cleared.
    ) else (
        echo Dir %%i does not exist.
    )
)

REM 删除当前目录下的所有 .cpp 文件
set "fileType=*.cpp"
if exist "%fileType%" (
    del /f /q %fileType%
)
echo Files .cpp has been cleared.

REM 删除当前目录下的所有 .py 文件
set "fileType=*.py"
if exist "%fileType%" (
    del /f /q %fileType%
)
echo Files .py has been cleared.

REM 删除当前目录下的所有 .exe 文件
set "fileType=*.exe"
if exist "%fileType%" (
    del /f /q %fileType%
)
echo Files .exe has been cleared.

endlocal
