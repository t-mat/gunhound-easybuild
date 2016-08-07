//------------------------------------------------------
//
// ステージの基底クラス
//
//------------------------------------------------------

#include "../GunHound/Cmenu.h"
#include "../effect/EffectBase.h"

enum {
	MaxEnemyNum = 256,
	enAddTimeMAX = 32,
};

class CStageBase
{
public:

	CStageBase();
	~CStageBase();
	ghPos m_Pos;

	Sint32 m_sZoneTimer;
	Sint32 m_sClearTimer;
	Sint32 m_sBGM;

	virtual void Init();
	virtual void Action();
	virtual void End();
	virtual void Draw()
	{

	}

	virtual void BgCommon(Sint32 x=0,Sint32 y=0);

	//-----------------------------------------------------
	//ゲームスタートを通達
	//-----------------------------------------------------
	void SetGamePlay(gxBool bFlag)
	{
		m_bGamePlay = bFlag;
	}
	//-----------------------------------------------------
	//エリアの変更を行う（ゾーンシーケンスもリセットされる）
	//-----------------------------------------------------
	void ChangeArea(Sint32 area);

	//-----------------------------------------------------
	//現在のエリアを得る
	//-----------------------------------------------------
	Sint32  GetArea( );

	//-----------------------------------------------------
	//現在のエリア／ゾーンシーケンスを設定
	//-----------------------------------------------------
	void SetZoneSeq( Sint32 n);

	//-----------------------------------------------------
	//現在のエリア／ゾーンシーケンスを得る
	//-----------------------------------------------------
	Sint32 GetZoneSeq( );

	//-----------------------------------------------------
	//特定ゾーンシーケンスに入っているか？
	//-----------------------------------------------------
	gxBool IsZoneSeq( Sint32 n);

	//-----------------------------------------------------
	//エリア／ゾーンタイマーをリセット
	//-----------------------------------------------------
	void ResetZoneTimer( );

	//-----------------------------------------------------
	//フェード終了したか？
	//-----------------------------------------------------
	gxBool IsFadeEnd();

	//-----------------------------------------------------
	//フェードイン
	//-----------------------------------------------------
	void SetFadeIn(Sint32 n=8,Sint32 col=0xff010101);

	//-----------------------------------------------------
	//フェードアウト
	//-----------------------------------------------------
	void SetFadeOut(Sint32 n=8,Sint32 col=0xFF010101 );

	//-----------------------------------------------------
	//共通の基本スクロール制御
	//-----------------------------------------------------
	void BgScrollAdjust();

	//-----------------------------------------------------
	//曇り空用のフィルター
	//-----------------------------------------------------
	void WeatherFilterCloudy();

	//-----------------------------------------------------
	//エディット用
	//-----------------------------------------------------

	//エネミーコンフィグファイルの読み込み
	void   LoadLeynosEnemyConfigFile(char *pFileName );
	gxBool LoadConfig(char *pFileName);

	//エネミーの生成
	void MakeLeynosEnemy(Sint32 x1,Sint32 y1,Sint32 no);
	void MakeHoundsEnemy(Sint32 x1,Sint32 y1,Sint32 no);

	//敵64体のインデックス
	Sint32 EnemyIndex[MaxEnemyNum];

	//BGMファイル名
	char BgmFileName[255];

	//BGレイヤー１～３
	char LayerFileName01[255];
	char LayerFileName02[255];
	char LayerFileName03[255];

	//BGレイヤースクロール速度
	typedef struct StLayerScroll
	{
		Sint32 x,y;
		Sint32 ox,oy;
	} StLayerScroll;

	StLayerScroll LayerScroll[enBgTypeMax];

	//BGスクロール値限界設定
	Sint32 m_sScrollMax;
	Sint32 m_sScrollMin;
	Sint32 m_sScrollYMin;
	Sint32 m_sScrollYMax;

	//VMPファイル名
	char VmpFileName[255];

	//リスタート時の初期位置
	Sint32 m_sResetPositionX;
	Sint32 m_sResetPositionY;

	gxBool m_bFirst;

	//-------------------------------------
	//ステージ共通の処理
	//-------------------------------------
	void StageCommon();

	//-------------------------------------
	//特定エリアに進行したか？
	//-------------------------------------
	gxBool IsAreaArrive(Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2);

	//-------------------------------------
	//DRQファイル内の設定ファイルを指定して実行
	//-------------------------------------
	gxBool SetDRQStage( char* pFileName );

	//-------------------------------------
	//座標で指定した矩形内のマップチップを破棄する
	//-------------------------------------
	void ClearMapTip( Sint32 sLayer , Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 );

	//-------------------------------------
	//座標で指定した矩形内のVMPチップを破棄する
	//-------------------------------------
	void ClearVmpTip( Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 );

	//-------------------------------------
	//デバッグ用マップビューモード
	//-------------------------------------
	void AreaTest();
	void AddMissionSec(Sint32 sZone , Sint32 sSec );

private:

	Sint32 m_sTimer;
	Sint32 m_sAreaSeq;
	Sint32 m_sZoneSeq;

	//エディット用
	gxBool m_bStageGoodClear;
	gxBool m_bStageBadClear;
	gxBool m_bStageClear;
	gxBool m_bGamePlay;

	void SetConfigData(Sint32 sCmd , char **pArg);
	void   DRQConfig(Sint32 sCmd , char **pArg);

	//-------------------------------------
	//バッドクリアしたのか（未使用）
	//-------------------------------------
	gxBool IsStageBadClear()
	{
		return m_bStageBadClear;
	}

	gxBool IsStageGoodClear()
	{
		return m_bStageGoodClear;
	}

	void SetStageGoodClear()
	{
		m_bStageGoodClear = gxTrue;
	}

	void SetStageBadClear()
	{
		m_bStageBadClear = gxTrue;
	}

	gxBool IsStageClear()
	{
		if( m_bStageGoodClear ) return gxTrue;
		if( m_bStageBadClear ) return gxTrue;
		return gxFalse;
	}

	gxBool m_bTimeAdd[enAddTimeMAX];
};


#include "../enemy/Enemy.h"

#include "CBaseJungle.h"
#include "CBaseKichi.h"
#include "CBaseMountain.h"
#include "CBaseRiver.h"
#include "CBaseExtra.h"
#include "CStageLabo.h"

#include "CStageStart.h"
#include "CStageTutorial.h"

CStageBase* InitStage(Sint32 num);
