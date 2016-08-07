/*!
 @file CWazaCommand.h
 @brief コマンド技によるボタン入力判定ルーチン
 @author ragi
 @date 2009.07.26
*/

//--------------------------------------------------------------------
//1.毎フレームの入力情報をSetLog()で渡してやること
//2.Check()でコマンドをチェックする
//3.うまくできないときはコマンドの優先順位（判定順序）を確認すること。
//--------------------------------------------------------------------
#include <gxLib.h>
#include "CWazaCommand.h"

#define LOG_KEY_FILENAME "keylog.bin"
/*!
 @brief キー入力の履歴を保存する。
 @param[in] sKey pshキー情報
 @return なし
*/
void CWazaCommand::SetLog(Sint32 sKey)
{
	//--------------------------------------
	//キー情報を保存する
	//--------------------------------------
	if( m_bReplay )
	{
	}
	else
	{
		m_pKeyLog[m_uKeyCnt] = (Uint32)sKey;
	}
	m_uKeyCnt ++;
	m_uKeyCnt = m_uKeyCnt%m_uKeyLogMax;
}


/*!
 @brief nフレーム前のキー入力の履歴を返す。
 @param[in] uFrm 参照したい過去フレーム数
 @return キー入力の履歴
*/
Uint32 CWazaCommand::GetLog( Uint32 uFrm )
{
	//--------------------------------------
	//uFrm前のログをチェックする
	//--------------------------------------
	Uint32 uLogPos = 0;
	if( uFrm == 0xffffffff )
	{
		uFrm = 0;//m_uKeyCnt;
	}

	uLogPos = (m_uKeyCnt + m_uKeyLogMax - uFrm-1)%m_uKeyLogMax;

	return m_pKeyLog[uLogPos];

}


/*!
 @brief コマンドの有無を確認する。
 @param[in] pCommand コマンドテーブルのアドレス
 @param[in] uSize    コマンドテーブルのサイズ
 @return 指定されたコマンドがあればTrue/なければFalseを返す
*/
gxBool CWazaCommand::Check(Uint32 *pCommand,Uint32 uSize ,gxBool bReverse )
{
	//--------------------------------------
	//コマンドをチェックする
	//--------------------------------------
	Uint32 uSinko  = 0;
	Uint32 uLogPos = 0;
	Uint32 uRireki = 0;
	Uint32 uPush   = 0;
	gxBool bExist = gxFalse;

	for( Uint32 ii=0; ii<uSize; ii++ )
	{
		bExist = gxFalse;

		//今回調べるコマンド
		uPush = pCommand[ (uSize-uSinko)-1 ];

		if( uPush == enKeyEnd )
		{
			//ダミーコマンドなので無視する
			uSinko ++;
			continue;
		}

		if( bReverse )
		{
			uPush = ConvertReverseCommand( uPush );
		}
		//待ちうけ時間中にそのコマンドがあるか？
		for(Uint32 jj=0;jj<enKeyValidFrm;jj++ )
		{
			//履歴のポジションを計算
			Uint32 uKey = GetLog( uRireki );

			if( CheckInput( uKey , uPush ) )
			{
				//あった
				bExist = gxTrue;
				break;
			}
			uRireki ++;
		}

		if( bExist )
		{
			uSinko ++;
		}
		else
		{
			//なかった
			return gxFalse;
		}
	}

	//ない、ということがなかったということは、あった、とみなす。

	return gxTrue;

}


