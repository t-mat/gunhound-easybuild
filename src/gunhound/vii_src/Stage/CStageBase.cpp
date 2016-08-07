//------------------------------------------------------
//
// ステージの基底クラス
//
//------------------------------------------------------
#include <gunvalken.h>
#include <gxLib/Util/csv/CCsv.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CStageBase.h"
#include "CScriptEngine.h"

char *CommandList[enCommandMax]={
/*00*/	"【なし】",
/*01*/	"【自機の位置】",
/*02*/	"【ＢＧＭ読み込み】",
/*03*/	"【背景読み込み】",
/*04*/	"【テクスチャ読み込み】",
/*05*/	"【ＶＭＰ読み込み】",
/*06*/	"【敵設定】",
/*07*/	"【スクロール設定】",
/*08*/	"【テクスチャ更新】",
/*09*/	"【ログ出力】",
/*10*/ "【dbg_EneSpr設定】",	//eneIndex,8,page,u,v,w,h,cx,cy,	//sprIndexは0～7まで
/*11*/ "【dbg_Eneアタリ判定】",	//eneIndex,x1,y1,x2,y2,damage
/*12*/ "【dbg_Eneクライ判定】",	//eneIndex,x1,y1,x2,y2,damage
/*13*/ "【dbg_Ene思考設定】",	//eneIndex,spd,atk				//移動速度(0～200)と攻撃種類(0:攻撃しない)を設定
/*14*/ "【dbg_Ene出現】",		
/*15*/ "【背景オフセット設定】",
/*16*/ "【フォント読み込み】",
///*16*/ "",

/*108*/	"【メッセージ設定】",
	"Command_None",
	"cmd_set",
	"msg_end",
	"cmd",
	"[LoadTexture]",
	"[Update]",
	NULL,
};


CStageBase::CStageBase()
{
#ifdef _VII_DEBUG_
	pGame->pBg->DebugTipDraw();
#endif

	m_sScrollYMin = 0;
	m_sScrollYMax = 9600*100;

	m_bFirst  = gxTrue;	//初回のInit()時にgxFlaseとなる
	m_bGamePlay = gxFalse;

	m_sAreaSeq  = 0;
	m_sZoneSeq  = 0;
//いらない	m_sMsgCnt   = 0;
	m_sClearTimer = 0;
	m_sBGM = 0;
	m_sZoneTimer = 0;
	m_sTimer  = 0;

//いらない	m_pTextName = NULL;

//いらない	for(Sint32 ii=0;ii<MaxMessageNum;ii++)
//いらない	{
//いらない		m_sMsgMem[ii] = -1;
//いらない	}


	m_Pos.x = 0;
	m_Pos.y = 0;

	m_bStageGoodClear = gxFalse;
	m_bStageBadClear  = gxFalse;
	m_bStageClear     = gxFalse;

	BgmFileName[0] = 0x00;
	VmpFileName[0] = 0x00;
	LayerFileName01[0] = 0x00;
	LayerFileName02[0] = 0x00;
	LayerFileName03[0] = 0x00;
	m_sResetPositionX = 0;
	m_sResetPositionY = 0;
	LayerScroll[enBgTypeBack].x = 100;
	LayerScroll[enBgTypeRear].x = 100;
	LayerScroll[enBgTypeFrnt].x = 100;

	LayerScroll[enBgTypeBack].y = 100;
	LayerScroll[enBgTypeRear].y = 100;
	LayerScroll[enBgTypeFrnt].y = 100;

	LayerScroll[enBgTypeBack].ox = 0;
	LayerScroll[enBgTypeRear].ox = 0;
	LayerScroll[enBgTypeFrnt].ox = 0;

	LayerScroll[enBgTypeBack].oy = 0;
	LayerScroll[enBgTypeRear].oy = 0;
	LayerScroll[enBgTypeFrnt].oy = 0;

	m_sScrollMax = 9660*100;
	m_sScrollMin = 0;

	for(Sint32 jj=0; jj<MaxEnemyNum; jj++)
	{
		EnemyIndex[ jj ] = jj;
	}

	//暫定的にタイムをフルにする
	CCockpit::GetInstance()->SetMissionSec( 69*99 );

	for(Sint32 ii=0; ii<enAddTimeMAX; ii++)
	{
		m_bTimeAdd[ii] = gxFalse;
	}
}

