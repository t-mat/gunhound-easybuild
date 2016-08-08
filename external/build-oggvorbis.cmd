@rem @echo off
pushd "%~dp0"

xcopy /IEY .\libogg-1.3.2\include\ogg .\libvorbis-1.3.5\include\ogg
xcopy /IEY .\VS2015\libogg-1.3.2 .\libogg-1.3.2
xcopy /IEY .\VS2015\libvorbis-1.3.5 .\libvorbis-1.3.5

call "%VS140COMNTOOLS%"vsvars32.bat
MSBuild .\libogg-1.3.2\win32\vs2015\libogg_static.sln /p:Configuration=Release /p:Platform=x64
MSBuild .\libvorbis-1.3.5\win32\vs2015\vorbis_static.sln /t:libvorbisfile /p:Configuration=Release /p:Platform=x64

popd
