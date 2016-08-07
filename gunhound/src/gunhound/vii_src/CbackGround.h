#define enBgTypeMax (4)

//#define SCRX (SCR_W*30)						//横画面サイズ(9600)VMPのデータ数があるので上限は変えないように
//#define SCRY (SCR_H*6)						//縦画面サイズ(1440)
//#define MAPTIP_POS_CALC(a,b) ((b)*(SCRX/MAP_BLOCK_SIZE)+(a))
//#define MAP_POSCALC(a,b) ((b)*(SCRX/8)+(a))	//あたり判定マップ位置
//#define MAPMAX_BLOCK_MAX  (SCRX/MAP_BLOCK_SIZE*SCRY/MAP_BLOCK_SIZE)	//120000

#define MAP_BLOCK_SIZE (16)					//１ブロックのサイズ

//1画面のデータサイズ（マックス値計算用で、実際のサイズとは異なる）
#define MAP_WIDTH  (320*30)	//横画面サイズ(9600)VMPのデータ数があるので上限は変えないように
#define MAP_HEIGHT (240*6)	//縦画面サイズ(1440)

//１テクスチャ(256x256)に収まるブロックの数
#define MAP256_BLOCK_MAX  (256/MAP_BLOCK_SIZE*256/MAP_BLOCK_SIZE)	//64

//.MAPデータのファイルサイズ
#define MAP_FILE_SIZE (sizeof(Sint16)* (MAP_WIDTH*MAP_HEIGHT)/(MAP_BLOCK_SIZE*MAP_BLOCK_SIZE))

//.VMPデータのファイルサイズ
#define VMP_DATA_SIZE ((MAP_WIDTH*MAP_HEIGHT)/(8*8)) 		//(40*10*30*10)			//あたり判定マップサイズ


#define STAR_MAX (200)
//---------------------------------------------------
//ＢＧ関連のアタリ判定用マッピングデータ(*.VMP)
//---------------------------------------------------
typedef struct StVitmap {
	char format[8];					//VII_HITMAP_DATA0
	Sint8 dummy[4];
	Sint16 sWidth;					//想定横幅（9660）
	Sint16 sHeight;					//想定縦幅（9660）
	Sint8 uHantei[VMP_DATA_SIZE];	//あたり判定バッファ
} StVitmap;

//---------------------------------------------------
//ＢＧ関連のグラフィックチップマッピングデータ(*.MAP)
//---------------------------------------------------
typedef struct MapFileData_t {
/*
	char fmt[14];
	char version;
	char num;
	char filename[16];
	Uint16 ptn[MAP_FILE_SIZE];
*/
	char fmt[8];
	char dummy[2];
	short sWidth;	//ver.4以降で追加
	short sHeight;	//ver.4以降で追加
	char version;
	char num;
	char filename[16];
	Uint16 ptn[MAP_FILE_SIZE];

} MapFileData_t;


typedef struct StRaster {
	Sint32 sSpeed;	//ずらし速度(8)
	Sint32 sWidth;	//細かさ(32)
	Sint32 sDetail;	//ずらし幅MAX(4)
	gxBool bEnable;	//有効／無効(False)
}StRaster;

typedef struct StMapData
{
	//----------------------------------------------------------
	MapFileData_t tips;	//外部ファイルからデータを入力される
	//----------------------------------------------------------
	Sint32   page;
	Sint32   x,y;
	StRaster Raster;
	gxBool   disp;
}StMapData;

class CBackGround
{
public:
	Sint32 offset_x;
	Sint32 offset_y;

	CBackGround();
	~CBackGround();

	void action();
	void Draw();
	void scroll_control( Sint32 x,Sint32 y );
	gxBool LoadVmpFile(char *filename);
	gxBool LoadVmpData(Uint8 *pData);

	void LoadMapFile(Sint32 n,gxChar *filename);
	void LoadMapData(Sint32 n, gxChar *filename);

	Sint32 get_kabetype(Sint32 x,Sint32 y);
	Sint32 get_kabetype8(Sint32 x,Sint32 y);

	void SetSkyColor(Sint32 HalfAvr, Sint32 col1,Sint32 Col2);

	void ResetFilter()
	{
		//フィルター（背景色の色味）をリセットする
		for(Sint32 ii=0;ii<enBgTypeMax; ii++)
		{
			SetLayerFilter( ii , 0xFFFFFFFF );
			m_bgAtribute[ii] = ATR_DFLT;
		}
	}

	void SetFilter(Sint32 col)
	{
		//フィルター（背景色の色味）をセットする
		for(Sint32 ii=0;ii<enBgTypeMax; ii++)
		{
			SetLayerFilter( ii , col );
			m_bgAtribute[ii] = ATR_DFLT;
		}
	}

	void SetLayerFilter(Sint32 n , Sint32 col , Sint32 sAtr=ATR_DFLT)
	{
		//フィルター（背景色の色味）をセットする
		m_bgFilterColor[n] = col;
		m_bgAtribute[n]    = sAtr;
	}

	void getscroll( Sint32 &x,Sint32 &y )
	{
		//スクロールイチを取得する
		x = scx_;
		y = scy_;
	}
	void setScroll100( Sint32 x,Sint32 y ){
		scx_ = x/100;
		scy_ = y/100;
	}

