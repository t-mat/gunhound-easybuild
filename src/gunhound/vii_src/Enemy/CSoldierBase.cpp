//@
//@
//@
//@
//@
#include <gunvalken.h>
#include "enemy.h"
#include "../effect/effectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,
	enScoreRescue = 3000,
};

//待ち
gxSprite AsSpriteSoldierWait[]={
	{enTexPageSoldier,0*32,0*32,32,32,16,32},
	{enTexPageSoldier,1*32,0*32,32,32,16,32},
	{enTexPageSoldier,2*32,0*32,32,32,16,32},
};

int SoldierWaitPatternArray[]={
	0,0,0,0,0,0,1,2,1,
};

//歩き
gxSprite AsSpriteSoldierWalk[]={
	{enTexPageSoldier,0*32,1*32,32,32,16,32},
	{enTexPageSoldier,1*32,1*32,32,32,16,32},
	{enTexPageSoldier,2*32,1*32,32,32,16,32},
};

int SoldierWalkPatternArray[]={
	0,1,2,
};

//逃げる
gxSprite AsSpriteSoldierEscape[]={
	{enTexPageSoldier,3*32,7*32,32,32,16,32},
	{enTexPageSoldier,4*32,7*32,32,32,16,32},
	{enTexPageSoldier,5*32,7*32,32,32,16,32},
};

int SoldierEscapePatternArray[]={
	0,1,2,
};

//ふっとび下
gxSprite AsSpriteSoldierDamageJumpDown[]={
	{enTexPageSoldier,0*32,2*32,32,32,16,32},
	{enTexPageSoldier,1*32,2*32,32,32,16,32},
	{enTexPageSoldier,2*32,2*32,32,32,16,32},
};

int SoldierDamageJumpDownPatternArray[]={
	0,1,2,
};

//ふっとび上
gxSprite AsSpriteSoldierDamageJumpUp[]={
	{enTexPageSoldier,0*32,3*32,32,32,16,32},
	{enTexPageSoldier,1*32,3*32,32,32,16,32},
	{enTexPageSoldier,2*32,3*32,32,32,16,32},
};

int SoldierDamageJumpUpPatternArray[]={
	0,1,2,
};

//ダウン上
gxSprite AsSpriteSoldierDownUp[]={
	{enTexPageSoldier,0*32,4*32,32,32,16,32},
	{enTexPageSoldier,1*32,4*32,32,32,16,32},
	{enTexPageSoldier,2*32,4*32,32,32,16,32},
};

int SoldierDownUpPatternArray[]={
	0,1,2,
};

//ダウン下
gxSprite AsSpriteSoldierDownDown[]={
	{enTexPageSoldier,0*32,5*32,32,32,16,32},
	{enTexPageSoldier,1*32,5*32,32,32,16,32},
	{enTexPageSoldier,2*32,5*32,32,32,16,32},
};

int SoldierDownDownPatternArray[]={
	0,1,2,
};

//起き上がり上
gxSprite AsSpriteSoldierGetUpU[]={
	{enTexPageSoldier,3*32,1*32,32,32,16,32},
	{enTexPageSoldier,4*32,1*32,32,32,16,32},
	{enTexPageSoldier,5*32,1*32,32,32,16,32},
};

int SoldierGetUpUPatternArray[]={
	0,1,2,
};


//起き上がり下
gxSprite AsSpriteSoldierGetUpD[]={
	{enTexPageSoldier,3*32,0*32,32,32,16,32},
	{enTexPageSoldier,4*32,0*32,32,32,16,32},
	{enTexPageSoldier,5*32,0*32,32,32,16,32},
};

int SoldierGetUpDPatternArray[]={
	0,1,2,
};


//段差のぼり
gxSprite AsSpriteSoldierClimb[]={
	{enTexPageSoldier,3*32,2*32,32,32,16,16+16},
	{enTexPageSoldier,4*32,2*32,32,32,16,16+22},
	{enTexPageSoldier,5*32,2*32,32,32,16,16+27},
	{enTexPageSoldier,6*32,2*32,32,32,16+8,16+32},
};

int SoldierClimbPatternArray[]={
	0,0,0,1,1,2,3
};


//起き上がり上
gxSprite AsSpriteSoldierBombSet[]={
	{enTexPageSoldier,3*32,5*32,32,32,16,32},	//歩く
	{enTexPageSoldier,4*32,5*32,32,32,16,32},	//歩く
	{enTexPageSoldier,5*32,5*32,32,32,16,32},	//歩く
	{enTexPageSoldier,6*32,5*32,32,32,16,32},	//セット
	{enTexPageSoldier,7*32,5*32,32,32,16,32},	//セット

	{enTexPageSoldier,5*32,6*32,32,32,16,32},	//リーディング
	{enTexPageSoldier,6*32,6*32,32,32,16,32},	//リーディング
	{enTexPageSoldier,7*32,6*32,32,32,16,32},	//リーディング
};


int SoldierBombLineSetPatternArray[]={
	0,1,2,1
};

int SoldierBombSetPatternArray[]={
	3,4,
};

int SoldierLeadingPatternArray[]={
	5,6,7,6
};

gxSprite AsSpriteSoldierBombSwitch[]={
	{enTexPageSoldier,3*32,6*32,32,32,16,32},	//スイッチ１
	{enTexPageSoldier,4*32,6*32,32,32,16,32},	//スイッチ２

};

