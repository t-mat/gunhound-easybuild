@rem @echo off
pushd "%~dp0"

rd /s /q 7z1602
rd /s /q AMD64
rd /s /q DXSDK
rd /s /q freeglut
rd /s /q I386
rd /s /q IA64
rd /s /q libogg-1.3.2
rd /s /q libvorbis-1.3.5
rd /s /q opencv
rd /s /q PortableGit

del CURL.EXE

del 7-zip.chm
del license.txt
del readme.txt

popd
