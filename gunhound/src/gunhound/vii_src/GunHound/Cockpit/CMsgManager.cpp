//@
//@
//@
//@
//@

#include <gunvalken.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CMsgManager.h"

void ConvertZenkaku(char* buf);

//〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜
//〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜
//〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜
//〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜
//〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜

void CMsgManager::Load(char* pFileName)
{
	//--------------------------------------------
	//ファイル読み込み
	//--------------------------------------------
	char rawName[256];
	char SaveName[256];
	//struct stat filestat;
	Uint32 sFileSize;

	//sprintf(rawName , "%s" , pFileName );
	gxUtil::GetFileNameWithoutExt( pFileName , rawName );

	//---------------------------------------------
	//スクリプトファイルの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.scr" , rawName );

	Uint8 *pData;
	pData = gxLib::LoadFile(SaveName, &sFileSize);

/*
	fh = open(SaveName,O_RDONLY|O_BINARY);

	ASSERT(fh>=0);

	fstat(fh,&filestat);
	sFileSize = filestat.st_size;
*/

	m_sScrNum  = sFileSize/sizeof(StMsgFmt);
	m_pStMsgFmt = (StMsgFmt*)malloc(sFileSize);
	//read( fh , (char*)m_pStMsgFmt , sFileSize );
	//close(fh);

	memcpy(m_pStMsgFmt, pData, sFileSize);
	delete[] pData;

	//---------------------------------------------
	//ジャンプテーブルの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.jmp" , rawName );

//	fh = open(SaveName,O_RDONLY|O_BINARY);
//	ASSERT(fh>=0);
//	fstat( fh , &filestat );
//	sFileSize = filestat.st_size;

	pData = gxLib::LoadFile(SaveName, &sFileSize);

	m_sJmpNum  = sFileSize/sizeof(StMsgJumpTbl);

	m_pStJumpTbl = (StMsgJumpTbl*)malloc(sFileSize);
	memcpy(m_pStJumpTbl, pData, sFileSize);
	delete[] pData;
	//	read( fh , (char*)m_pStJumpTbl , sFileSize );
//	close(fh);


	//---------------------------------------------
	//メッセージテーブルの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.Str" , rawName );
//	fh = open(SaveName,O_RDONLY|O_BINARY);

	pData = gxLib::LoadFile(SaveName, &sFileSize);

/*
	ASSERT(fh>=0);
	fstat( fh , &filestat );
	sFileSize = filestat.st_size;
*/
	m_pString = (char*)malloc( sFileSize );
//	read( fh , (char*)m_pString , sFileSize );
//	close(fh);
	memcpy(m_pString, pData, sFileSize);
	delete[] pData;


	//---------------------------------------------
	//漢字テーブルの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.Knj" , rawName );

	pData = gxLib::LoadFile(SaveName, &sFileSize);

/*
	fh = open(SaveName,O_RDONLY|O_BINARY);

	ASSERT(fh>=0);

	fstat( fh , &filestat );
	sFileSize = filestat.st_size;
*/
	m_sKanjiNum  = sFileSize/4;

	m_pKanjiTbl = (Sint16*)malloc( sFileSize );
//	read( fh , (char*)m_pKanjiTbl , sFileSize );
//	close(fh);
	memcpy(m_pKanjiTbl, pData, sFileSize);
	delete[] pData;

	//---------------------------------------------
	//フォントテクスチャの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.Bmp" , rawName );
	LoadTexture(enTexPageFont,SaveName,0xfe00fe00);
	UploadTexture();

	Reset();

}


void CMsgManager::LoadMemory(char* pFileName)
{
	//--------------------------------------------
	//ファイル読み込み
	//--------------------------------------------
	gxChar rawName[256];
	gxChar SaveName[256];
	Uint32 sFileSize;
	Uint8 *p;
	Sint32 sz;

	//sprintf(rawName , "%s" , pFileName );
	gxUtil::GetFileNameWithoutExt( pFileName , rawName );

	//---------------------------------------------
	//スクリプトファイルの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.scr" , rawName );

	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );

	sz = sizeof(StMsgFmt);
	m_sScrNum  = sFileSize/sz;
	m_pStMsgFmt = (StMsgFmt*)malloc(sFileSize);
	memcpy(m_pStMsgFmt,p,sFileSize);

	//---------------------------------------------
	//ジャンプテーブルの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.jmp" , rawName );

	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );
	sz = sizeof(StMsgJumpTbl);
	m_sJmpNum  = sFileSize/sz;//sizeof(StMsgJumpTbl);
	m_pStJumpTbl = (StMsgJumpTbl*)malloc(sFileSize);
	memcpy(m_pStJumpTbl,p,sFileSize);

	//---------------------------------------------
	//メッセージテーブルの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.Str" , rawName );

	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );
	m_pString = (char*)malloc( sFileSize );
	memcpy(m_pString,p,sFileSize);

	//---------------------------------------------
	//漢字テーブルの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.Knj" , rawName );
	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );

	m_sKanjiNum  = sFileSize/4;

	m_pKanjiTbl = (Sint16*)malloc( sFileSize );
	memcpy(m_pKanjiTbl,p,sFileSize);

	//---------------------------------------------
	//フォントテクスチャの読み込み
	//---------------------------------------------
	sprintf(SaveName , "%s.Bmp" , rawName );

