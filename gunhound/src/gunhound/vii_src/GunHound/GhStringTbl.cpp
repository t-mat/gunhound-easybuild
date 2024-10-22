//----------------------------------------------------------------------
//
// メッセージテーブル
//
//----------------------------------------------------------------------
#include <gunhound/gga2.h>
#include <gunhound/gga2x.h>
//----------------------------------------------------------------------
//ハウンド工場
//----------------------------------------------------------------------

char *FactoryMessage[]={
	"メインショットを変更します。",
	"サブショットを変更します。",
	"対空メインショットを変更します。",
	"対空サブショットを変更します。",

	"ベースカラーを変更します。",
	"アーマーカラーを変更します。",
	"設定を保存して終了します。",
	"アーマーを変更します。",
	"アーマーを変更します。",
};

char *WeaponNameTbl[]=
{
	"ガトリングガン（デフォルト）",	//実装
	"３ＷＡＹショット",				//実装

	"ハードブロウ（デフォルト）",	//実装
	"ブルータスクロー",				//実装
	"クラッチファング",				//--------------------------
	"アイアンファング",				//--------------------------
	"ハードボード",				//--------------------------

	"ジャイアントライフル（デフォルト）",	//実装
	"ショットガン",							//実装
	"ナパームスロワー",						//--------------------------

	"ロケットランチャー（デフォルト）",		//実装
	"リニアレール砲",							//実装
	"対空マシンガン",							//実装

	"グレネード",							//実装

	"？？？？？？？？？？",//13
};

//マヌーバキャノン
//グラビティブラスター/トマホーク
//ファング/コレダー/アンカー/ドロッパーズフォールディングガン

//char *MaterMainWeaponName[]=
//{
//	"ガトリングガン",
//	"３ＷＡＹショット",
//};
//
//char *MasterSubWeaponName[]=
//{
//	"ヘビーナックル",
//	"アームガトリング",
//	"パワークロー",
//	"トリプルクロー",
//};
//
//char *SlaveMainWeaponName[]=
//{
//	"バズーカ",
//	"ショットガン",
//};
//
//char *SlaveSubWeaponName[]=
//{
//	"マイクロミサイルｘ４",
//	"リニアキャノン",
//	"高速ロケット砲",
//};

char *ColorBodyName[]=
{
	"デフォルト",
	"ホワイト",
	"レッド",
	"グリーン",
	"ブルー",
	"イエロー",
	"ブラック",
	"ノーアーマー",
	NULL,
//	"ライトハウンド",
};

char *ColorArmorName[]=
{
	"デフォルト",
	"ホワイト",
	"レッド",
	"グリーン",
	"ブルー",
	"イエロー",
	"ブラック",
	"ノーアーマー",
	NULL,
};


char *EndingName[]={

	"「機装猟兵ガンハウンド製作スタッフ」",
	"",
	"プロデューサー",
	"　富野　裕樹",
	"",
	"ディレクター",
	"　玉城　正至",
	"",
	"グラフィックマネージャー",
	"　雲野　六郎",
	"",
	"ミュージックプロデューサー",
	"　山下　絹代",
	"",
	"キャラクターデザイン",
	"　雑君保プ",
	"",
	"シナリオ監修",
	"　加藤　直樹",
	"",
	"",
	"「Ｗｉｎｄｏｗｓ版開発スタッフ」",
	"",
	"プログラム",
	"　富野　裕樹",
	"",
	"プランニング",
	"　玉城　正至",
	"",
	"グラフィック",
	"　雲野　六郎",
	"　降野　政樹",
	"　宮永　幸一郎",
	"　西村　香織",
	"　松野　容子",
	"　大藪　善太",
	"",
	"シナリオ/ステージ構成",
	"　富野　裕樹",
	"",
	"楽曲",
	"　山下　絹代",
	"　新谷　友直",
	"",
	"効果音",
	"　今　工",
	"",
	"企画・営業",
	"　富野　裕樹",
	"",
	"広報",
	"　中村　豪",
	"　橋本　健志",
	"",
	"デバッグ",
	"株式会社デジタルハーツ",
	"",
	"ＳＰＥＣＩＡＬ　ＴＨＡＮＫＳ",
	"　市場　隆弘",
	"　廣岡　忍",
	"　道下　桃",
	"　湊　伸一",
	"　浅田　美香",
	"　浅田　一馬",
	"　浅田　雄馬",
	"",
	"協力",
	"株式会社アクアブルー",
	"株式会社メディアリュウム",
	"有限会社サウンドエイムス",
	"有限会社タウンファクトリー",
	"森井紙器工業株式会社",
	"",
	"製作進行",
	"　山本　桃世",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"@",
	"",
	"",
	"　　　　　　　　　　　　　　　　　　　　　　制作・著作：ガルルソフトウェア",
	NULL,
};

