--require "gxLib"

-- //----------------------------------------------
-- //ＰＡＬＭ　ＳＨＯＯＴＥＲ
-- //
-- //----------------------------------------------

-- //----------------------------------------------
-- //インクルード
-- //----------------------------------------------

-- //----------------------------------------------
-- //マクロ
-- //----------------------------------------------

ORIGIN_ONE  = (1)		--配列の添字が１スタートの場合は１

enObjMax     = (90)				-- //マックスオブジェクト数
enMyShipIndex= (ORIGIN_ONE)		-- //バスターのオブジェクトナンバー
MAX_MY_SHOT  = (25)				-- //弾数上限
MAX_EN_SHOT  = (35)				-- //敵の弾数上限
MAX_ENEMY    = (15)				-- //敵数上限
enTpgSpr     = (0)

PRIO_GROUND	= (5)
PRIO_JIKI	= (10)
PRIO_TEKI	= (15)
PRIO_TAMA	= (20)
PRIO_EFF	= (25)
PRIO_SYS	= (30)

ScrollY = 0;

-- @@@ void visorizerInit(void);
-- @@@ static int _GetAbs(int n);					-- //絶対値を求める
-- @@@ static int _GetRand(void);					-- //ランダムを作る
-- @@@ static int GetStylus(int *x, int *y);
-- @@@ static void _PadControl(void);
-- @@@ 
GBATR_ALPHA_PLUS = ATR_DFLT;

-- //----------------------------------------------
-- //型宣言
-- //----------------------------------------------
-- //ソフトウェアスプライト定義用構造体

-- //仮想パッドを扱う構造体
StGamePad = {
	psh=0;	-- //押しっぱなし
	trg=0;	-- //ワンプッシュ
	x,y=0,0;
	keydown=0;
};

-- //オブジェクト構造体

StObj =
{
	category;		-- //生存フラグ
	x,y;		-- //Ｘ、Ｙ
	vx,vy;		-- //増分値など
	eneType;	-- //敵の種類
	hp;			-- //耐久力

	-- //汎用の共用体

--	union FREE {
		b = { 0,0,0,0,0,0,0,0 };
		w = { 0,0,0,0 };
		l = { 0,0 };
--	} free ;

};


-- //ゲーム全般を扱う構造体

StGame = {
	x = 0;
	scene = 0;
	Score = 0;		-- //スコア

 -- //オブジェ数記録用

	my_shot_num = 0;	-- //弾数
	en_shot_num = 0;	-- //敵弾数
	enemy_num   = 0;	-- //敵数
	bb_remain   = 0;
	timer       = 0;
	level		= 0;
	option_x    = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; -- //35
	option_y    = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; -- //35
	option_pos  = 0;
	bgm_flag 	 = 0;
	r = 0;
	g = 0;
	b = 0;
	VsyncTimer = 0;
};

 
 
-- //----------------------------------------------
-- //グローバル変数
-- //----------------------------------------------
SprObj = {};					-- //オブジェクト管理用配列		--@@@ static StObj SprObj[ enObjMax ];		
g_GameSystem = { StGame };		-- //ゲーム管理用				-- @@@ static StGame g_GameSystem;		
g_GamePad  = { StGamePad };		-- //パッド入力管理用
g_sGameSeq = 0;
g_HighScore,Score_temp = 0,0;

-- //--------------------------------
-- //関数プロトタイプ
-- //--------------------------------

--[[
static void visorControl(void);
static void SprObj_control(void);
static void game_result(void);

static void enemy_set(void);
static void enemy_set_lv0(void);
static void enemy_set_lv1(void);
static void enemy_set_lv2(void);

static void shot_init(int n,int x,int y);
static void shot_main(int n);

static void enemy_main(int n);
static void enemy01_init();
static void enemy01_main(int n);
static void enemy02_init();
static void enemy02_main(int n);
static void enemy03_init();
static void enemy03_main(int n);
static void enemy04_init();
static void enemy04_main(int n);
static void enemy05_init();
static void enemy05_main(int n);
static void enemy06_init(int n);
static void enemy06_main(int n);

static void ene_shot_init(int s,int eneType);
static void ene_shot_main(int s);
static void ene_shot_normal(int n);
static void ene_shot_laser01(int n);
static void ene_shot_bigblitz(int n);

static void bomb01_init(int s,int x, int y,int eneType);
static void bomb01_main(int n);
static void eff_star_init(int s,int x, int y);
static void eff_star_main(int n);
static void clear_enemy_shot(void);

static void background(void);
static void score_disp(int n,int x, int y);

static int hit_check(int n);
static int hit_check2(int n);
static int GetObjNum();
static void kill_obj(int n);

void cloud_init(void);
void cloud_main(int n);

extern StSpriteDef SprVisor[];
extern char sin_tbl[];	-- //サインテーブル
extern char cos_tbl[];	-- //コサインテーブル
//void PutSpriteVisor(int x,int y,int spr,int prio,int atr=0, unsigned long col=ARGB_DFLT,float ras=0);
--]]
	
function _VisorPutSprite( px , py , sprNo ,prio,flip )
	--[[ スプライト表示 ]]

	sprNo = sprNo //1;

	spr = SprVisor[ sprNo + 1 ];

	gxLib:PutSprite(
		px ,py , prio ,
		spr[1],spr[2]*8,spr[3]*8,spr[4]*8,spr[5]*8,spr[6],spr[7],
		flip,0xffffffff,
		0 ,
		1.0, 1.0 );
end

function _VisorPutSpriteVisor( px , py , sprNo , prio , ... )
	--[[ スプライト表示 ]]


	local flip , argb , scale = ...;

	if( flip  == nil ) then flip = 0;			end
	if( argb  == nil ) then argb = 0xffffffff;	end
	if( scale == nil ) then scale = 1.0;		end

	sprNo = sprNo //1;

	spr = SprVisor[ sprNo + 1 ];

	gxLib:PutSprite(
		px ,py , prio ,
		spr[1],spr[2]*8,spr[3]*8,spr[4]*8,spr[5]*8,spr[6],spr[7],
		 flip,0xffffffff,
		 0 ,
		 1.0, 1.0 );
end

function _SetBgColor( rgb )

	a = 0xff;
	gxLib:SetBgColor( (a<<24)|(rgb) );

end

function _LoadSound( n , fileName )
	gxLib:LoadAudio( n, fileName );
end

function _PlaySound( n , bLoop )

	if( bLoop == nil ) then 
		bLoop = gxFalse;
	end

	gxLib:PlayAudio( n , bLoop );

end

function _GetAbs( n )

	-- //-----------------------------
	-- //絶対値を求める
	-- //-----------------------------
--	if ( n < 0 ) then
--		return n*-1;
--	else
--		return n;
--	end

	-- 三項演算子っぽく書こうと思うとこうなる

	n = ( n < 0 ) and n*-1 or n*1

	return n;
end

function _GetRand()

	-- //-----------------------------
	-- //ランダムな数字を作り出す。
	-- //-----------------------------

	return gxLib:Rand()%10000;
end


function _PadControl( void )

	-- //-----------------------------
	-- //毎フレームのパッド情報を取得
	-- //-----------------------------

	-- g_GamePad.keydown = GetStylus( g_GamePad.x , g_GamePad.y );

	-- 複数の戻り値を受け取れるのこう書ける

	g_GamePad.keydown , g_GamePad.x , g_GamePad.y =  GetStylus( g_GamePad.x , g_GamePad.y );

end



 g_sFirst = 0;

 -- //---------------------------------------------------------
 -- //ゲームメイン
 -- //---------------------------------------------------------

function Visoriser()

	-- //-----------------------------
	-- //メインコントローラー
	-- //-----------------------------
	lev = 0;

	if( g_sFirst == 0 ) then
		-- //--------------------------------
		-- //１度だけ通る初期化
		-- //--------------------------------
		g_sFirst = g_sFirst + 1;
		visorizerInit();
	end

	_PadControl();

	if( g_GameSystem.scene >= 2 ) then

		-- //ゲーム終了（タイトル）へ
		game_result();
		return;
	end

	lev = g_GameSystem.level;

	background();			-- //背景

	visorControl();	-- //自機

	enemy_set();			-- //敵の出現順番を管理

	SprObj_control();		-- //enObjMax個のオブジェを動かす

	if( lev ~= g_GameSystem.level ) then

		-- //ゲームレベルがオブジェを動かす前と、
		-- //後で変わってたら面クリアとして、タイマーをリセット

		g_GameSystem.timer=0;

		if( g_GameSystem.bgm_flag == 0 ) then

			g_GameSystem.bgm_flag=1;
		end
	end

	g_GameSystem.VsyncTimer = g_GameSystem.VsyncTimer + 1;

	gxLib:PutSprite( 0,0,200, 4,0,0,320,320,0,0, ATR_DFLT , 0x80404000 );
	gxLib:PutSprite( 0,0,210, 6,0,0,320,480,0,0, ATR_DFLT , 0xFFFFFFFF );
end




-- //---------------------------------------------------------
-- //初期化
-- //---------------------------------------------------------

function visorizerInit(void)

	-- //ゲーム初期化

	-- @@@ 	gxUtil::MemSet( SprObj,0,sizeof(StObj)*enObjMax);
	for i=1,enObjMax do
		SprObj[ i ] = {StObj};
		--SprObj[ i ].new();
		-- 構造体の配列は適宜確保

		SprObj[ i ].category    = 0;	-- //生存フラグ
		SprObj[ i ].x        = 0;
		SprObj[ i ].y        = 0;	-- //Ｘ、Ｙ
		SprObj[ i ].vx       = 0;	-- //増分値など
		SprObj[ i ].vy       = 0;	-- //増分値など
		SprObj[ i ].eneType  = 0;	-- //種類
		SprObj[ i ].hp       = 0;	-- //耐久力

		SprObj[ i ].b = { 0,0,0,0,0,0,0,0 };
		SprObj[ i ].w = { 0,0,0,0 };
		SprObj[ i ].l = { 0,0 };
	end

	-- @@@ 	gxUtil::MemSet( &g_GameSystem,0,sizeof(StGame) );
	g_GameSystem = {StGame};

	g_GameSystem.x = 0;
	g_GameSystem.scene = 0;
	g_GameSystem.Score = 0;		-- //スコア
	g_GameSystem.my_shot_num = 0;	-- //弾数
	g_GameSystem.en_shot_num = 0;	-- //敵弾数
	g_GameSystem.enemy_num   = 0;	-- //敵数
	g_GameSystem.bb_remain   = 0;
	g_GameSystem.timer       = 0;
	g_GameSystem.level		= 0;
	g_GameSystem.option_x    = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; -- //35
	g_GameSystem.option_y    = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; -- //35
	g_GameSystem.option_pos  = 0;
	g_GameSystem.bgm_flag 	 = 0;
	g_GameSystem.r = 0;
	g_GameSystem.g = 0;
	g_GameSystem.b = 0;
	g_GameSystem.VsyncTimer = 0;

	gxLib:SetBgColor( 0xff010101 );

	if ( g_sGameSeq == 0 ) then
		g_sGameSeq = g_sGameSeq + 1;
		g_HighScore=5100;
	end

	g_GameSystem.VsyncTimer = 0;
	g_GameSystem.x = 0;
	g_GameSystem.bb_remain = 3;
	g_GameSystem.timer = 0;
	g_GameSystem.level = 0;
	g_GameSystem.scene = 0;

	_LoadSound( 0, "asset/visor/wav/bgm.wav" );
	_LoadSound( 1, "asset/visor/wav/valcun.wav");
	_LoadSound( 2, "asset/visor/wav/bomb01.wav");
	_LoadSound( 3, "asset/visor/wav/bomb02.wav");
	_LoadSound( 4, "asset/visor/wav/warning.wav");

	_PlaySound( 0 , gxTrue );

	gxLib:LoadTexture( 4        ,"asset/visor/wire.tga"    , 0xff00ff00 );
	gxLib:LoadTexture( 6        ,"asset/visor/gameboy.bmp" , 0xff00ff00 );
	gxLib:LoadTexture ( enTpgSpr,"asset/visor/shtimg.bmp"  , 0xff004080 );
	gxLib:UploadTexture();