//	LoadTexture(enTexPageFont,SaveName,0xff000000);
//	viiSub::LoadTexture(enTexPageFont,CCockpit::GetInstance()->Loader(),SaveName);
	CGXImage::GetInstance()->SetTexture(enTexPageFont,SaveName);

	UploadTexture();

	Reset();

}


gxBool CMsgManager::Run( Sint32 num )
{
	//---------------------------------------------
	//スクリプトの呼び出し
	//---------------------------------------------
	m_sCurrent = -1;

	//Num番目のスクリプトへジャンプする
	for(Sint32 ii=0; ii<m_sJmpNum; ii++)
	{
		if( m_pStJumpTbl[ii].sJumpID == num )
		{
			m_sCurrent = m_pStJumpTbl[ii].sJumpPos;
			break;
		}
	}
	if( m_sCurrent < 0 ) return gxFalse;

	m_sWaitTime = 0;
	m_bFreeze = gxFalse;
	m_bButtonWait = gxFalse;

	m_sMsgNum = 0;
	m_sMsgPos[0] = -1;
	m_sMsgPos[1] = -1;
	m_bRun = gxTrue;
	return gxTrue;
}

gxBool CMsgManager::Prev()
{
	if( m_bButtonWait )
	{
		m_bButtonWait = gxFalse;
		m_sMsgNum = 0;
		m_sCurrent ++;
		return gxTrue;
	}

	return gxFalse;
}


gxBool CMsgManager::Execute()
{
	//---------------------------------------------
	//スクリプトの実行
	//---------------------------------------------
	StMsgFmt *p;

	if( !m_bRun ) return gxFalse;

	while( gxTrue ) {

		p = &m_pStMsgFmt[m_sCurrent];
		m_sLastOpeCode = p->sOpCode;
		switch ( m_sLastOpeCode ) {
		case enCodeNone:
			break;

		case enCodeStop:
			//画面停止を指示
			m_bFreeze = gxTrue;
			break;

		case enCodeFaceChange:
			//顔番号を記録する
			m_sFaceNum = p->sData;
			break;

		case enCodeMessageDisp:
			//メッセージをストアする
			{
				Sint32 n1=0;
				m_sMsgPos[ m_sMsgNum ] = p->sData;	//&m_pString[p->sData]);
				m_sMsgNum ++;

				if( m_sMsgNum == 1 )
				{
					n1 = m_sMsgPos[ 0 ];
					sprintf( RetStr , "%s" , &m_pString[ n1 ] );
				}
				else if( m_sMsgNum == 2 )
				{
					n1 = m_sMsgPos[ 0 ];
					sprintf( RetStr , "%s" , &m_pString[ n1 ] );

					n1 = m_sMsgPos[ 1 ];
					sprintf( RetStr , "%s�c%s" , RetStr, &m_pString[ n1 ] );
				}
			}
			break;

		case enCodeWaitButton:
			//ボタン待ちを行う
			m_bButtonWait = gxTrue;
			return gxTrue;

		case enCodeWaitTime:
			m_bButtonWait = gxTrue;
//			m_sWaitTime = enWaitTime;
			return gxTrue;

		case enCodeFinish:
			return gxTrue;
		}

		m_sCurrent ++;
	}

	return gxTrue;
}

gxBool CMsgManager::IsWaitTime()
{
	//---------------------------------------------
	//時間待ち？
	//---------------------------------------------

	if(m_sWaitTime)
	{
		return gxTrue;
	}
	return gxFalse;

}


gxBool CMsgManager::IsWaitButton()
{
	//---------------------------------------------
	//ボタン待ち？
	//---------------------------------------------

	if( m_bButtonWait )
	{
		return gxTrue;
	}
	return gxFalse;

}


gxBool CMsgManager::IsFreeze()
{
	//---------------------------------------------
	//フリーズ中？
	//---------------------------------------------
	return m_bFreeze;

}