//----------------------------------------------------------------------
//ヘルプ
//----------------------------------------------------------------------

char *HelpMessage[]=
{
	"対艦攻略作戦を開始します",
	"操作方法など基本的な操作をマスターします。",
	"特定のミッションを選んでプレイします。",
	"コントローラーや環境の設定を行います。",
	"ゲームを終了します。",
	"その他",
	"その他",
	"その他",
	"その他",
	"その他",
	"その他",
	"その他",
};

char *HelpTutorialMessage[]=
{
	"移動、ジャンプ、ダッシュなど基本的な移動制御の操作練習です",
	"攻撃、シフトなどの基本的な攻撃演習です",
	"総合的なミッショントレーニングです",
};

char *HelpOptionMessage[]=
{
	"画面サイズ、ワイド設定などの環境設定を行います",
	"コントローラーの設定を行います。",
	"ハウンドのカスタマイズを行います。",
	"スタッフリストを参照します。",
	"最終ステージをクリアするとオープンされます",

	"このバージョンではご使用になれません",
};


char *HelpMissionMessage[]=
{
	"オロキド密林ステージ",
	"ロンボウ河川ステージ",
	"バンテラ山岳ステージ",
	"イゼル基地ステージ",
	"マレアシ荒野ステージ",
	"シナリオモード未クリアです",
	"その他ステージ",
	"その他",
	"その他",
	"その他",
};

char* sTitleTbl[]={
	"「プラント強襲」",
	"「輸送船護衛」",
	"「邦人救出」",
	"「ラボ襲撃」",
	"「戦闘艦迎撃」",
	"「補給線分断１」",
	"「補給線分断２」",
	"「補給線分断３」",
};


char* pGyaraTbl[]={
	"オロキド密林",
	"ロンボウ河川",
	"バンテラ山岳",
	"イゼル湾",
	"マレアシ荒野",
	"4000",
	"4000",
	"4000",
};

char* sCostTbl[]={
	"敵プラント内の重燃料を確保せよ",
	"輸送船、及び重燃料を護衛せよ",
	"重燃料採掘場で人命救助せよ",
	"敵地下ラボ内の重燃料を破棄せよ",
	"巨大戦闘艦の進行を阻止せよ",

	"0",
	"0",
	"0",
};

char* MissionJungle[]={
	"−プラント強襲作戦−",
	"　　　　　　　　",
	"敵軍を圧倒的優位に導いている移動",
	"砲台ドレッドトータスの燃料であるニュ",
	"ートロンポリマーを活発に精製している",
	"敵燃料プラントを襲撃する。",
	"プラント内にある精製済みニュートロン",
	"ポリマー燃料は敵陣へ渡る前に、なん",
	"としても阻止せよ。",
	"",
	"ミッションには重戦車グリフォンを使用、",
	"また、プラント内部への進行を阻む城",
	"門は特殊任務班が破壊する。",
	"",
	"パープル小隊はグリフォン隊の護衛、",
	"及びニュートロンポリマープラントへの",
	"強行突入を行い、ニュートロンポリマー",
	"燃料を確保せよ。",
	"",
	"以上。",
	NULL,
};

char* MissionRiver[]={
	"−重要燃料輸送作戦−",
	"",
	"敵プラントで差し押さえたニュートロン",
	"ポリマー燃料を対ドレッドトータス用",
	"光学兵器研究所へ輸送する。",
	"互いにニュートロンポリマープラントを",
	"失った今、残された原料、及び、精製",
	"済みニュートロンポリマー燃料は非常",
	"に貴重な存在となった。",
	"",
	"敵軍は、その燃料がこちらに渡り活",
	"用されることを危惧し、その運搬ルー",
	"ト近くへの攻撃部隊の配備を厚くして",
	"いる。",
	"",
	"安全なルートは存在しないため、この",
	"輸送作戦は複数のルートを用いて同時",
	"に行う。パープル小隊は、ロンボウ河",
	"川の遡行により輸送船による最短ルート",
	"での運搬を成功させよ。",
	"",
	"以上。",
	NULL,
};