end


function game_result( void )

	-- //ゲーム結果＆タイトル
	local px,py = 0,0;

	-- //ハイスコア更新

	if(Score_temp >= g_HighScore) then
		g_HighScore = Score_temp;
	end

	px = g_GamePad.x;
	py = g_GamePad.y;

	if( g_GameSystem.scene == 2 ) then 
	
		if( g_GamePad.keydown == 0 ) then 
			g_GameSystem.scene=3;
		end
	elseif( g_GameSystem.scene == 3 ) then

		if( g_GamePad.keydown ) then
		
			visorizerInit();
			return;
		end
	end

	_VisorPutSprite(32,50,66,PRIO_SYS,0);		-- //タイトル

	_VisorPutSprite(48,80,67,PRIO_SYS,0);		-- //ハイスコア
	score_disp( g_HighScore , 80 , 95 );				-- //ハイスコア

	_VisorPutSprite(56,110,68,PRIO_SYS,0);	-- //スコア
	score_disp( Score_temp , 80 , 125 );		-- //スコア

	g_GameSystem.enemy_num = 0;

	if( g_GameSystem.VsyncTimer%60 == _GetRand()%60 ) then
		enemy01_init();
	end

	SprObj_control();	-- //enObjMax個のオブジェを動かす
end


function SprObj_control( void )

	-- //-----------------------------
	-- //オブジェクトコントロール
	-- //-----------------------------
	local CntBlt,CntSht,CntEne = 0,0,0;

	for j=0,enObjMax-ORIGIN_ONE do

		i = j + 1;

 		if(  SprObj[ i ].category == 0 ) then
 			-- //なにもなし
		elseif(  SprObj[ i ].category == 1) then	--		case 1:	 //自分

		elseif(  SprObj[ i ].category == 2) then	--		case 2:	-- //たま
 			CntSht = CntSht +1;
 			shot_main(i);

		elseif(  SprObj[ i ].category == 3) then	 --		case 3:	-- //てき
 			CntEne = CntEne +1;
 			enemy_main(i);

		elseif(  SprObj[ i ].category == 4) then	 --		case 4:	-- //爆発
 			bomb01_main(i);

		elseif(  SprObj[ i ].category == 5) then	 --		case 5:	-- //敵の弾
 			CntBlt = CntBlt +1;
 			ene_shot_main(i);

		elseif(  SprObj[ i ].category == 6) then	 --		case 6:	-- //効果
 			CntBlt = CntBlt +1;
 			eff_star_main(i);

		elseif(  SprObj[ i ].category == 7) then	 --		case 7:	-- //雲
 			CntBlt = CntBlt +1;
 			cloud_main(i);
 		else

 		end
	end

	g_GameSystem.en_shot_num = CntBlt;
	g_GameSystem.my_shot_num = CntSht;
	g_GameSystem.enemy_num   = CntEne;

end


function enemy_set( void )

	-- //---------
	-- //面制御
	-- //---------

	if( g_GameSystem.level == 0 ) then
		enemy_set_lv0();

	elseif( g_GameSystem.level == 1) then
		enemy_set_lv1();

	elseif( g_GameSystem.level == 2) then
		enemy_set_lv2();

	elseif( g_GameSystem.level == 3) then
		-- //クリア

	else

	end

end



function enemy_set_lv0( void )

	-- //-------------
	-- //１面
	-- //-------------
	local timer = 0;

	timer = g_GameSystem.timer;

	if(g_GameSystem.VsyncTimer%60==0) then

		g_GameSystem.timer = g_GameSystem.timer + 1;

	end

	-- //スタンピート
	if( timer > 1 and timer <= 10 ) then
		if ( g_GameSystem.VsyncTimer%300 == 225 ) then
			-- //時々Ｇスイング
			enemy05_init();
		end

		if( g_GameSystem.VsyncTimer%10 == 0 ) then
			enemy01_init();
		end
	end

	-- //Ｇスイング

	if( timer > 10 and timer <= 20 ) then
		if( g_GameSystem.VsyncTimer%40 == 0 ) then
			enemy05_init();
		end
	end

	-- //Ｇスイング＆スタンピート
	if ( timer > 20 and timer <= 25 ) then
		if( g_GameSystem.VsyncTimer%5 == 0 ) then
			enemy01_init();
		end

		if( g_GameSystem.VsyncTimer%60 == 0 ) then
		enemy05_init();
		end
	end

	-- //ボス
	if( timer == 30 ) then

 		if( g_GameSystem.VsyncTimer%60 == 10 ) then
			enemy02_init();
		end

	end

end


function enemy_set_lv1(void)

 	-- //-------------
 	-- //２面
 	-- //-------------
 	local timer;
 
 	if ( g_GameSystem.VsyncTimer%60 == 0 ) then
 		g_GameSystem.timer = g_GameSystem.timer +1;
 	end

 	timer = g_GameSystem.timer;
 
 	-- //ウラカン・ラナ
 	if( timer > 1 and timer <= 10 ) then
 		if ( g_GameSystem.VsyncTimer%30 == 0 ) then
 			enemy06_init(0);
 		end
 	end
 
 	-- //Ｇスイング＆スタンピート
 	if ( timer > 10 and timer <= 25 ) then
 		if(g_GameSystem.VsyncTimer%15==60) then
 			enemy01_init();
 		end

 		if( g_GameSystem.VsyncTimer%45 == 0 ) then
 			enemy06_init(0);
 		end
 	end
 
 	-- //スタンピート＆ウラカン・ラナ
 	if ( timer > 25 and timer <= 28 ) then
 		if( g_GameSystem.VsyncTimer%20 == 0 ) then
 			enemy01_init();
 		end

 		if( g_GameSystem.VsyncTimer%60 == 15 ) then
 			enemy05_init();
 		end

 		if( g_GameSystem.VsyncTimer%120==0) then
 			enemy06_init(0);
 		end
 	end
 
 	-- //ボス
 	if ( timer == 30 ) then
 		if( g_GameSystem.VsyncTimer%60 == 10 ) then
 			enemy03_init();
 		end
 	end
end


function enemy_set_lv2( void )

 	-- //-------------
 	-- //３面
 	-- //-------------
 	local timer;
 
 	if ( g_GameSystem.VsyncTimer%60 == 0) then
 		g_GameSystem.timer = g_GameSystem.timer +1;
 	end

 	timer = g_GameSystem.timer;
 
 	-- //Ｇスイング＆ウラカン・ラナ＆スタンピート

 	if ( timer > 1 and timer <= 10 ) then
 		if ( g_GameSystem.VsyncTimer%20 == 0 ) then
 			enemy01_init();
 		end
 		if( g_GameSystem.VsyncTimer%120 == 60 ) then
 			enemy05_init();
 		end

 		if( g_GameSystem.VsyncTimer%120 == 30 ) then
 			enemy06_init(0);
 		end
 	end
 
 	-- //Ｇスイング＆スタンピート
 	if( timer > 10 and timer <= 28 ) then

 		if( g_GameSystem.VsyncTimer%10 == 0 ) then
 			enemy01_init();
 		end
 		if( g_GameSystem.VsyncTimer%120 == 0 ) then
 			enemy05_init();
 		end
 	end
 
 	-- //ボス
 	if ( timer == 30 ) then
 		if ( g_GameSystem.VsyncTimer%60 == 10 ) then
 			enemy04_init();
 		end
 	end

end


function background( void )

	-- //-----------------------------
	-- //背景：スクロール
	-- //-----------------------------
	local i,py = 0,0;
	local r,g,b = 0xff,0xff,0xff;

	if( g_GameSystem.VsyncTimer%2 == 0 ) then
		ScrollY = ScrollY + 1;
	end

	ScrollY = ScrollY%168;

	for i=0,21-ORIGIN_ONE do
		py = ScrollY + i*8;
		py = py%168;
		_VisorPutSprite(160-8*5+g_GameSystem.x,py-8,10+i,PRIO_GROUND,0);
	end

	if( g_GameSystem.timer == 28 ) then
		if( g_GameSystem.VsyncTimer%60 == 5 ) then
		_PlaySound( 4 );
		end
	end

	-- //スコア＆残機
	score_disp( g_GameSystem.Score , 40 , 0 );

	for i=0,g_GameSystem.bb_remain-ORIGIN_ONE do
	_VisorPutSprite(112+i*9,2,54,PRIO_SYS,0);
	end

	if ( g_GameSystem.level == 0 ) then
		r = 0x00; g = 0x40; b = 0x80;

	elseif ( g_GameSystem.level == 1 ) then
		r = 0x80; g = 0x40; b = 0x00;

	elseif ( g_GameSystem.level == 2 ) then
		r = 0xc0; g = 0xa0; b = 0xf0;
		r = 0x80; g = 0x00; b = 0x80;

	else
		r = 0xff; g = 0xff; b = 0xff;
	end

	r = 8; g = 24; b = 32;

	-- //ＢＧカラーの変更
	if( g_GameSystem.r < r) then g_GameSystem.r = g_GameSystem.r + 1;	end
	if( g_GameSystem.r > r) then g_GameSystem.r = g_GameSystem.r - 1;	end
	if( g_GameSystem.g < g) then g_GameSystem.g = g_GameSystem.r + 1;	end
	if( g_GameSystem.g > g) then g_GameSystem.g = g_GameSystem.r - 1;	end
	if( g_GameSystem.b < b) then g_GameSystem.b = g_GameSystem.r + 1;	end
	if( g_GameSystem.b > b) then g_GameSystem.b = g_GameSystem.r - 1;	end

	_SetBgColor( (g_GameSystem.r<<16)|(g_GameSystem.g<<8)|(g_GameSystem.b) );

end


function score_disp( now_score , x , y )

	-- //-----------------
	-- //スコア表示
	-- //-----------------
	local i,score = 0,0;

	_VisorPutSprite(x+8,y,52,PRIO_SYS,0);

	if( now_score == 0 ) then
		return;
	end

	i = 0;

	while ( gxTrue ) do
		score = now_score%10;

		_VisorPutSprite( x-8*i , y , 42+score , PRIO_SYS , 0 );

		now_score = now_score // 10;	-- 整数除算ね(Lua 5.3で対応されています)

		if( now_score == 0 ) then
			return;
		end

		i = i + 1;
	end

end


