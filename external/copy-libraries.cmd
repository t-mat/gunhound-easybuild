xcopy /IEY .\opencv\build\x64\vc14\lib\*.lib ..\gxLib\platform\Windows\lib\openCV\
xcopy /IEY .\opencv\build\x64\vc14\bin\opencv_world*.dll ..\gunhound\
echo f | xcopy /y /f .\libogg-1.3.2\win32\vs2015\x64\Release\libogg_static.lib ..\gxLib\platform\Windows\lib\oggVorbis\r_libogg_static_x64.lib
echo f | xcopy /y /f .\libvorbis-1.3.5\win32\vs2015\x64\Release\libvorbisfile_static.lib ..\gxLib\platform\Windows\lib\oggVorbis\r_libvorbisfile_static_x64.lib
echo f | xcopy /y /f .\libvorbis-1.3.5\win32\vs2015\x64\Release\libvorbis_static.lib ..\gxLib\platform\Windows\lib\oggVorbis\r_libvorbis_static_x64.lib
expand DXSDK\Redist\Aug2009_d3dx9_42_x64.cab -f:d3dx9_42.dll ..\gunhound\
