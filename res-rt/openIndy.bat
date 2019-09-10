@echo on

rem set `QT_DIR` to your Qt install path
set QT_DIR=c:\Qt\Qt5.5.1

@rem release
@rem SET PATH=%QT_DIR%\5.5\mingw492_32\bin;%QT_DIR%\Tools\mingw492_32\bin;%PATH%

@rem debug
SET PATH=%QT_DIR%\5.5\mingw492_32\bin;%QT_DIR%\Tools\mingw492_32\bin;%PATH%

openIndy.exe

