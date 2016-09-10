@echo off
pushd "%~dp0"

mkdir bootstrap
pushd bootstrap

if not exist curl-7.50.1.cab powershell -command "(New-Object Net.WebClient).DownloadFile('http://home.arcor.de/skanthak/download/curl-7.50.1.cab','curl-7.50.1.cab')"
expand curl-7.50.1.cab -f:curl.exe .
copy AMD64\curl.exe .

if not exist 7za920.zip curl -LO http://www.7-zip.org/a/7za920.zip
powershell -command "$s=new-object -c shell.application;$n=$s.namespace('%cd%\7za920.zip');foreach($f in $n.items()){$s.namespace('%cd%').copyhere($f,0x14)}"

if not exist MinGit-2.10.0-64-bit.zip curl -LO https://github.com/git-for-windows/git/releases/download/v2.10.0.windows.1/MinGit-2.10.0-64-bit.zip
.\7za x -y MinGit-2.10.0-64-bit.zip

popd

echo @"%%~dp0bootstrap\cmd\git.exe" %%* >git.cmd

popd