char* CMsgManager::GetMessage()
{
	//---------------------------------------------
	//メッセージ表示
	//---------------------------------------------

	if( m_sMsgNum == 0 ) return NULL;

	return RetStr;
}

Sint32 CMsgManager::GetLen()
{
	//---------------------------------------------
	//メッセージの長さを数える
	//---------------------------------------------
	if(RetStr[0] == 0x00) return 0;

	return (Sint32)(strlen(RetStr)/2);
}


Sint32 CMsgManager::GetKanjiPos( Sint32 sIndex )
{

	if(m_pKanjiTbl==NULL) return 0;
	char *p = (char*)&m_pKanjiTbl[0];
	return (Sint32)(p[sIndex*4+2]);
}

Sint32 CMsgManager::GetKanjiWidth( Sint32 sIndex )
{
	char *p = (char*)&m_pKanjiTbl[0];

	return (Sint32)(p[sIndex*4+3]);
	return (Sint32)(p[3]);
}


Sint32 CMsgManager::GetKanjiIndex( Sint32 sMojiNum )
{
	//---------------------------------------------
	//漢字表示(文字番号から)
	//---------------------------------------------
	char buf[4];
	Uint16* pKanji1;
	Uint16* pKanji2;

	char *Cmd[]={
		"�c",	//enKaigyoKode
	};

	buf[0] = RetStr[sMojiNum*2+0];
	buf[1] = RetStr[sMojiNum*2+1];
	buf[2] = 0x00;

	pKanji1 = (Uint16*)buf;

	if( *pKanji1 == *(short*)Cmd[0] )
	{
		return enKaigyoKode;
	}

	for(Sint32 ii=0; ii<m_sKanjiNum ;ii++)
	{
		pKanji2 = (Uint16*)&m_pKanjiTbl[ii*2];

		if( *pKanji1 == *pKanji2 )
		{
			return ii;
		}
	}

	return 0;
}


Sint32 CMsgManager::GetKanjiIndex(char *p)
{
	//---------------------------------------------
	//漢字表示(文字列から)
	//---------------------------------------------

	char buf[4];
	Uint16* pKanji1;
	Uint16* pKanji2;

	buf[0] = p[0];
	buf[1] = p[1];
	buf[2] = 0x00;

	ConvertZenkaku(buf);
	pKanji1 = (Uint16*)buf;

	if(buf[0] == 0 )
	{
		return 0;
	}
	for(Sint32 ii=0; ii<m_sKanjiNum ;ii++)
	{
		pKanji2 = (Uint16*)&m_pKanjiTbl[ii*2];

		if( *pKanji1 == *pKanji2 )
		{
			return ii;
		}
	}

	return 0;
}


