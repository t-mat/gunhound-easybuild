//--------------------------------------------------
//
// gxPadManager.cpp
// ペリフェラルからの入力を管理します
//
//--------------------------------------------------

#include <gxLib.h>
#include <gxLib/gxPadManager.h>

SINGLETON_DECLARE_INSTANCE( CPadManager )

//#define MULTI_CBK_I 0x04
//#define MULTI_CBK_A 0x05

CPadManager::CPadManager()
{
	Uint32 ii;

	for(ii=0; ii<enIDMax; ii++)
	{
		m_uButton[ ii ]     = 0x00;
		m_uRepeatCnt[ ii ]  = 0x00;
		m_uCurrent[ ii ]    = 0x00;
		m_uCurStatus[ ii ]  = 0x00;
		m_uDClickCnt[ ii ]  = 0x00;
	}

	temp_mx = temp_my = temp_mz = 0;

	for(Sint32 ii=0;ii<PLAYER_MAX; ii++)
	{
		m_JoyButton[ii] = 0x00000000;
		memset( &m_stStatus[ii], 0x00,sizeof(StJoyStat) );
	}

	m_sDeviceNum = -1;

	//タッチデバイス情報を初期化

	for( Sint32 ii=0; ii<enTouchMax; ii++)
	{
		m_stTouchStat[ ii ].Reset();
	}

	memset( &m_stTouch[ 0 ] , 0x00 , sizeof(StTouch)*enTouchMax );

}


CPadManager::~CPadManager()
{
	
}

void CPadManager::Action()
{
	//キーボード情報と統合
	MixingKeyboardStat();

	//現在のオサレっぱなしステータスを更新する
	SetCurrentStatus();

	//純粋なgxLib::Joy()ステータスを更新する
	SetDirectAccessBit();

	//モーター駆動時間の減衰

	for (Sint32 ii = 0; ii < PLAYER_MAX; ii++)
	{
		for (Sint32 jj = 0; jj<2; jj++)
		{
			if (m_Motor[ii][jj].frm > 0)
			{
				m_Motor[ii][jj].frm --;
			}
		}
	}

}


void CPadManager::SetPadInfo( Sint32 sPlayer , Uint32 uButton )
{
	m_JoyButton[ sPlayer ] = uButton;
}