CStageBase::~CStageBase()
{

}


void CStageBase::AddMissionSec(Sint32 sZone , Sint32 sSec )
{
	//ミッションタイムを更新する

//	if( !m_bTimeAdd[sZone] )
//	{
		m_bTimeAdd[sZone] = gxTrue;
//		CCockpit::GetInstance()->AddMissionSec( sSec );
//	}

}


void CStageBase::Init()
{
	//virtual
}

void CStageBase::Action()
{
	//virtual
}


void CStageBase::End()
{
	//virtual
}


void CStageBase::ClearMapTip( Sint32 sLayer , Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 )
{
	//-------------------------------------
	//マップチップを削除する
	//-------------------------------------

	for(Sint32 y=y1; y<y2; y+=MAP_BLOCK_SIZE)
	{
		for(Sint32 x=x1; x<x2; x+=MAP_BLOCK_SIZE)
		{
//			pGame->pBg->del_vmptip((sx+x)/8,(sy+y)/8);
			pGame->pBg->del_maptip( sLayer, x/MAP_BLOCK_SIZE, y/MAP_BLOCK_SIZE );
		}
	}

}

void CStageBase::ClearVmpTip( Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 )
{
	//-------------------------------------
	//マップチップを削除する
	//-------------------------------------

	for(Sint32 y=y1; y<y2; y+=8)
	{
		for(Sint32 x=x1; x<x2; x+=8)
		{
			pGame->pBg->del_vmptip( x/8 , y/8 );
		}
	}

}

//いらないvoid CStageBase::SetTimer(Sint32 time)
//いらない{
//いらない	m_sTimer = time;
//いらない}


Sint32 CStageBase::GetArea( )
{
	return m_sAreaSeq;
}

void CStageBase::ChangeArea(Sint32 area)
{
	//---------------------------
	//エリア変更
	//---------------------------

	m_sAreaSeq = area;
	m_sZoneSeq   = 0;
	m_sZoneTimer = 0;
}

gxBool CStageBase::IsZoneSeq( Sint32 n)
{
	//---------------------------
	//ゾーンシーケンスを得る
	//---------------------------

	return ( m_sZoneSeq == n);
}

Sint32 CStageBase::GetZoneSeq( )
{
	//---------------------------
	//ゾーンシーケンスを得る
	//---------------------------

	return m_sZoneSeq;
}

void CStageBase::SetZoneSeq( Sint32 n)
{
	//---------------------------
	//ゾーンシーケンスをセット
	//---------------------------

	m_sZoneSeq = n;

}

void CStageBase::ResetZoneTimer( )
{
	//---------------------------
	//ゾーンタイマーをリセット
	//---------------------------

	m_sZoneTimer = 0;

}

gxBool CStageBase::IsFadeEnd()
{
	//---------------------------
	//フェード終わりましたか？
	//---------------------------

	return  !CFadeManager::GetInstance()->is_fade();

}

void CStageBase::SetFadeIn(Sint32 n,Sint32 col)
{
	//---------------------------
	//フェードイン
	//---------------------------

	CFadeManager::GetInstance()->set_fadein(8,col);
}

void CStageBase::SetFadeOut(Sint32 n,Sint32 col)
{
	//---------------------------
	//フェードアウト
	//---------------------------

	CFadeManager::GetInstance()->set_fadeout( STAGE_CLEAR_FADE_TIME ,col );
}


void CStageBase::BgScrollAdjust()
{
	//---------------------------
	//BGの基本処理
	//---------------------------

	Sint32 sx,sy;
	viiSub::GetIdealScroll_l(sx,sy);

	Sint32 sYmax = pGame->pBg->getHeight()+512;

	if(sx < m_sScrollMin) sx = m_sScrollMin;
	if(sy < m_sScrollYMin) sy = m_sScrollYMin;
	if(sx > m_sScrollMax) sx = m_sScrollMax;
	if(sy > sYmax*100)
	{
		sy = sYmax*100;
	}

	if(sy > m_sScrollYMax )
	{
		sy = m_sScrollYMax;
	}

	viiSub::AdjScroll_l( sx,sy );
	viiSub::GetScroll_s( sx,sy );

	viiSub::SetBgDisp ( enBgTypeStar , gxFalse );
	viiSub::SetBgLayer( enBgTypeStar , sx*LayerScroll[enBgTypeBack].x/10,sy*LayerScroll[enBgTypeBack].y/100 );

/*
	Sint32 sx,sy;

	viiSub::GetIdealScroll_l(sx,sy);

	if(sx <= x1) sx = x1;
	if(sy <= y1) sy = y1;
	if(sx >= x2) sx = x2;
	if(sy >= y2) sy = y2;

	viiSub::AdjScroll_l( sx,sy );
*/

}

