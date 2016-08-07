//----------------------------------------------------------------------------
//
//　エクストラステージ １（アイアンギア戦）
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"

enum
{
	enAreaSeq01,
	enAreaSeq02,
	enAreaSeq03,
	enAreaSeq04,
	enAreaSeq05,
	enAreaSeqMax,
};

enum {
	enRollerDashHeight = 850*100,
};

void CStageLabo::Init()
{
	//---------------------------------------------------------------------------
	//初期化
	//---------------------------------------------------------------------------
	SetFadeIn();

	Sint32 col1 =  0xF0408080;
	Sint32 col2 =  0xF0808040;
	pGame->pBg->SetSkyColor(50,col1,col2);

	//---------------------------------------------------------------------------
	//ファイル初期化
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	//エリア初期化
	//---------------------------------------------------------------------------

	switch(GetArea( )){
	case enAreaSeq01:
		LoadConfig( "HoundData\\StageLabo.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenJungle\\DatMsg.txt");	//毒
		break;

	case enAreaSeq02:
		pGame->SetMarineLine(900*100);
		LoadConfig( "HoundData\\StageLabo2.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenRiver\\DatMsg.txt");	//毒
		break;

	case enAreaSeq03:
		LoadConfig( "HoundData\\StageLabo3.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenMount\\DatMsg.txt");	//毒
		break;

	case enAreaSeq04:
		LoadConfig( "HoundData\\StageLabo4.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenKichi\\DatMsg.txt");	//毒
		break;

	case enAreaSeq05:
		LoadConfig( "HoundData\\StageLabo5.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenExtra\\DatMsg.txt");	//毒
		break;
	}

	InitEnemies();

}


void CStageLabo::End()
{
	//---------------------------------------------------------------------------
	//終了処理
	//---------------------------------------------------------------------------


}

void CStageLabo::InitEnemies()
{
	//---------------------------------------------------------------------------
	//敵の初期化
	//---------------------------------------------------------------------------

	SetEnemy();

}

void CStageLabo::Action()
{
	//---------------------------------------------------------------------------
	//毎フレームのアクション
	//---------------------------------------------------------------------------
	StageCommon();

	AreaCommon();

	switch(GetArea( )){
	case enAreaSeq01:
		Area01();
		break;

	case enAreaSeq02:
		Area02();
		break;

	case enAreaSeq03:
		Area03();
		break;

	case enAreaSeq04:
		Area04();
		break;

	case enAreaSeq05:
		Area05();
		break;
	}

//	viiDbg::printf(100,0,"ZoneTime= %d",m_sZoneTimer);
//	viiDbg::printf(100,16,"Area    = %d",GetArea( ) );

}


void CStageLabo::Area01()
{
	//---------------------------------------------------------------------------
	//エリア１
	//---------------------------------------------------------------------------
	static ghPos s_Pos;
	m_sScrollMax = 9600*100;
	static CEneCmnSpark* m_pCEneCmnSpark=NULL;
	static CEneH0503HoundFly *pHoundFly = NULL;
	static CEneH0007YusouHeli *m_pYusouHeli = NULL;

	static Sint32 sTimer = 0;
	sTimer ++;

	s_Pos.x = pGame->GetHound()->m_Pos.x;
	s_Pos.y = pGame->GetHound()->m_Pos.y;

	if(Joy[0].trg&BTN_C)
	{
		CCockpit::GetInstance()->SetDanmakuIndex( 0 , gxFalse );
	}
	if( m_pYusouHeli )
	{
		if(Joy[0].trg&BTN_C)
		{
			m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( m_pYusouHeli->m_Pos.x-16*100 );
			m_pYusouHeli->GetSoldierLeader()->CommandMember( enMoveTypeRideOn );
		}
	}

	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
			1050,//パープルリーダーより司令部へ。
			1100,//こちらライザ、目標地点に到達！
			1200,//まったくトロくせえ戦車だ、
			1300,//電磁シャッターを降ろせ、
			1350,//こちら輸送ヘリ。合流地点に敵影、
			1400,//パープル２より、リーダーへ
			1600,//対爆用の特殊隔壁か。

			1700,//Ｃ４０爆薬をセット、
			1800,//電磁シャッターの消失を確認。

			1850,//突入するぞ、
			1900,//グリフォンの砲撃で

			2000,//こちらパープル２、格納庫に
			3000,//オペレーション・レポート。
			9900,9910,9920,
		};
	}

	BgCommon();
	viiSub::LimitMoveVH();

}