function visorControl( void )

	-- //-----------------------------
	-- //マイシップコントロール
	-- //-----------------------------
	--[[
		eneType
		0:
		1:復活中
		2:無敵時間
		3:通常
	--]]

	local px,py = 0,0;
	local x,y,opt,i = 0,0,0,0;
	local dir = 0;

	x = SprObj[ enMyShipIndex ].x;
	y = SprObj[ enMyShipIndex ].y;

	if( SprObj[enMyShipIndex].category == 0 ) then

		y , SprObj[ enMyShipIndex ].y = 200,200;

		SprObj[ enMyShipIndex ].eneType = 0;

		if( g_GameSystem.bb_remain > 0 ) then

			-- //残機あれば
			g_GameSystem.bb_remain = g_GameSystem.bb_remain -1;

			SprObj[ enMyShipIndex ].category   = 1;
			SprObj[ enMyShipIndex ].eneType = 1;

			x ,SprObj[ enMyShipIndex ].x = 80,80;

			for i=0,30 do
				-- //オプション位置初期化
				g_GameSystem.option_x[ i ] = 80;
				g_GameSystem.option_x[ i ] = 180;
				g_GameSystem.option_pos  = 0;
			end
		end
	end

	if( SprObj[enMyShipIndex].eneType == 1 ) then

		if ( SprObj[enMyShipIndex].y > 140 ) then

			SprObj[enMyShipIndex].y = SprObj[enMyShipIndex].y -1;
		else
			SprObj[enMyShipIndex].eneType=2;
		end

		x = SprObj[ enMyShipIndex ].x;
		y = SprObj[ enMyShipIndex ].y;

		SprObj[ enMyShipIndex ].b[0+ORIGIN_ONE] = 10;
	end

	px = g_GamePad.x;
	py = g_GamePad.y;


	if( (SprObj[ enMyShipIndex ].eneType >= 2 ) and ( SprObj[ enMyShipIndex ].eneType < 4 ) ) then
		if( g_GamePad.keydown ) then

--[[
//			if((_GetAbs(x-px)>=16)||(_GetAbs(y-py)>=16)){
//				-- //ワープしないための補正
//				if(x<px) x+=3;
//				if(x>px) x-=3;
//				if(y<py) y+=3;
//				if(y>py) y-=3;
//			}
//			else
--]]
				x = px;
				y = py;
			-- end
		end
	end


	-- //パッド操作

	dir = -1;

	j = gxLib:JoyPadPush()&(JOY_U|JOY_L|JOY_R|JOY_D);

	if( j == JOY_R ) then
		dir = 0;
	elseif( j == JOY_D ) then
		dir = 90;
	elseif( j == JOY_L ) then
		dir = 180;
	elseif( j == JOY_U ) then
		dir = 270;
	elseif( j == JOY_R|JOY_D ) then
		dir = 45;
	elseif( j == JOY_D|JOY_L ) then
		dir = 135;
	elseif( j == JOY_L|JOY_U ) then
		dir = 225;
	elseif( j == JOY_U|JOY_R ) then
		dir = 315;
	end

	if ( dir ~= -1 ) then
	
		 x = x + cos_tbl[ dir+ORIGIN_ONE ]/35;
		 y = y + sin_tbl[ dir+ORIGIN_ONE ]/35;
	end

	if ( SprObj[ enMyShipIndex ].eneType == 0 ) then
		if( g_GameSystem.bb_remain == 0 ) then
			-- //ゲームオーバー表示
			_VisorPutSprite(40,80,65,PRIO_SYS,0);

			if( g_GameSystem.scene == 0 ) then

				if( g_GamePad.keydown == 0 ) then
					g_GameSystem.scene = 1;
				end

			elseif( g_GameSystem.scene == 1 ) then

				if( g_GamePad.keydown ) then
					Score_temp = g_GameSystem.Score;
					visorizerInit();
					g_GameSystem.scene = 2;
				end

			end

		end

		return;
	end


	if( SprObj[ enMyShipIndex ].b[ (0+ORIGIN_ONE) ] > 0 ) then
		-- //無敵時間
		SprObj[ enMyShipIndex ].b[ (0+ORIGIN_ONE) ] = SprObj[ enMyShipIndex ].b[ (0+ORIGIN_ONE) ] -1;

		if ( SprObj[ enMyShipIndex ].b[ (0+ORIGIN_ONE) ] == 5 ) then
			clear_enemy_shot();
		end
	else
		SprObj[ enMyShipIndex ].eneType = 3;
	end

	-- //画面外補正

	if( x <= 0   ) then x = 0;   end
	if( x >= 160 ) then x = 160; end
	if( y <= 0   ) then y = 0;   end

	if( SprObj[ enMyShipIndex ].eneType ~= 1 ) then

		if( y >= 160 ) then
			y=160;
		end

	end

	if( SprObj[ enMyShipIndex ].category == 0 ) then
		if ( y >= 160-16 ) then
			y = 160-16;
		end
	end

	if( g_GameSystem.level == 3 ) then

		-- //クリア

		if ( SprObj[ enMyShipIndex ].y > -100 ) then
			SprObj[ enMyShipIndex ].y = SprObj[enMyShipIndex].y - 4;
		else
			Score_temp = g_GameSystem.Score;
			visorizerInit();
			g_GameSystem.scene = 2;
			return;
		end

		x = SprObj[ enMyShipIndex ].x;
		y = SprObj[ enMyShipIndex ].y;
	end

	_VisorPutSprite(x,y,0,PRIO_JIKI,0);

	g_GamePad.x = x;
	g_GamePad.y = y;

	SprObj[ enMyShipIndex ].x = x;
	SprObj[ enMyShipIndex ].y = y;

	-- //------------
	-- //ショット
	-- //------------


	if( g_GameSystem.VsyncTimer%4 == 0 ) then

		_PlaySound( 1 );
		shot_init( enMyShipIndex , -5 ,-1 );
		shot_init( enMyShipIndex ,  6 ,-1 );

	end

	-- //------------
	-- //オプション
	-- //------------
	if( (g_GameSystem.option_x[ g_GameSystem.option_pos ] ~= x ) or ( g_GameSystem.option_y[ g_GameSystem.option_pos ] ~= y ) ) then
		g_GameSystem.option_pos = g_GameSystem.option_pos + 1;
		g_GameSystem.option_pos = g_GameSystem.option_pos%30;
		g_GameSystem.option_x[ g_GameSystem.option_pos ] = x;
		g_GameSystem.option_y[ g_GameSystem.option_pos ] = y;
	end

	opt = (g_GameSystem.option_pos + 20 )%30 + ORIGIN_ONE;

	_VisorPutSprite(g_GameSystem.option_x[opt],g_GameSystem.option_y[opt],9,PRIO_JIKI-1,0);

	-- //オプションショット

	if ( g_GameSystem.VsyncTimer%4 == 1 ) then
		shot_init( -1 , g_GameSystem.option_x[ opt ] - x + 1 , g_GameSystem.option_y[ opt ] - y + 2 );
	end

end


-- @@@ void cloud_init(void)
-- @@@ {
-- @@@ 	int n;
-- @@@ 
-- @@@ 	n=GetObjNum();
-- @@@ 	if(n==-1) return;
-- @@@ 
-- @@@ 	SprObj[ n ].b[0+ORIGIN_ONE]=2+rand()%4;
-- @@@ 	SprObj[ n ].category = 7;
-- @@@ 	SprObj[ n ].x = -32+(rand()%20)*6;
-- @@@ 	SprObj[ n ].y = -24*(SprObj[ n ].b[0+ORIGIN_ONE])*2;
-- @@@ 	SprObj[ n ].vy = (3+rand()%2);-- //*SprObj[ n ].b[0+ORIGIN_ONE]/2;
-- @@@ 	SprObj[ n ].vx = 40+rand()%80;
-- @@@ 
-- @@@ }
-- @@@ 
-- @@@ void cloud_main(int n)
-- @@@ {
-- @@@ 	SprObj[ n ].y+=SprObj[ n ].vy;
-- @@@ 
-- @@@ 	_VisorPutSpriteVisor(SprObj[ n ].x,SprObj[ n ].y,69,20+rand()%10,ATR_ALPHA_NML,ARGB(0x40,0xff,0xff,0xff),SprObj[ n ].b[0+ORIGIN_ONE]);
-- @@@ 
-- @@@ 	if((SprObj[ n ].x<=-32)||(SprObj[ n ].x>=160+32)||(SprObj[ n ].y>=160+32)){
-- @@@ 		kill_obj(n);
-- @@@ 		return;
-- @@@ 	}
-- @@@ 
-- @@@ }


-- //---------------------------------------------------------
-- //　　ショット
-- //---------------------------------------------------------
function shot_init( s , x, y )

	local n;

	if( s == -1 ) then
		s = enMyShipIndex;
	else

		if( g_GameSystem.my_shot_num >= MAX_MY_SHOT ) then
			return;
		end

	end

	n = GetObjNum();

	if( n == -1 ) then
		return;
	end

	SprObj[ n ].category = 2;
	SprObj[ n ].x    = SprObj[ s ].x + x;
	SprObj[ n ].y    = SprObj[ s ].y + y;
end


function shot_main( n )

	-- //-----------------------------
	-- //ショットメイン
	-- //-----------------------------

	SprObj[ n ].y = SprObj[ n ].y - 7;

	_VisorPutSprite( SprObj[ n ].x , SprObj[ n ].y , 1 , PRIO_TAMA , 0 );

	if( hit_check( n ) ) then
		kill_obj(n);
	end

	if( SprObj[ n ].y <= -16 ) then
		kill_obj(n);
	end

end


-- //-------------
-- //敵ショット
-- //-------------