void CStageBase::WeatherFilterCloudy()
{
	//----------------------------------------------
	//天候再現用のフィルター
	//----------------------------------------------

	viiDraw::Box(0,0,WINDOW_W,WINDOW_H,PRIO_BG1+1,gxTrue,0x800000f0,ATR_ALPHA_MINUS);

}

gxBool CStageBase::IsAreaArrive(Sint32 x1, Sint32 y1, Sint32 w, Sint32 h)
{
	//--------------------------------------------
	//特定エリアに入っているか？
	//--------------------------------------------
	gxBool bFlag = gxFalse;

	if( GetTargetPlayer() == NULL )
	{
		return gxFalse;
	}

	Sint32 x = GetTargetPlayer()->x/100;
	Sint32 y = GetTargetPlayer()->y/100;
	Sint32 x2,y2;

	x2 = x1+w;
	y2 = y1+h;

	if( x>x1 && x<x2 )
	{
		if( y>y1 && y<y2 )
		{
			bFlag = gxTrue;
		}
	} 

#ifdef _VII_DEBUG_
	if( bFlag )
	{
		viiDraw::BoxScrl_s( x1 , y1 , x2 , y2 , PRIO_DEBUG , gxTrue , 0x8000FF00 , ATR_ALPHA_PLUS );
	}
	else
	{
		viiDraw::BoxScrl_s( x1 , y1 , x2 , y2 , PRIO_DEBUG , gxTrue , 0x80FFFF00 , ATR_ALPHA_PLUS );
	}
#endif

	return bFlag;
}

void CStageBase::BgCommon(Sint32 x,Sint32 y)
{
	
	//virtual
}


void CStageBase::AreaTest()
{
	//-----------------------------------------------------
	//エリアB：テスト
	//-----------------------------------------------------

	static gxBool s_bFlag;

	pGame->GetHound()->m_Pos.x = 0;
	pGame->GetHound()->m_Pos.y = 0;

	if( IsZoneSeq(0) )
	{
		SetFadeIn();
		s_bFlag = gxFalse;
		m_Pos.x = pGame->GetHound()->m_Pos.x;
		m_Pos.y = pGame->GetHound()->m_Pos.y;
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNone );
		SetZoneSeq(10);
	}
	else if( IsZoneSeq(10) )
	{
		Sint32 sSpd = 300;
		if( Joy[0].psh&BTN_A) sSpd = 1000;
		if( Joy[0].trg&BTN_B)
		{
			s_bFlag = !s_bFlag;
			pGame->pBg->DebugTipDraw( s_bFlag );
		}
		if( Joy[0].psh&JOY_U) m_Pos.y -= sSpd;
		if( Joy[0].psh&JOY_D) m_Pos.y += sSpd;
		if( Joy[0].psh&JOY_L) m_Pos.x -= sSpd;
		if( Joy[0].psh&JOY_R) m_Pos.x += sSpd;


		m_sScrollMax  = m_Pos.x;
		m_sScrollYMax = m_Pos.y;

		m_sScrollMin  = m_sScrollMax;
		m_sScrollYMin = m_sScrollYMax;
	}

	BgCommon();
	viiSub::LimitMoveVH();
}