int SoldierBombSwitchPatternArray[]={
	5,5,5,6,6,6
};

CSoldierBase::CSoldierBase()
{
//	m_pObj   = NULL;
	m_pObj = new CObj();
	m_sMoveX   = m_sMoveY = 0;
	m_sForce = enForceVersis;
	//m_bMuteki  = gxFalse;
	m_sMaxSpeed = m_sSpeed   = 90+viiSub::Rand()%20;
	m_bGravit  = gxTrue;
	m_sGravit  = 0;
	m_sWalkCnt = 0;
	m_sDir     = DIR_RIGHT;
	m_sActSeq = 0;
	//m_sPrio    = 
	SetPrioDefault();

	m_sSprite  = 0;
	m_sTargetPosition = 0;
	m_pSprite = NULL;

	m_sWaitTimer = 0;
	m_sJumpDamageTimer = 0;
	m_sDownTimer = 0;
	m_bEnd = gxFalse;

	m_sWalkType = enWalkTypeNormal;

	m_pLeader = NULL;
	m_sCivilType = 0;

	m_HitKurai.DisableAiming();

	for(Sint32 ii=0; ii<enMemberMax; ii++ )
	{
		m_pMember[ ii ] = NULL;
	}
	m_sMemberNum = 0;
	m_stOffset.x = m_stOffset.y = 0;
	ChangeMoveType( enMoveTypeWait );
	m_fScale=0.8f;	//体格
	m_fScale += (viiSub::Rand()%10)/100.f;
	m_bMoveEnd = gxFalse;
	m_bWaitScReenIn = gxFalse;
}


CSoldierBase::~CSoldierBase()
{
	if(	m_pObj ) delete m_pObj;
	m_pObj = NULL;

}


void CSoldierBase::SeqInit()
{
//	if( m_pObj == NULL )
	m_pObj->pos.x = _x;
	m_pObj->pos.y = _y;

	m_sPositionX = _x;
	m_sPositionY = _y;

	m_Pos.x = m_pObj->pos.x;
	m_Pos.y = m_pObj->pos.y;

	if( m_bWaitScReenIn )
	{
		if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
		{
			return;
		}
	}

	SetActionSeq( enActionSeqMain );
}

void CSoldierBase::SeqMain()
{

	EveryAction();

	Main();

}

void CSoldierBase::SeqCrash()
{
	
	
}

void CSoldierBase::SeqEnd()
{
	
	
}

Sint32 CSoldierBase::AutoDetectWalkType()
{
	Sint32 sOrder = m_sWalkType;

	if( pGame->GetMarineLine() )
	{
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			sOrder = enWalkTypeDrawn;
		}
		else
		{
			sOrder = m_sWalkType;//enWalkTypeEscape;
		}
	}
	else
	{

	}

	if( sOrder != m_sWalkType )
	{
		m_sWalkType = sOrder;

		switch( m_sWalkType ) {
		case enWalkTypeNormal:
			ChangeMoveType( enMoveTypeMove );
			break;
		case enWalkTypeEscape:
			ChangeMoveType( enMoveTypeEscape );
			break;
		case enWalkTypeDrawn:
			ChangeMoveType( enMoveTypeDrawn );
			break;
		}
	}

	return sOrder;

}

void CSoldierBase::ChangeMoveType(enMoveType _eMoveType)
{
	eMoveType = _eMoveType;
	m_sWaitTimer = 0;
	m_sActSeq = 0;
	m_stOffset.x = 0;
	m_stOffset.y = 0;
	//m_bMuteki = gxFalse;
}

bool CSoldierBase::IsNodamage()
{
//		if( m_bMuteki ) return gxTrue;

	if( m_HitKurai.IsImpacted() )
	{
//		return gxTrue;
	}

	switch(eMoveType){
	case enMoveTypeFlyDown:
	case enMoveTypeFlyUp:
	case enMoveTypeRideOffHeli:
	case enMoveTypeBombSet:		//爆弾セット
	case enMoveTypeBombSwitch:	//爆弾スイッチ
	case enMoveTypeLeading:		//部下をリード
	case enMoveTypeRideOn:		//乗り込み
	case enMoveTypeGetUpU:
	case enMoveTypeGetUpD:
	case enMoveTypeDrawn:
	case enMoveTypeFrontFalling:	//落下
	case enMoveTypeDownUp:
	case enMoveTypeDownDown:
		return gxTrue;
	}
	return gxFalse;
}

void CSoldierBase::SetLeader( CSoldierBase *p )
{
	//リーダーの設定

	m_pLeader = p;
	m_pLeader->SetMember(this);
}


void CSoldierBase::SetMember( CSoldierBase *p )
{
	//部下の追加

	if( m_sMemberNum < enMemberMax )
	{
		m_pMember[ m_sMemberNum ] = p;
		m_sMemberNum ++;
	}

}

void CSoldierBase::SetForce( Sint32 sForce )
{
	m_sForce = sForce;
}


void CSoldierBase::SetFuttobi( Sint32 sSpd )
{

	m_sMaxSpeed = m_sSpeed = sSpd;
	m_sMoveX = sSpd;
	ResetJumpDamageTimer();

}

