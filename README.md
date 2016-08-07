## これはなに？

[ガルル研ライブラリにて公開されている「機装猟兵ガンハウンド」](http://garuru.co.jp/lib/index.php?HelloWorld)」を楽にビルドするものです。


## どうすればいいの？

- Visual Studio (VC++) 2015 をインストールする
- `git clone` を行う
- 以下のコマンドを実行

```
cd /d \path\to\your\gunhound-easybuild\
.\prepare.cmd
open .\gunhound-easybuild.sln
```

- ソリューションファイルが開くので、 `x64 - Release` をビルドする
- `gunhound\gunhound-easybuild_x64_Release.exe` が出力されるので、実行


## 既知の制限

- `x64 - Release` かつ DirectX9 出力のみしか設定していません
- 効果音が出ません (どうすると出るのか理解していません)


## ライセンス

[公開が行われているサイトに掲示されているライセンス](http://garuru.co.jp/lib/index.php?%E3%83%A9%E3%82%A4%E3%82%BB%E3%83%B3%E3%82%B9)に従います。

ただし、内容は策定中とのことですので、現時点で本リポジトリにはライセンスのコピーは配置しません。

