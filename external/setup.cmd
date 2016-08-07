@rem @echo off
pushd "%~dp0"

call .\remove-all-directories.cmd

if not exist curl-7.50.1.cab powershell -command "(New-Object Net.WebClient).DownloadFile('http://home.arcor.de/skanthak/download/curl-7.50.1.cab','curl-7.50.1.cab')"
expand curl-7.50.1.cab -f:curl.exe .
copy AMD64\curl.exe .

if not exist 7za920.zip curl -LO http://www.7-zip.org/a/7za920.zip
powershell -command "$s=new-object -c shell.application;$n=$s.namespace('%cd%\7za920.zip');foreach($f in $n.items()){$s.namespace('%cd%').copyhere($f,0x14)}"

if not exist 7z1602-x64.msi curl -LO http://www.7-zip.org/a/7z1602-x64.msi
.\7za x -y 7z1602-x64.msi -o7z1602
copy 7z1602\_7z.exe .\7z.exe

if not exist libogg-1.3.2.zip .\curl -LO http://downloads.xiph.org/releases/ogg/libogg-1.3.2.zip
if not exist libvorbis-1.3.5.zip .\curl -LO http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.5.zip
if not exist opencv-3.1.0.exe .\curl -LO https://github.com/opencv/opencv/releases/download/3.1.0/opencv-3.1.0.exe
if not exist DXSDK_Aug09.exe .\curl -LO https://download.microsoft.com/download/4/C/F/4CFED5F5-B11C-4159-9ADC-E133B7E42E5C/DXSDK_Aug09.exe
if not exist PortableGit-2.9.2-64-bit.7z.exe .\curl -LO https://github.com/git-for-windows/git/releases/download/v2.9.2.windows.1/PortableGit-2.9.2-64-bit.7z.exe
if not exist freeglut-MSVC.zip .\curl -LO http://files.transmissionzero.co.uk/software/development/GLUT/freeglut-MSVC.zip

.\7z x -y libogg-1.3.2.zip
.\7z x -y libvorbis-1.3.5.zip
.\7z x -y opencv-3.1.0.exe
.\7z x -y DXSDK_Aug09.exe
.\7z x -y PortableGit-2.9.2-64-bit.7z.exe -oPortableGit
.\7z x -y freeglut-MSVC.zip

call .\build-oggvorbis.cmd
call .\copy-libraries.cmd
