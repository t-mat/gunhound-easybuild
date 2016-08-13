## これはなに？

[ガルル研ライブラリにて公開されている「機装猟兵ガンハウンド」](http://garuru.co.jp/lib/index.php?HelloWorld)を楽にビルドするものです。

VC++2015 以外のビルドに必要なソフトについては、インストールは行わず、ファイルの展開だけを行います。これによってシステムへの影響が少ないようにしています。


## どうすればいいの？

- Visual Studio (VC++) 2015 をインストールする
- 以下のコマンドを実行

```
cd /d C:\
git clone https://github.com/t-mat/gunhound-easybuild.git
cd gunhound-easybuild
.\prepare.cmd
msbuild .\gunhound-easybuild.sln /p:Configuration=Release /p:Platform=x64
start /D .\gunhound gunhound-easybuild_x64_Release.exe
```


## Visual Studio 2015 での操作

上記「どうすればいいの？」の操作を行ったあと、
Visual Studio 2015 でソリューションファイル `C:\gunhound-easybuild\gunhound-easybuild.sln` を開くことで、
Visual Studio 2015 を用いた各種操作を行うことができます。


## git subtree の更新

Bitbucket の `garuruLabs` アカウント下にある [gxlib](https://bitbucket.org/garuruLabs/gxlib) および [gunhound](https://bitbucket.org/garuruLabs/gunhound) リポジトリから
コードを取得/更新する際は以下のように行います

```
cd /d C:\gunhound-easybuild
git subtree pull --prefix gxlib https://bitbucket.org/garuruLabs/gxlib.git master --squash
git subtree pull --prefix gunhound https://bitbucket.org/garuruLabs/gunhound.git master --squash
```


## 既知の制限

- `x64 - Release` もしくは `x64 - Debug` かつ DirectX9 出力のみしか設定していません


## ライセンス

[公開が行われているサイトに掲示されているライセンス](http://garuru.co.jp/lib/index.php?%E3%83%A9%E3%82%A4%E3%82%BB%E3%83%B3%E3%82%B9)に従います。

内容が複雑であり、また策定中とのことですので、現時点 (2016年 8月 13日) において本リポジトリにはライセンスのコピーは配置しません。
