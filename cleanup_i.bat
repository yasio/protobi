@echo off

set sln_name=%1

if not defined sln_name goto :eof

rmdir /q /s ipch

del /q /f *.sdf
del /q /f *.ncb
del /q /f *.db
del /q /f /a *.suo
del /q /f .\Debug\*.exe
del /q /f .\Debug\*.ilk
del /q /f .\Debug\*.pdb
del /q /f .\Release\*.pdb
rem del /q /f .\Release\*.exe
del /q /f .\%sln_name%\*.user
del /q /f /a .\p%sln_name%\*.suo
del /q /f .\%sln_name%\Debug\*.*
del /q /f .\%sln_name%\Release\*.*
ping /n 3 127.0.1 >nul

goto :eof