void CSoldierBase::CommandMember( enMoveType _eMoveType )
{
	if( _eMoveType==enMoveTypeLeading )
	{
		for( Sint32 ii=0; ii<m_sMemberNum ; ii++ )
		{
			GetMember( 0 )->SetTargetPosition( m_Pos.x - 120*100 );
			GetMember( 1 )->SetTargetPosition( m_Pos.x + 120*100 );
			GetMember( 2 )->SetTargetPosition( m_Pos.x + 240*100 );
		}
	}
	if( _eMoveType==enMoveTypeMove )
	{
		for( Sint32 ii=0; ii<m_sMemberNum ; ii++ )
		{
			m_pMember[ ii ]->SetTargetPosition( m_sTargetPosition );
		}
	}
	if( _eMoveType==enMoveTypeRideOn )
	{
		for( Sint32 ii=0; ii<m_sMemberNum ; ii++ )
		{
			m_pMember[ ii ]->SetTargetPosition( m_sTargetPosition );
			m_pMember[ ii ]->ChangeMoveType( enMoveTypeRideOn );
		}
	}
	else if( _eMoveType==enMoveTypeBombSet )
	{
		if( m_sMemberNum >= 3 )
		{
			//ステージ山の爆弾セット
			GetMember( 0 )->ChangeMoveType( enMoveTypeBombSet );
			GetMember( 0 )->SetTargetPosition(m_sTargetPosition+150*100);
			GetMember( 1 )->ChangeMoveType( enMoveTypeBombSwitch );
			GetMember( 2 )->SetTargetPosition(m_sTargetPosition+200*100);
			GetMember( 2 )->ChangeMoveType( enMoveTypeMove );
		}
	}

}


void CSoldierBase::EveryAction()
{

	if(m_HitKurai.is_damage())
	{
		if( !IsNodamage() )
		{
			if(m_HitKurai.dx < 0)
			{
				m_sMoveX = +(viiSub::Rand()%200);
				ChangeMoveType( enMoveTypeFlyDown );
				m_sJumpDamageTimer = 0;
			}
			else
			{
				m_sMoveX = -(viiSub::Rand()%200);
				ChangeMoveType( enMoveTypeFlyUp );
				m_sJumpDamageTimer = 0;
			}
		}
	}

	m_pObj->mov.x += m_sMoveX;
	m_pObj->mov.y =  120+m_sGravit;

	if(m_pObj->mov.x <= -m_sMaxSpeed) m_pObj->mov.x = -m_sMaxSpeed;
	if(m_pObj->mov.x >=  m_sMaxSpeed) m_pObj->mov.x =  m_sMaxSpeed;

	if( m_bGravit )
	{
		if( m_pObj->easy_collision_check() && m_sGravit >= 0 )
		{
			//着地中
			m_sGravit = 0;
	//		if( m_pObj->mov.x ==  m_sSpeed) m_sDir = DIR_RIGHT;
	//		if( m_pObj->mov.x == -m_sSpeed) m_sDir = DIR_LEFT;
		}
		else
		{
			//落下中

			m_sWalkCnt = 1;	//歩きモーションをリセット
			m_sGravit += 12;
		}
	}
	else
	{
		m_sGravit = 0;
		m_pObj->mov.y = 0;
	}

	m_pObj->pos.x += m_pObj->mov.x;
	m_pObj->pos.y += m_pObj->mov.y;

	m_sPositionX = m_pObj->pos.x;
	m_sPositionY = m_pObj->pos.y;

	//くらい判定登録
	m_Pos.x = m_sPositionX;
	m_Pos.y = m_sPositionY;

	m_HitKurai.ax1 = -1;
	m_HitKurai.ay1 = -4;
	m_HitKurai.ax2 =  1;
	m_HitKurai.ay2 =  0;

	if( !IsNodamage() )
	{
		if( m_sForce == enForceVersis)
		{
			m_HitKurai.SetHantei( ID_VERSIS_DEF , &m_Pos );
		}
		else
		{
			m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );
		}
	}

	if( m_bEnd )
	{
		if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
		{
			SetActionSeq( enActionSeqEnd );
		}
	}

}