void CStageBase::StageCommon()
{
	//------------------------------------------------
	//ステージ共通処理
	//------------------------------------------------

	//時限装置
	DummyEnemyWaitCheck( m_sTimer );

	if( m_bGamePlay )
	{
		if( pGame )
		{
			//体力やばいの警告
			if( pGame->GetHound()->GetHitPointStatus() == CGunHound::enHitPointStausAlert )
			{
				if( !CCockpit::GetInstance()->IsMessageNow() )
				{
					CCockpit::GetInstance()->SetMessage( 9900 );
				}
			}
		}

		//時間やばいの警告
//		if( CCockpit::GetInstance()->GetMissionSec() < 60 )
//		{
//			if( !CCockpit::GetInstance()->IsMessageNow() )
//			{
//				CCockpit::GetInstance()->SetMessage( 9910 );
//			}
//		}

		//時間切れの警告
//		if( CCockpit::GetInstance()->IsTimeOver() )
//		{
//			if( !CCockpit::GetInstance()->IsMessageNow() )
//			{
//				CCockpit::GetInstance()->SetMessage( 9920 );
//			}
//		}
	}

	m_sZoneTimer ++;

#ifdef _VII_DEBUG_
	DEBUG_ATARIMAP = 1;
	DEBUG_ATARIOBJ = 1;

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );
//いらない	if( m_pTextName )
//いらない	{
//いらない		viiDbg::printf(312,WINDOW_H,"%s",m_pTextName);
//いらない	}
	viiDbg::printf(200,WINDOW_H+16*0,"Area (%d-%d)",m_sAreaSeq, m_sZoneSeq );
	viiDbg::printf(200,WINDOW_H+16*1,"ZTime (%d)"  ,m_sZoneTimer);

	viiDbg::printf(312,WINDOW_H+16*1,"HOUND (%d,%d)",GetTargetPlayer()->x/100 ,GetTargetPlayer()->y/100);
	viiDbg::printf(312,WINDOW_H+16*2,"SCROLL(%d,%d)",sx,sy);
#endif


}

void CStageBase::MakeHoundsEnemy(Sint32 x,Sint32 y,Sint32 sIndex)
{
	//----------------------------------------------------------
	//敵生成
	//----------------------------------------------------------
	Sint32 no = sIndex;
	Sint32 enemyID = 0;

	enemyID = sIndex;//EnemyIndex[no];

	if( enemyID >= 1000 && enemyID <=1063 )
	{
		//ダミーキャラ設定
		new CEne0000Dummy( enemyID-1000 , x , y );
		return;
	}

	viiSub::MakeEnemy( enemyID , x , y );

}

CStageBase* InitStage(Sint32 num)
{
	CStageBase* p=NULL;
	CCockpit::GetInstance()->AutoStartUp();

	switch(num) {
	case 99:
		p = new CStageLabo();
		break;

	case enScenarioJungle:
		p = new CStageStart();
//いらない		p->SetStageIndex( enScenarioJungle , NULL );
		break;

	case enScenarioRiver:
		p = new CBaseRiver();
//いらない		p->SetStageIndex(enScenarioRiver , NULL );
		break;

	case enScenarioMountain:
		p = new CBaseMountain();
//いらない		p->SetStageIndex(enScenarioMountain, NULL );
		break;

	case enScenarioBase:
		p = new CBaseKichi();
//いらない		p->SetStageIndex(enScenarioBase, NULL );
		break;

	case enScenarioIronGear:
		p = new CBaseExtra();
//いらない		p->SetStageIndex(enScenarioExtra, NULL );
		break;

	case enScenarioTutorial:
		p = new CStageTutorial();
//いらない		p->SetStageIndex(enScenarioTutorial, NULL );
		break;

	case enScenarioExtra:
	case enScenarioExtra2:
	case enScenarioExtra3:
	default:
		break;
	}

	return p;

}