function ene_shot_init( src , eneType )

	-- //-----------------------------
	-- //敵ショット：初期化
	-- //-----------------------------
	local n,kaku,i;

	if( g_GameSystem.en_shot_num >= MAX_EN_SHOT ) then
		return;
	end

	n = GetObjNum();

	if( n == -1 ) then
		return;
	end

	SprObj[ n ].category   = 5;
	SprObj[ n ].eneType = eneType;
	SprObj[ n ].vx = ( SprObj[ src ].x*100 );
	SprObj[ n ].vy = ( SprObj[ src ].y*100 );

	--@@@ switch( eneType ){

	if( eneType == 1 ) then
		-- //囲い込みレーザー弾1

		SprObj[ n ].w[ 0+ORIGIN_ONE ] = 80;
		SprObj[ n ].w[ 1+ORIGIN_ONE ] = -100;

	elseif( eneType == 2 ) then
		-- //囲い込みレーザー弾2
		SprObj[ n ].eneType = 1;
		SprObj[ n ].w[ 0 + ORIGIN_ONE ] = -80;
		SprObj[ n ].w[ 1 + ORIGIN_ONE ] = -100;

	elseif( eneType == 3 or eneType == 4 ) then
		-- //８の字1
		-- //８の字2

		SprObj[ n ].category = 0;

		for i=0,360-1,18 do

			n = GetObjNum();

			if ( n == -1 ) then
				return;
			end

			SprObj[ n ].category = 5;
			SprObj[ n ].eneType = 0;	-- //通常弾あつかい
			SprObj[ n ].vx = ( SprObj[ src ].x*100 ) + cos_tbl[ i + ORIGIN_ONE ]*30;
			SprObj[ n ].vy = ( SprObj[ src ].y*100 ) + sin_tbl[ i + ORIGIN_ONE ]*30;

			if ( eneType == 3 ) then
				kaku=( i + 180 )%360;
			else
				kaku=( i )%360;
			end
			SprObj[ n ].w[ 0+ORIGIN_ONE ] = sin_tbl[ kaku + ORIGIN_ONE ]*2;
			SprObj[ n ].w[ 1+ORIGIN_ONE ] = cos_tbl[ kaku + ORIGIN_ONE ]*2;
		end

	elseif( eneType == 5 ) then -- //最強弾

		SprObj[ n ].vx = ( SprObj[ src ].x*100 );
		SprObj[ n ].vy = ( SprObj[ src ].y*100 ) + 16*100;
		SprObj[ n ].w[ 0+ORIGIN_ONE ] = 0;
		SprObj[ n ].w[ 1+ORIGIN_ONE ] = 300;

	else					-- //ノーマル弾
		if ( SprObj[ enMyShipIndex ].x <  SprObj[ src ].x ) then kaku = 0;	end
		if ( SprObj[ enMyShipIndex ].x >= SprObj[ src ].x ) then kaku = -45;	end

		SprObj[ n ].w[ 0+ORIGIN_ONE ] = cos_tbl[ 90+kaku+_GetRand()%45 + ORIGIN_ONE ]*2 / 3;
		SprObj[ n ].w[ 1+ORIGIN_ONE ] = sin_tbl[ 90+kaku+_GetRand()%45 + ORIGIN_ONE ]*2 / 3;

	end

end


function ene_shot_main( n )

	-- //-----------------------------
	-- //敵ショットメイン分類わけ
	-- //-----------------------------

	if( SprObj[ n ].eneType == 1 ) then
	-- case 1:		-- //囲い込みレーザー
		ene_shot_laser01( n );

	elseif( SprObj[ n ].eneType == 2 ) then
	-- case 2:		-- //囲い込みレーザー
		ene_shot_laser01( n );

	elseif( SprObj[ n ].eneType == 5 ) then
	-- case 5:		-- //最強弾
		ene_shot_bigblitz( n );

	else
	-- default:	-- //ノーマルショット
		ene_shot_normal( n );

	end

	if ( hit_check2(n) ) then 
		-- //kill_obj(n);
	end

	if ( ( SprObj[ n ].x >= 165 ) or ( SprObj[ n ].x <= -5 )
		 or( SprObj[ n ].y >= 165 ) or ( SprObj[ n ].y <= -5 ) ) then

		kill_obj( n );

	end

end

function ene_shot_normal( n )

	-- //-----------------------------
	-- //ノーマル弾
	-- //-----------------------------

	SprObj[ n ].vx = SprObj[ n ].vx + SprObj[ n ].w[ 0 + ORIGIN_ONE ];
	SprObj[ n ].vy = SprObj[ n ].vy + SprObj[ n ].w[ 1 + ORIGIN_ONE ];

	SprObj[ n ].x = SprObj[ n ].vx / 100;
	SprObj[ n ].y = SprObj[ n ].vy / 100;

	_VisorPutSprite( SprObj[ n ].x , SprObj[ n ].y , 6 , PRIO_TAMA , 0 );

end


function ene_shot_laser01( n )

	-- //-----------------------------
	-- //レーザー弾
	-- //-----------------------------

	if( SprObj[ n ].w[0+ORIGIN_ONE] > 0)   then SprObj[ n ].w[ 0+ORIGIN_ONE ] = SprObj[ n ].w[ 0+ORIGIN_ONE ] - 2;  end
	if( SprObj[ n ].w[0+ORIGIN_ONE] < 0)   then SprObj[ n ].w[ 0+ORIGIN_ONE ] = SprObj[ n ].w[ 0+ORIGIN_ONE ] + 2;  end
	if( SprObj[ n ].w[1+ORIGIN_ONE] < 320) then SprObj[ n ].w[ 1+ORIGIN_ONE ] = SprObj[ n ].w[ 1+ORIGIN_ONE ] + 10; end

	SprObj[ n ].vx = SprObj[ n ].vx + SprObj[ n ].w[ 0+ORIGIN_ONE ];
	SprObj[ n ].vy = SprObj[ n ].vy + SprObj[ n ].w[ 1+ORIGIN_ONE ];

	SprObj[ n ].x = SprObj[ n ].vx/100;
	SprObj[ n ].y = SprObj[ n ].vy/100;

	_VisorPutSprite( SprObj[ n ].x , SprObj[ n ].y , 36 , PRIO_TAMA+1 , 0 );
end



function ene_shot_bigblitz( n )

	-- //-----------------------------
	-- //最強弾
	-- //-----------------------------

	if ( SprObj[ n ].w[ 1+ORIGIN_ONE ] > 120 ) then
		SprObj[ n ].w[ 1+ORIGIN_ONE ] = SprObj[ n ].w[ 1+ORIGIN_ONE ] - 5;
	end

	SprObj[ n ].vx = SprObj[ n ].vx + SprObj[ n ].w[ 0 + ORIGIN_ONE ];
	SprObj[ n ].vy = SprObj[ n ].vy + SprObj[ n ].w[ 1 + ORIGIN_ONE ];

	SprObj[ n ].x = SprObj[ n ].vx / 100;
	SprObj[ n ].y = SprObj[ n ].vy / 100;

	_VisorPutSprite( SprObj[ n ].x , SprObj[ n ].y , 37 , PRIO_TAMA+2 , 0 );

end



-- //---------------------------------------------------------
-- //　　敵
-- //---------------------------------------------------------
function enemy_main( n )

	-- //-----------------------------
	-- //敵分類わけ
	-- //-----------------------------

	if( SprObj[ n ].eneType == 0 )		then	--//:		-- //スタンピート
 		enemy01_main(n);

	elseif( SprObj[ n ].eneType == 1 )	then	--//:		-- //ケブラーダ１
 		enemy02_main(n);

	elseif( SprObj[ n ].eneType == 2 )	then	--//:		-- //ケブラーダ２
 		enemy03_main(n);

	elseif( SprObj[ n ].eneType == 3 )	then	--//:		-- //ケブラーダ３
 		enemy04_main(n);

	elseif( SprObj[ n ].eneType == 4 )	then	--//:		-- //Ｇスイング
 		enemy05_main(n);

	elseif( SprObj[ n ].eneType == 5 )	then	--//:		-- //ウラカン・ラナ
 		enemy06_main(n);

	elseif( SprObj[ n ].eneType == 6 )	then	--//:		-- //スタンピート４
 		enemy04_main(n);

	else

	end
end


-- //--------------------
-- //スタンピート（ザコ）
-- //--------------------

function enemy01_init( void )
	-- //-----------------------------
	-- //スタンピート：初期化
	-- //-----------------------------
	local n;

	if( g_GameSystem.enemy_num >= MAX_ENEMY ) then
		return;
	end

	n = GetObjNum();

	if( n == -1 ) then
		return;
	end

	SprObj[ n ].hp      = 3;
	SprObj[ n ].eneType = 0;
	SprObj[ n ].category   = 3;
	SprObj[ n ].x       = 80;
	SprObj[ n ].vy      = -16*10;

	if( SprObj[ enMyShipIndex ].x < 80 ) then
		SprObj[ n ].vx = 50;
		SprObj[ n ].b[0+ORIGIN_ONE]=-2;
	else
		SprObj[ n ].vx = -50;
		SprObj[ n ].b[0+ORIGIN_ONE] = 2;
	end

end


function enemy01_main( n )

	-- //-----------------------------
	-- //スタンピート：メイン
	-- //-----------------------------

	local shoot_timing =
	{
		180,90,60,30,240
	};

	SprObj[ n ].x  = SprObj[ n ].x  + SprObj[ n ].vx/10;
	SprObj[ n ].vx = SprObj[ n ].vx + SprObj[ n ].b[0+ORIGIN_ONE];

	SprObj[ n ].vy = SprObj[ n ].vy + 12;

	if ( _GetAbs(SprObj[ n ].vx) >= 50 ) then
		SprObj[ n ].b[0+ORIGIN_ONE] = SprObj[ n ].b[0+ORIGIN_ONE]*-1;
	end

	SprObj[ n ].x = SprObj[ n ].x;
	SprObj[ n ].y = SprObj[ n ].vy/10;

	_VisorPutSprite( SprObj[ n ].x , SprObj[ n ].y , 31 , PRIO_TEKI , 0 );

	if( _GetRand()%shoot_timing[g_GameSystem.level + ORIGIN_ONE ] == g_GameSystem.VsyncTimer%shoot_timing[ g_GameSystem.level + ORIGIN_ONE ] ) then
		ene_shot_init( n , 0 );
	end

	if ( SprObj[ n ].hp == 0 ) then
		-- //bomb01_init(n,0,0,0);
		kill_obj( n );
		return;
	end

	if( SprObj[ n ].y >= 176 ) then
		kill_obj( n );
		return;
	end

end


-- //--------------
-- //ケブラーダ１
-- //--------------
function enemy02_init( void )

	-- //-----------------------------
	-- //ケブラーダ１：初期化
	-- //-----------------------------
	local n;

	if( g_GameSystem.enemy_num >= MAX_ENEMY) then
		return;
	end

	n = GetObjNum();

	if( n == -1 ) then
		return;
	end

	SprObj[ n ].hp      = 160;
	SprObj[ n ].eneType = 1;
	SprObj[ n ].category   = 3;
	SprObj[ n ].x       = 80;
	SprObj[ n ].vy      = -16*10;
	SprObj[ n ].w[ 1+ORIGIN_ONE ] = 0;
	SprObj[ n ].w[ 2+ORIGIN_ONE ] = 0;

	if(SprObj[enMyShipIndex].x<80) then

		SprObj[ n ].x  = 110;
		SprObj[ n ].vx = 25;
		SprObj[ n ].b[0+ORIGIN_ONE] = -1;
	else
		SprObj[ n ].x  = 50;
		SprObj[ n ].vx = -25;
		SprObj[ n ].b[0+ORIGIN_ONE] = 1;
	end

end


function enemy02_main( n )
	-- //-----------------------------
	-- //ケブラーダ１：メイン
	-- //-----------------------------

	if( SprObj[ n ].w[2+ORIGIN_ONE] == 0 ) then
		if ( SprObj[ n ].hp == 80 ) then
			bomb01_init(n, -18 ,-18 , 1 );
			bomb01_init(n, -18 , 18 , 1 );
			bomb01_init(n,  18 ,-18 , 1 );
			bomb01_init(n,  18 , 18 , 1 );
			SprObj[ n ].w[2+ORIGIN_ONE] = -45;
		end
	end

	if( SprObj[ n ].y < 48 ) then
		-- //下に下りてくる
		SprObj[ n ].vy = SprObj[ n ].vy + 4;
	else
		-- //左右に振る
		if( SprObj[ n ].w[2+ORIGIN_ONE] < -1 ) then
			SprObj[ n ].w[2+ORIGIN_ONE] = SprObj[ n ].w[2+ORIGIN_ONE] +1;
		else
			SprObj[ n ].y = 48;
			SprObj[ n ].x  = SprObj[ n ].x  + SprObj[ n ].vx/10;
			SprObj[ n ].vx = SprObj[ n ].vx + SprObj[ n ].b[0+ORIGIN_ONE];
		end
	end

	if ( _GetAbs(SprObj[ n ].vx) >= 25 ) then
		SprObj[ n ].b[0+ORIGIN_ONE] = SprObj[ n ].b[0+ORIGIN_ONE] * -1;
	end

	SprObj[ n ].x = SprObj[ n ].x;
	SprObj[ n ].y = SprObj[ n ].vy/10;

	_VisorPutSpriteVisor( SprObj[ n ].x , SprObj[ n ].y , 32 , PRIO_TEKI+2 , 0 , ARGB_DFLT , 1 );

	SprObj[ n ].w[1+ORIGIN_ONE] = SprObj[ n ].w[1+ORIGIN_ONE] +1;
	SprObj[ n ].w[1+ORIGIN_ONE] = SprObj[ n ].w[1+ORIGIN_ONE]%280;

	-- //攻撃パターン
	if ( SprObj[ n ].w[1+ORIGIN_ONE] < 100 ) then
		-- //ばら撒き弾
		if( g_GameSystem.VsyncTimer%4 == 0 ) then
			ene_shot_init(n,0);
		end

	elseif( SprObj[ n ].w[1+ORIGIN_ONE] == 200 ) then
		-- //８の字弾1
		ene_shot_init(n,3);

	elseif( SprObj[ n ].w[1+ORIGIN_ONE] == 250 ) then
		-- //８の字弾2
		ene_shot_init(n,4);
	end

	if( g_GameSystem.VsyncTimer%120 == 0 ) then

		enemy01_init();

	end

	if( SprObj[ n ].hp == 0 ) then

		bomb01_init(n, 18 ,0  ,1);
		bomb01_init(n,-18 ,0  ,1);
		bomb01_init(n, 0  ,-18,1);
		bomb01_init(n, 0  , 18,1);
		bomb01_init(n, -18,-18,1);
		bomb01_init(n, -18 ,18,1);
		bomb01_init(n,  18,-18,1);
		bomb01_init(n,  18 ,18,1);

		clear_enemy_shot();

		kill_obj( n );

		g_GameSystem.level = g_GameSystem.level +1;
		g_GameSystem.Score = g_GameSystem.Score + 500;

		return;

	end

	if( SprObj[ n ].y >= 176 ) then

		kill_obj( n );

	end

end


-- //--------------
-- //ケブラーダ２
-- //--------------
function enemy03_init( void )

	-- //-----------------------------
	-- //ケブラーダ２：初期化
	-- //-----------------------------
	local n;

	if( g_GameSystem.enemy_num >= MAX_ENEMY ) then
		return;
	end

	n = GetObjNum();

	if( n == -1 ) then
		return;
	end

	SprObj[ n ].hp      = 210;
	SprObj[ n ].eneType = 2;
	SprObj[ n ].category   = 3;
	SprObj[ n ].x       = 80;
	SprObj[ n ].vy      = -16*10;

	SprObj[ n ].x       = 80;
	SprObj[ n ].vx      = 25;
	SprObj[ n ].b[ 0+ORIGIN_ONE ] = -1;

end


function enemy03_main( n )

	-- //-----------------------------
	-- //ケブラーダ２：メイン
	-- //-----------------------------

	if( SprObj[ n ].y < 32 ) then
		-- //下に下りてくる
		SprObj[ n ].vy = SprObj[ n ].vy + 4;
	else
		-- //左右に振る
		SprObj[ n ].x  = SprObj[ n ].x  + SprObj[ n ].vx / 10;
		SprObj[ n ].vx = SprObj[ n ].vx + SprObj[ n ].b[ 0+ORIGIN_ONE ];
	end

	if( _GetAbs( SprObj[ n ].vx ) >= 28 ) then
		SprObj[ n ].b[0+ORIGIN_ONE] = SprObj[ n ].b[0+ORIGIN_ONE] * -1;
	end

	SprObj[ n ].x = SprObj[ n ].x;
	SprObj[ n ].y = SprObj[ n ].vy / 10;

	_VisorPutSprite( SprObj[ n ].x    ,SprObj[ n ].y  ,32, PRIO_TEKI  ,0 );
	_VisorPutSprite( SprObj[ n ].x-24 ,SprObj[ n ].y+8,33, PRIO_TEKI+1,0 );	-- //ウイング
	_VisorPutSprite( SprObj[ n ].x+32 ,SprObj[ n ].y+8,34, PRIO_TEKI+1,0 );	-- //ウイング

	-- //囲い込みレーザー
	if ( g_GameSystem.VsyncTimer%30 < 15 ) then
		if( g_GameSystem.VsyncTimer%5 == 0 ) then
			ene_shot_init(n,1);
			ene_shot_init(n,2);
		end
	end

	if ( g_GameSystem.VsyncTimer%120 == 0 ) then
		enemy01_init();
	end

	if ( SprObj[ n ].hp == 0 ) then
		bomb01_init( n,   8,-16, 1 );
		bomb01_init( n,  -8,-16, 1 );
		bomb01_init( n,   0,-16, 1 );
		bomb01_init( n,   0, 16, 1 );
		bomb01_init( n, -18,-18, 1 );
		bomb01_init( n, -18, 18, 1 );
		bomb01_init( n,  18,-18, 1 );
		bomb01_init( n,  18 ,18, 1 );
		g_GameSystem.Score = g_GameSystem.Score + 1000;
		clear_enemy_shot();
		kill_obj(n);
		g_GameSystem.level = g_GameSystem.level +1;
		return;
	end

	if ( SprObj[ n ].y >= 176 ) then
		kill_obj( n );
	end
end


-- //--------------
-- //ケブラーダ３
-- //--------------
function enemy04_init( void )

	-- //-----------------------------
	-- //ケブラーダ３：初期化
	-- //-----------------------------
	local n;

	if ( g_GameSystem.enemy_num >= MAX_ENEMY ) then 
		return;
	end

	n = GetObjNum();

	if( n == -1 ) then
		 return;
	end

	SprObj[ n ].hp      = 400;
	SprObj[ n ].eneType = 3;
	SprObj[ n ].category   = 3;
	SprObj[ n ].x       = 80;
	SprObj[ n ].vy      = -16*10;

	SprObj[ n ].x       = 80;
	SprObj[ n ].vx      = 25;
	SprObj[ n ].b[ 0+ORIGIN_ONE ] = -1;
	SprObj[ n ].w[ 1+ORIGIN_ONE ] =  0;
	SprObj[ n ].w[ 2+ORIGIN_ONE ] =  0;

end


function enemy04_main( n )

	-- //-----------------------------
	-- //ケブラーダ３：メイン
	-- //-----------------------------

	if ( SprObj[ n ].y < 32 ) then
		-- //下に下りてくる
		SprObj[ n ].vy = SprObj[ n ].vy + 4;
	else
		-- //左右に振る
		SprObj[ n ].x  = SprObj[ n ].x  + SprObj[ n ].vx / 10;
		SprObj[ n ].vx = SprObj[ n ].vx + SprObj[ n ].b[ 0+ORIGIN_ONE ];
	end

	if ( _GetAbs( SprObj[ n ].vx ) >= 27 ) then
		SprObj[ n ].b[ 0+ORIGIN_ONE ] = SprObj[ n ].b[ 0+ORIGIN_ONE ] * -1;
	end

	SprObj[ n ].x = SprObj[ n ].x;
	SprObj[ n ].y = SprObj[ n ].vy/10;

	if ( (SprObj[ n ].hp <= 350 ) and ( SprObj[ n ].w[ 2+ORIGIN_ONE ] == 0 ) ) then
		bomb01_init(n,   0, 16,1 );
		bomb01_init(n, -18, 18,1 );
		bomb01_init(n,  18,-18,1 );
		SprObj[ n ].w[ 2+ORIGIN_ONE ] = 1;

	elseif( ( SprObj[ n ].hp <= 200 ) and ( SprObj[ n ].w[ 2+ORIGIN_ONE ] == 1 ) ) then
		bomb01_init(n,-24,  8,1 );
		bomb01_init(n, 32,  8,1 );
		bomb01_init(n,-18,-18,1 );
		bomb01_init(n, 18, 18,1 );
		SprObj[ n ].w[ 2+ORIGIN_ONE ] = 2;
	end

	if( g_GameSystem.VsyncTimer%60 == 0 ) then
		enemy01_init();
	end

	-- //ダメージに応じた形で表示
	_VisorPutSprite( SprObj[ n ].x , SprObj[ n ].y , 32 , PRIO_TEKI , 0 );

	if(SprObj[ n ].w[ 2+ORIGIN_ONE ] <= 1 ) then
		_VisorPutSprite( SprObj[ n ].x-24, SprObj[ n ].y+8,33,PRIO_TEKI+1 , 0);	-- //ウイング左
		_VisorPutSprite( SprObj[ n ].x+32, SprObj[ n ].y+8,34,PRIO_TEKI+1 , 0);	-- //ウイング右
	end

	if( SprObj[ n ].w[ 2+ORIGIN_ONE ] <= 0 ) then
		_VisorPutSprite( SprObj[ n ].x , SprObj[ n ].y+16 , 35 , PRIO_TAMA+2 , 0 );	-- //キャノン
	end


	SprObj[ n ].w[ 1+ORIGIN_ONE ] = SprObj[ n ].w[ 1+ORIGIN_ONE ] +1;
	SprObj[ n ].w[ 1+ORIGIN_ONE ] = SprObj[ n ].w[ 1+ORIGIN_ONE ]%100;


	if( SprObj[ n ].w[2+ORIGIN_ONE] == 0 ) then		--case 0:		-- //攻撃パターン1
		if( SprObj[ n ].w[ 1+ORIGIN_ONE ]%100 == 70 ) then
			ene_shot_init(n,3);			-- //８の字
		end

		if ( SprObj[ n ].w[ 1+ORIGIN_ONE ]%100 < 60 ) then
			if( SprObj[ n ].w[ 1+ORIGIN_ONE ]%15 == 0 ) then
				ene_shot_init(n,5);		-- //最強弾
			end
		end

	elseif( SprObj[ n ].w[2+ORIGIN_ONE] == 1 ) then --	case 1:		-- //攻撃パターン2
		if( g_GameSystem.VsyncTimer%35 == 20 ) then
			ene_shot_init(n,5);			-- //最強弾
		end

		if( g_GameSystem.VsyncTimer%35 < 20 ) then
			if( g_GameSystem.VsyncTimer%5 == 0 ) then
				ene_shot_init(n,1);		-- //囲い込みレーザー
				ene_shot_init(n,2);		-- //囲い込みレーザー
			end
		end

	elseif( SprObj[ n ].w[2+ORIGIN_ONE] == 2 ) then	--	case 2:		-- //攻撃パターン3
		if( SprObj[ n ].w[1+ORIGIN_ONE] == 0 ) then
			ene_shot_init(n,3);			-- //８の字
		elseif( SprObj[ n ].w[1+ORIGIN_ONE] == 50 ) then
			ene_shot_init(n,4);			-- //８の字
		end
	else

	end

	if ( SprObj[ n ].hp == 0 ) then
		bomb01_init(n,   8,-16,1);
		bomb01_init(n,  -8,-16,1);
		bomb01_init(n,   0,-16,1);
		bomb01_init(n,   0, 16,1);
		bomb01_init(n, -18,-18,1);
		bomb01_init(n, -18, 18,1);
		bomb01_init(n,  18,-18,1);
		bomb01_init(n,  18, 18,1);
		kill_obj(n);
		clear_enemy_shot();
		g_GameSystem.level = g_GameSystem.level +1;
		g_GameSystem.Score = g_GameSystem.Score + 2000;
		return;
	end

	if( SprObj[ n ].y >= 176 ) then
		kill_obj( n );
	end

end



function enemy05_init( void )

	-- //-----------------------------
	-- //Ｇスイング：初期化
	-- //-----------------------------
	local n;

	if ( g_GameSystem.enemy_num >= MAX_ENEMY ) then
		return;
	end

	n = GetObjNum();

	if( n == -1 ) then
		return;
	end

	SprObj[ n ].hp      = 10;
	SprObj[ n ].eneType = 4;
	SprObj[ n ].category   = 3;
	SprObj[ n ].vy      = -160;

	SprObj[ n ].vx = 0;

	if( SprObj[ enMyShipIndex ].x < 80 ) then

		SprObj[ n ].x=80+10+_GetRand()%30;
		SprObj[ n ].w[0+ORIGIN_ONE] = -2;
		SprObj[ n ].w[1+ORIGIN_ONE] = 130 + _GetRand()%3*10;

	else

		SprObj[ n ].x = 80-10-_GetRand()%30;
		SprObj[ n ].w[0+ORIGIN_ONE] = 2;
		SprObj[ n ].w[1+ORIGIN_ONE] = 130 + _GetRand()%30;

	end

end


function enemy05_main( n )

	-- //-----------------------------
	-- //Ｇスイング：メイン
	-- //-----------------------------

	local shoot_timing = {
		50,30,20,240,30,10,
	};

	if ( SprObj[ n ].w[1+ORIGIN_ONE] > 0 ) then

		SprObj[ n ].w[1+ORIGIN_ONE] = SprObj[ n ].w[1+ORIGIN_ONE] - 1;

	else

		if( _GetAbs( SprObj[ n ].w[1+ORIGIN_ONE] ) <= 50 ) then
			SprObj[ n ].vx = SprObj[ n ].vx + SprObj[ n ].w[0+ORIGIN_ONE];
		end
	end

	SprObj[ n ].vy = SprObj[ n ].vy + SprObj[ n ].w[ 1+ORIGIN_ONE ];

	SprObj[ n ].x = SprObj[ n ].x + SprObj[ n ].vx/10;
	SprObj[ n ].y = SprObj[ n ].vy/100;

	_VisorPutSprite( SprObj[ n ].x , SprObj[ n ].y , 53,PRIO_TEKI , 0 );

	if( _GetRand()%shoot_timing[ g_GameSystem.level+ORIGIN_ONE ] == g_GameSystem.VsyncTimer%shoot_timing[ g_GameSystem.level+ORIGIN_ONE ] ) then
		ene_shot_init( n , 0 );
	end

	if( SprObj[ n ].hp == 0 ) then
		bomb01_init(n,-16,0,1);
		bomb01_init(n,0,-16,1);
		bomb01_init(n,0,16,1);
		bomb01_init(n,16,0,1);

		kill_obj(n);
		return;
	end

	if( (SprObj[ n ].x<=-32) or (SprObj[ n ].x>=160+32) ) then
		kill_obj(n);
		return;
	end

end



function enemy06_init( src )
	-- //-----------------------------
	-- //ウラカン・ラナ：初期化
	-- //-----------------------------

	local n,kaku;

	if ( g_GameSystem.enemy_num>=MAX_ENEMY) then
		return;
	end

	n = GetObjNum();

	if ( n == -1) then 
		return;
	end

	SprObj[ n ].eneType  = 5;
	SprObj[ n ].category = 3;

	bDivide = gxFalse;

	if( src > 0 ) then
		if ( (SprObj[ src ].category == 3 ) and ( SprObj[ src ].w[2+ORIGIN_ONE] > 0 ) ) then
			bDivide = gxTrue;
		end
	end

	if ( bDivide == gxFalse ) then
		-- //第１段階
		SprObj[ n ].w[ 2+ORIGIN_ONE ] = 0;
		SprObj[ n ].hp =  5;
		SprObj[ n ].vy = -180;
		SprObj[ n ].vx = (40+_GetRand()%80)*100;

		kaku=_GetRand()%30;

		if ( SprObj[ n ].vx < 80 ) then
			SprObj[ n ].w[ 0 + ORIGIN_ONE ] = cos_tbl[ 90-kaku+ORIGIN_ONE ]*2/3;
			SprObj[ n ].w[ 1 + ORIGIN_ONE ] = sin_tbl[ 90-kaku+ORIGIN_ONE ]*2/3;
		else
			SprObj[ n ].w[ 0 + ORIGIN_ONE ] = cos_tbl[ 90 + kaku+ORIGIN_ONE ]*2/3;
			SprObj[ n ].w[ 1 + ORIGIN_ONE ] = sin_tbl[ 90 + kaku+ORIGIN_ONE ]*2/3;
		end
	else
		-- //分裂作成
		if ( SprObj[ src ].w[ 2+ORIGIN_ONE ] == 1 ) then
			-- //第２段階

			kaku = 30+(_GetRand()%4)*30;

			SprObj[ n ].hp=2;
			SprObj[ n ].w[0+ORIGIN_ONE] = cos_tbl[ kaku + ORIGIN_ONE ];
			SprObj[ n ].w[1+ORIGIN_ONE] = sin_tbl[ kaku + ORIGIN_ONE ];

		elseif ( SprObj[ src ].w[ 2+ORIGIN_ONE ] == 2 ) then
			-- //第３段階
			kaku = 15 + _GetRand()%150;

			SprObj[ n ].hp = 2;
			SprObj[ n ].w[ 0+ORIGIN_ONE ] = cos_tbl[ kaku+ORIGIN_ONE ]*2 / 3;
			SprObj[ n ].w[ 1+ORIGIN_ONE ] = sin_tbl[ kaku+ORIGIN_ONE ]*2 / 3;
		end

		SprObj[ n ].vx = SprObj[ src ].x*100;
		SprObj[ n ].vy = SprObj[ src ].y*100;
		SprObj[ n ].w[ 2+ORIGIN_ONE ] = SprObj[ src ].w[ 2+ORIGIN_ONE ];
		SprObj[ n ].w[ 3+ORIGIN_ONE ] = _GetRand()%2;
	end

end


function enemy06_main( n )
	-- //-----------------------------
	-- //ウラカン・ラナ：メイン
	-- //-----------------------------
	local i,ptn;

	SprObj[ n ].vx = SprObj[ n ].vx + SprObj[ n ].w[0+ORIGIN_ONE];
	SprObj[ n ].vy = SprObj[ n ].vy + SprObj[ n ].w[1+ORIGIN_ONE];

	SprObj[ n ].x = SprObj[ n ].vx/100;
	SprObj[ n ].y = SprObj[ n ].vy/100;

	SprObj[ n ].w[3+ORIGIN_ONE] = SprObj[ n ].w[3+ORIGIN_ONE] + _GetRand()%3;

	if ( SprObj[ n ].w[3+ORIGIN_ONE]%20<10) then
		ptn=0;
	else
		ptn=1;
	end

	if ( SprObj[ n ].w[2+ORIGIN_ONE]==0) then
		_VisorPutSprite(SprObj[ n ].x,SprObj[ n ].y,60,PRIO_TEKI+4,0);

	elseif ( SprObj[ n ].w[2+ORIGIN_ONE]==1) then
		_VisorPutSprite(SprObj[ n ].x,SprObj[ n ].y,61+ptn,PRIO_TEKI+4,0);

	else
		_VisorPutSprite(SprObj[ n ].x,SprObj[ n ].y,63+ptn,PRIO_TEKI+4,0);
	end

	if ( SprObj[ n ].hp<=0) then
		if ( SprObj[ n ].w[2+ORIGIN_ONE]==2) then
			bomb01_init(n,0,0,0);
			kill_obj(n);
			return;
		end

		SprObj[ n ].w[2+ORIGIN_ONE] = SprObj[ n ].w[2+ORIGIN_ONE] +1;

		for i=0,SprObj[ n ].w[2+ORIGIN_ONE]*2-ORIGIN_ONE do
			enemy06_init(n);
		end

		kill_obj(n);
			_PlaySound( 3 );
		return;
	end

	if ( (SprObj[ n ].x <= -32) or (SprObj[ n ].x >= 160+32) or (SprObj[ n ].y >= 160+32) ) then
		kill_obj(n);
		return;
	end

end


-- //---------------------------------------------------------
-- //　　効果
-- //---------------------------------------------------------
function clear_enemy_shot( void )

	-- //--------------
	-- //敵の弾全消し
	-- //--------------
	local i;

	for i=0,enObjMax-ORIGIN_ONE do

		local jj = i+1;

		if( SprObj[ jj ].category == 5 ) then
			eff_star_init( jj , 0 , 0 );
			kill_obj( jj );
		end
	end

end


function eff_star_init( s, x, y)
	-- //--------------
	-- //きらきら：初期化
	-- //--------------

	n = GetObjNum();

	if( n == -1 ) then 
		return;
	end

	SprObj[ n ].category = 6;

	SprObj[ n ].b[ 0 ] = 0;
	SprObj[ n ].x = SprObj[ s ].x + x;
	SprObj[ n ].y = SprObj[ s ].y + y;

end



function eff_star_main( n )

	-- //--------------
	-- //きらきら：メイン
	-- //--------------

	local spr = 55;

	SprObj[ n ].b[0+ORIGIN_ONE] = SprObj[ n ].b[0+ORIGIN_ONE] + 1;

	spr = spr + ( SprObj[ n ].b[0+ORIGIN_ONE] / 3 );

	if( spr >= 60 ) then
		kill_obj( n );
		return;
	end

	SprObj[ n ].y = SprObj[ n ].y -1;

	_VisorPutSpriteVisor (SprObj[ n ].x , SprObj[ n ].y , spr , PRIO_EFF , GBATR_ALPHA_PLUS );
end


function bomb01_init( s , x, y , eneType )

	-- //--------------
	-- //爆発：初期化
	-- //--------------
	local n = 0;

	if ( s == -1 ) then
		-- //強制的に爆発パターンを出す。
		n = 1;
		s = enMyShipIndex;
--@@		gxUtil::MemSet(&SprObj[1],0,sizeof(StObj));
--毒
		SprObj[1+ORIGIN_ONE].category    = 0;		-- //生存フラグ
		SprObj[1+ORIGIN_ONE].x = 0;
		SprObj[1+ORIGIN_ONE].y = 0;
		SprObj[1+ORIGIN_ONE].vx   = 0;
		SprObj[1+ORIGIN_ONE].vy   = 0;
		SprObj[1+ORIGIN_ONE].eneType = 0;		-- //種類
		SprObj[1+ORIGIN_ONE].hp      = 0;		-- //耐久力

		SprObj[1+ORIGIN_ONE].b = { 0,0,0,0,0,0,0,0 };
		SprObj[1+ORIGIN_ONE].w = { 0,0,0,0 };
		SprObj[1+ORIGIN_ONE].l = { 0,0 };

	else
		n = GetObjNum();

		if ( n == -1 ) then
			return;
		end
	end

	SprObj[ n ].category  = 4;
	SprObj[ n ].b[ 0 ] = 0;
	SprObj[ n ].eneType = eneType;
	SprObj[ n ].x = SprObj[ s ].x + x;
	SprObj[ n ].y = SprObj[ s ].y + y;

	if( eneType == 0 ) then
	
		_PlaySound( 2 );
	
	else
	
		_PlaySound( 3 );
	end

end


function bomb01_main( n )

	-- //--------------
	-- //爆発：メイン
	-- //--------------

	local s , spr = 5;
	local col = 0xffffffff;

	SprObj[ n ].b[0+ORIGIN_ONE] = SprObj[ n ].b[0+ORIGIN_ONE] + 1;

	s = SprObj[ n ].b[0+ORIGIN_ONE];

	if( SprObj[ n ].eneType == 0 ) then 
		-- //爆発A
		if( s < 2 ) then
			spr = 38;
		elseif ( s < 4 ) then
			spr = 39;
		elseif ( s < 6 ) then
			spr = 40;
		elseif ( s < 8 ) then
			spr = 2;
		elseif ( s < 16 ) then
			spr = 3;
		elseif( s < 24 ) then
			spr = 4;
		else
			kill_obj( n );
			return;
		end

		_VisorPutSpriteVisor(
			SprObj[ n ].x+(-1+_GetRand()%2),
			SprObj[ n ].y+(-1+_GetRand()%2)-s,
			spr,
			PRIO_EFF,
			GBATR_ALPHA_PLUS,
			0x80ffffff );

	else
		-- //爆発B
		col = 0xff*( 25 - s ) / 25;

		if( s < 6 ) then
			spr = 38;
			_VisorPutSpriteVisor( SprObj[ n ].x , SprObj[ n ].y , spr , PRIO_EFF , GBATR_ALPHA_PLUS , ARGB(col,0xff,0xff,0xff) );

		elseif( s < 12 ) then
			spr = 39;
			_VisorPutSpriteVisor(SprObj[ n ].x,SprObj[ n ].y,spr,PRIO_EFF,GBATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff) );

		elseif( s < 18 ) then
			spr = 40;
			_VisorPutSpriteVisor(SprObj[ n ].x,SprObj[ n ].y,spr,PRIO_EFF,GBATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff) );

		elseif( s < 24 ) then
			spr = 41;
			_VisorPutSpriteVisor(SprObj[ n ].x,SprObj[ n ].y,spr,PRIO_EFF,GBATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff) );

		else
			kill_obj( n );
			return;
		end

	end

