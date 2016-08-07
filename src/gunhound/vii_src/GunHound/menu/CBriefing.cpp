//-----------------------------------------------
//
// Briefing画面
//
//-----------------------------------------------

//#include <windows.h>
#include <gunvalken.h>
#include <gunhound/DashBoard/CDashBoard.h>
#include <gxLib/Util/gxImage/CGXImage.h>
#include "../CMenu.h"
#include "CBriefing.h"
#include "CMissionMsg.h"
#include "../../GunHound/GhStringTbl.h"

//↓これが定義されている場合には、GROWモードから抜けません。
//GROWのテスト用・リリース版では定義されていても効果がありません。
//-------------------------
enum {
	enSeqInit,
	enSeqSelect,
	enSeqDocuemnt,
	enSeqFadeOutWait,
};

enum {
	enColRed   = 0x80,
	enColGreen = 0x20,
	enColBlue  = 0x20,
};

gxSprite SprCursor[]={
	{enTexPageGrowObj,64+32*0,64+32*0+32,32,32, 16,16,},		//3:カーソル
	{enTexPageGrowObj,64+32*1,64+32*0+32,32,32, 16,16,},		//3:カーソル
	{enTexPageGrowObj,64+32*0,64+32*1+32,32,32, 16,16,},		//3:カーソル
	{enTexPageGrowObj,64+32*1,64+32*1+32,32,32, 16,16,},		//3:カーソル

};

gxSprite SprBgObj[]={
	{ enTexPageGrowObj,0+64*0,0+64*0+32,64,64, 32,32,},	//St1
	{ enTexPageGrowObj,0+64*1,0+64*0+32,64,64, 32,32,},	//St2
	{ enTexPageGrowObj,0+64*2,0+64*0+32,64,64, 32,32,},	//St3
	{ enTexPageGrowObj,0+64*3,0+64*0+32,64,64, 32,32,},	//St4
	{ enTexPageGrowObj,0+64*0,0+64*1+32,64,64, 32,32,},	//St5

	{ enTexPageGrowObj,0+64*3,0+64*1+32,64,64, 32,32,},	//St6	//味方基地
};

gxSprite SprTargetPos[]=
{
	{ enTexPageGrowObj,64    ,64    +32,64,64, 32,32,},	//ターゲットポジション
	{ enTexPageGrowObj,0+64*2,0+64*1+32,64,64, 32,32,},	//Batsu
};

ghPos StagePositionTbl[]={
	{380,32,},	//"「プラント強襲」",
	{322,148,},	//"「燃料輸送」",
	{220,32,},	//"「エンジニア解放」",
	{60,156,},	//"「ラボ襲撃」",
	{200,200,},	//"「アイアンギア迎撃」",
	{208,168,},	//"「補給線分断１」",河
	{312,142,},	//"「補給線分断２」",ヘリ
	{262,199,},	//"「補給線分断３」",基地
};

CBriefing::CBriefing(gxBool bExtra)
{
	//-----------------------------------------------
	//ブリーフィング
	//-----------------------------------------------

	m_sStage = 0;
	m_sSelectEvent = 0;
	m_bEnd = gxFalse;

	//g_pActManager = new CActManager();
	CActManager::GetInstance();

	m_sSequence = 0;

	//-----------------------------------------------
	m_sCursor = 0;
	m_sGold = 0;

	m_sWaitTimer = 0;
	m_sWaitY = 0;
	m_sWaitYMax = 0;

	AlphaBoad[0] = 0;
	AlphaBoad[1] = 0;
	AlphaBoad[2] = 0;

	m_sCursorTimer = 0;
	m_pClearStage = NULL;

	m_sColorBoard = ARGB(0xA0 , 0x80,0x20,0x20  );

	m_CursorPos.x = StagePositionTbl[0].x*100;
	m_CursorPos.y = StagePositionTbl[0].y*100;

	m_sDistance = 32*100;
	m_sCursorUpDown = 0;

	m_sMaximumItem = 0;

	CDashBoard::GetInstance()->SetIcon( enIconNone );
}