void CStageBase::SetConfigData(Sint32 sCmd , char **pArg)
{
	//-------------------------------------
	//コマンドを実行する
	//-------------------------------------

	switch(sCmd){
	case 1:	//"【自機の位置】"
		SetTargetPlayerPosX( atoi(pArg[0])*100 );
		SetTargetPlayerPosY( atoi(pArg[1])*100 );
		break;

	case 2:	//"【ＢＧＭ読み込み】"
		//viiMus::ReadBGM( atoi(pArg[0]) , pArg[1] );
		break;

	case 3:	//"【背景読み込み】"
		pGame->pBg->LoadMapFile( atoi(pArg[0]) , pArg[1] );
		LayerScroll[ atoi(pArg[0]) ].x = atoi(pArg[2]);
		LayerScroll[ atoi(pArg[0]) ].y = atoi(pArg[3]);
		break;

	case 4:	//"【テクスチャ読み込み】",
		LoadTexture( atoi(pArg[0])*16+atoi(pArg[1]) , pArg[2] , 0xff00ff00);
		break;

	case 5:	//"【ＶＭＰ読み込み】",
		pGame->pBg->LoadVmpFile( pArg[0] );
		break;

	case 6:	//"【敵設定】",
		EnemyIndex[ atoi(pArg[0]) ] = atoi(pArg[1]);
		break;

	case 7://"【スクロール設定】",
		m_sScrollMin = atoi(pArg[0])*100;
		m_sScrollMax = atoi(pArg[1])*100;
		break;

	case 8:	//【テクスチャ更新】
		UploadTexture();
		break;

	case 9://【ログ出力】
		viiDbg::log( "スクリプト：%s",pArg[0] );
		break;

	case 10: // "【dbg_EneSpr設定】",	//eneIndex,page,u,v,w,h,cx,cy,
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].page =  atoi(pArg[1])*16+atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].u    =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].v    =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].w    =  atoi(pArg[5]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].h    =  atoi(pArg[6]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].cx   =  atoi(pArg[7]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].cy   =  atoi(pArg[8]);
		break;
	case 11: // "【dbg_Eneアタリ判定】",	//eneIndex,x1,y1,x2,y2,damage
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].m_bHit = atoi(pArg[5])? gxTrue : gxFalse;
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_u =  atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_v  =  atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_w  =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_h  =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_dmg=  atoi(pArg[5]);
		break;
	case 12: // "【dbg_Eneクライ判定】",	//eneIndex,x1,y1,x2,y2,damage
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].m_bKurai = atoi(pArg[5])? gxTrue : gxFalse;
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_u =  atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_v  =  atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_w  =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_h  =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_dmg=  atoi(pArg[5]);
		break;

	case 13: // "【dbg_Ene思考設定】",	//eneIndex,spd,atk				//移動速度(0～200)と攻撃種類(0:攻撃しない)を設定
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sSpeed      = atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sWeaponType = atoi(pArg[2]);
		break;

	case 14:
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sWaitTime = atoi( pArg[1] );
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sEneIndex = EnemyIndex[ atoi( pArg[2] ) ];
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].bExist = gxTrue;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sX  = atoi( pArg[3] )*100;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sY  = atoi( pArg[4] )*100;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sTx = atoi( pArg[5] )*100;
		break;

	case 15:	//"【背景オフセット】"
		LayerScroll[ atoi(pArg[0]) ].ox = atoi(pArg[1]);
		LayerScroll[ atoi(pArg[0]) ].oy = atoi(pArg[2]);
		break;

	case 16:	//"【フォント読み込み】"
		CCockpit::GetInstance()->LoadMessage( pArg[0] );
		break;
	default:
		break;
	}
}


gxBool CStageBase::LoadConfig(char* pFileName )
{
	//-------------------------------------
	//スクリプトデータから読み込む
	//-------------------------------------
	DummyEnemyWaitInit();

	CScriptEngine *pScript = new CScriptEngine(CommandList);

	if( !pScript->Load(pFileName) ) return gxFalse;

	for(Sint32 ii=0; ii<pScript->GetLineMax(); ii++)
	{
		pScript->Read( ii );
		SetConfigData( pScript->GetCommandIndex( ) , pScript->GetArguments() );
	}

	delete pScript;
	return gxTrue;
}


gxBool CStageBase::SetDRQStage( char* pFileName )
{
	//-------------------------------------
	//スクリプトデータから読み込む
	//-------------------------------------
	DummyEnemyWaitInit();

	CScriptEngine *pScript = new CScriptEngine(CommandList);

	Uint32 sSize = 0;
	Uint8 *pFile = CGXImage::GetInstance()->GetFile( pFileName ,&sSize );

	if( !pScript->LoadMemory( pFile , sSize) ) return gxFalse;

	for(Sint32 ii=0; ii<pScript->GetLineMax(); ii++)
	{
		pScript->Read( ii );
		DRQConfig( pScript->GetCommandIndex( ) , pScript->GetArguments() );
	}

	delete pScript;

	pGame->GameStart(gxTrue);

	return gxTrue;
}