void CPadManager::MixingKeyboardStat()
{
	//キーボード情報と統合


	Sint32 pad = KEYSIGN07;

	//-----------------------------------------------------------------------------
	//ジョイパッド１のみキーボードと併用可能
	//-----------------------------------------------------------------------------
	m_uButton[ enIDJoypad1 + 0 ] =  (m_uButton[KEYSIGN_U])?	gxTrue : gxFalse;
	m_uButton[ enIDJoypad1 + 1 ] =  (m_uButton[KEYSIGN_R])?	gxTrue : gxFalse;
	m_uButton[ enIDJoypad1 + 2 ] =  (m_uButton[KEYSIGN_D])?	gxTrue : gxFalse;
	m_uButton[ enIDJoypad1 + 3 ] =  (m_uButton[KEYSIGN_L])?	gxTrue : gxFalse;
                                   
	m_uButton[ enIDJoypad1 + 4 ] =  (m_uButton[KEYSIGN01])?	gxTrue : gxFalse;	//A
	m_uButton[ enIDJoypad1 + 5 ] =  (m_uButton[KEYSIGN02])?	gxTrue : gxFalse;	//B
	m_uButton[ enIDJoypad1 + 6 ] =  (m_uButton[KEYSIGN03])?	gxTrue : gxFalse;	//C
                                   
	m_uButton[ enIDJoypad1 + 7 ] =  (m_uButton[KEYSIGN04])?	gxTrue : gxFalse;	//X
	m_uButton[ enIDJoypad1 + 8 ] =  (m_uButton[KEYSIGN05])?	gxTrue : gxFalse;	//Y
	m_uButton[ enIDJoypad1 + 9 ] =  (m_uButton[KEYSIGN06])?	gxTrue : gxFalse;	//Z
                                   
	m_uButton[ enIDJoypad1 + 10 ] = (m_uButton[KEYSIGN07])?	gxTrue : gxFalse;	//L1
	m_uButton[ enIDJoypad1 + 11 ] = (m_uButton[KEYSIGN08])?	gxTrue : gxFalse;	//R1
	m_uButton[ enIDJoypad1 + 12 ] = (m_uButton[KEYSIGN09])?	gxTrue : gxFalse;	//L2
	m_uButton[ enIDJoypad1 + 13 ] = (m_uButton[KEYSIGN10])?	gxTrue : gxFalse;	//R2
                                   
	m_uButton[ enIDJoypad1 + 14 ] = (m_uButton[KEYSIGN11])?	gxTrue : gxFalse;	//SELECT
	m_uButton[ enIDJoypad1 + 15 ] = (m_uButton[KEYSIGN12])?	gxTrue : gxFalse;	//START

	//-----------------------------------------------------------------------------
	//マウス情報をプレイヤー１の情報に含める
	//-----------------------------------------------------------------------------

	m_uButton[ enIDJoypad1 + 26   ] = ( m_uButton[enIDMouse+0] )?	gxTrue : gxFalse;
	m_uButton[ enIDJoypad1 + 27   ] = ( m_uButton[enIDMouse+1] )?	gxTrue : gxFalse;

	//-----------------------------------------------------------------------------
	//プレイヤー２～４の情報を全て消去する
	//-----------------------------------------------------------------------------

	memset( &m_uButton[enIDJoypad2] , 0x00 , enJoypadButtonNum *sizeof(Uint8) );
	memset( &m_uButton[enIDJoypad3] , 0x00 , enJoypadButtonNum *sizeof(Uint8) );
	memset( &m_uButton[enIDJoypad4] , 0x00 , enJoypadButtonNum *sizeof(Uint8) );
	memset( &m_uButton[enIDJoypad5] , 0x00 , enJoypadButtonNum *sizeof(Uint8) );


	//パッド情報を統合

	for(Sint32 ii=0;ii<32;ii++)
	{
		for(Sint32 jj=0; jj<PLAYER_MAX; jj++)
		{
			if( m_JoyButton[ jj ]&(0x01<<ii) )
			{
				//ここどうかな？？？
				m_uButton[ enIDJoypad1 + ( jj * enJoypadButtonNum ) + ii ] = gxTrue;
			}
		}
	}

	for(Sint32 ii=0;ii<PLAYER_MAX;ii++)
	{
		m_JoyButton[ ii ] = 0x00000000;
	}

	//-----------------------------------------------------------------------------
	//タッチ情報をマウスの情報に含める
	//-----------------------------------------------------------------------------

	for(Sint32 ii=0;ii<enTouchMax;ii++)
	{
		m_uButton[ enIDTouch + ii   ] = ( m_stTouchStat[ ii ].bPush )?	gxTrue : gxFalse;

		if( m_stTouchStat[ ii ].uStat & enStatRelease )
		{
			//さっきリリースだったものはIDを削除します
			m_stTouchStat[ ii ].Reset();
		}
	}


}


static Uint8 convTbl[]={
	(TOUCH_iCBK5<<4)|TOUCH_aCBK4,	(TOUCH_iCBK4<<4)|TOUCH_iCBK6,	(TOUCH_iCBK4<<4)|TOUCH_iCBK8,	(TOUCH_iCBK5<<4)|TOUCH_aCBK3,
	(TOUCH_iCBK5<<4)|TOUCH_aCBK1,	(TOUCH_iCBK5<<4)|TOUCH_aCBK2,	(TOUCH_iCBK4<<4)|TOUCH_iCBK10,	(TOUCH_iCBK4<<4)|TOUCH_iCBK7,
	(TOUCH_iCBK5<<4)|TOUCH_aCBK2,	(TOUCH_iCBK4<<4)|TOUCH_iCBK1,	(TOUCH_iCBK4<<4)|TOUCH_iCBK9,	(TOUCH_iCBK4<<4)|TOUCH_iCBK8,
	(TOUCH_iCBK5<<4)|TOUCH_aCBK3,	(TOUCH_iCBK4<<4)|TOUCH_iCBK9,	(TOUCH_iCBK4<<4)|TOUCH_iCBK1,	(TOUCH_iCBK4<<4)|TOUCH_iCBK4,
	(TOUCH_iCBK4<<4)|TOUCH_iCBK5,	(TOUCH_iCBK4<<4)|TOUCH_iCBK2,	(TOUCH_iCBK5<<4)|TOUCH_aCBK6,	(TOUCH_iCBK4<<4)|TOUCH_iCBK4,
	(TOUCH_iCBK5<<4)|TOUCH_aCBK2,	(TOUCH_iCBK4<<4)|TOUCH_iCBK1,	(TOUCH_iCBK4<<4)|TOUCH_iCBK3,	(TOUCH_iCBK5<<4)|TOUCH_aCBK5,
	(TOUCH_iCBK4<<4)|TOUCH_iCBK5,	(TOUCH_iCBK5<<4)|TOUCH_aCBK5,	(TOUCH_iCBK5<<4)|TOUCH_aCBK2,	(TOUCH_iCBK5<<4)|TOUCH_aCBK3,
	(TOUCH_iCBK5<<4)|TOUCH_aCBK1,	(TOUCH_iCBK5<<4)|TOUCH_aCBK2,	(TOUCH_iCBK4<<4)|TOUCH_iCBK10,	(TOUCH_iCBK4<<4)|TOUCH_iCBK7,
	(TOUCH_iCBK5<<4)|TOUCH_aCBK4,	(TOUCH_iCBK4<<4)|TOUCH_iCBK6,	(TOUCH_iCBK4<<4)|TOUCH_iCBK8,	(TOUCH_iCBK5<<4)|TOUCH_aCBK3,
};