void ConvertZenkaku(char* buf)
{
	if( ISKANJI( *buf) )
	{
		return;
	}
	else
	{
		buf[1] = 0;
	}

	switch(*buf){
	case    0:	buf[0] = 0;			break;
	case    ' ': sprintf(buf,"　");break;
	case    '!': sprintf(buf,"！");break;
	case    '"': sprintf(buf,"”");break;
	case    '#': sprintf(buf,"＃");break;
	case    '$': sprintf(buf,"＄");break;
	case    '%': sprintf(buf,"％");break;
	case    '&': sprintf(buf,"＆");break;
	case    '\'':sprintf(buf,"’");break;
	case    '(': sprintf(buf,"（");break;
	case    ')': sprintf(buf,"）");break;
	case    '*': sprintf(buf,"＊");break;
	case    '+': sprintf(buf,"＋");break;
	case    ',': sprintf(buf,"，");break;
	case    '-': sprintf(buf,"−");break;
	case    '.': sprintf(buf,"．");break;
	case    '/': sprintf(buf,"／");break;
	case    '0': sprintf(buf,"０");break;
	case    '1': sprintf(buf,"１");break;
	case    '2': sprintf(buf,"２");break;
	case    '3': sprintf(buf,"３");break;
	case    '4': sprintf(buf,"４");break;
	case    '5': sprintf(buf,"５");break;
	case    '6': sprintf(buf,"６");break;
	case    '7': sprintf(buf,"７");break;
	case    '8': sprintf(buf,"８");break;
	case    '9': sprintf(buf,"９");break;
	case    ':': sprintf(buf,"：");break;
	case    ';': sprintf(buf,"；");break;
	case    '<': sprintf(buf,"＜");break;
	case    '=': sprintf(buf,"＝");break;
	case    '>': sprintf(buf,"＞");break;
	case    '?': sprintf(buf,"？");break;
	case    '@': sprintf(buf,"＠");break;
	case    'A': sprintf(buf,"Ａ");break;
	case    'B': sprintf(buf,"Ｂ");break;
	case    'C': sprintf(buf,"Ｃ");break;
	case    'D': sprintf(buf,"Ｄ");break;
	case    'E': sprintf(buf,"Ｅ");break;
	case    'F': sprintf(buf,"Ｆ");break;
	case    'G': sprintf(buf,"Ｇ");break;
	case    'H': sprintf(buf,"Ｈ");break;
	case    'I': sprintf(buf,"Ｉ");break;
	case    'J': sprintf(buf,"Ｊ");break;
	case    'K': sprintf(buf,"Ｋ");break;
	case    'L': sprintf(buf,"Ｌ");break;
	case    'M': sprintf(buf,"Ｍ");break;
	case    'N': sprintf(buf,"Ｎ");break;
	case    'O': sprintf(buf,"Ｏ");break;
	case    'P': sprintf(buf,"Ｐ");break;
	case    'Q': sprintf(buf,"Ｑ");break;
	case    'R': sprintf(buf,"Ｒ");break;
	case    'S': sprintf(buf,"Ｓ");break;
	case    'T': sprintf(buf,"Ｔ");break;
	case    'U': sprintf(buf,"Ｕ");break;
	case    'V': sprintf(buf,"Ｖ");break;
	case    'W': sprintf(buf,"Ｗ");break;
	case    'X': sprintf(buf,"Ｘ");break;
	case    'Y': sprintf(buf,"Ｙ");break;
	case    'Z': sprintf(buf,"Ｚ");break;
	case    '[': sprintf(buf,"［");break;
	case    '\\':sprintf(buf,"￥");break;
	case    ']': sprintf(buf,"］");break;
	case    '^': sprintf(buf,"＾");break;
	case    '_': sprintf(buf,"＿");break;
	case    '`': sprintf(buf,"‘");break;
	case    'a': sprintf(buf,"ａ");break;
	case    'b': sprintf(buf,"ｂ");break;
	case    'c': sprintf(buf,"ｃ");break;
	case    'd': sprintf(buf,"ｄ");break;
	case    'e': sprintf(buf,"ｅ");break;
	case    'f': sprintf(buf,"ｆ");break;
	case    'g': sprintf(buf,"ｇ");break;
	case    'h': sprintf(buf,"ｈ");break;
	case    'i': sprintf(buf,"ｉ");break;
	case    'j': sprintf(buf,"ｊ");break;
	case    'k': sprintf(buf,"ｋ");break;
	case    'l': sprintf(buf,"ｌ");break;
	case    'm': sprintf(buf,"ｍ");break;
	case    'n': sprintf(buf,"ｎ");break;
	case    'o': sprintf(buf,"ｏ");break;
	case    'p': sprintf(buf,"ｐ");break;
	case    'q': sprintf(buf,"ｑ");break;
	case    'r': sprintf(buf,"ｒ");break;
	case    's': sprintf(buf,"ｓ");break;
	case    't': sprintf(buf,"ｔ");break;
	case    'u': sprintf(buf,"ｕ");break;
	case    'v': sprintf(buf,"ｖ");break;
	case    'w': sprintf(buf,"ｗ");break;
	case    'x': sprintf(buf,"ｘ");break;
	case    'y': sprintf(buf,"ｙ");break;
	case    'z': sprintf(buf,"ｚ");break;
	case    '{': sprintf(buf,"｛");break;
	case    '|': sprintf(buf,"｜");break;
	case    '}': sprintf(buf,"｝");break;
	default:
		return;
	}

}


void CMsgManager::Printf(Sint32 x, Sint32 y , Sint32 sPrio , Sint32 sCol , char* pMsg)
{
	//---------------------------------------------
	//漢字表示
	//---------------------------------------------

	Sint32 sLen = (Sint32)strlen(pMsg);
	Sint32 u,v,n,w,s;

	Sint32 xx = 0;

	for(Sint32 ii=0;ii<sLen; ii++)
	{
		n = GetKanjiIndex( &pMsg[ii] );
		s = GetKanjiPos( n );
		w = GetKanjiWidth( n );

		u = (n%32)*16;
		v = (n/32)*16;

		xx -= s;

		PutSpriteDX( x + xx, y , sPrio,	enTexPageFont,u,v,16,16,0,0	,ATR_DFLT,sCol);

//		pos ++;

		xx += (s+w);
		xx -= 2;

		if( ISKANJI( pMsg[ii]) )
		{
			ii++;
		}
	}

//	msg_printfwithCol(x,y,enMaxPriority-1,0xffffffff,&buf[0]);

}