void CSoldierBase::Draw()
{

	//DEBUG
/*
	viiDbg::printf(100,80,"test Soldier(%d,%d)[%d]",m_pObj->pos.x/100,m_pObj->pos.y/100,eMoveType);
	viiDbg::printf(100,96,"test Soldier(%d,%d)",m_pObj->mov.x,m_pObj->mov.y);

	viiDbg::printf(100,114,"%s%s%s" ,
		(pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*-1,m_pObj->pos.y/100-16))? "■" : "□",
		(pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*0 ,m_pObj->pos.y/100-16))? "■" : "□",
		(pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*1 ,m_pObj->pos.y/100-16))? "■" : "□"
		);
	viiDbg::printf(100,124,"%s%s%s" ,
		(pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*-1,m_pObj->pos.y/100-8))? "■" : "□",
		(pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*0 ,m_pObj->pos.y/100-8))? "■" : "□",
		(pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*1 ,m_pObj->pos.y/100-8))? "■" : "□"
		);
	viiDbg::printf(100,134,"%s%s%s" ,
		(pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*-1,m_pObj->pos.y/100+0))? "■" : "□",
		(pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*0 ,m_pObj->pos.y/100+0))? "■" : "□",
		(pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*1 ,m_pObj->pos.y/100+0))? "■" : "□"
		);
*/
	if( m_pSprite == NULL ) return;

	int atr = (m_sDir == DIR_LEFT)? ATR_DFLT : ATR_FLIP_X;

	Sint32 sCol = ARGB(0xff,0x00,0x00,0x80);

	if( m_sForce == enForceVersis)
	{
		sCol = ARGB(0xff,0xe0,0xff,0xff);
		if( m_pLeader == NULL )
		{
			sCol = ARGB(0xff,0xff,0xff,0xff);
		}
	}
	else
	{
		sCol = ARGB(0xff,0xF0,0xA0,0xA0);
		if( m_pLeader == NULL )
		{
			sCol = ARGB(0xff,0xF0,0x80,0x80);
		}
	}

	Sint32 x,y;
	x = m_pObj->pos.x + m_stOffset.x;
	y = m_pObj->pos.y + m_stOffset.y;

	gxSprite s_Temp;

	s_Temp = *m_pSprite;

	if( m_sCivilType == 1 )
	{
		s_Temp.page = enTexPageEnemyStage05;
	}
	if( m_sCivilType == 2 )
	{
		s_Temp.page = enTexPageEnemyStage06;
	}
	viiDraw::Sprite( &s_Temp  , x , y , m_sPrio,atr ,sCol , m_fScale );
}


void CSoldierBase::SetJump()
{

	m_sGravit = -(280+viiSub::Rand()%40);

}


void CSoldierBase::DebugControl()
{
	if( Joy[0].trg&BTN_A ) SetJump();
	if( Joy[0].trg&BTN_X )
	{
		ChangeMoveType( enMoveTypeFlyUp );
		m_sJumpDamageTimer = 0;
	}
	if( Joy[0].trg&BTN_Y )
	{
		ChangeMoveType( enMoveTypeFlyDown );
		m_sJumpDamageTimer = 0;
	}

	if( Joy[0].psh&JOY_L )
	{
		m_sTargetPosition = m_sPositionX - 16*100;
		ChangeMoveType( enMoveTypeMove );
	}

	if( Joy[0].psh&JOY_R )
	{
		m_sTargetPosition = m_sPositionX + 16*100;
		ChangeMoveType( enMoveTypeMove );
	}


}


void CSoldierBase::Main()
{
	//行動
	//DebugControl();

	switch(eMoveType) {
	case enMoveTypeWait:			//指示待ち
		MoveTypeWait();
		break;
	case enMoveTypeMove:			//目標地点へ移動(中腰)
		MoveTypeMove();
		break;
	case enMoveTypeFlyUp:			//ふっとび(仰向け)
		MoveTypeFlyUp();
		break;
	case enMoveTypeFlyDown:			//ふっとび(うつぶせ)
		MoveTypeFlyDown();
		break;
	case enMoveTypeGetUpU:			//起き上がり（仰向け）
		MoveTypeGetUpD();
		break;
	case enMoveTypeGetUpD:			//起き上がり（うつ伏せ）
		MoveTypeGetUpU();
		break;
	case enMoveTypeDownUp:			//ダウン(仰向け)
		MoveTypeDownUp();
		break;
	case enMoveTypeDownDown:		//ダウン(うつぶせ)
		MoveTypeDownDown();
		break;
	case enMoveTypeClimb:		//段差を上る
		MoveTypeClimb();
		break;
	case enMoveTypeRideOffHeli:
		MoveTypeRideOffHeli();
		break;
	case enMoveTypeRideOn:			//トラックへのる
		MoveTypeRideOn();
		break;
	case enMoveTypeRideOff:			//トラックから降りる
		MoveTypeRideOff();
		break;
	case enMoveTypeRopeUp:			//ロープをのぼる
		MoveTypeRopeUp();
		break;
	case enMoveTypeLadderDown:		//ロープを降りる
		MoveTypeLadderDown();
		break;
	case enMoveTypeLadderUp:		//はしごをつたう
		MoveTypeLadderUp();
		break;
	case enMoveTypeSearch:			//探す
		MoveTypeSearch();
		break;
	case enMoveTypeThrowBomb:		//投げる
		MoveTypeThrowBomb();
		break;
	case enMoveTypeTrgtGun:			//銃を構える
		MoveTypeTrgtGun();
		break;
	case enMoveTypeShottGun:		//銃を撃つ
		MoveTypeShottGun();
		break;
	case enMoveTypeTrgtBuz:			//バズーカを構える
		MoveTypeTrgtBuz();
		break;
	case enMoveTypeShotBuz:			//バズーカを撃つ
		MoveTypeShotBuz();
		break;
	case enMoveTypeShotMG:			//機銃砲座で撃つ
		MoveTypeShotMG();
		break;
	case enMoveTypeEscape:			//逃げる
		MoveTypeEscape();
		break;
	case enMoveTypeDrawn:
		MoveTypeDrawn();
		break;
	case enMoveTypeSupportStand:	//弾薬サポート(たち)
		MoveTypeSupportStand();
		break;
	case enMoveTypeSupportSit:		//弾薬サポート(すわり)
		MoveTypeSupportSit();
		break;
	case enMoveTypePush:			//押す
		MoveTypePush();
		break;
	case enMoveTypePull:			//引く
		MoveTypePull();
		break;
	case enMoveTypeDirectAssault:	//突撃を指示
		MoveTypeDirectAssault();
		break;
	case enMoveTypeDirectEscape:	//退却を指示
		MoveTypeDirectEscape();
		break;
	case enMoveTypeBackRolling:		//後転
		MoveTypeBackRolling();
		break;
	case enMoveTypeFrontRolling:	//前転
		MoveTypeFrontRolling();
		break;
	case enMoveTypeFrontFalling:	//落下
		MoveTypeFalling();
		break;
	case enMoveTypeBombSet:		//爆弾セット
		MoveTypeBombSet();		//爆弾セット
		break;
	case enMoveTypeBombSwitch:	//爆弾スイッチ
		MoveTypeBombSwitch();	//爆弾スイッチ
		break;
	case enMoveTypeLeading:		//部下をリード
		MoveTypeLeading();		//部下をリード
		break;
	case enMoveTypeEscapeMachine:
		MoveTypeEscapeMachine();//マシンから脱出
		break;

	default:
		break;
	};

}