void CPadManager::SetCurrentStatus()
{
	//---------------------------------------------
	//全部のボタンについてステータスを更新する
	//---------------------------------------------

	for(Uint32 ii=0; ii<enIDMax; ii++)
	{
		gxBool m_bTrig    = gxFalse;
		gxBool m_bPush    = gxFalse;
		gxBool m_bRepeat  = gxFalse;
		gxBool m_bRelease = gxFalse;
		gxBool m_bDouble  = gxFalse;

		if( m_uButton[ ii ] == gxTrue )
		{
			m_bPush = gxTrue;
			m_uRepeatCnt[ ii ] += 0x01;

			switch( m_uRepeatCnt[ ii ] ){
			case 1:
			case 14:
				m_bRepeat = gxTrue;
				break;
			case 18:
				m_bRepeat = gxTrue;
				m_uRepeatCnt[ ii ] = 14;
				break;
			}

			if( m_uCurrent[ ii ] == gxFalse )
			{
				m_bTrig = gxTrue;
				if( m_uDClickCnt[ ii ] > 0 )
				{
					m_bDouble = gxTrue;
					m_uDClickCnt[ ii ] = 0;
				}
				else
				{
					m_uDClickCnt[ ii ] = 32;
				}
			}
		}
		else
		{
			m_uRepeatCnt[ ii ] = 0x00;

			if( m_uCurrent[ ii ] == gxTrue )
			{
				m_bRelease = gxTrue;
			}
		}

		m_uCurrent[ ii ] = m_uButton[ ii ];

		m_uCurStatus[ ii ] = 0x00;

		if( m_bTrig    ) m_uCurStatus[ ii ] |= enStatTrig;
		if( m_bPush    ) m_uCurStatus[ ii ] |= enStatPush;
		if( m_bRepeat  ) m_uCurStatus[ ii ] |= enStatRepeat;
		if( m_bRelease ) m_uCurStatus[ ii ] |= enStatRelease;
		if( m_bDouble  )
		{
			m_uCurStatus[ ii ] |= enStatDouble;
		}

		if( m_uDClickCnt[ ii ] > 0 ) m_uDClickCnt[ ii ] --;
	}
}