end


-- //---------------------------------
-- //ゲーム固有のサブルーチン
-- //---------------------------------

function GetObjNum()

	-- //------------------------------------
	-- //あいているオブジェクトナンバーを返す
	-- //------------------------------------

	for i=2+ORIGIN_ONE,enObjMax-ORIGIN_ONE do 
		if( SprObj[ i ].category == 0 ) then
			SprObj[ i ] = {StObj};

			SprObj[ i ].category    = 0;	-- //生存フラグ
			SprObj[ i ].x        = 0;
			SprObj[ i ].y        = 0;	-- //Ｘ、Ｙ
			SprObj[ i ].vx       = 0;	-- //増分値など
			SprObj[ i ].vy       = 0;	-- //増分値など
			SprObj[ i ].eneType  = 0;	-- //種類
			SprObj[ i ].hp       = 0;	-- //耐久力

			SprObj[ i ].b = { 0,0,0,0,0,0,0,0 };
			SprObj[ i ].w = { 0,0,0,0 };
			SprObj[ i ].l = { 0,0 };

			return i;
		end
	end

	return -1;

end


function kill_obj( n )

	-- //-----------------
	-- //オブジェクト削除
	-- //-----------------
	SprObj[ n ].category = 0;
end


function hit_check( n )

	-- //-----------------
	-- //あたり判定
	-- //-----------------

	-- //画面外

	if( SprObj[ n ].y <= 8 ) then
		return gxFalse;
	end


	for j=0,enObjMax-ORIGIN_ONE do

		i = j + ORIGIN_ONE;

		if( SprObj[ i ].category == 0 ) then
			goto continue;
		end

		if( i == n ) then
			goto continue;
		end

		if( SprObj[ i ].category ~= 3 ) then 
			goto continue;
		end

		if ( _GetAbs( SprObj[ i ].x - SprObj[ n ].x ) <= 16 ) then

			if( _GetAbs(SprObj[ i ].y - SprObj[ n ].y) <= 16 ) then
				-- //HIT検出

				if( ( SprObj[ i ].eneType >=1 ) and ( SprObj[ i ].eneType <= 3) ) then

					bomb01_init(n,0,0,0);

				elseif( SprObj[ i ].eneType == 5 ) then

					eff_star_init(n,0,0);

				else

					bomb01_init(i,0,0,0);

				end

				SprObj[ i ].hp = SprObj[ i ].hp - 1;

				g_GameSystem.Score = g_GameSystem.Score + 1;

				kill_obj( n );

				if( SprObj[ i ].hp <= 0 ) then 
					SprObj[ i ].hp = 0;
				end

				return gxTrue;
			end	--//if
		end	--//if

		::continue::

	end	--//for

	return gxFalse;

