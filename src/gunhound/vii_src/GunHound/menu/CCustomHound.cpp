//--------------------------------------------------------
//
// カスタムハウンド工場
//
//--------------------------------------------------------

#include <gunvalken.h>
#include "CCustomHound.h"
#include <gunhound/DashBoard/CDashBoard.h>
#include "../GhStringTbl.h"

enum {
	enWeaponNameUnable = 14,
	enLightClass = 7,
};

enum {
	enMenuSeqInit,
	enMenuSeqMain,
	enMenuSeqMasterMain,
	enMenuSeqMasterSub,
	enMenuSeqSlaveMain,
	enMenuSeqSlaveSub,
};

enum {
	enItemMasterMainShot,
	enItemMasterSubShot,
	enItemSlaveMainShot,
	enItemSlaveSubShot,
	enItemArmorBody,
	enItemArmorArmor,
	enItemDecision,
	enMaxCustomParts,
};


Sint32 MasterMainWeaponTbl[]={
	enWeaponMachineGun,
	enWeapon3Way,
	enWeaponNone,
};


Sint32 MasterSubWeaponTbl[]={
	enWeaponArmHardBrow,
	enWeaponArmGattling,
	enWeaponArmZgokCrow,
	enWeaponArmStrikeCrow,
	enWeaponArmShieldArm,
	enWeaponNone,
};


Sint32 SlaveMainWeaponTbl[]={
	enWeaponBazooka,
	enWeaponShotGun,
	enWeaponNapalm,
	enWeaponGrenade,
	enWeaponNone,
};

Sint32 SlaveSubWeaponTbl[]={
	enWeaponPodMissile,
	enWeaponCannonLinar,
	enWeaponCannonGun,
	enWeaponNone,
};

gxSprite SprCustomHound[]={
	{enTexPageLineHound ,0,0,255,255,0,0},
};

extern gxSprite stSprSubWeapon[];
extern gxSprite stSprMainWeapon[];
extern gxSprite stSprMech[];
extern gxSprite stSprBody[];
extern gxSprite stSprOption[];

CCustomHound::CCustomHound()
{
	Sint32 ii;

	m_bEnd = gxFalse;
	m_sAlpha = 0;
	m_bDestroy = gxFalse;
	m_sScrollW = 128;
	m_pAnnounce = NULL;

	m_sSequence = 0;
	m_sSelect   = 0;
	m_sSelectYoko = 0;
	m_pMech = new CMechBone();

	m_fWindowSlideX = WINDOW_W*1.f;

	ii = 0;
	m_sColoringMax[0] = 0;
	while( ColorBodyName[ii] != NULL )
	{
		m_sColoringMax[0] ++;
		ii ++;
	}

	ii = 0;
	m_sColoringMax[1] = 0;
	while( ColorArmorName[ii] != NULL )
	{
		m_sColoringMax[1] ++;
		ii ++;
	}

	m_stConfig = g_CHoundSaveData.m_StConfig;

	m_sWeaponSelect[0] = GetWeaponIndex( MasterMainWeaponTbl,m_stConfig.sMasterMainShot);
	m_sWeaponSelect[1] = GetWeaponIndex( MasterSubWeaponTbl ,m_stConfig.sMasterSubShot);
	m_sWeaponSelect[2] = GetWeaponIndex( SlaveMainWeaponTbl ,m_stConfig.sSlaveMainShot  );
	m_sWeaponSelect[3] = GetWeaponIndex( SlaveSubWeaponTbl  ,m_stConfig.sSlaveSubShot  );
}


CCustomHound::~CCustomHound()
{
	//セーブデータに保存する

	g_CHoundSaveData.m_StConfig = m_stConfig;

	delete m_pMech;
}

Sint32 CCustomHound::GetWeaponIndex(Sint32 *pTbl , Sint32 sID )
{
	//-----------------------------------------
	//武器番号IDを返す
	//-----------------------------------------
	Sint32 ii = 0;

	while( pTbl[ii] != enWeaponNone )
	{
		if( pTbl[ii] == sID ) return ii;

		ii ++;
	}

	return enWeaponMachineGun;

}