void CSoldierBase::MoveTypeWait()
{
	//指示待ち

	int pattern = GetSpritePattern(m_sWaitTimer,16,ARRAY_LENGTH(SoldierWaitPatternArray) );

	if( !IsTargetPosition() )
	{
		switch( m_sWalkType ) {
		case enWalkTypeNormal:
			ChangeMoveType( enMoveTypeMove );
			break;
		case enWalkTypeEscape:
			ChangeMoveType( enMoveTypeEscape );
			break;
		case enWalkTypeDrawn:
			ChangeMoveType( enMoveTypeDrawn );
			break;
		}

	}

	m_sSprite = SoldierWaitPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierWait[m_sSprite];

	m_sWaitTimer ++;

}

void CSoldierBase::MoveCommon()
{
	//目標地点へ移動(中腰)
	if( m_sTargetPosition < m_sPositionX )	//+800
	{
		m_sMoveX = -4;
		m_sDir = DIR_LEFT;
		if( IsStopped() && IsLanding() )
		{
//			if( pGame->pBg->get_kabetype(m_pObj->pos.x/100-0*1,m_pObj->pos.y/100) == 0
//				&& viiSub::IsGroundBlock(m_pObj->pos.x/100-0*1,m_pObj->pos.y/100) )
			if( pGame->pBg->get_kabetype(m_pObj->pos.x/100-1*1,m_pObj->pos.y/100-8*0) == 1
			&&  pGame->pBg->get_kabetype(m_pObj->pos.x/100-1*1,m_pObj->pos.y/100-8*1) == 1
			&&  pGame->pBg->get_kabetype(m_pObj->pos.x/100-1*1,m_pObj->pos.y/100-8*2) == 0	)

			{
				//壁がなければジャンプさせる
				ChangeMoveType( enMoveTypeClimb );
				//SetJump();
			}
			
		}
	}
	else if( m_sTargetPosition > m_sPositionX )		//-800
	{
		m_sMoveX = +4;
		m_sDir = DIR_RIGHT;
		if( IsStopped() && IsLanding() )
		{
//			if( pGame->pBg->get_kabetype(m_pObj->pos.x/100+8*1,m_pObj->pos.y/100-16) == 0
//				&&  viiSub::IsGroundBlock(m_pObj->pos.x/100+8*1,m_pObj->pos.y/100) )
			if( pGame->pBg->get_kabetype(m_pObj->pos.x/100+1*1,m_pObj->pos.y/100-8*0) == 1
			&&  pGame->pBg->get_kabetype(m_pObj->pos.x/100+1*1,m_pObj->pos.y/100-8*1) == 1
			&&  pGame->pBg->get_kabetype(m_pObj->pos.x/100+1*1,m_pObj->pos.y/100-8*2) == 0)
			{
				//壁がなければジャンプさせる
				ChangeMoveType( enMoveTypeClimb );
//				SetJump();
			}
			
		}
	}
	else
	{
		m_sMoveX = 0;
		if( MoveSpeedDecrement() )
		{
			ChangeMoveType( enMoveTypeWait );
		}
	}
}


void CSoldierBase::MoveTypeMove()
{
	//目標地点へ移動(中腰)

	AutoDetectWalkType();

		Sint32 sSpeed = m_sSpeed;

		Sint32 sDist = gxAbs(m_pObj->pos.x - m_sTargetPosition);

		if( sDist < 50 )
		{
			m_pObj->mov.x = 0;
			m_sMoveX = 0;
			ChangeMoveType( enMoveTypeWait );
		}
		else
		{
			if( sDist < 100 )
			{
				sSpeed = 10;
			}
			else if( sDist < 200 )
			{
				sSpeed = 30;
			}

			if( m_pObj->pos.x < m_sTargetPosition )
			{
				m_pObj->mov.x = sSpeed;
			}
			else
			{
				m_pObj->mov.x = -sSpeed;
			}
			MoveCommon();
		}

	int pattern = GetSpritePattern(m_sWaitTimer,8,ARRAY_LENGTH(SoldierWalkPatternArray) );

	m_sSprite = SoldierWalkPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierWalk[m_sSprite];

	m_sWaitTimer ++;

}