char* MissionMountain[]={
	"−エンジニア救助−",
	"",
	"ニュートロンポリマーの原料の採掘を",
	"行っていたバンテラ山岳の採掘場が",
	"襲撃にあい、完全に敵の手に落ちた。",
	"この採掘場襲撃時に逃げ遅れた統合",
	"軍加盟国のエンジニアの救出を行う。",
	""
	"採掘場内は既に敵に占拠されており",
	"また、北東側に敵部隊の増援も確認",
	"しているため現状での採掘場の奪還",
	"は不能とみなし、救出のみを行うもの",
	"とする。",
	"パープル小隊は採掘場ふもとのバンゴ",
	"村のエンジニア、工員を輸送ヘリへ誘",
	"導の後、ただちに撤退せよ。",
	"",
	"以上。",
	NULL,
};

char* MissionKichi[]={
	"−新燃料研究阻止作戦−",
	"",
	"敵のニュートロンポリマーエネルギー",
	"を利用した超兵器研究施設の襲撃を",
	"行う。",
	"施設内にはトータスの修理用、補給に",
	"使用される、研究用のニュートロンポリ",
	"マーエネルギー増幅器が複数稼働して",
	"いる",
	"パープル小隊はこの研究施設内のニ",
	"ュートロンポリマーエネルギー増幅器",
	"を全て破壊し、研究施設の機能停止を",
	"もってドレッドトータス対策の一環とせ",
	"よ。",
	"",
	"潜入に際しては強襲用ポッドを用いて",
	"施設周辺の海中に敷設された輸送用",
	"ダクトに侵入する。",
	"脱出に際しては、ラボ最深部にある",
	"ニュートロンポリマー原料の遠心分離",
	"装置であるメインオーガナイザーを",
	"破壊し、脱出口を確保せよ。",
	"",
	"以上。",
	NULL,
};

char* MissionExtra1[]={
	"−最終防衛線死守−",
	"",
	"ニュートロンポリマー砲を主砲に備えた",
	"移動砲台ドレッドトータスが統合軍の",
	"警戒領域に進入。",
	"我が軍はこれを殲滅するため、グリフ",
	"ォン隊を含む迎撃チームを組織したが、",
	"圧倒的な火力により有効射程,距離内",
	"への接近が不可能であった。",
	""
	"遠距離からのレーザー兵器による攻",
	"撃もレーザーコーティングされたボディ",
	"を貫通させることは難しく、もはや",
	"機動性に優れた機装猟兵による近接",
	"攻撃に頼るしか方法がない状態となっ",
	"ている。",
	"ドレッドトータスの表面を覆うレーザー",
	"コーティングされたボディを７０％以上",
	"露出させることで、我が軍のニュート",
	"ロンポリマー砲は有効なダメージを与",
	"えることが可能になる。",
	""
	"敵の有効射程距離内に本部が捉えら",
	"れれる前に進行を阻止せよ。",
	"",
	"以上。",
	NULL,
};

char* TutorialCommand01[]={
	"【ハウンドの基本操作】",
	"",
	"　　　←→　・・・　移動",
	"",
	"　　　Ｚ　　・・・　ジャンプ",
	"　　　　　　　　　　※押しっぱなしでブースト",
	"",
	"　　　Ｘ　　・・・　ダッシュ",
	"　　　　　　　　　　※→・→でも可能 ",
	NULL,
};

char* TutorialCommand02[]={
	"【ハウンドの攻撃操作】",
	"",
	"　　　Ａ　・・・メインショット",
	"　　　※ガトリングガン/ミサイルライフル",
	"",
	"　　　Ｓ　・・・サブショット",
	"　　　※ハードブロウ/ロケットランチャー",
	"",
	"ＳＨＩＦＴ・・・対地／対空兵装切り替え",
	"",
	NULL,
};

char* TutorialCommand03[]={
	"【ハウンドの攻撃操作】",
	"",
	"　　　Ａ　・・・メインショット",
	"　　　※ガトリングガン/ミサイルライフル",
	"",
	"　　　Ｓ　・・・サブショット",
	"　　　※ハードブロウ/ロケットランチャー",
	"",
	"ＳＨＩＦＴ・・・対地／対空兵装切り替え",
	"",
	NULL,
};