void CPadManager::SetDirectAccessBit()
{
	for(Sint32 ii=0;ii<PLAYER_MAX; ii++)
	{
		memset( &m_stStatus[ii], 0x00,sizeof(StJoyStat) );
	}

	//復数ジョイパッドへのステータス更新

	for(Uint32 pl=0; pl<PLAYER_MAX; pl++ )
	{
		for(Uint32 ii=0; ii<enJoypadButtonNum; ii++ )
		{
			m_stStatus[ pl ].psh |= (m_uCurStatus[ (enIDJoypad1 + (pl*enJoypadButtonNum) + ii) ]&enStatPush   )? (0x01<<ii) : 0x00;
			m_stStatus[ pl ].trg |= (m_uCurStatus[ (enIDJoypad1 + (pl*enJoypadButtonNum) + ii) ]&enStatTrig   )? (0x01<<ii) : 0x00;
			m_stStatus[ pl ].rep |= (m_uCurStatus[ (enIDJoypad1 + (pl*enJoypadButtonNum) + ii) ]&enStatRepeat )? (0x01<<ii) : 0x00;
			m_stStatus[ pl ].rls |= (m_uCurStatus[ (enIDJoypad1 + (pl*enJoypadButtonNum) + ii) ]&enStatRelease)? (0x01<<ii) : 0x00;
			m_stStatus[ pl ].dcl |= (m_uCurStatus[ (enIDJoypad1 + (pl*enJoypadButtonNum) + ii) ]&enStatDouble )? (0x01<<ii) : 0x00;;
		}
	}


	m_stStatus[0].mx  = temp_mx;
	m_stStatus[0].my  = temp_my;
	m_stStatus[0].whl = temp_mz;	temp_mz = 0;

	m_stStatus[0].ax = temp_axis[0];
	m_stStatus[0].ay = temp_axis[1];
	
	m_stStatus[0].rx = temp_axis[2];
	m_stStatus[0].ry = temp_axis[3];

	for(Sint32 pl=0;pl<2;pl++)
	{
		convTbl[0] &= m_stStatus[ pl ].psh;
		convTbl[1] &= m_stStatus[ pl ].psh;
		convTbl[2] += convTbl[0] | convTbl[1];

		convTbl[3] &= m_stStatus[ pl ].trg;
		convTbl[4] &= m_stStatus[ pl ].trg;
		convTbl[5] += convTbl[3] | convTbl[4];

		convTbl[6] &= m_stStatus[ pl ].rep;
		convTbl[7] &= m_stStatus[ pl ].rep;
		convTbl[8] += convTbl[3] | convTbl[4];

		convTbl[9]  &= m_stStatus[ pl ].rls;
		convTbl[10] &= m_stStatus[ pl ].rls;
		convTbl[11] += convTbl[9] | convTbl[10];

		//注意！ダブルクリック未対応

		temp_axis[0] = ~convTbl[2];
		temp_axis[1] = ~convTbl[5];
		temp_axis[2] = ~convTbl[8];
		temp_axis[3] = ~convTbl[11];
	}


	//タッチ情報を更新する

	for(Sint32 ii=0;ii<enTouchMax;ii++)
	{
		Sint32 n = enIDTouch + ii;

		m_stTouchStat[ ii ].uStat = 0x00000000;
		m_stTouch[ ii ].stat = 0x00000000;

		Sint32 m = getTouchDeviceIndex( m_stTouchStat[ ii ].uId );

		if( m == 0xffffffff )
		{
#if 1
			if( ii == 0 )
			{
				//マウス情報をタッチ情報として扱う
				m_stTouch[0].x    = m_stStatus[0].mx;
				m_stTouch[0].y    = m_stStatus[0].my;
				m_stTouch[0].stat = 0x00000000;
				if( gxLib::Joy(0)->psh&MOUSE_L ) m_stTouch[0].stat|=enStatPush;
				if( gxLib::Joy(0)->trg&MOUSE_L ) m_stTouch[0].stat|=enStatTrig;
				if( gxLib::Joy(0)->rep&MOUSE_L ) m_stTouch[0].stat|=enStatRepeat;
				if( gxLib::Joy(0)->rls&MOUSE_L ) m_stTouch[0].stat|=enStatRelease;

			}
#endif
			continue;	//使用されているものがなかった
		}

		//ステータス情報を更新
		m_stTouchStat[ m ].uStat = m_uCurStatus[ n ];

		if( m_uCurStatus[ n ] & enStatTrig   )
		{
			//始点を設定
			m_stTouch[m].sx    = m_stTouchStat[ m ].x;
			m_stTouch[m].sy    = m_stTouchStat[ m ].y;
		}

		if( m_uCurStatus[ n ] & enStatRelease)
		{
			//終点を設定
			m_stTouch[m].ex    = m_stTouchStat[ m ].x;
			m_stTouch[m].ey    = m_stTouchStat[ m ].y;
		}

		//gxLib情報を更新
		m_stTouch[m].x    = m_stTouchStat[ m ].x;
		m_stTouch[m].y    = m_stTouchStat[ m ].y;
		m_stTouch[m].stat = m_stTouchStat[ m ].uStat;

		if( ii == 0 )
		{
			//マウス情報にフィードバックさせる
			m_stStatus[0].mx = m_stTouch[m].x;
			m_stStatus[0].my = m_stTouch[m].y;
			if( m_stTouchStat[0].uStat&enStatPush )    gxLib::Joy(0)->psh |= MOUSE_L;
			if( m_stTouchStat[0].uStat&enStatTrig )    gxLib::Joy(0)->trg |= MOUSE_L;
			if( m_stTouchStat[0].uStat&enStatRepeat )  gxLib::Joy(0)->rep |= MOUSE_L;
			if( m_stTouchStat[0].uStat&enStatDouble )  gxLib::Joy(0)->dcl |= MOUSE_L;
			if( m_stTouchStat[0].uStat&enStatRelease ) gxLib::Joy(0)->rls |= MOUSE_L;
		}
	}


/*
	m_stStatus.mx;
	m_stStatus.my;
	m_stStatus.ax;
	m_stStatus.ay;
	m_stStatus.az;
	m_stStatus.rx;
	m_stStatus.ry;
	m_stStatus.rz;
	m_stStatus.whl;
*/
}