CBriefing::~CBriefing()
{
	//-----------------------------------------------
	//デストラクタ
	//-----------------------------------------------

//	delete g_pActManager;
//	g_pActManager = NULL;
	CActManager::DeleteInstance();
}


void CBriefing::GetStageInfo()
{
	//----------------------------------------
	//ステージ情報初期化
	//----------------------------------------
	Sint32 sCursor = -1;
	Sint32 sGold=m_sGold;
	StStageInfo *p;

	m_sMaximumItem = 0;

	if( m_pClearStage == NULL )
	{
		return;
	}

	for(Sint32 ii=0;ii<enScenarioMax;ii++)
	{
		p = &m_stStageInfo[ii];

		p->sCost       = g_sCostTbl[ii];
		p->sGyara      = g_sGyaraTbl[ii];
		p->pGyaraMsg   = pGyaraTbl[ii];
		p->pCostMsg    = sCostTbl[ii];
		p->pStageName  = sTitleTbl[ii];
		p->bClear      = (m_pClearStage[ii])? gxTrue : gxFalse;

		p->bSelectable = gxFalse;

//		//クリア済み、もしくは資金が足りなければ選択できない
//		if( p->bClear )        p->bSelectable = gxFalse;
//		if( sGold < p->sCost ) p->bSelectable = gxFalse;

//#ifdef _TRIAL_VERSION_
//		//--------------------------------------------------
//		//体験版ではステージジャングルのみしか選択できない
//		//--------------------------------------------------
//		if( ii )
//		{
//			p->bSelectable = gxFalse;
//		}
//#endif

		if( p->bClear )
		{
			p->bSelectable = gxTrue;
			m_sMaximumItem ++;
		}
		else
		{
			if( sCursor == -1 )
			{
				p->bSelectable = gxTrue;
				m_sMaximumItem ++;
				sCursor = ii;
			}
		}

		switch(ii){
		case enScenarioJungle:
			p->pStageDoc = &MissionJungle[0];
			break;
		case enScenarioRiver:
			p->pStageDoc = &MissionRiver[0];
			break;
		case enScenarioMountain:
			p->pStageDoc = &MissionMountain[0];
			break;
		case enScenarioBase:
			p->pStageDoc = &MissionKichi[0];
			break;
		case enScenarioIronGear:
			p->pStageDoc = &MissionExtra1[0];
			break;
		default:
			p->pStageDoc = &MissionJungle[0];
			break;
		}
	}

	m_sCursor = sCursor;

}