void CStageBase::DRQConfig(Sint32 sCmd , char **pArg)
{
	//-------------------------------------
	//コマンドを実行する
	//-------------------------------------

	switch(sCmd){
	case 1:	//"【自機の位置】"
		SetTargetPlayerPosX( atoi(pArg[0])*100 );
		SetTargetPlayerPosY( atoi(pArg[1])*100 );
		break;

	case 2:	//"【ＢＧＭ読み込み】"
		{
			Uint32 sSize;
			Uint8 *p;
			p = CGXImage::GetInstance()->GetFile( pArg[1] , &sSize );
			viiMus::ReadBGM( enSoundStartPage+atoi( pArg[0]) , p ,sSize );
			//CGXImage::GetInstance()->DeleteFile( pArg[1] );
		}
		break;

	case 3:	//"【背景読み込み】"
		pGame->pBg->LoadMapData( atoi(pArg[0]) , pArg[1] );
		LayerScroll[ atoi(pArg[0]) ].x = atoi(pArg[2]);
		LayerScroll[ atoi(pArg[0]) ].y = atoi(pArg[3]);
		break;

	case 4:	//"【テクスチャ読み込み】",
//		viiSub::LoadTexture( atoi(pArg[0])*16+atoi(pArg[1]) , CCockpit::GetInstance()->Loader() , pArg[2] , 0xff00ff00);
		CGXImage::GetInstance()->SetTexture( atoi(pArg[0])*16+atoi(pArg[1]),pArg[2] );
		break;

	case 5:	//"【ＶＭＰ読み込み】",
		{
			Uint32 uSize;
			Uint8 *p = CGXImage::GetInstance()->GetFile( pArg[0] ,&uSize );
			pGame->pBg->LoadVmpData( p );
		}
		break;

	case 6:	//"【敵設定】",
		EnemyIndex[ atoi(pArg[0]) ] = atoi(pArg[1]);
		break;

	case 7://"【スクロール設定】",
		m_sScrollMin = atoi(pArg[0])*100;
		m_sScrollMax = atoi(pArg[1])*100;
		break;

	case 8:	//【テクスチャ更新】
		UploadTexture();
		break;

	case 9://【ログ出力】
		viiDbg::log( "スクリプト：%s",pArg[0] );
		break;

	case 10: // "【dbg_EneSpr設定】",	//eneIndex,page,u,v,w,h,cx,cy,
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].page =  atoi(pArg[1])*16+atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].u    =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].v    =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].w    =  atoi(pArg[5]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].h    =  atoi(pArg[6]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].cx   =  atoi(pArg[7]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].cy   =  atoi(pArg[8]);
		break;
	case 11: // "【dbg_Eneアタリ判定】",	//eneIndex,x1,y1,x2,y2,damage
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].m_bHit = atoi(pArg[5])? gxTrue : gxFalse;
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_u =  atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_v  =  atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_w  =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_h  =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_dmg=  atoi(pArg[5]);
		break;
	case 12: // "【dbg_Eneクライ判定】",	//eneIndex,x1,y1,x2,y2,damage
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].m_bKurai = atoi(pArg[5])? gxTrue : gxFalse;
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_u =  atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_v  =  atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_w  =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_h  =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_dmg=  atoi(pArg[5]);
		break;

	case 13: // "【dbg_Ene思考設定】",	//eneIndex,spd,atk				//移動速度(0～200)と攻撃種類(0:攻撃しない)を設定
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sSpeed      = atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sWeaponType = atoi(pArg[2]);
		break;

	case 14:
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sWaitTime = atoi( pArg[1] );
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sEneIndex = EnemyIndex[ atoi( pArg[2] ) ];
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].bExist = gxTrue;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sX  = atoi( pArg[3] )*100;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sY  = atoi( pArg[4] )*100;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sTx = atoi( pArg[5] )*100;
		break;

	case 15:	//"【背景オフセット】"
		LayerScroll[ atoi(pArg[0]) ].ox = atoi(pArg[1]);
		LayerScroll[ atoi(pArg[0]) ].oy = atoi(pArg[2]);
		break;

	case 16:	//"【フォント読み込み】"
		{
			CCockpit::GetInstance()->LoadMessageFromMemory( pArg[0] );
		}
		break;
	default:
		break;
	}
}