/*!
 @brief 過去の入力に特定の入力があるか確認する。
 @param[in] uLogKey ログ内のキー情報
 @param[in] uPush   コマンド（指定フォーマットによる）
 @return 指定されたコマンドがあればTrue/なければFalseを返す
*/
gxBool CWazaCommand::CheckInput( Uint32 uLogKey , Uint32 uPush )
{
	Uint32 Key = uLogKey;
	Uint32 Lev = (uLogKey&(JOY_U|JOY_D|JOY_L|JOY_R) );
	Uint32 Btn = (uLogKey&(BTN_A|BTN_B|BTN_C|BTN_X|BTN_Y|BTN_Z|BTN_L1|BTN_R1|BTN_L2|BTN_R2) );

	switch( uPush ) {
	//何も押していない
	case enKeyNone:
		return (Key == 0)? gxTrue : gxFalse;

	//なんでも許可
	case enKeyEvery:
		return gxTrue;

	//ボタン押しがなければ許可
	case enKeyBtnNone:
		return (Btn==0)? gxTrue : gxFalse;

	//レバー入れがなければ許可
	case enKeyLeverNone:
		return (Lev==0)? gxTrue : gxFalse;

	case enKeyUp:
		return (Lev == JOY_U)? gxTrue : gxFalse;

	case enKeyUpRight:
		return (Lev == (JOY_U|JOY_R) )? gxTrue : gxFalse;

	case enKeyRight:
		return (Lev == JOY_R)? gxTrue : gxFalse;

	case enKeyRightDown:
		return (Lev == (JOY_R|JOY_D) )? gxTrue : gxFalse;

	case enKeyDown:
		return (Lev == JOY_D)? gxTrue : gxFalse;

	case enKeyDownLeft:
		return (Lev == (JOY_L|JOY_D) )? gxTrue : gxFalse;

	case enKeyLeft:
		return (Lev == JOY_L)? gxTrue : gxFalse;

	case enKeyLeftUp:
		return (Lev == (JOY_L|JOY_U) )? gxTrue : gxFalse;

	case enKeyBtnA:
		return ( Btn&(BTN_A) )? gxTrue : gxFalse;

	case enKeyBtnB:
		return ( Btn&(BTN_B) )? gxTrue : gxFalse;

	case enKeyBtnC:
		return ( Btn&(BTN_C) )? gxTrue : gxFalse;

	case enKeyBtnX:
		return ( Btn&(BTN_X) )? gxTrue : gxFalse;

	case enKeyBtnY:
		return ( Btn&(BTN_Y) )? gxTrue : gxFalse;

	case enKeyBtnZ:
		return ( Btn&(BTN_Z) )? gxTrue : gxFalse;

	case enKeyBtnL1:
		return ( Btn&(BTN_L1) )? gxTrue : gxFalse;

	case enKeyBtnR1:
		return ( Btn&(BTN_R1) )? gxTrue : gxFalse;

	case enKeyBtnL2:
		return ( Btn&(BTN_L2) )? gxTrue : gxFalse;

	case enKeyBtnR2:
		return ( Btn&(BTN_R2) )? gxTrue : gxFalse;

	//---------------------
	//その方向を含んでいれば許可
	case enKeyIncU:
		return (Lev&JOY_U)? gxTrue : gxFalse;

	case enKeyIncR:
		return (Lev&JOY_R)? gxTrue : gxFalse;

	case enKeyIncD:
		return (Lev&JOY_D)? gxTrue : gxFalse;

	case enKeyIncL:
		return (Lev&JOY_L)? gxTrue : gxFalse;

	//----------------------
	//未実装
	//---------------------

	//タメ判定許容範囲（小）
	case enKeyWaitU:
	case enKeyWaitR:
	case enKeyWaitD:
	case enKeyWaitL:

	//タメ判定許容範囲（大）
	case enKeyWaitIncU:
	case enKeyWaitIncR:
	case enKeyWaitIncD:
	case enKeyWaitIncL:
		break;
	}

	return gxFalse;
}

/*!
 @brief 入力方向を左右逆に変換する
 @param[in] uCmd 正規のキー情報
 @return 変換済みのキー情報
*/
Uint32 CWazaCommand::ConvertReverseCommand(Uint32 uCmd)
{
	switch(uCmd){
	case enKeyUpRight:
		return enKeyLeftUp;

	case enKeyRight:
		return enKeyLeft;

	case enKeyRightDown:
		return enKeyDownLeft;

	case enKeyDownLeft:
		return enKeyRightDown;

	case enKeyLeft:
		return enKeyRight;

	case enKeyLeftUp:
		return enKeyUpRight;

	case enKeyIncR:
		return enKeyIncL;

	case enKeyIncL:
		return enKeyIncR;

	case enKeyWaitR:
		return enKeyWaitL;

	case enKeyWaitL:
		return enKeyWaitR;

	case enKeyWaitIncR:
		return enKeyWaitIncL;

	case enKeyWaitIncL:
		return enKeyWaitIncR;
	default:
		break;
	};

	return uCmd;
}


void CWazaCommand::Play(gxChar* pBuffer)
{
	if( m_bReplay ) return;
	m_bReplay = gxTrue;
	m_pReplayKeyLog = (Uint32*)pBuffer;

	for(Uint32 ii=0; ii<m_uKeyLogMax; ii++)
	{
		m_pKeyLog[ii] = m_pReplayKeyLog[ii];
	}
	m_uKeyCnt = 1;
}

void CWazaCommand::Rec()
{

	m_bRecord = gxTrue;

}


void CWazaCommand::Stop()
{
	if( m_bRecord )
	{
		gxLib::SaveFile( LOG_KEY_FILENAME ,(Uint8*)m_pKeyLog , sizeof(Uint32)*m_uKeyLogMax );
	}
}

//-------------------------------------------------
//
//サンプルコード
//
//-------------------------------------------------

CWazaCommand *pWaza=NULL;