	void set_atari_flag(){
		no_judge_flag = gxFalse;
	}
	void cancel_atari_flag(){
		no_judge_flag = gxTrue;
	}
	void SetPos( Sint32 n,Sint32 x,Sint32 y ){
		//各ＢＧレイヤーの座標を設定する
		if(n<0 || n>=enBgTypeMax) return;	//安全ソフト
		stMapData[n].x = x;
		stMapData[n].y = y;
	}

	Sint32 get_vmpdat(Sint32 x,Sint32 y)
	{
		//当たりＶＭＰデータを得る
		if(x<0 || y<0) return 0;
		if(x>=(m_sScreenX/8) || y>=(m_sScreenY/8)) return 0;
		Uint32 u8 = (Uint8)stVitmap.uHantei[ getVmpPos(x,y) ];
		Sint32 s32 = (Sint32)u8;
		return s32;
	}

	void make_vmptip(Sint32 x,Sint32 y,Sint32 kabetype=3)
	{
		//当たりＶＭＰデータを作成
		if(x<0 || y<0) return;
		if(x>=(m_sScreenX/8) || y>=(m_sScreenY/8)) return;
		stVitmap.uHantei[ getVmpPos(x,y) ] = kabetype;
	}

	void del_vmptip(Sint32 x,Sint32 y){
		//当たりＶＭＰデータを削除
		if(x<0 || y<0) return;
		if(x>=(m_sScreenX/8) || y>=(m_sScreenY/8)) return;
		stVitmap.uHantei[ getVmpPos(x,y) ] = 0;
	}

	void del_maptip(Sint32 n,Sint32 x,Sint32 y){
		//グラフィックマップチップを削除する
		if(x<0 || y<0 || x>=stMapData[n].tips.sWidth/*m_sScreenX*//MAP_BLOCK_SIZE || y>=stMapData[n].tips.sHeight/*m_sScreenY*//MAP_BLOCK_SIZE) return;
		short *q;
		q = (short*)&stMapData[n].tips.ptn[0];
		q[getMapTipPos( n, x , y )] = 0;
	}

	void set_disp(Sint32 n,gxBool flag)
	{
		stMapData[n].disp = flag;
	}


	void SetMapScreen(Sint32 x , Sint32 y )
	{
		m_sScreenX = x;
		m_sScreenY = y;
	}

	Sint32 getWidth( )
	{
		return m_sScreenX;
	}

	Sint32 getHeight( )
	{
		return m_sScreenY;
	}

	Sint32 getMapTipPos( Sint32 n , Sint32 x, Sint32 y )
	{
		return y*(stMapData[n].tips.sWidth/MAP_BLOCK_SIZE)+x;
	}

	Sint32 getVmpPos(Sint32 x, Sint32 y)
	{
		Sint32 n = y*( m_sScreenX / 8 )+x;

		return  ( n < 0 || n >= VMP_DATA_SIZE)? 0 : n;
	}

	void SetPriority(Sint32 n , Sint32 prio )
	{
		m_sChangePrio[n] = prio;
	}

	void DebugTipDraw(gxBool bFlag = gxTrue)
	{
		m_bDebugTipDraw = bFlag;
	}

	void SetRaster( Sint32 sLayer , StRaster* p_stRaster=NULL );

	gxBool IsBgObj( Sint32 x, Sint32 y);

private:

	typedef struct StStar
	{
		Sint16 x,y,layer;
	}StStar;

	//------------------------------------------------

	Sint32 m_sScreenX,m_sScreenY;
	Sint32 m_sChangePrio[enBgTypeMax];

	//------------------------------------------------

	gxBool no_judge_flag;
	Sint32 scx_,scy_;	//スクロール座標
	Sint32 sx_,sy_;	//あたり判定の座標
	Sint32 repeat_x_goal_;
	Sint32 repeat_x_start_;
	StVitmap stVitmap;
	StMapData stMapData[enBgTypeMax];
	void debug_mode();
	void pos_cnv(Sint32 &x,Sint32 &y);
	void debugMapDraw(Sint32 x,Sint32 y,Sint32 no);
	void star_screen();
//	short stx[STAR_MAX],sty[STAR_MAX],slr[STAR_MAX];
	StStar stStar[STAR_MAX];
	void PutSprMapParts(Sint32 x,Sint32 y,Sint32 prio,Sint32 no,Sint32 bg_no,Sint32 sFilter ,Sint32 sAtr);
	void disp(Sint32 pg);

	Sint32 m_bgFilterColor[enBgTypeMax];	//フィルター色
	Sint32 m_bgAtribute[enBgTypeMax];	//フィルター色
	Sint32 sColor[2];
	Sint32 sHalfAvr;

	gxBool m_bDebugTipDraw;
	gxBool m_bDummy;
};


enum
{
	//-----------------------------------
	//壁のタイプ
	//-----------------------------------

	enKabeTypeNone = 0,
	enKabeTypeBlock  = 1,
	enKabeTypeBlock2 = 3,
	enKabeTypehalf   = 2,

	enKabeTypeTriangleUL   = 12,
	enKabeTypeTriangleUR   = 13,
	enKabeTypeTriangleDR   = 14,
	enKabeTypeTriangleDL   = 15,
	enKabeTypeUnderGround  = 16,

	enKabeTypeGake         = 4,
//	enKabeTypeThrough = 4,
};

