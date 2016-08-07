//@
//@
//@
//@
//@

#include <gunvalken.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CMsgManager.h"

void ConvertZenkaku(char* buf);

//Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`
//Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`
//Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`
//Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`
//Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`Å`

void CMsgManager::Load(char* pFileName)
{
	//--------------------------------------------
	//ÉtÉ@ÉCÉãì«Ç›çûÇ›
	//--------------------------------------------
	char rawName[256];
	char SaveName[256];
	//struct stat filestat;
	Uint32 sFileSize;

	//sprintf(rawName , "%s" , pFileName );
	gxUtil::GetFileNameWithoutExt( pFileName , rawName );

	//---------------------------------------------
	//ÉXÉNÉäÉvÉgÉtÉ@ÉCÉãÇÃì«Ç›çûÇ›
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
	//ÉWÉÉÉìÉvÉeÅ[ÉuÉãÇÃì«Ç›çûÇ›
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
	//ÉÅÉbÉZÅ[ÉWÉeÅ[ÉuÉãÇÃì«Ç›çûÇ›
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
	//äøéöÉeÅ[ÉuÉãÇÃì«Ç›çûÇ›
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
	//ÉtÉHÉìÉgÉeÉNÉXÉ`ÉÉÇÃì«Ç›çûÇ›
	//---------------------------------------------
	sprintf(SaveName , "%s.Bmp" , rawName );
	LoadTexture(enTexPageFont,SaveName,0xfe00fe00);
	UploadTexture();

	Reset();

}


void CMsgManager::LoadMemory(char* pFileName)
{
	//--------------------------------------------
	//ÉtÉ@ÉCÉãì«Ç›çûÇ›
	//--------------------------------------------
	gxChar rawName[256];
	gxChar SaveName[256];
	Uint32 sFileSize;
	Uint8 *p;
	Sint32 sz;

	//sprintf(rawName , "%s" , pFileName );
	gxUtil::GetFileNameWithoutExt( pFileName , rawName );

	//---------------------------------------------
	//ÉXÉNÉäÉvÉgÉtÉ@ÉCÉãÇÃì«Ç›çûÇ›
	//---------------------------------------------
	sprintf(SaveName , "%s.scr" , rawName );

	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );

	sz = sizeof(StMsgFmt);
	m_sScrNum  = sFileSize/sz;
	m_pStMsgFmt = (StMsgFmt*)malloc(sFileSize);
	memcpy(m_pStMsgFmt,p,sFileSize);

	//---------------------------------------------
	//ÉWÉÉÉìÉvÉeÅ[ÉuÉãÇÃì«Ç›çûÇ›
	//---------------------------------------------
	sprintf(SaveName , "%s.jmp" , rawName );

	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );
	sz = sizeof(StMsgJumpTbl);
	m_sJmpNum  = sFileSize/sz;//sizeof(StMsgJumpTbl);
	m_pStJumpTbl = (StMsgJumpTbl*)malloc(sFileSize);
	memcpy(m_pStJumpTbl,p,sFileSize);

	//---------------------------------------------
	//ÉÅÉbÉZÅ[ÉWÉeÅ[ÉuÉãÇÃì«Ç›çûÇ›
	//---------------------------------------------
	sprintf(SaveName , "%s.Str" , rawName );

	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );
	m_pString = (char*)malloc( sFileSize );
	memcpy(m_pString,p,sFileSize);

	//---------------------------------------------
	//äøéöÉeÅ[ÉuÉãÇÃì«Ç›çûÇ›
	//---------------------------------------------
	sprintf(SaveName , "%s.Knj" , rawName );
	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );

	m_sKanjiNum  = sFileSize/4;

	m_pKanjiTbl = (Sint16*)malloc( sFileSize );
	memcpy(m_pKanjiTbl,p,sFileSize);

	//---------------------------------------------
	//ÉtÉHÉìÉgÉeÉNÉXÉ`ÉÉÇÃì«Ç›çûÇ›
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
	//ÉXÉNÉäÉvÉgÇÃåƒÇ—èoÇµ
	//---------------------------------------------
	m_sCurrent = -1;

	//Numî‘ñ⁄ÇÃÉXÉNÉäÉvÉgÇ÷ÉWÉÉÉìÉvÇ∑ÇÈ
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
	//ÉXÉNÉäÉvÉgÇÃé¿çs
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
			//âÊñ í‚é~Çéwé¶
			m_bFreeze = gxTrue;
			break;

		case enCodeFaceChange:
			//äÁî‘çÜÇãLò^Ç∑ÇÈ
			m_sFaceNum = p->sData;
			break;

		case enCodeMessageDisp:
			//ÉÅÉbÉZÅ[ÉWÇÉXÉgÉAÇ∑ÇÈ
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
					sprintf( RetStr , "%sác%s" , RetStr, &m_pString[ n1 ] );
				}
			}
			break;

		case enCodeWaitButton:
			//É{É^Éìë“ÇøÇçsÇ§
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
	//éûä‘ë“ÇøÅH
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
	//É{É^Éìë“ÇøÅH
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
	//ÉtÉäÅ[ÉYíÜÅH
	//---------------------------------------------
	return m_bFreeze;

}