void CStageLabo::Area02()
{
	//-----------------------------------------------------
	//エリア２
	//-----------------------------------------------------
	static Sint32 s_Col = 0;

	double n = viiMath::Sqrt(300);
	pGame->GetHound()->SetWaterProof();
	
	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
			2000,//敵襲ーーっ！！
			2100,//くそっ、空中からの爆雷はなんとか
			2200,//隊長っ！！
			2300,//ライザ！そっちに１機逃げた
			2500,//バツ！逃げろ。こいつはただもん
			2600,//輸送船、安全圏に脱出！
			3000,//オペレーションレポート。
			9900,9910,9920,
		};
		static Sint32 msg_id=0;
		CCockpit::GetInstance()->SetMessage( msgTbl[msg_id] );
		msg_id ++;

		Sint32 max = ARRAY_LENGTH(msgTbl);
		msg_id = (max+msg_id)%max;
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageLabo::Area03()
{
	//-----------------------------------------------------
	//エリア３
	//-----------------------------------------------------

	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
			3000,//全機散会！
			3100,//救助班、まもなく到着します。
			3200,//全員退避しろーーっ
			3300,//住民の避難完了！
			3400,//隊長！！私はバツを連れ戻します
			3500,//ニュートロンポリマー採掘所に潜入。
			3600,//こっちの防衛施設は

			3700,//待ち伏せ！
			3800,//こちら司令室。パープルツー

			3900,//敵、航空部隊を確認！！
			4000,//「救助ヘリ、エンジン部に被弾！」
			4100,//「ダメだ、救助ヘリを放棄する！！」
			4200,//全員救助完了、輸送ヘリ
			4300,//やった！あのやろう、こっちに
			4400,//オペレーションレポート！
			9900,9910,9920,

		};
		static Sint32 msg_id=0;
		CCockpit::GetInstance()->SetMessage( msgTbl[msg_id] );
		msg_id ++;

		Sint32 max = ARRAY_LENGTH(msgTbl);
		msg_id = (max+msg_id)%max;
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageLabo::Area04()
{
	//-----------------------------------------------------
	//エリア４
	//-----------------------------------------------------
 	m_sScrollMax = 7600*100;

	static CEneH0409SlideCannon *pCEneH0409SlideCannon = NULL;

	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
			4000,//まもなく作戦エリアに到達します。
			4100,//研究施設内部のダクトに進入成功！
			4200,//ダクトを抜けた！
			4300,//ニュートロンポリマー研究施設に
			4400,//ちくしょう、脚に一発食らっちまった
			4500,//まもなく脱出ポイントへ到達
			4600,//メイン融合炉を確認
			4700,//融合炉を破壊した、脱出口が開くぞ！
			4800,//オペレーションレポート
			9900,9910,9920,
		};
		static Sint32 msg_id=0;
		CCockpit::GetInstance()->SetMessage( msgTbl[msg_id] );
		msg_id ++;

		Sint32 max = ARRAY_LENGTH(msgTbl);
		msg_id = (max+msg_id)%max;
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageLabo::Area05()
{
	//-----------------------------------------------------
	//エリア５：ボス戦
	//-----------------------------------------------------

	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
/*
			9000,//いてててて
*/
			5000,//陸上戦闘艦がエリアＤに進行！
			5100,//エンジンブロックの破壊を確認！
			5200,//主砲の熱源反応が拡大中。
			5300,//主砲を破壊した、
			5400,//全機に通達！ニュートロンポリマー
			5500,//エネルギーが臨界点を突破します！
			9900,9910,9920,
		};
		static Sint32 msg_id=0;
		CCockpit::GetInstance()->SetMessage( msgTbl[msg_id] );
		msg_id ++;

		Sint32 max = ARRAY_LENGTH(msgTbl);
		msg_id = (max+msg_id)%max;
	}

	BgCommon();
	viiSub::LimitMoveVH();
}


void CStageLabo::AreaCommon()
{
	//-----------------------------------------------------
	//エリア：共通
	//-----------------------------------------------------

	m_sLoop += 8;
	if( m_sLoop >= 480)
	{
		m_sLoop -= 480;
	}
}


//---------------------------------------------------------------------------
//ＶＭＰから敵の生成
//---------------------------------------------------------------------------


void CStageLabo::SetEnemy()
{
	return;
}


void CStageLabo::BgCommon(Sint32 x,Sint32 y)
{
	//----------------------------------------------------
	//背景共通
	//----------------------------------------------------
	BgScrollAdjust();

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	StLayerScroll *pScr;

	for(Sint32 ii=enBgTypeBack; ii<=enBgTypeFrnt; ii++)
	{
		pScr = &LayerScroll[ii];
		viiSub::SetBgLayer( ii , pScr->ox+(sx*pScr->x)/100 , pScr->oy+(sy*pScr->y)/100 );
	}

}