void CCustomHound::Action()
{
	if(m_bEnd)
	{
		m_sAlpha -= 0x08;
//		if( m_sAlpha <= 0 )
		{
			m_sAlpha = 0;
			m_bDestroy = gxTrue;
		}
	}
	else
	{
		m_sScrollW += (0-m_sScrollW)/10;
		m_sAlpha += 0x08;
		if(m_sAlpha >= 255) m_sAlpha = 255;
	}


	if( m_bEnd ) return;

	//----------------------------------------------
	//Seq1
	//----------------------------------------------

	switch( m_sSequence ){
	case enMenuSeqInit:	//初期化
		Init();
		m_sSequence = enMenuSeqMain;
		return;

	case enMenuSeqMain:
		m_fWindowSlideX += (WINDOW_W-m_fWindowSlideX)/3.f;
		MenuMain();
		break;

	case enMenuSeqMasterMain:
	case enMenuSeqMasterSub:
	case enMenuSeqSlaveMain:
	case enMenuSeqSlaveSub:
		m_fWindowSlideX += (0-m_fWindowSlideX)/3.f;
		MenuWeaponSet();
		break;
	}


	if( m_pMech )
	{
		m_pMech->SetWalk(8);
		m_pMech->SetPos( 400*100 , 240*100 );
		m_pMech->Action();
	}

}