void CBriefing::Action()
{
	//-----------------------------------------------
	//毎フレームのアクション
	//-----------------------------------------------

	if( CDashBoard::GetInstance()->IsMenuBar() )
	{
		return;
	}


	switch(m_sSequence){
	case enSeqInit:
		if( CGXImage::GetInstance()->IsLoadFinish() )
		{
			CCockpit::GetInstance()->LoadMessageFromMemory( "ScenCommon\\CommonMsg.txt" );

			//------------------------------------------
			Uint32 sSize;
			Uint8 *p;
			p = CGXImage::GetInstance()->GetFile( "briefing.WAV" , &sSize );
			viiMus::ReadBGM( enSoundBgm1, p , sSize );

			//------------------------------------------

			viiMus::PlayBGM( enSoundBgm1 ,enMusicBgmBriefing );
			GetStageInfo();
			CFadeManager::GetInstance()->set_fadein();
			m_sSequence = enSeqSelect;
		}
		break;

	case enSeqSelect:
		m_sWaitY = 0;
		m_sWaitTimer = 0;
		CDashBoard::GetInstance()->SetIcon( enIconDecision );
		SelectMission();
		if( Joy[0].trg&enBtnDecision )
		{
			viiMus::PlaySound( enSoundMenuDec );
			m_sSequence = enSeqDocuemnt;
		}
		break;

	case enSeqDocuemnt:
		m_CursorPos.x = StagePositionTbl[GetStage()].x*100;
		m_CursorPos.y = StagePositionTbl[GetStage()].y*100;

		CDashBoard::GetInstance()->SetIcon( enIconDecision|enIconCancel );
		if( AlphaBoad[1] < 255 )
		{
			AlphaBoad[1] += 16;
			if( AlphaBoad[1] >= 255 )
			{
				AlphaBoad[1] = 255;
			}
		}
		if( Joy[0].trg&enBtnCancel )
		{
			viiMus::PlaySound( enSoundMenuCan );
			m_sSequence = enSeqSelect;
		}
		else if( Joy[0].trg&enBtnDecision )
		{
#ifdef _TRIAL_VERSION_
			//--------------------------------------------
			//体験版のみの特別仕様
			//--------------------------------------------
			if( m_sCursor == 0 )
			{
				CFadeManager::GetInstance()->set_fadeout();
				viiMus::PlaySound( enSoundMenuDec );
				m_sSequence = enSeqFadeOutWait;
			}
			else
			{
				viiMus::PlaySound( enSoundMenuCan );
			}
#else
			CFadeManager::GetInstance()->set_fadeout();
			viiMus::PlaySound( enSoundMenuDec );
			m_sSequence = enSeqFadeOutWait;
#endif
		}
		break;

	case enSeqFadeOutWait:
		if( !CFadeManager::GetInstance()->is_fade() )
		{
			m_bEnd = gxTrue;
		}
		break;
	}

	if( m_sSequence == enSeqDocuemnt )
	{
		MissionDocument();
	}
	else
	{
		if( AlphaBoad[1] > 0 )
		{
			AlphaBoad[1] -= 16;
			if( AlphaBoad[1] <= 0 )
			{
				AlphaBoad[1] = 0;
			}
		}
	}

	//ボードのアルファを調整

	if( AlphaBoad[0] < 255 )
	{
		AlphaBoad[0] += 16;
		if( AlphaBoad[0] >= 255 )
		{
			AlphaBoad[0] = 255;
		}
	}

	//g_pActManager->Action();
	CActManager::GetInstance()->Action();

}

gxBool CBriefing::IsPreviewOK()
{
	if( m_sSequence >= enSeqDocuemnt ) return gxTrue;
	
	return gxFalse;
}


Sint32 CBriefing::GetStage()
{
	//-------------------------------------
	//決定したステージを返す
	//-------------------------------------
	Sint32 sStage[]={
		enScenarioJungle,
		enScenarioRiver,
		enScenarioMountain,
		enScenarioBase,
		enScenarioIronGear,
		enScenarioExtra,
		enScenarioExtra2,
		enScenarioExtra3,
		enScenarioMax,
	};

	return sStage[m_sCursor];
}


gxBool CBriefing::IsEnd()
{
	//-------------------------------------
	//Briefing終了ですか？
	//-------------------------------------

	return m_bEnd;
}


//void CBriefing::GrowEnd()
//{
//
//	return;
//}


//void CBriefing::Grow()
//{
//
//
//}


void CBriefing::SelectMission()
{
	//----------------------------------------
	//ミッション選択
	//----------------------------------------
	Sint32 max = m_sMaximumItem;

#ifdef _TRIAL_VERSION_
	max = 1;
#endif

//	g_CHoundSaveData.SetHiddenOpen( enHiddenItemStageSelect );

	if( g_CHoundSaveData.IsHiddenOpen( enHiddenItemStageSelect ) )
	{
		//------------------------------------------------------
		//ステージセレクト可能
		//------------------------------------------------------

		if( Joy[0].rep&JOY_L )
		{
			m_sCursor --;
			m_sWaitTimer = 0;
			m_sWaitY = 0;
			m_sDistance = 32*100;

			if( max > 1 ) viiMus::PlaySound( enSoundMenuSel );

		}
		else if( Joy[0].rep&JOY_R )
		{
			m_sCursor ++;
			m_sWaitTimer = 0;
			m_sWaitY = 0;

			m_sDistance = 32*100;
			if( max > 1 ) viiMus::PlaySound( enSoundMenuSel );
		}
	}

	m_sCursor = (m_sCursor+max)%max;
	m_sStage = GetStage();


	//-------------------------------------
	//カーソル移動
	//-------------------------------------

	ghPos Tgt = StagePositionTbl[m_sStage];

	Tgt.x = Tgt.x*100;
	Tgt.y = Tgt.y*100;

	m_CursorPos.x += (Tgt.x-m_CursorPos.x)/10;
	m_CursorPos.y += (Tgt.y-m_CursorPos.y)/10;


	m_sDistance += ((32*100)-m_sDistance)/10;


	m_sCursorTimer ++;
}