char* CMsgManager::GetMessage()
{
	//---------------------------------------------
	//ÉÅÉbÉZÅ[ÉWï\é¶
	//---------------------------------------------

	if( m_sMsgNum == 0 ) return NULL;

	return RetStr;
}

Sint32 CMsgManager::GetLen()
{
	//---------------------------------------------
	//ÉÅÉbÉZÅ[ÉWÇÃí∑Ç≥ÇêîÇ¶ÇÈ
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
	//äøéöï\é¶(ï∂éöî‘çÜÇ©ÇÁ)
	//---------------------------------------------
	char buf[4];
	Uint16* pKanji1;
	Uint16* pKanji2;

	char *Cmd[]={
		"ác",	//enKaigyoKode
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
	//äøéöï\é¶(ï∂éöóÒÇ©ÇÁ)
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
	case    ' ': sprintf(buf,"Å@");break;
	case    '!': sprintf(buf,"ÅI");break;
	case    '"': sprintf(buf,"Åh");break;
	case    '#': sprintf(buf,"Åî");break;
	case    '$': sprintf(buf,"Åê");break;
	case    '%': sprintf(buf,"Åì");break;
	case    '&': sprintf(buf,"Åï");break;
	case    '\'':sprintf(buf,"Åf");break;
	case    '(': sprintf(buf,"Åi");break;
	case    ')': sprintf(buf,"Åj");break;
	case    '*': sprintf(buf,"Åñ");break;
	case    '+': sprintf(buf,"Å{");break;
	case    ',': sprintf(buf,"ÅC");break;
	case    '-': sprintf(buf,"Å|");break;
	case    '.': sprintf(buf,"ÅD");break;
	case    '/': sprintf(buf,"Å^");break;
	case    '0': sprintf(buf,"ÇO");break;
	case    '1': sprintf(buf,"ÇP");break;
	case    '2': sprintf(buf,"ÇQ");break;
	case    '3': sprintf(buf,"ÇR");break;
	case    '4': sprintf(buf,"ÇS");break;
	case    '5': sprintf(buf,"ÇT");break;
	case    '6': sprintf(buf,"ÇU");break;
	case    '7': sprintf(buf,"ÇV");break;
	case    '8': sprintf(buf,"ÇW");break;
	case    '9': sprintf(buf,"ÇX");break;
	case    ':': sprintf(buf,"ÅF");break;
	case    ';': sprintf(buf,"ÅG");break;
	case    '<': sprintf(buf,"ÅÉ");break;
	case    '=': sprintf(buf,"ÅÅ");break;
	case    '>': sprintf(buf,"ÅÑ");break;
	case    '?': sprintf(buf,"ÅH");break;
	case    '@': sprintf(buf,"Åó");break;
	case    'A': sprintf(buf,"Ç`");break;
	case    'B': sprintf(buf,"Ça");break;
	case    'C': sprintf(buf,"Çb");break;
	case    'D': sprintf(buf,"Çc");break;
	case    'E': sprintf(buf,"Çd");break;
	case    'F': sprintf(buf,"Çe");break;
	case    'G': sprintf(buf,"Çf");break;
	case    'H': sprintf(buf,"Çg");break;
	case    'I': sprintf(buf,"Çh");break;
	case    'J': sprintf(buf,"Çi");break;
	case    'K': sprintf(buf,"Çj");break;
	case    'L': sprintf(buf,"Çk");break;
	case    'M': sprintf(buf,"Çl");break;
	case    'N': sprintf(buf,"Çm");break;
	case    'O': sprintf(buf,"Çn");break;
	case    'P': sprintf(buf,"Ço");break;
	case    'Q': sprintf(buf,"Çp");break;
	case    'R': sprintf(buf,"Çq");break;
	case    'S': sprintf(buf,"Çr");break;
	case    'T': sprintf(buf,"Çs");break;
	case    'U': sprintf(buf,"Çt");break;
	case    'V': sprintf(buf,"Çu");break;
	case    'W': sprintf(buf,"Çv");break;
	case    'X': sprintf(buf,"Çw");break;
	case    'Y': sprintf(buf,"Çx");break;
	case    'Z': sprintf(buf,"Çy");break;
	case    '[': sprintf(buf,"Åm");break;
	case    '\\':sprintf(buf,"Åè");break;
	case    ']': sprintf(buf,"Ån");break;
	case    '^': sprintf(buf,"ÅO");break;
	case    '_': sprintf(buf,"ÅQ");break;
	case    '`': sprintf(buf,"Åe");break;
	case    'a': sprintf(buf,"ÇÅ");break;
	case    'b': sprintf(buf,"ÇÇ");break;
	case    'c': sprintf(buf,"ÇÉ");break;
	case    'd': sprintf(buf,"ÇÑ");break;
	case    'e': sprintf(buf,"ÇÖ");break;
	case    'f': sprintf(buf,"ÇÜ");break;
	case    'g': sprintf(buf,"Çá");break;
	case    'h': sprintf(buf,"Çà");break;
	case    'i': sprintf(buf,"Çâ");break;
	case    'j': sprintf(buf,"Çä");break;
	case    'k': sprintf(buf,"Çã");break;
	case    'l': sprintf(buf,"Çå");break;
	case    'm': sprintf(buf,"Çç");break;
	case    'n': sprintf(buf,"Çé");break;
	case    'o': sprintf(buf,"Çè");break;
	case    'p': sprintf(buf,"Çê");break;
	case    'q': sprintf(buf,"Çë");break;
	case    'r': sprintf(buf,"Çí");break;
	case    's': sprintf(buf,"Çì");break;
	case    't': sprintf(buf,"Çî");break;
	case    'u': sprintf(buf,"Çï");break;
	case    'v': sprintf(buf,"Çñ");break;
	case    'w': sprintf(buf,"Çó");break;
	case    'x': sprintf(buf,"Çò");break;
	case    'y': sprintf(buf,"Çô");break;
	case    'z': sprintf(buf,"Çö");break;
	case    '{': sprintf(buf,"Åo");break;
	case    '|': sprintf(buf,"Åb");break;
	case    '}': sprintf(buf,"Åp");break;
	default:
		return;
	}

}


void CMsgManager::Printf(Sint32 x, Sint32 y , Sint32 sPrio , Sint32 sCol , char* pMsg)
{
	//---------------------------------------------
	//äøéöï\é¶
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