void CCustomHound::MenuMain()
{
	//-----------------------------------------
	//メインメニュー
	//-----------------------------------------
	Sint32 max = enMaxCustomParts;

	CDashBoard::GetInstance()->SetIcon( enIconDecision|enIconCancel );

	if( Joy[0].trg&BTN_MENU_DECISION )
	{
		switch(m_sSelect){
		case enItemMasterMainShot:
		case enItemMasterSubShot:
		case enItemSlaveMainShot:
		case enItemSlaveSubShot:
			//とりあえずバックアップ
			m_sTempWeaponSelect[0] = m_sWeaponSelect[0];
			m_sTempWeaponSelect[1] = m_sWeaponSelect[1];
			m_sTempWeaponSelect[2] = m_sWeaponSelect[2];
			m_sTempWeaponSelect[3] = m_sWeaponSelect[3];

			m_sSequence = enMenuSeqMasterMain;
			viiMus::PlaySound( enSoundMenuDec );
			break;

		case enItemDecision:
			viiMus::PlaySound( enSoundMenuDec );
			m_bEnd = gxTrue;
			break;
		}
	}
	else if( Joy[0].trg&enBtnCancel )
	{
		viiMus::PlaySound( enSoundMenuCan );
		m_bEnd = gxTrue;
	}
	else if( Joy[0].trg&BTN_Y )
	{
		m_pMech->SetPunchMotion( gxTrue , 0 );
	}

	//--------------------------------------------
	//項目選択
	//--------------------------------------------
	if( Joy[0].rep&JOY_U )
	{
		viiMus::PlaySound( enSoundMenuSel );
		m_sSelect --;
		m_sSelect = (m_sSelect+max)%max;
	}
	else if( Joy[0].rep&JOY_D )
	{
		viiMus::PlaySound( enSoundMenuSel );
		m_sSelect ++;
		m_sSelect = (m_sSelect+max)%max;
	}

	//--------------------------------------------
	//カラーリング変更
	//--------------------------------------------

	if( Joy[0].rep&JOY_L )
	{
		viiMus::PlaySound( enSoundMenuSel );

		switch( m_sSelect ){
		case enItemArmorBody:
			m_stConfig.sBaseColor -= 1;
			m_stConfig.sBaseColor = ( m_sColoringMax[0]+m_stConfig.sBaseColor)%m_sColoringMax[0];
			//m_pMech->SetPunchMotion( gxTrue , 0 );
			if( m_stConfig.sBaseColor == m_sColoringMax[0]-1 )
			{
				//ノーアーマー設定
				m_stConfig.sNoArmor |= (0x01|0x02);
			}
			else
			{
				//ノーアーマー設定
				m_stConfig.sNoArmor &= ~0x01;
				if( m_stConfig.sArmorColor != m_sColoringMax[1]-1 )
				{
					//鎧がノーアーマーでなければ復活させる
					m_stConfig.sNoArmor &= ~0x02;
				}

			}
			SetBodyColor();
			break;

		case enItemArmorArmor:
			if( m_stConfig.sNoArmor&0x01) break;
			m_stConfig.sArmorColor -= 1;
			m_stConfig.sArmorColor = ( m_sColoringMax[1]+m_stConfig.sArmorColor)%m_sColoringMax[1];
			if( m_stConfig.sNoArmor&0x01) m_stConfig.sArmorColor = m_sColoringMax[1]-1;
			if( m_stConfig.sArmorColor == m_sColoringMax[1]-1 )
			{
				//ノーアーマー設定
				m_stConfig.sNoArmor |= 0x02;
			}
			else
			{
				m_stConfig.sNoArmor &= ~0x02;
			}
			SetBodyColor();
			break;
		}
	}
	else if( Joy[0].rep&JOY_R )
	{
		viiMus::PlaySound( enSoundMenuSel );

		switch( m_sSelect ){
		case enItemArmorBody:
			//m_pMech->SetPunchMotion( gxTrue , 0 );
			m_stConfig.sBaseColor += 1;
			m_stConfig.sBaseColor = ( m_sColoringMax[0]+m_stConfig.sBaseColor)%m_sColoringMax[0];
			if( m_stConfig.sBaseColor == m_sColoringMax[0]-1 )
			{
				//ノーアーマー設定
				m_stConfig.sNoArmor |= (0x01|0x02);
			}
			else
			{
				//ノーアーマー設定
				m_stConfig.sNoArmor &= ~0x01;
				if( m_stConfig.sArmorColor != m_sColoringMax[1]-1 )
				{
					//鎧がノーアーマーでなければ復活させる
					m_stConfig.sNoArmor &= ~0x02;
				}

			}
			SetBodyColor();
			break;

		case enItemArmorArmor:
			if( m_stConfig.sNoArmor&0x01) break;
			m_stConfig.sArmorColor += 1;
			m_stConfig.sArmorColor = ( m_sColoringMax[1]+m_stConfig.sArmorColor)%m_sColoringMax[1];

			if( m_stConfig.sArmorColor == m_sColoringMax[1]-1 )
			{
				//ノーアーマー設定
				m_stConfig.sNoArmor |= 0x02;
			}
			else
			{
				//ノーアーマー設定
				m_stConfig.sNoArmor &= ~0x02;

			}
			SetBodyColor();
			break;
		}
	}


	//-------------------------------------------------
	//終了処理
	//-------------------------------------------------

	m_pAnnounce = FactoryMessage[m_sSelect];

	if( m_pAnnounce )
	{
		CDashBoard::GetInstance()->SetAnnounce(m_pAnnounce);
	}

}