end


function hit_check2( n )

	-- //-----------------------------
	-- //敵弾ｖｓ自分のあたり判定検出
	-- //-----------------------------

	i = enMyShipIndex;

	if ( SprObj[ i ].eneType ~= 3 ) then
		return gxFalse;
	end

	if ( _GetAbs( SprObj[ i ].x-SprObj[ n ].x ) <= 2 ) then

		if(_GetAbs( SprObj[ i ].y-SprObj[ n ].y ) <= 3 ) then 
			-- //HIT検出
			-- //clear_enemy_shot();
			bomb01_init( -1 , 0 , 0 , 1 );
			kill_obj( i );
			return 1;
		end
	end

	return gxTrue;
end


function GetStylus( x, y )
-- @@@ {
-- @@@ 	-- //-----------------------------
-- @@@ 	-- //スタイラスの情報を得る
-- @@@ 	-- //-----------------------------
-- @@@ 	static gxPos m_Old,m_Joy;
-- @@@ 	Sint32 mx,my;
-- @@@ 
-- @@@ 	mx = gxLib::Joy(0)->mx;
-- @@@ 	my = gxLib::Joy(0)->my;
-- @@@ 
-- @@@ 	if( gxLib::Joy(0)->trg&MOUSE_L )
-- @@@ 	{
-- @@@ 		m_Old.x = *x;
-- @@@ 		m_Old.y = *y;
-- @@@ 		m_Joy.x = mx;
-- @@@ 		m_Joy.y = my;
-- @@@ 	}
-- @@@ 
-- @@@ 	if( gxLib::Joy(0)->psh&MOUSE_L )
-- @@@ 	{
-- @@@ 		Sint32 ax,ay;
-- @@@ 		ax = (mx - m_Joy.x)/2;
-- @@@ 		ay = (my - m_Joy.y)/2;
-- @@@ 
-- @@@ 		ax = 5*ax/4;
-- @@@ 		ay = 5*ay/4;
-- @@@ 		*x = (m_Old.x + ax);
-- @@@ 		*y = (m_Old.y + ay);
-- @@@ 	}
-- @@@ 
-- @@@ -- //	*x = Joy[0+ORIGIN_ONE].mx;
-- @@@ -- //	*y = Joy[0+ORIGIN_ONE].my;
-- @@@ 
-- @@@ -- //	*x = gxLib::Joy(0)->mx/2;
-- @@@ -- //	*y = gxLib::Joy(0)->my/2;
-- @@@ 
-- @@@ #ifdef _DEBUG
-- @@@ 	gxLib::Printf( 32,32,32,ATR_DFLT,ARGB_DFLT,"%d,%d",*x,*y);
-- @@@ #endif
-- @@@ 
-- @@@ 	if( gxLib::Joy(0)->psh&MOUSE_L )
-- @@@ 	{
-- @@@ 		return 1,x,y;
-- @@@ 	}

	return 0,x,y;

end


-- //-------------------------------
-- //SINテーブル
-- //右を０度として右回りに３６０°
-- //-------------------------------

sin_tbl={
	   0,	   1,	   3,	   5,	   6,	   8,	  10,	  12,	  13,	  15,
	  17,	  19,	  20,	  22,	  24,	  25,	  27,	  29,	  30,	  32,
	  34,	  35,	  37,	  39,	  40,	  42,	  43,	  45,	  46,	  48,
	  50,	  51,	  52,	  54,	  55,	  57,	  58,	  60,	  61,	  62,
	  64,	  65,	  66,	  68,	  69,	  70,	  71,	  73,	  74,	  75,
	  76,	  77,	  78,	  79,	  80,	  81,	  82,	  83,	  84,	  85,
	  86,	  87,	  88,	  89,	  89,	  90,	  91,	  92,	  92,	  93,
	  93,	  94,	  95,	  95,	  96,	  96,	  97,	  97,	  97,	  98,
	  98,	  98,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,
	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  98,
	  98,	  98,	  97,	  97,	  97,	  96,	  96,	  95,	  95,	  94,
	  93,	  93,	  92,	  92,	  91,	  90,	  89,	  89,	  88,	  87,
	  86,	  85,	  84,	  83,	  82,	  81,	  80,	  79,	  78,	  77,
	  76,	  75,	  74,	  73,	  71,	  70,	  69,	  68,	  66,	  65,
	  64,	  62,	  61,	  60,	  58,	  57,	  55,	  54,	  52,	  51,
	  49,	  48,	  46,	  45,	  43,	  42,	  40,	  39,	  37,	  35,
	  34,	  32,	  30,	  29,	  27,	  25,	  24,	  22,	  20,	  19,
	  17,	  15,	  13,	  12,	  10,	   8,	   6,	   5,	   3,	   1,
	   0,	  -1,	  -3,	  -5,	  -6,	  -8,	 -10,	 -12,	 -13,	 -15,
	 -17,	 -19,	 -20,	 -22,	 -24,	 -25,	 -27,	 -29,	 -30,	 -32,
	 -34,	 -35,	 -37,	 -39,	 -40,	 -42,	 -43,	 -45,	 -46,	 -48,
	 -50,	 -51,	 -52,	 -54,	 -55,	 -57,	 -58,	 -60,	 -61,	 -62,
	 -64,	 -65,	 -66,	 -68,	 -69,	 -70,	 -71,	 -73,	 -74,	 -75,
	 -76,	 -77,	 -78,	 -79,	 -80,	 -81,	 -82,	 -83,	 -84,	 -85,
	 -86,	 -87,	 -88,	 -89,	 -89,	 -90,	 -91,	 -92,	 -92,	 -93,
	 -93,	 -94,	 -95,	 -95,	 -96,	 -96,	 -97,	 -97,	 -97,	 -98,
	 -98,	 -98,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,
	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -98,
	 -98,	 -98,	 -97,	 -97,	 -97,	 -96,	 -96,	 -95,	 -95,	 -94,
	 -93,	 -93,	 -92,	 -92,	 -91,	 -90,	 -89,	 -89,	 -88,	 -87,
	 -86,	 -85,	 -84,	 -83,	 -82,	 -81,	 -80,	 -79,	 -78,	 -77,
	 -76,	 -75,	 -74,	 -73,	 -71,	 -70,	 -69,	 -68,	 -66,	 -65,
	 -64,	 -62,	 -61,	 -60,	 -58,	 -57,	 -55,	 -54,	 -52,	 -51,
	 -49,	 -48,	 -46,	 -45,	 -43,	 -42,	 -40,	 -39,	 -37,	 -35,
	 -34,	 -32,	 -30,	 -29,	 -27,	 -25,	 -24,	 -22,	 -20,	 -19,
	 -17,	 -15,	 -13,	 -12,	 -10,	  -8,	  -6,	  -5,	  -3,	  -1,
};

-- //-------------------------------
-- //COSテーブル
-- //右を０度として右回りに３６０°
-- //-------------------------------
cos_tbl={
	 100,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  98,
	  98,	  98,	  97,	  97,	  97,	  96,	  96,	  95,	  95,	  94,
	  93,	  93,	  92,	  92,	  91,	  90,	  89,	  89,	  88,	  87,
	  86,	  85,	  84,	  83,	  82,	  81,	  80,	  79,	  78,	  77,
	  76,	  75,	  74,	  73,	  71,	  70,	  69,	  68,	  66,	  65,
	  64,	  62,	  61,	  60,	  58,	  57,	  55,	  54,	  52,	  51,
	  49,	  48,	  46,	  45,	  43,	  42,	  40,	  39,	  37,	  35,
	  34,	  32,	  30,	  29,	  27,	  25,	  24,	  22,	  20,	  19,
	  17,	  15,	  13,	  12,	  10,	   8,	   6,	   5,	   3,	   1,
	   0,	  -1,	  -3,	  -5,	  -6,	  -8,	 -10,	 -12,	 -13,	 -15,
	 -17,	 -19,	 -20,	 -22,	 -24,	 -25,	 -27,	 -29,	 -30,	 -32,
	 -34,	 -35,	 -37,	 -39,	 -40,	 -42,	 -43,	 -45,	 -46,	 -48,
	 -50,	 -51,	 -52,	 -54,	 -55,	 -57,	 -58,	 -60,	 -61,	 -62,
	 -64,	 -65,	 -66,	 -68,	 -69,	 -70,	 -71,	 -73,	 -74,	 -75,
	 -76,	 -77,	 -78,	 -79,	 -80,	 -81,	 -82,	 -83,	 -84,	 -85,
	 -86,	 -87,	 -88,	 -89,	 -89,	 -90,	 -91,	 -92,	 -92,	 -93,
	 -93,	 -94,	 -95,	 -95,	 -96,	 -96,	 -97,	 -97,	 -97,	 -98,
	 -98,	 -98,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,
	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -98,
	 -98,	 -98,	 -97,	 -97,	 -97,	 -96,	 -96,	 -95,	 -95,	 -94,
	 -93,	 -93,	 -92,	 -92,	 -91,	 -90,	 -89,	 -89,	 -88,	 -87,
	 -86,	 -85,	 -84,	 -83,	 -82,	 -81,	 -80,	 -79,	 -78,	 -77,
	 -76,	 -75,	 -74,	 -73,	 -71,	 -70,	 -69,	 -68,	 -66,	 -65,
	 -64,	 -62,	 -61,	 -60,	 -58,	 -57,	 -55,	 -54,	 -52,	 -51,
	 -49,	 -48,	 -46,	 -45,	 -43,	 -42,	 -40,	 -39,	 -37,	 -35,
	 -34,	 -32,	 -30,	 -29,	 -27,	 -25,	 -24,	 -22,	 -20,	 -19,
	 -17,	 -15,	 -13,	 -12,	 -10,	  -8,	  -6,	  -5,	  -3,	  -1,
	   0,	   1,	   3,	   5,	   6,	   8,	  10,	  12,	  13,	  15,
	  17,	  19,	  20,	  22,	  24,	  25,	  27,	  29,	  30,	  32,
	  34,	  35,	  37,	  39,	  40,	  42,	  43,	  45,	  46,	  48,
	  50,	  51,	  52,	  54,	  55,	  57,	  58,	  60,	  61,	  62,
	  64,	  65,	  66,	  68,	  69,	  70,	  71,	  73,	  74,	  75,
	  76,	  77,	  78,	  79,	  80,	  81,	  82,	  83,	  84,	  85,
	  86,	  87,	  88,	  89,	  89,	  90,	  91,	  92,	  92,	  93,
	  93,	  94,	  95,	  95,	  96,	  96,	  97,	  97,	  97,	  98,
	  98,	  98,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,
};

-- //-------------------------
-- //スプライト定義テーブル
-- //-------------------------

SprVisor = {
	-- //スプライト定義テーブル
	{enTpgSpr,0,0,2,2, 8,8},	-- //0　バスター
	{enTpgSpr,2,0,1,1, 4,4},	-- //1　ショット
	{enTpgSpr,4,0,2,2, 8,8},	-- //2　爆発A_0
	{enTpgSpr,6,0,2,2, 8,8},	-- //3　爆発A_1
	{enTpgSpr,8,0,2,2, 8,8},	-- //4　爆発A_2
	{enTpgSpr,3,0,1,2, 4,0},	-- //5　バスターレーザー

	{enTpgSpr,2,1,1,1, 4,4},	-- //6　敵ショット
	{enTpgSpr,2,3,1,1, 4,4},	-- //7　バスターレーザーヒット1
	{enTpgSpr,3,3,1,1, 4,4},	-- //8　バスターレーザーヒット2

	{enTpgSpr,0,2,2,2, 8,8},	-- //9　フロウジョン

	-- //地形
	{enTpgSpr,15,0,5,1, 0,0},	-- //10　バックグラウンド0
	{enTpgSpr,15,1,5,1, 0,0},	-- //11　バックグラウンド1
	{enTpgSpr,15,2,5,1, 0,0},	-- //12　バックグラウンド2
	{enTpgSpr,15,3,5,1, 0,0},	-- //13　バックグラウンド3
	{enTpgSpr,15,4,5,1, 0,0},	-- //14　バックグラウンド4
	{enTpgSpr,15,5,5,1, 0,0},	-- //15　バックグラウンド5
	{enTpgSpr,15,6,5,1, 0,0},	-- //16　バックグラウンド6
	{enTpgSpr,15,7,5,1, 0,0},	-- //17　バックグラウンド7
	{enTpgSpr,15,8,5,1, 0,0},	-- //18　バックグラウンド8
	{enTpgSpr,15,9,5,1, 0,0},	-- //19　バックグラウンド9 -----
	{enTpgSpr,15,10,5,1, 0,0},	-- //20　バックグラウンド10
	{enTpgSpr,15,11,5,1, 0,0},	-- //21　バックグラウンド11
	{enTpgSpr,15,12,5,1, 0,0},	-- //22　バックグラウンド12
	{enTpgSpr,15,13,5,1, 0,0},	-- //23　バックグラウンド13
	{enTpgSpr,15,14,5,1, 0,0},	-- //24　バックグラウンド14
	{enTpgSpr,15,15,5,1, 0,0},	-- //25　バックグラウンド15
	{enTpgSpr,15,16,5,1, 0,0},	-- //26　バックグラウンド16
	{enTpgSpr,15,17,5,1, 0,0},	-- //27　バックグラウンド17
	{enTpgSpr,15,18,5,1, 0,0},	-- //28　バックグラウンド18
	{enTpgSpr,15,19,5,1, 0,0},	-- //29　バックグラウンド19 -----
	{enTpgSpr,10,0,5,1, 0,0},	-- //30　バックグラウンド20 -----

	{enTpgSpr,4,4,2,2, 8,8},		-- //31　敵ザコ　スタンピート
	{enTpgSpr,0,4,4,4, 16,16},	-- //32　敵ボス　ケブラーダ

	{enTpgSpr,6,4,3,2, 16,16},	-- //33　敵ボス　ケブラーダ用オプションウイング左
	{enTpgSpr,6,6,3,2, 16,16},	-- //34　敵ボス　ケブラーダ用オプションウイング右
	{enTpgSpr,4,6,2,2, 8,8},		-- //35　敵ボス　ケブラーダ用オプションキャノン

	{enTpgSpr,9,4,1,2, 4,4},		-- //36　敵　　　レーザー弾
	{enTpgSpr,9,6,2,2, 8,8},		-- //37　敵　　　最強弾

	{enTpgSpr,0 ,8,4,4, 16,16},	-- //38　爆発B_0
	{enTpgSpr,4 ,8,4,4, 16,16},	-- //39　爆発B_1
	{enTpgSpr,8 ,8,4,4, 16,16},	-- //40　爆発B_2
	{enTpgSpr,11,4,4,4, 16,16},	-- //41　爆発B_3

	{enTpgSpr,2,2,1,1, 0,0},		-- //42　数字０
	{enTpgSpr,3,2,1,1, 0,0},		-- //43　数字１
	{enTpgSpr,4,2,1,1, 0,0},		-- //44　数字２
	{enTpgSpr,5,2,1,1, 0,0},		-- //45　数字３
	{enTpgSpr,6,2,1,1, 0,0},		-- //46　数字４
	{enTpgSpr,7,2,1,1, 0,0},		-- //47　数字５
	{enTpgSpr,8,2,1,1, 0,0},		-- //48　数字６
	{enTpgSpr,9,2,1,1, 0,0},		-- //49　数字７
	{enTpgSpr,10,2,1,1, 0,0},	-- //50　数字８
	{enTpgSpr,11,2,1,1, 0,0},	-- //51　数字９
	{enTpgSpr,12,2,2,1, 0,0},	-- //52　数字00

	{enTpgSpr,12,8,3,3, 12,12},	-- //53　ジャイアントスイング
	{enTpgSpr,4,3,1,1, 0,0},		-- //54　残機表示

	{enTpgSpr,5,14,1,1, 0,0},	-- //55　きらきら
	{enTpgSpr,6,14,1,1, 0,0},	-- //56　きらきら
	{enTpgSpr,7,14,1,1, 0,0},	-- //57　きらきら
	{enTpgSpr,8,14,1,1, 0,0},	-- //58　きらきら
	{enTpgSpr,9,14,1,1, 0,0},	-- //59　きらきら

	{enTpgSpr,0,12,3,3, 12,12},	-- //60　ウラカン・ラナ
	{enTpgSpr,3,12,2,2, 8,8},	-- //61　ウラカン・ラナ（中１）
	{enTpgSpr,5,12,2,2, 8,8},	-- //62　ウラカン・ラナ（中２）
	{enTpgSpr,3,14,1,1, 4,4},	-- //63　ウラカン・ラナ（小１）
	{enTpgSpr,4,14,1,1, 4,4},	-- //64　ウラカン・ラナ（小２）

	{enTpgSpr,0,15,10,1, 0,0},	-- //65　ゲームオーバー
	{enTpgSpr,0,17,11,2, 0,0},	-- //66　VISORIZER

	{enTpgSpr,0,16,9,1, 0,0},	-- //67　HISCORE
	{enTpgSpr,2,16,7,1, 0,0},	-- //68　SCORE

	{enTpgSpr,11,12,4,2, 0,0},	-- //69　SCORE

};