void CSoldierBase::MoveTypeFlyUp()
{
	//ふっとび(仰向け)

	if( m_sJumpDamageTimer == 0)
	{
		SetJump();
	}
	else if( IsLanding() )
	{
		ResetJumpDamageTimer();
		m_sDownTimer = 60+viiSub::Rand()%80;
		ChangeMoveType( enMoveTypeDownDown );
	}

	int pattern = GetSpritePattern(m_sJumpDamageTimer,12,ARRAY_LENGTH(SoldierDamageJumpUpPatternArray),gxFalse );

	m_sSprite = SoldierDamageJumpUpPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierDamageJumpUp[m_sSprite];

	m_sJumpDamageTimer ++;
}


void CSoldierBase::MoveTypeFlyDown()
{
	//ふっとび(うつぶせ)

	if( m_sJumpDamageTimer == 0)
	{
		SetJump();
	}
	else if( IsLanding() )
	{
		ResetJumpDamageTimer();
		m_sDownTimer = 60+viiSub::Rand()%80;
		ChangeMoveType( enMoveTypeDownUp );

	}

	int pattern = GetSpritePattern(m_sJumpDamageTimer,12,ARRAY_LENGTH(SoldierDamageJumpDownPatternArray),gxFalse );

	m_sSprite = SoldierDamageJumpDownPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierDamageJumpDown[m_sSprite];

	m_sJumpDamageTimer ++;

}


void CSoldierBase::MoveTypeDownUp()
{
	//ダウン(仰向け)
	int pattern = GetSpritePattern(m_sJumpDamageTimer,12,ARRAY_LENGTH(SoldierDownUpPatternArray),gxFalse );

	m_sMoveX = 0;

	m_sSprite = SoldierDownUpPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierDownUp[m_sSprite];

	//速度減衰
	if( MoveSpeedDecrement() )
	{
		m_sDownTimer --;
		if(m_sDownTimer <= 0)
		{
			ChangeMoveType( enMoveTypeGetUpU );
			ResetJumpDamageTimer();
			return;
		}
	}

	m_sJumpDamageTimer ++;

}


void CSoldierBase::MoveTypeDownDown()
{
	//ダウン(うつぶせ)

	int pattern = GetSpritePattern(m_sJumpDamageTimer,12,ARRAY_LENGTH(SoldierDownDownPatternArray),gxFalse );

	m_sMoveX = 0;

	m_sSprite = SoldierDownDownPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierDownDown[m_sSprite];

	//速度減衰
	if( MoveSpeedDecrement() )
	{
		m_sDownTimer --;
		if(m_sDownTimer <= 0)
		{
			ChangeMoveType( enMoveTypeGetUpD );
			ResetJumpDamageTimer();
			return;
		}
	}

	m_sJumpDamageTimer ++;

}


void CSoldierBase::MoveTypeGetUpU()
{
	//起き上がり（上）

	int pattern = GetSpritePattern(m_sJumpDamageTimer,8,ARRAY_LENGTH(SoldierGetUpUPatternArray)+1,gxFalse );
	if( pattern == ARRAY_LENGTH(SoldierGetUpUPatternArray))
	{
		pattern --;
		ChangeMoveType( enMoveTypeWait );
	}

	m_sSprite = SoldierGetUpUPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierGetUpU[m_sSprite];

	m_sJumpDamageTimer ++;
}


void CSoldierBase::MoveTypeGetUpD()
{
	//起き上がり（下）

	int pattern = GetSpritePattern(m_sJumpDamageTimer,8,ARRAY_LENGTH(SoldierGetUpDPatternArray)+1,gxFalse );

	if( pattern == ARRAY_LENGTH(SoldierGetUpUPatternArray))
	{
		pattern --;
		ChangeMoveType( enMoveTypeWait );
	}

	m_sSprite = SoldierGetUpDPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierGetUpD[m_sSprite];

	m_sJumpDamageTimer ++;
}


gxBool CSoldierBase::MoveTypeClimb()
{
	//段差を上る

	int pattern = GetSpritePattern(m_sWaitTimer,8,ARRAY_LENGTH(SoldierClimbPatternArray)+1,gxFalse );

	if( pattern == ARRAY_LENGTH(SoldierClimbPatternArray))
	{
		m_pObj->pos.y -= 1600;
		ChangeMoveType( enMoveTypeMove );
		Main();
		return gxTrue;
	}

	m_sSprite = SoldierClimbPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierClimb[m_sSprite];

	m_sWaitTimer ++;

	return gxFalse;
}


void CSoldierBase::MoveTypeRideOffHeli()
{
	//ヘリから降りる
	Sint32 sRunTime = 42;
	if( m_sWaitTimer == 0)
	{
		m_sPrio = PRIO_ENEMY-1;
		m_sDir = DIR_LEFT;
		//m_bMuteki = gxTrue;
		m_sMoveX = -40;
		m_bGravit = gxFalse;
	}
	else if( m_sWaitTimer == sRunTime )
	{
		m_bGravit = gxTrue;
		//SetJump();
		SetPrioDefault();
		m_sGravit = -120;
	}
	else if( m_sWaitTimer > sRunTime )
	{
		if( IsLanding() )
		{
			ChangeMoveType( enMoveTypeMove );
			return;
		}
	}

	int pattern = GetSpritePattern(m_sWaitTimer,16,ARRAY_LENGTH(SoldierWalkPatternArray) );

	m_sSprite = SoldierWalkPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierWalk[m_sSprite];

	m_sWaitTimer ++;
}