void CCustomHound::MenuWeaponSet()
{
	//-------------------------------------------
	//武器位置の割り当て
	//-------------------------------------------

	Sint32 *pTbl = NULL;
	Sint32 *pGun = NULL;
	Sint32 sIndex = 0;

	switch( m_sSelect ){
	case enItemMasterMainShot:
		sIndex = 0;
		pGun = &m_stConfig.sMasterMainShot;
		pTbl = MasterMainWeaponTbl;
		break;
	case enItemMasterSubShot:
		sIndex = 1;
		pGun = &m_stConfig.sMasterSubShot;
		pTbl = MasterSubWeaponTbl;
		break;
	case enItemSlaveMainShot:
		sIndex = 2;
		pGun = &m_stConfig.sSlaveMainShot;
		pTbl = SlaveMainWeaponTbl;
		break;
	case enItemSlaveSubShot:
		sIndex = 3;
		pGun = &m_stConfig.sSlaveSubShot;
		pTbl = SlaveSubWeaponTbl;
		break;
	}


	//-------------------------------------------
	//武器限界数の割り当て
	//-------------------------------------------

	Sint32 sMax = 0;
	for(Sint32 ii=0;ii<enWeaponMax;ii++)
	{
		if( pTbl[ii] == enWeaponNone ) break;
		sMax ++;
	}

	if( Joy[0].rep&JOY_U )
	{
		m_sWeaponSelect[sIndex] -= 1;
		m_sWeaponSelect[sIndex] = (m_sWeaponSelect[sIndex]+sMax)%sMax;
		*pGun = pTbl[ m_sWeaponSelect[sIndex] ] ;

		if( GetWeaponNameIndex( pTbl[ m_sWeaponSelect[sIndex] ] ) >= 0 )
		{
			SetBodyColor();
			m_pMech->SetPunchMotion( gxTrue , 0 );
		}

		viiMus::PlaySound( enSoundMenuSel );
	}
	else if( Joy[0].rep&JOY_D )
	{
		m_sWeaponSelect[ sIndex ] += 1;
		m_sWeaponSelect[ sIndex ] = (m_sWeaponSelect[sIndex]+sMax)%sMax;
		*pGun = pTbl[ m_sWeaponSelect[sIndex] ] ;

		if( GetWeaponNameIndex( pTbl[ m_sWeaponSelect[sIndex] ] ) >= 0 )
		{
			SetBodyColor();
			m_pMech->SetPunchMotion( gxTrue , 0 );
		}

		viiMus::PlaySound( enSoundMenuSel );
	}
	else if( Joy[0].trg&BTN_MENU_DECISION )
	{
		//決定

		if( GetWeaponNameIndex( pTbl[ m_sWeaponSelect[sIndex] ] ) < 0 )
		{
			//隠しの場合はダメ
			viiMus::PlaySound( enSoundMenuCan );
		}
		else
		{
			m_sSequence = enMenuSeqMain;
			viiMus::PlaySound( enSoundMenuDec );
		}
		return;
	}
	else if( Joy[0].trg&enBtnCancel )
	{
		m_sWeaponSelect[sIndex] = m_sTempWeaponSelect[sIndex];
		*pGun = pTbl[ m_sTempWeaponSelect[sIndex] ] ;
		SetBodyColor();

		m_sSequence = enMenuSeqMain;
		viiMus::PlaySound( enSoundMenuCan );
		return;
	}
	else if( Joy[0].trg&BTN_Y )
	{
		m_pMech->SetPunchMotion( gxTrue , 0 );
	}

}