//-----------------------------------
//メインメニュー項目
//-----------------------------------
char *MainMenuNameTbl[]={
"  Game Start",
"  Tutorial",
"  Mission Mode",
"  Option",
"  Debug Mode",
"  Exit",
};

//-----------------------------------
//キャプション
//-----------------------------------
char *CaptionNameTbl[]={
	"TUTORIAL",
	"MISSION" ,
	"OPTION"  ,
	"FACTORY" ,
};


//-----------------------------------
//レッスン項目
//-----------------------------------
char *LessonMenuNameTbl[]={
	"Lesson 1",
	"Lesson 2",
	"Lesson 3",
};

//-----------------------------------
//オプション項目
//-----------------------------------
char *OptionMenuNameTbl[]={
	"CONFIG",
	"CONTROLLER",
	"HOUND FACTORY",
	"STAFF ROLL",
	"??????????",
};

//-----------------------------------
//ミッションネーム
//-----------------------------------
char *MissionNameTbl[]={
	"Mission 1",
	"Mission 2",
	"Mission 3",
	"Mission 4",
	"Final Mission",
	"?????????",
};


char *MainMenuEtcNameTbl[]={
	"TOTAL SCORE",
};

char *MissionInfoJungle[]={
	"",//MISSION 1",
	"",//MISSION 1",
	"",//MISSION 1",
	"",//MISSION 1",
	"",//MISSION 1",
	"",//MISSION 1",
	"",//MISSION 1",
	"",//MISSION 1",
};
char *MissionInfoRiver[]={
	"",//MISSION 2",
	"",//MISSION 2",
	"",//MISSION 2",
	"",//MISSION 2",
	"",//MISSION 2",
	"",//MISSION 2",
	"",//MISSION 2",
	"",//MISSION 2",
};
char *MissionInfoMountain[]={
	"",//MISSION 3",
	"",//MISSION 3",
	"",//MISSION 3",
	"",//MISSION 3",
	"",//MISSION 3",
	"",//MISSION 3",
	"",//MISSION 3",
	"",//MISSION 3",
};
char *MissionInfoBase[]={
	"",//MISSION 4",
	"",//MISSION 4",
	"",//MISSION 4",
	"",//MISSION 4",
	"",//MISSION 4",
	"",//MISSION 4",
	"",//MISSION 4",
	"",//MISSION 4",
};
char *MissionInfoExtra[]={
	"",//MISSION 5",
	"",//MISSION 5",
	"",//MISSION 5",
	"",//MISSION 5",
	"",//MISSION 5",
	"",//MISSION 5",
	"",//MISSION 5",
	"",//MISSION 5",
};

char *HiddenOpenMessageTbl[]={
	"軍曹に昇格しました",
	"上級曹長に昇格しました",
	"少尉に昇格しました",
	"中尉に昇格しました",
	"大尉に昇格しました",
	"ステージセレクト機能が追加されました",

	"少佐に昇格しました",
	"中佐に昇格しました",
	"大佐に昇格しました",
	"ダイバーアンテナが追加されました",
	"新しい武器が追加されました",
};

char *WeaponSupply[]={
	"が支給されました",
	"> HOUND FACTORYで装備できます。",
	"シナリオモードでステージが選択可能になります",
	"自機にアクセサリとして装備されます",
};


//ハウンドファクトリー
char *CustomHoundString[]={
	"兵装" ,
	"　メインショット",
	"　サブショット",
	"　対空メイン",
	"　対空サブ",
	"装甲",
	"　ベース",
	"　アーマー",
	"決定",
};


//エンディング
char *EndingString[]={
	"THE END",
};

char *GameOverString[]={
	"GAME OVER",
	"CONTINUE",
	"C",
	"SCORE",
	"OUT OF RANGE",
};

char *StrMissionClear[]={
	"MISSION CLEAR",
};

char *StrShortMessage[]={
	"WARNING!!BODY SHIELD IS BROKEN",
	"ALERT!!BODY ARMOR IS BROKEN",
	"ALERT!!ARM ARMOR IS BROKEN",
	"WARNING!!LEG SHIELD IS BROKEN",
	"ALERT!!LEG ARMOR IS BROKEN",
	"WARNING!!LEG SHIELD IS BROKEN",
	"ALERT!!LEG ARMOR IS BROKEN",
	"BOOST POWER IS EMPTY...",
	"WARNING.....HIGH-SPEED ARMS APROACHES.",
};