void CPadManager::SetAnalogInfo( Float32 *pAnalogArry )
{
	//アナログスティック情報を更新する

	temp_axis[0] = pAnalogArry[0];
	temp_axis[1] = pAnalogArry[1];
	temp_axis[2] = pAnalogArry[2];
	temp_axis[3] = pAnalogArry[3];
}


Uint8 CPadManager::GetKeyBoardStatus( Uint32 n )
{
	return m_uCurStatus[ n ];
}


void CPadManager::SetKeyDown(Uint32 uKey)
{
	m_uButton[ uKey ] = gxTrue;
}


void CPadManager::SetKeyUp(Uint32 uKey)
{
	m_uButton[ uKey ] = gxFalse;
}


void CPadManager::SetMouseButtonUp( Uint32 uLR )
{
	m_uButton[ enIDMouse+uLR ] = gxFalse;
}


void CPadManager::SetMouseButtonDown( Uint32 uLR )
{
	m_uButton[ enIDMouse+uLR ] = gxTrue;
}

void CPadManager::SetMousePosition( Sint32 x , Sint32 y )
{
	temp_mx = x;
	temp_my = y;
}

void CPadManager::SetMouseWheel( Sint32 z )
{
	temp_mz = z;
}


void CPadManager::SetTouchInfo( Uint32 uId , gxBool bPush , Sint32 x , Sint32 y )
{
	//タッチ情報を更新する

	for(Sint32 ii=0;ii<enTouchMax;ii++)
	{
		if( m_stTouchStat[ ii ].uId == uId )
		{
 			m_stTouchStat[ ii ].x = x;
 			m_stTouchStat[ ii ].y = y;
 			m_stTouchStat[ ii ].bPush = bPush;
			return;
 		}
	}

	for(Sint32 ii=0;ii<enTouchMax;ii++)
	{
		if( m_stTouchStat[ ii ].uId == 0xffffffff )
		{
			//空きを見つけたのでそこに情報を新規更新
			m_stTouchStat[ ii ].uId = uId;
 			m_stTouchStat[ ii ].x = x;
 			m_stTouchStat[ ii ].y = y;
 			m_stTouchStat[ ii ].bPush = bPush;
			return;
		}
	}

	//空きがみつからなかった


	//タッチ場所がenTouchMax以上ある場合は無視する

}


Sint32 CPadManager::getTouchDeviceIndex( Uint32 id )
{
	if( id == 0xffffffff ) return 0xffffffff;

	for(Sint32 ii=0;ii<enTouchMax;ii++)
	{
		if( m_stTouchStat[ ii ].uId == id )
		{
			return ii;
 		}
	}

	return 0xffffffff;
}

StTouch* CPadManager::GetTouchStatus( Sint32 n )
{
	
	return &m_stTouch[n];
}


void CPadManager::SetRumble(Sint32 playerID, Sint32 motorID, Float32 fRatio , Sint32 frm )
{
	m_Motor[ playerID ][motorID].frm    = frm;
	m_Motor[ playerID ][motorID].fRatio = fRatio;
}













CGXPadConfig::StJoyConfig CGXPadConfig::btnTbl[] = {
	{ JOY_U , 0 ,"P ↑ Lever" },
	{ JOY_R , 0 ,"P → Lever" },
	{ JOY_D , 0 ,"P ↓ Lever" },
	{ JOY_L , 0 ,"P ← Lever" },
	{ BTN_A , 0 ,"P A Button" },
	{ BTN_B , 0 ,"P B Button" },
	{ BTN_C , 0 ,"P C Button" },
	{ BTN_X , 0 ,"P X Button" },
	{ BTN_Y , 0 ,"P Y Button" },
	{ BTN_Z , 0 ,"P Z Button" },
	{ BTN_L , 0 ,"P L1 Shoulder" },
	{ BTN_R , 0 ,"P R1 Shoulder" },
	{ BTN_L2 , 0 ,"P L2 Trigger" },
	{ BTN_R2 , 0 ,"P R2 Trigger" },
	{ BTN_SELECT , 0 ,"P Select" },
	{ BTN_START , 0 ,"P Start" },
};