void CBriefing::MissionDocument()
{
	//----------------------------------------
	//ミッション書類制御
	//----------------------------------------
	m_sCursorFlashU = 1;
	m_sCursorFlashD = 1;

	if( Joy[0].psh&JOY_U)
	{
		if( m_sWaitY >= 0)
		{
			m_sCursorFlashU = 4;
			m_sWaitY -= 200;
		}
		if( m_sWaitY <= 0)
		{
			m_sWaitY = 0;
		}
		m_sWaitTimer = 0;

	}
	else if( Joy[0].psh&JOY_D )
	{
		if( m_sWaitY < m_sWaitYMax*100)
		{
			m_sCursorFlashD = 4;
			m_sWaitY += 200;
		}
		if( m_sWaitY >= m_sWaitYMax*100)
		{
			m_sWaitY = m_sWaitYMax*100;
		}
		m_sWaitTimer = 0;
	}
	else
	{
		m_sWaitTimer +=1;
	}

	//スクロール制御

	if( m_sWaitTimer >= 180 )
	{
		m_sWaitY += 50;
		if( m_sWaitY >= m_sWaitYMax*100)
		{
			m_sWaitY = m_sWaitYMax*100;
		}
	}

	m_sCursorUpDown = 0;
	if( m_sWaitY < m_sWaitYMax*100)
	{
		//まだ下段がある
		m_sCursorUpDown |= JOY_D;
	}
	if( m_sWaitY > 0 )
	{
		m_sCursorUpDown |= JOY_U;
	}
	
}


void CBriefing::Draw()
{
	//----------------------------------------
	//描画
	//----------------------------------------
	if( m_sSequence == enSeqInit ) return;

	DrawBackGround();

	DrawUI();

	DrawMission();

	if( m_sSequence == enSeqSelect )
	{
		DrawCursor();
	}

	//if( g_pActManager )
	{
		//g_pActManager->Draw();
		CActManager::GetInstance()->Draw();
	}

}


void CBriefing::DrawBackGround()
{
	//----------------------------------------
	//背景
	//----------------------------------------

	ghPos m_StBase;

	m_StBase.x = 0;
	m_StBase.y = 1600;
	m_StBase.z = PRIO_BG1;

	for(Sint32 ii=0;ii<enScenarioMax;ii++)
	{
		StStageInfo *p = &m_stStageInfo[ii];
		Sint32 sCol = ARGB(0xA0,0xff,0xFF,0xFF);
		Sint32 x,y;
		x = StagePositionTbl[ii].x*100 + m_StBase.x;
		y = StagePositionTbl[ii].y*100 + m_StBase.y;

		//味方基地

		if( !p->bSelectable && ii!= 3)
		{
			continue;
		}

		if( p->bClear )
		{
			sCol = ARGB(0xA0,0x80,0xFF,0xFF);
		}
		else
		{
			if( ii == 0 || ii == 3 || ii== 4 )
			{
				sCol = ARGB(0xF0,0xFF,0x80,0x80);
			}
			else
			{
				sCol = ARGB(0xF0,0xFF,0xff,0xff);
			}
		}


		if( p->bClear )
		{
			//バツマーク
//			viiSub::MenuSprite( &SprTargetPos[1], x/100 , y/100 , m_StBase.z+1,ATR_DFLT,ARGB(0x40,0xff,0xFF,0xFF) );
		}

		//オブジェクト
		viiSub::MenuSprite( &SprBgObj[ii], x/100 , y/100 , m_StBase.z+2,ATR_DFLT,sCol );

	}

	viiSub::MenuSprite( &SprBgObj[5] , (m_StBase.x/100)+438 , (m_StBase.y/100)+180 , m_StBase.z+1,ATR_DFLT,ARGB(0xf0,0x80,0xF0,0xFF) );

	viiSub::MenuSprite( m_StBase.x/100,m_StBase.y/100,m_StBase.z, enTexPageGrowMap,0,0,WINDOW_W,256-24,0,0 ,ATR_DFLT,ARGB(0xFF,0xff,0xff,0xff) );

}