void CCustomHound::Draw()
{
	Sint32 ux = 32+m_sScrollW;
	Sint32 uy = 80;
	Sint32 h = 16;
	Sint32 sCol[enMaxCustomParts];

	for(Sint32 ii=0;ii<enMaxCustomParts;ii++)
	{
		sCol[ ii ] = 0xffffffff;
		if( ii == m_sSelect )
		{
			sCol[ii] = 0xff00ff00;
		}
	}

	viiSub::MenuPrintf( ux+16*0,uy+h*0,0xFFffFFff,CustomHoundString[0] );//"兵装" );
	viiSub::MenuPrintf( ux+16*0,uy+h*1,sCol[enItemMasterMainShot]  ,CustomHoundString[1] );	//"　メインショット" );
	viiSub::MenuPrintf( ux+16*0,uy+h*2,sCol[enItemMasterSubShot ]  ,CustomHoundString[2] );	//"　サブショット" );
	viiSub::MenuPrintf( ux+16*0,uy+h*3,sCol[enItemSlaveMainShot]   ,CustomHoundString[3] );	//"　対空メイン" );
	viiSub::MenuPrintf( ux+16*0,uy+h*4,sCol[enItemSlaveSubShot ]   ,CustomHoundString[4] );	//"　対空サブ" );

	viiSub::MenuPrintf( ux+16*0,uy+h*5,0xFFffFFff,CustomHoundString[5] );	//"装甲" );
	viiSub::MenuPrintf( ux+16*0,uy+h*6,sCol[enItemArmorBody    ]   ,CustomHoundString[6] );	//"　ベース" );
	viiSub::MenuPrintf( ux+16*0,uy+h*7,sCol[enItemArmorArmor   ]   ,CustomHoundString[7] );	//"　アーマー" );
	viiSub::MenuPrintf( ux+16*0,uy+h*8,sCol[enItemDecision   ]     ,CustomHoundString[8] );	//"決定" );

	viiSub::MenuSprite( &SprCustomHound[0] , 0,0,32+2 ,ATR_DFLT, ARGB(m_sAlpha/2,0x40,0x80,0x40) ,WINDOW_W/255.f);

	//--------------------------------------------------

	DrawWeaponSet();

	switch( m_sSequence ){
	case enMenuSeqMasterMain:
	case enMenuSeqMasterSub:
	case enMenuSeqSlaveMain:
	case enMenuSeqSlaveSub:
		break;
	default:
		DrawMain();
		break;
	}

	if( m_pMech )
	{
		m_pMech->Draw();
	}

	viiSub::MenuSprite( &SprCustomHound[0] , 0,0,32+2 ,ATR_DFLT, ARGB(m_sAlpha/2,0x40,0x80,0x40) ,WINDOW_W/255.f);

}




void CCustomHound::DrawMain()
{
	//-------------------------------------
	//メインウインドウ
	//-------------------------------------
	Sint32 ux = 32+m_sScrollW;
	Sint32 uy = 80+16;
	Sint32 h = 16;
	Sint32 sCol[enMaxCustomParts];

	Sint32 sPosTbl[]={
		0+16*0-16,
		0+16*1-16,
		0+16*2-16,
		0+16*3-16,
		0+16*5-16,
		0+16*6-16,
		0+16*7-16,
	};

	for(Sint32 ii=0;ii<enMaxCustomParts;ii++)
	{
		Sint32 x,y;
		sCol[ ii ] = 0xffffffff;
		if( ii == m_sSelect )
		{
			sCol[ii] = 0xff00ff00;
			x = ux;
			y = uy+sPosTbl[ii]+16;
			viiDraw::Box( x-2, y-2, x+WINDOW_W , y+14 , 128-1   , gxTrue , 0x80F0A0F0  , ATR_DFLT );
		}
	}

	viiSub::MenuPrintf( ux+16*10,16+uy+sPosTbl[0],sCol[enItemMasterMainShot]  ,"%s" , GetWeaponName( m_stConfig.sMasterMainShot ) );
	viiSub::MenuPrintf( ux+16*10,16+uy+sPosTbl[1],sCol[enItemMasterSubShot ]  ,"%s" , GetWeaponName( m_stConfig.sMasterSubShot  ) );
	viiSub::MenuPrintf( ux+16*10,16+uy+sPosTbl[2],sCol[enItemSlaveMainShot]   ,"%s" , GetWeaponName( m_stConfig.sSlaveMainShot  ) );
	viiSub::MenuPrintf( ux+16*10,16+uy+sPosTbl[3],sCol[enItemSlaveSubShot ]   ,"%s" , GetWeaponName( m_stConfig.sSlaveSubShot   ) );


	if( m_stConfig.sNoArmor&0x01 )
	{
		//ライト級設定
		viiSub::MenuPrintf( ux+16*10,16+uy+sPosTbl[4],sCol[enItemArmorBody    ]   ,"%s" ,ColorBodyName [ enLightClass ]  );
		viiSub::MenuPrintf( ux+16*10,16+uy+sPosTbl[5],0xf0010101   ,"%s" ,ColorArmorName[ enLightClass ] );
	}
	else
	{

		viiSub::MenuPrintf( ux+16*10,16+uy+sPosTbl[4],sCol[enItemArmorBody    ]   ,"%s" ,ColorBodyName [ m_stConfig.sBaseColor ]  );
		viiSub::MenuPrintf( ux+16*10,16+uy+sPosTbl[5],sCol[enItemArmorArmor   ]   ,"%s" ,ColorArmorName[ m_stConfig.sArmorColor ] );
	}
}