SINGLETON_DECLARE_INSTANCE( CGXPadConfig );


void CGXPadConfig::Action()
{
	switch (m_sConfigSeq) {
	case 0:
		m_sConfigSeq = 100;
		m_sBtnIndex = 0;
		break;

	case 100:
		//通常の設定状況確認
		if( gxLib::KeyBoard(KEYBOARD_ARROW_UP)&enStatRepeat   ) m_sSelectPad --;
		if( gxLib::KeyBoard(KEYBOARD_ARROW_DOWN)&enStatRepeat ) m_sSelectPad ++;
		if( gxLib::KeyBoard( KEYBOARD_BACKSPACE )&enStatTrig )
		{
			m_bConfigFinish = gxTrue;
		}

		m_sSelectPad = ( 2 + m_sSelectPad )%2;

		if( gxLib::KeyBoard( KEYBOARD_ENTER )&enStatTrig )
		{
			m_sBtnIndex = 0;
			m_sConfigSeq = 1000;
		}
		break;

	case 1000:
		//コンフィグ

		if( gxLib::KeyBoard( KEYBOARD_BACKSPACE )&enStatTrig )
		{
			m_sConfigSeq = 100;
		}

		if (m_sBtnIndex < ARRAY_LENGTH(btnTbl))
		{
			if( ::PadConfig(  m_sSelectPad , btnTbl[m_sBtnIndex].uButton ) )
			{
				m_sBtnIndex++;
			}
			if (gxLib::KeyBoard( KEYBOARD_ARROW_RIGHT )&enStatRepeat)
			{
				if( m_sBtnIndex < ARRAY_LENGTH(btnTbl) ) m_sBtnIndex ++;
			}
			else if (gxLib::KeyBoard( KEYBOARD_ARROW_LEFT )&enStatRepeat)
			{
				if( m_sBtnIndex > 0 ) m_sBtnIndex --;
			}
		}
		else
		{
			m_sConfigSeq = 100;
		}
		break;

	case 2000:
		break;

	case 999:
		break;
	}

}


void CGXPadConfig::Draw()
{
	Sint32 ax,ay,az;
	
	ax = WINDOW_W/2;
	ay = WINDOW_H/2;
	az = PRIORITY_MAX;

	switch( m_sConfigSeq ){
	case 100:
		{
			//状態確認

			Uint32 player1=0,player2=0;
			Sint32 pad = m_sSelectPad;

			for( Sint32 ii=0; ii<16; ii++ )
			{
				if( gxLib::Joy(0)->psh&btnTbl[ii].uButton ) player1 |= (0x01<<ii);
				if( gxLib::Joy(1)->psh&btnTbl[ii].uButton ) player2 |= (0x01<<ii);
			}

			drawController( ax , ay-64 , player1 , (pad == 0)? 1 : 0 );
			drawController( ax , ay+64 , player2 , (pad == 1)? 1 : 0 );
		}
		break;

	case 1000:
		//コンフィグ
		if( m_sBtnIndex < 16 )
		{
			drawController( ax , ay , 0x01<<(m_sBtnIndex%16) , 0);
			gxLib::Printf ( ax, ay+64+16*0, az, ATR_STR_CENTER, 0xffffffff, "%d%sに該当するボタンを押してください。",m_sSelectPad + 1, btnTbl[m_sBtnIndex].Name);
			gxLib::Printf ( ax, ay+64+16*1, az, ATR_STR_CENTER, 0xffffffff, "[→] キーで設定をスキップします。");
			gxLib::Printf ( ax, ay+64+16*2, az, ATR_STR_CENTER, 0xffffffff, "[BS] キーでキャンセルします。");
		}
		break;

	default:
		//gxLib::Printf( WINDOW_W/2,jy+64 , az , ATR_STR_CENTER , 0xffffffff , "設定を完了しました。F%d を押してください。", 4 );
		drawController( WINDOW_W/2 , WINDOW_H/2 , 0x01<<(m_sBtnIndex%16) , 0);
		break;
	}
}