void CBriefing::DrawUI()
{
	//----------------------------------------
	//UI描画
	//----------------------------------------
	Sint32 sStage = GetStage();
	StStageInfo *p = &m_stStageInfo[sStage];
	Sint32 cColor = 0xF0FFFFFF;

	Sint32 x,y;

	x=32;
	y=56;

	viiDbg::printf( x,y+16*0,sTitleTbl[0] );//要：00000 / 報酬：3000
	viiDbg::printf( x,y+16*1,sTitleTbl[1] );//要：12000 / 報酬：20000
	viiDbg::printf( x,y+16*2,sTitleTbl[2] );//要：12000 / 報酬：20000
	viiDbg::printf( x,y+16*3,sTitleTbl[3] );//要：12000 / 報酬：20000
	viiDbg::printf( x,y+16*4,sTitleTbl[4] );//要：00000 / 報酬：5000
	viiDbg::printf( x,y+16*5,sTitleTbl[5] );//要：00000 / 報酬：5000
	viiDbg::printf( x,y+16*6,sTitleTbl[6] );//要：00000 / 報酬：5000
	viiDbg::printf( x,y+16*7,sTitleTbl[7] );//要：10000 / 報酬：10000
	viiDbg::printf( x-16,y+16*m_sCursor,"→");

//	//作戦情報
//
//	Sint32 ux,uy,uz;
//	ux = 32;
//	uy = 24;
//	uz = PRIO_BG1+1;
//
//	//現在資金
//	if( !p->bClear && !p->bSelectable )
//	{
//		//資金不足
//		if( viiSub::GameCounter()%32<16) cColor = 0xF0F00000;
//	}
//	viiDraw::Box( ux-8,uy-8,ux+200,uy+16+8,uz , gxTrue , m_sColorBoard );
//	CCockpit::GetInstance()->KanjiColorPrintf( ux,uy+16*0,cColor,"資金:￠%d-",m_sGold );

}


void CBriefing::DrawCursor()
{
	//----------------------------------------
	//カーソル描画
	//----------------------------------------
	//m_sCursorTimer
	Sint32 x = m_CursorPos.x/100;
	Sint32 y = m_CursorPos.y/100;

	Sint32 dst = (m_sDistance/100) + (viiMath::Cos100( (m_sCursorTimer*8)%360 )*2)/100;

	Sint32 ux,uy,uz;
	ux = x;
	uy = y+16;
	uz = PRIO_BG1+2;

	Sint32 ax,ay;
	ax = ux;
	ay = uy;

	for(Sint32 ii=0;ii<4;ii++)
	{
		for(Sint32 jj=0;jj<1;jj++)
		{
			Sint32 ax,ay,r;
			r = (viiSub::GameCounter()*3)%360;	
			r += jj*16;
			ax = dst*viiMath::Cos100( ii*90+r ) /100;
			ay = dst*viiMath::Sin100( ii*90+r ) /100;

			ax += ux;
			ay += uy;
			viiSub::MenuSprite( &SprCursor[2] , ax , ay , uz, ATR_ALPHA_PLUS , ARGB(0x80,0xff,0xff,0xff) ,0.75f , ii*90+r+45 );
			viiSub::MenuSprite( &SprCursor[2] , ax , ay , uz, ATR_DFLT , ARGB(0x80,0xff,0xff,0xff) ,0.5f , ii*90+r+45 );
		}
	}

}


