@echo off
setlocal enabledelayedexpansion

REM 设置文件夹路径数组，使用分号分隔不同的路径
set "folders=.cph; build"
for %%i in (%folders%) do (
    if exist "%%i\" (
        pushd "%%i"
        del /q *
        popd
        echo %%i has been cleared.
    ) else (
        echo %%i does not exist.
    )
)

endlocal