void CCustomHound::DrawWeaponSet()
{
	//----------------------------------------
	//武器選択
	//----------------------------------------

	Sint32 *pTbl = NULL;
	Sint32 *pGun = NULL;
	Sint32 sIndex = 0;

	Sint32 ux = s_cast<Sint32>(192+m_fWindowSlideX);
	Sint32 uy = 92;
	Sint32 h  = 16;

	switch( m_sSelect ){
	case enItemMasterMainShot:
		sIndex = 0;
		pGun = &m_stConfig.sMasterMainShot;
		pTbl = MasterMainWeaponTbl;
		break;
	case enItemMasterSubShot:
		sIndex = 1;
		pGun = &m_stConfig.sMasterSubShot;
		pTbl = MasterSubWeaponTbl;
		break;
	case enItemSlaveMainShot:
		sIndex = 2;
		pGun = &m_stConfig.sSlaveMainShot;
		pTbl = SlaveMainWeaponTbl;
		break;
	case enItemSlaveSubShot:
		sIndex = 3;
		pGun = &m_stConfig.sSlaveSubShot;
		pTbl = SlaveSubWeaponTbl;
		break;
	default:
		return;
	}

	viiDraw::Box( ux, uy, ux+WINDOW_W , uy+128 , PRIO_GAMETOP-3 , gxTrue  , 0x80101010  , ATR_ALPHA_MINUS );
	viiDraw::Box( ux, uy, ux+WINDOW_W , uy+128 , PRIO_GAMETOP-2   , gxFalse , 0x8010A0F0  , ATR_DFLT );

	Sint32 ii=0,sID;

	while( gxTrue )
	{
		Sint32 sCol = 0xffffffff;
		Sint32 x,y;

		x = ux+16;
		y = ii*16+uy+16;

		sID = pTbl[ii];

		if( sID == enWeaponNone ) break;

		if( m_sWeaponSelect[sIndex] == ii )
		{
			sCol = 0xff00ff00;
			viiDraw::Box( x-2, y-2, x+WINDOW_W , y+14 , PRIO_GAMETOP-1   , gxTrue , 0x80F0A0F0  , ATR_DFLT );
		}

		viiSub::MenuPrintf( x , y , sCol   ,"%s" , GetWeaponName( sID )  );

		ii++;
	}



}

char *CCustomHound::GetWeaponName(Sint32 sID )
{
	//----------------------------------------
	//武器名を取得する（隠しの場合は？？？？にする）
	//----------------------------------------

	char *pName = NULL;

	sID = GetWeaponNameIndex( sID );

	if( sID < 0 )
	{
		sID = enWeaponNameUnable;	//？？？？？？？？？？
	}
	return WeaponNameTbl[ sID ];

}