void CommandCheck()
{
	//------------------------------------------
	//コマンドチェックのサンプル
	//------------------------------------------

	static Sint32 SuccessWait = 0;
	static gxChar *pSuccessName=NULL;

	Uint32 uCmdSinkuHadouken[]={
		CWazaCommand::enKeyDown,
		CWazaCommand::enKeyRightDown,
		CWazaCommand::enKeyRight,
		CWazaCommand::enKeyDown,
		CWazaCommand::enKeyRightDown,
		CWazaCommand::enKeyRight,
		CWazaCommand::enKeyBtnA,
		CWazaCommand::enKeyEnd,
	};
	Uint32 uCmdHadouken[]={
		CWazaCommand::enKeyDown,
		CWazaCommand::enKeyRightDown,
		CWazaCommand::enKeyRight,
		CWazaCommand::enKeyBtnA,
		CWazaCommand::enKeyEnd,
	};
	Uint32 uCmdSyouryuken[]={
		CWazaCommand::enKeyRight,
		CWazaCommand::enKeyDown,
		CWazaCommand::enKeyRightDown,
		CWazaCommand::enKeyBtnA,
		CWazaCommand::enKeyEnd,
	};
	Uint32 uCmdSenpukyaku[]={
		CWazaCommand::enKeyDown,
		CWazaCommand::enKeyDownLeft,
		CWazaCommand::enKeyLeft,
		CWazaCommand::enKeyBtnB,
		CWazaCommand::enKeyEnd,
	};
	Uint32 uCmdDash[]={
		CWazaCommand::enKeyLeverNone,
		CWazaCommand::enKeyRight,
		CWazaCommand::enKeyLeverNone,
		CWazaCommand::enKeyRight,
		CWazaCommand::enKeyEnd,
	};
	Uint32 uCmdBack[]={
		CWazaCommand::enKeyLeverNone,
		CWazaCommand::enKeyLeft,
		CWazaCommand::enKeyLeverNone,
		CWazaCommand::enKeyLeft,
		CWazaCommand::enKeyEnd,
	};

	Uint32 uCmdBtnOnly[]={
		CWazaCommand::enKeyLeverNone,
		CWazaCommand::enKeyBtnX,
		CWazaCommand::enKeyLeverNone,
		CWazaCommand::enKeyBtnX,
		CWazaCommand::enKeyLeft,
		CWazaCommand::enKeyBtnA,
		CWazaCommand::enKeyLeverNone,
		CWazaCommand::enKeyBtnB,
		CWazaCommand::enKeyEnd,
	};

	if(pWaza==NULL)
	{
		pWaza = new CWazaCommand();
	}


	pWaza->SetLog( gxLib::Joy(0)->psh );

	gxLib::Printf(32,8,32,ATR_DFLT,ARGB_DFLT,"NOW(%d)" ,gxLib::Joy(0)->psh );

	for(Sint32 ii=0;ii<24;ii++)
	{
		Sint32 sCmd;
		sCmd = pWaza->GetLog( ii );
		gxLib::Printf(32,32+ii*16,32,ATR_DFLT,ARGB_DFLT,"%d" ,sCmd );
	}

	if( SuccessWait > 0 )
	{
		SuccessWait --;
		gxLib::Printf(128,128,32,ATR_DFLT,ARGB_DFLT,pSuccessName);
		return;
	}

	if( pWaza->Check( uCmdSinkuHadouken , sizeof(uCmdSinkuHadouken)/sizeof(Uint32) ) )
	{
		SuccessWait = 30;
		pSuccessName = "しんくうはどうけん";
	}
	else if( pWaza->Check( uCmdSyouryuken , sizeof(uCmdSyouryuken)/sizeof(Uint32) ) )
	{
		SuccessWait = 30;
		pSuccessName = "しょうりゅうけん";
	}
	else if( pWaza->Check( uCmdHadouken , sizeof(uCmdHadouken)/sizeof(Uint32) ) )
	{
		SuccessWait = 30;
		pSuccessName = "はどうけん";
	}
	else if( pWaza->Check( uCmdSenpukyaku , sizeof(uCmdSenpukyaku)/sizeof(Uint32) ) )
	{
		SuccessWait = 30;
		pSuccessName = "せんぷうきゃく";
	}
	else if( pWaza->Check( uCmdDash , sizeof(uCmdDash)/sizeof(Uint32) ) )
	{
		SuccessWait = 30;
		pSuccessName = "ダッシュ";
	}
	else if( pWaza->Check( uCmdBack , sizeof(uCmdBack)/sizeof(Uint32) ) )
	{
		SuccessWait = 30;
		pSuccessName = "バックステップ";
	}
	else if( pWaza->Check( uCmdBtnOnly , sizeof(uCmdBtnOnly)/sizeof(Uint32) ) )
	{
		SuccessWait = 30;
		pSuccessName = "しゅんごくさつ";
	}


}