void CSoldierBase::MoveTypeRideOn()
{
	//輸送ヘリにのる

	if( m_sActSeq == 0)
	{
		SetPrioDefault();
		m_sDir = DIR_LEFT;
		m_bGravit = gxTrue;
		SetPrioDefault();
		m_sActSeq = 10;
	}
	else if( m_sActSeq == 10 )
	{
		//目的地周辺

		MoveTypeMove();

		if( IsTargetPosition() )
		{
			m_sWaitTimer = 0;
			m_sActSeq = 15;
		}
	}
	else if( m_sActSeq == 15 )
	{
		//精密な目的地へ移動
		Sint32 sSpeed = m_sSpeed;

		Sint32 sDist = gxAbs(m_pObj->pos.x - m_sTargetPosition);

		if( sDist < 50 )
		{
			m_sActSeq = 20;
			return;
		}
		else if( sDist < 100 )
		{
			sSpeed = 10;
		}
		else if( sDist < 200 )
		{
			sSpeed = 30;
		}

		if( m_pObj->pos.x < m_sTargetPosition )
		{
			m_pObj->mov.x = sSpeed;
		}
		else
		{
			m_pObj->mov.x = -sSpeed;
		}

		MoveTypeMove();
	}
	else if( m_sActSeq == 20 )
	{
		//乗り込む
		m_sPrio = PRIO_ENEMY-1;
		m_sDir = DIR_RIGHT;
		m_sMoveX = 0;
		m_pObj->mov.x = 0;
		if( MoveTypeClimb() )
		{
			m_pObj->pos.y -= 3200*100;
			if( m_sCivilType )
			{
				new CEffScore( m_Pos.x , m_Pos.y-32*100 , enScoreRescue );
				viiSub::AddScore( enScoreRescue );
			}
			m_bMoveEnd = gxTrue;
			m_sActSeq = 30;
		}
	}
	else if( m_sActSeq == 30 )
	{
		m_sActSeq = 50;
	}
	else if( m_sActSeq == 50 )
	{
		//if( m_bClear )
		{
			m_bEnd = gxTrue;
		}
		return;
	}

	eMoveType = enMoveTypeRideOn;

}


void CSoldierBase::MoveTypeRideOff()
{
	//トラックから降りる
}


void CSoldierBase::MoveTypeRopeUp()
{
	//ロープをのぼる
}


void CSoldierBase::MoveTypeLadderDown()
{
	//ロープを降りる
}


void CSoldierBase::MoveTypeLadderUp()
{
	//はしごをつたう
}


void CSoldierBase::MoveTypeSearch()
{
	//探す
}


void CSoldierBase::MoveTypeThrowBomb()
{
	//投げる
}


void CSoldierBase::MoveTypeTrgtGun()
{
	//銃を構える
}


void CSoldierBase::MoveTypeShottGun()
{
	//銃を撃つ
}


void CSoldierBase::MoveTypeTrgtBuz()
{
	//バズーカを構える
}


void CSoldierBase::MoveTypeShotBuz()
{
	//バズーカを撃つ
}


void CSoldierBase::MoveTypeShotMG()
{
	//機銃砲座で撃つ
}


void CSoldierBase::MoveTypeEscape()
{
	//逃げる::目標地点へ移動(中腰)

	AutoDetectWalkType();

	if( enWalkTypeDrawn != m_sWalkType )
	{
		if( pGame->GetHound() )
		{
			Sint32 vx = pGame->GetHound()->m_Pos.x;
			if( m_pObj->pos.x < vx ) m_sTargetPosition = vx - 9600*100;
			if( m_pObj->pos.x > vx ) m_sTargetPosition = vx + 9600*100;
		}
	}

	MoveCommon();

	int pattern = GetSpritePattern(m_sWaitTimer,8,ARRAY_LENGTH(SoldierEscapePatternArray) );

	m_sSprite = SoldierEscapePatternArray[pattern];
	m_pSprite = &AsSpriteSoldierEscape[m_sSprite];

	m_sWaitTimer ++;

}


void CSoldierBase::MoveTypeDrawn()
{
	//溺れる::目標地点へ移動(中腰)
	AutoDetectWalkType();

	MoveCommon();

	m_sTargetPosition = 0;

	int pattern = GetSpritePattern(m_sWaitTimer,8,ARRAY_LENGTH(SoldierEscapePatternArray) );

	m_sSprite = SoldierEscapePatternArray[pattern];
	m_pSprite = &AsSpriteSoldierEscape[m_sSprite];

	Sint32 yy = pGame->GetMarineLine() - m_pObj->pos.y;

	m_stOffset.y = yy/4 + viiMath::Cos100( (m_sWaitTimer*4)%360 )*4;

	m_sWaitTimer ++;

}



void CSoldierBase::MoveTypeSupportStand()
{
	//弾薬サポート(たち)
}


void CSoldierBase::MoveTypeSupportSit()
{
	//弾薬サポート(すわり)
}


void CSoldierBase::MoveTypePush()
{
	//押す
}


void CSoldierBase::MoveTypePull()
{
	//引く
}


void CSoldierBase::MoveTypeDirectAssault()
{
	//突撃を指示
}


void CSoldierBase::MoveTypeDirectEscape()
{
	//退却を指示
}