Sint32 CCustomHound::GetWeaponNameIndex( Sint32 sID )
{
	//---------------------------------------------------
	//武器名のインデックスを返す（隠しの場合は-1を返す）
	//---------------------------------------------------

	switch( sID ){
	case enWeaponMachineGun:
	case enWeaponArmHardBrow:
	case enWeaponBazooka:
	case enWeaponPodMissile:
		break;

	case enWeapon3Way:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeapon3Way ) )
		{
			return -1;
		}
		break;

	case enWeaponArmGattling:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponGattling ) )
		{
			return -1;
		}
		break;

	case enWeaponArmZgokCrow:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponZgokCrow ) )
		{
			return -1;
		}
		break;

	case enWeaponArmStrikeCrow:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponIronCrow ) )
		{
			return -1;
		}
		break;

	case enWeaponArmShieldArm:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponShieldArm ) )
		{
			return -1;
		}
		break;

	case enWeaponShotGun:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponShotGun ) )
		{
			return -1;
		}
		break;

	case enWeaponNapalm:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponNapalmGun ) )
		{
			return -1;
		}
		break;

	case enWeaponCannonLinar:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponLinearCannon ) )
		{
			return -1;
		}
		break;

	case enWeaponCannonGun:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponAirGun ) )
		{
			return -1;
		}
		break;
	case enWeaponGrenade:
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponGrenade ) )
		{
			return -1;
		}
		break;
	}

	return sID;
}


void CCustomHound::SetDestroy()
{
	
	
	
	
}


gxBool CCustomHound::IsDead()
{
	//フェードも含めて終了しているか？

	if(m_bDestroy) return gxTrue;

	return gxFalse;
}


void CCustomHound::Clear()
{
	
	
	
}

void CCustomHound::Init()
{
	m_pMech->SetPos( 400*100 , 240*100 );
	m_pMech->SetDefault();

	SetBodyColor();
}