void CBriefing::DrawMission()
{
	//----------------------------------------
	//ミッション書類表示
	//----------------------------------------

	Sint32 ux,uy,uz;
	Sint32 sStage = GetStage();
	Sint32 cColor = 0xF0FFFFFF;
	ux = 32;
	uy = 200;
	uz = PRIO_BG2;

	if( sStage == 3  || sStage == 4)
	{
		if( m_sSequence != enSeqDocuemnt )
		{
			uy = 32;
		}
	}

	StStageInfo *p = &m_stStageInfo[sStage];

	viiDraw::Box( ux-8,uy-8,ux+200,uy+48,uz , gxTrue , ARGB( ( 0xa0*AlphaBoad[0]/255 ) , enColRed,enColGreen,enColBlue )  );

	cColor = 0xF0FFFFFF;
	if( p->bClear )
	{
		//クリア済み
		cColor = 0xF0808080;
	}

	CCockpit::GetInstance()->KanjiColorPrintf( ux , uy+16*0-4,cColor,"%s",p->pStageName );

	//報酬
	cColor = 0xF0FFFFFF;
//	if( p->bClear )
//	{
//		//クリア済み
//		cColor = 0xF0404040;
//	}
//	else if( p->bSelectable )
//	{
//		//選択可能
//		cColor = 0xF040F040;
//	}
	CCockpit::GetInstance()->KanjiColorPrintf( ux,uy+16*1-2,cColor,"%s",p->pCostMsg );
//
//	//必要経費
	cColor = 0xF0FFFFFF;
//	if( p->bClear )
//	{
//		//クリア済み
//		cColor = 0xF0404040;
//	}
//	else if( !p->bSelectable )
//	{
//		//資金不足
//		if( viiSub::GameCounter()%32<16) cColor = 0xF0F00000;
//	}
//
	CCockpit::GetInstance()->KanjiColorPrintf( ux,uy+16*2-2,cColor,"%s",p->pGyaraMsg );


	//-----------------------------------------

	CCockpit::GetInstance()->KanjiColorPrintf( 32,WINDOW_H-16,0xfff0f0f0,"ＳＣＯＲＥ %d",g_StGameInfo.m_uAllScore );

	//-----------------------------------------

	if( m_sSequence != enSeqDocuemnt )
	{
		return;
	}

	ux = 256;
	uy  =16;
	uz = PRIO_BG2+1;

	viiDraw::Box( ux-8,uy,ux+216,uy+232,uz , gxTrue , ARGB( ( 0xa0*AlphaBoad[0]/255 ) , enColRed,enColGreen,enColBlue ) );//,ATR_ALPHA_MINUS );

	Sint32 sLine = 0;

	while(gxTrue){
		Sint32 y,c=255;
		if( p->pStageDoc[sLine] == NULL )
		{
			m_sWaitYMax = 16+sLine*16-(WINDOW_H-16*2);
			break;
		}
		else
		{
			y = 16+uy+16*sLine - m_sWaitY/100;
			if( y<32 )
			{
				c = 255+(y-32)*32;
				if( c < 0 )
				{
					c = 0;
				}
			}
			else if( y>(WINDOW_H-32) )
			{
				c = 255+((WINDOW_H-32)-y)*32;
				if( c < 0 )
				{
					c = 0;
				}
			}
			else
			{
				c = 255;
			}

			c = c*AlphaBoad[1]/255;
			//if( c ) viiSub::MenuPrintf( ux , y, ARGB( c,0xff,0xff,0xff) , "%s",p->pStageDoc[sLine] );
			if( c ) CCockpit::GetInstance()->KanjiColorPrintf( ux , y-8, ARGB( c,0xff,0xff,0xff) , "%s",p->pStageDoc[sLine] );
		}
		sLine ++;
	}

	//---------------------------------
	//上下のカーソル表示
	//---------------------------------

	if( m_sCursorUpDown&JOY_U )
	{
		Sint32 sCol = 255 - 64+64*viiMath::Cos100( (viiSub::GameCounter()*m_sCursorFlashU*8)%360 )/100;
		sysMenu::Sprite( ux+96 ,uy+8 , uz , 0,0,160,16,16,8,0 ,ATR_FLIP_Y , ARGB(sCol,0xff,0xff,0xff) );
	}

	if( m_sCursorUpDown&JOY_D )
	{
		Sint32 sCol = 255 - 64+64*viiMath::Cos100( (viiSub::GameCounter()*m_sCursorFlashD*8)%360 )/100;
		sysMenu::Sprite( ux+96 ,uy+232-8 , uz , 0,0,160,16,16,8,0 ,ATR_DFLT , ARGB(sCol,0xff,0xff,0xff) );
	}

}