void CSoldierBase::MoveTypeFrontRolling()
{
	//前転

}


void CSoldierBase::MoveTypeBackRolling()
{
	//後転
}


void CSoldierBase::MoveTypeFalling()
{
	//落下
	
}


void CSoldierBase::MoveTypeBombSet()
{
	//爆弾セット

	if( m_sActSeq == 0)
	{
		m_sPrio = PRIO_ENEMY;
		m_sDir = DIR_LEFT;
		m_sMoveX = 10;
		m_bGravit = gxTrue;
		SetPrioDefault();
		//m_bGravit = gxFalse;
		m_sActSeq = 10;
	}
	else if( m_sActSeq == 10 )
	{
		if( m_sTargetPosition < m_sPositionX )
		{
			m_sMoveX = 0;
			m_pObj->mov.x = 0;
			m_pObj->pos.x = m_sTargetPosition;
			m_sWaitTimer = 0;
			m_sActSeq = 20;
		}
		else
		{
			m_sMoveX = 10;
			m_pObj->mov.x = 40;
		}

		int pattern = GetSpritePattern( m_sWaitTimer,16,ARRAY_LENGTH( SoldierBombLineSetPatternArray ) );
		m_sSprite = SoldierBombLineSetPatternArray[ pattern ];
		m_pSprite = &AsSpriteSoldierBombSet[ m_sSprite ];
	}
	else if( m_sActSeq == 20 )
	{
		m_sDir = DIR_LEFT;

		int pattern = GetSpritePattern( m_sWaitTimer,8,ARRAY_LENGTH( SoldierBombSetPatternArray ) );
		m_sSprite = SoldierBombSetPatternArray[ pattern ];
		m_pSprite = &AsSpriteSoldierBombSet[ m_sSprite ];

		if( m_sWaitTimer >= 80 )
		{
			m_sWaitTimer = 0;
			m_sActSeq = 30;
		}
	}
	else if( m_sActSeq == 30 )
	{
		m_sDir = DIR_LEFT;

		int pattern = GetSpritePattern( m_sWaitTimer,12,ARRAY_LENGTH( SoldierLeadingPatternArray ) );
		m_sSprite = SoldierLeadingPatternArray[ pattern ];
		m_pSprite = &AsSpriteSoldierBombSet[ m_sSprite ];

		if( m_sWaitTimer >= 192 )
		{
			m_sWaitTimer = 0;
			m_sActSeq = 40;
		}
	}
	else if( m_sActSeq == 40 )
	{
		if( m_sWaitTimer >= 16 )
		{
			//viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 5 );
			viiEff::Set(EFF_RING,m_Pos.x , m_Pos.y,argset(250));
			//SetFuttobi(120);
			ChangeMoveType( enMoveTypeMove );
			m_sMaxSpeed = 180;
			m_sWaitTimer = 0;
			m_sActSeq = 50;
		}
	}
	else if( m_sActSeq == 50 )
	{
		return;
	}

	m_sWaitTimer ++;

}

void CSoldierBase::MoveTypeBombSwitch()
{
	//爆弾スイッチ

	m_sMoveX = 0;
	m_pObj->mov.x = 0;
	m_pObj->pos.x = m_sTargetPosition;

	switch(m_sActSeq){
	case 0:		//爆弾セット待ち
		m_sSprite = 0;
		break;
	case 10:	//スイッチを押した
		m_sWaitTimer = 0;
		m_sSprite = 1;
		m_sActSeq = 20;
		break;
	case 20:	//待ち時間
		m_sSprite = 1;
		m_sWaitTimer ++;
		if( m_sWaitTimer >= 180 )
		{
			ChangeMoveType( enMoveTypeLeading );
		}
		break;
	}

	m_pSprite = &AsSpriteSoldierBombSwitch[m_sSprite];


}

void CSoldierBase::MoveTypeLeading()
{
	//部下をリード

	m_sDir = DIR_RIGHT;

	int pattern = GetSpritePattern( m_sWaitTimer,12,ARRAY_LENGTH( SoldierLeadingPatternArray ) );
	m_sSprite = SoldierLeadingPatternArray[ pattern ];
	m_pSprite = &AsSpriteSoldierBombSet[ m_sSprite ];
	m_sWaitTimer ++;

}


void CSoldierBase::MoveTypeEscapeMachine()
{
	//脱出

	int pattern = GetSpritePattern(m_sWaitTimer,8,ARRAY_LENGTH(SoldierClimbPatternArray)+1,gxFalse );

	if( pattern == ARRAY_LENGTH(SoldierClimbPatternArray))
	{
		m_pObj->pos.y -= 1600;
		ChangeMoveType( enMoveTypeMove );
		Main();
		return;
	}

	m_sSprite = SoldierClimbPatternArray[pattern];
	m_pSprite = &AsSpriteSoldierClimb[m_sSprite];

	m_sWaitTimer ++;

	return;
}

void CSoldierBase::Command(Sint32 n)
{
	switch( n ){
	case enSoldierCommandFuttobi:
		//吹っ飛びダウン
		m_sWalkType = enWalkTypeEscape;
		SetTargetPosition(9600*100);
		ChangeMoveType( enMoveTypeFlyDown );
		SetFuttobi(180);
		SetEnd();
		break;
	}
	
}

