@REM 注意文件换行 CRLF

@echo off
setlocal enabledelayedexpansion

REM 白名单目录（要保留的）
set KEEP=src .vscode

for /d %%D in (*) do (
    set DELETE=1
    for %%K in (%KEEP%) do (
        if /I "%%D"=="%%K" set DELETE=0
    )
    if !DELETE! EQU 1 (
        rmdir /s /q "%%D"
    )
)
echo Dir has been cleared.

REM 删除当前目录下的所有 .go 文件
set "fileType=*.go"
if exist "%fileType%" (
    del /f /q %fileType%
)
echo Files .go has been cleared.

REM 删除当前目录下的所有 .exe 文件
set "fileType=*.exe"
if exist "%fileType%" (
    del /f /q %fileType%
)
echo Files .exe has been cleared.

endlocal
