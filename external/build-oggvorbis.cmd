@rem @echo off
pushd "%~dp0"

xcopy /IE .\libogg-1.3.2\include\ogg .\libvorbis-1.3.5\include\ogg

call "%VS140COMNTOOLS%"vsvars32.bat
MSBuild .\libogg-1.3.2\win32\vs2010\libogg_static.sln /tv:14.0 /p:Configuration=Release /p:Platform=x64
MSBuild .\libvorbis-1.3.5\win32\vs2010\vorbis_static.sln /tv:14.0 /t:libvorbisfile /p:Configuration=Release /p:Platform=x64