void CCustomHound::SetBodyColor( CMechBone *pMech ,Sint32 col1 , Sint32 col2)
{
	//----------------------------------------------
	//ボディカラー変更
	//----------------------------------------------

	if( pMech == NULL )
	{
		pMech = m_pMech;
	}

//	if( m_sSequence != 0 )
	{
		StSprConfig m_sMech,m_Body,m_Armor;

		Sint32 m = m_stConfig.sBaseColor;
		Sint32 n = m_stConfig.sArmorColor;

		if( col1!=-1) m = col1;
		if( col2!=-1) n = col2;

		//メカ
		m_sMech.sIndex[enPartsArmL]    = m;
		m_sMech.sIndex[ enPartsSubWeapon1  ] = 0;

		//ボディ設定
		m_Body.sIndex[enPartsBase]     = -1;
		m_Body.sIndex[enPartsBody]     =  5*m+1;
		m_Body.sIndex[enPartsArmL]     = -1;
		m_Body.sIndex[enPartsArmR]     =  5*m+0;
		m_Body.sIndex[enPartsLegL]     =  5*m+3;
		m_Body.sIndex[enPartsLegR]     =  5*m+2;
		m_Body.sIndex[enPartsFootL]    = -1;
		m_Body.sIndex[enPartsFootR]    = -1;
		m_Body.sIndex[enPartsShoulder] =  5*m+4;

		//アーマー設定
		m_Armor.sIndex[ enPartsBase ]     = -1;
		m_Armor.sIndex[ enPartsBody ]     =  n;
		m_Armor.sIndex[ enPartsArmL ]     = -1;
		m_Armor.sIndex[ enPartsArmR ]     =  n;
		m_Armor.sIndex[ enPartsLegL ]     =  n;
		m_Armor.sIndex[ enPartsLegR ]     =  n;
		m_Armor.sIndex[ enPartsFootL ]    = -1;
		m_Armor.sIndex[ enPartsFootR ]    = -1;
		m_Armor.sIndex[ enPartsShoulder ] =  n;

		//オプションパーツ
		m_Armor.sIndex[ enPartsBurnia      ] = -1;
		m_Armor.sIndex[ enPartsNozzle      ] = -1;
		m_Armor.sIndex[ enPartsBurniaOption] = -1;
		m_Armor.sIndex[ enPartsSubWeapon1  ] = -1;
		m_Armor.sIndex[ enPartsSubWeapon2  ] = -1;
		m_Armor.sIndex[ enPartsSubWeapon3  ] = -1;

		//アームの特殊処理
		switch(m_stConfig.sMasterSubShot){
		default:
		case enWeaponArmHardBrow:
			m_sMech.sIndex[enPartsArmL]    = 1;
			break;
		case enWeaponArmGattling:
			m_sMech.sIndex[enPartsArmL]    = 2;
			break;
		case enWeaponArmZgokCrow:
			m_sMech.sIndex[enPartsArmL]    = 5;
			break;
		case enWeaponArmStrikeCrow:
			m_sMech.sIndex[enPartsArmL]    = 4;
			break;
		case enWeaponArmShieldArm:
			m_sMech.sIndex[enPartsArmL]    = 6;
			break;
		}

		//メインショットの特殊処理
		switch(m_stConfig.sMasterMainShot){
		default:
		case enWeaponMachineGun:
			m_sMech.sIndex[enPartsWeapon]    = 0;
			break;
		case enWeapon3Way:
			m_sMech.sIndex[enPartsWeapon]    = 7;
			break;
/*
		case enWeaponBazooka:
			m_sMech.sIndex[enPartsWeapon]    = 4;
			break;
		case enWeaponShotGun:
			m_sMech.sIndex[enPartsWeapon]    = 6;
			break;
		case enWeaponNapalm:
			m_sMech.sIndex[enPartsWeapon]    = 5;
			break;
*/
		}

		//サブショットの特殊処理
		switch(m_stConfig.sSlaveMainShot){
		default:
		case enWeaponBazooka:
			m_Armor.sIndex[enPartsWeapon]    = 4;
			break;
		case enWeaponShotGun:
			m_Armor.sIndex[enPartsWeapon]    = 6;
			break;
		case enWeaponNapalm:
			m_Armor.sIndex[enPartsWeapon]    = 5;
			break;
		case enWeaponGrenade:
			m_Armor.sIndex[enPartsWeapon]    = 8;	//表示なし
			break;
		}

		//ショルダーショットの特殊処理
		switch(m_stConfig.sSlaveSubShot){
		default:
		case enWeaponPodMissile:
			m_sMech.sIndex[ enPartsSubWeapon1  ] =  0;
			break;
		case enWeaponCannonLinar:
			m_sMech.sIndex[ enPartsSubWeapon1  ] =  2;
			break;
		case enWeaponCannonGun:
			m_sMech.sIndex[ enPartsSubWeapon1  ] =  3;
			break;
		}

		pMech->SetSprite( &m_sMech , &m_Body , &m_Armor );
	}

		if( m_stConfig.sNoArmor )
		{
			if( m_stConfig.sNoArmor&0x01 )
			{
				pMech->SetPartsNone( enPartsBody , 1 );
				pMech->SetPartsNone( enPartsArmR , 1 );
				pMech->SetPartsNone( enPartsLegL , 1 );
				pMech->SetPartsNone( enPartsLegR , 1 );
				pMech->SetPartsNone( enPartsFootL , 1 );
				pMech->SetPartsNone( enPartsFootR , 1 );
				pMech->SetPartsNone( enPartsShoulder , 1 );

				pMech->SetPartsNone( enPartsBody , 2 );
				pMech->SetPartsNone( enPartsArmR , 2 );
				pMech->SetPartsNone( enPartsLegL , 2 );
				pMech->SetPartsNone( enPartsLegR , 2 );
				pMech->SetPartsNone( enPartsFootL , 2 );
				pMech->SetPartsNone( enPartsFootR , 2 );
				pMech->SetPartsNone( enPartsShoulder , 2 );

			}
			else if( m_stConfig.sNoArmor&0x02 )
			{
				pMech->SetPartsNone( enPartsBody , 2 );
				pMech->SetPartsNone( enPartsArmR , 2 );
				pMech->SetPartsNone( enPartsLegL , 2 );
				pMech->SetPartsNone( enPartsLegR , 2 );
				pMech->SetPartsNone( enPartsFootL , 2 );
				pMech->SetPartsNone( enPartsFootR , 2 );
				pMech->SetPartsNone( enPartsShoulder , 2 );
			}
		}

}


