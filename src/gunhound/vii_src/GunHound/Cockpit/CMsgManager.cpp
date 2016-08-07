//@
//@
//@
//@
//@

#include <gunvalken.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CMsgManager.h"

void ConvertZenkaku(char* buf);

//�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`
//�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`
//�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`
//�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`
//�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`�`

void CMsgManager::Load(char* pFileName)
{
	//--------------------------------------------
	//�t�@�C���ǂݍ���
	//--------------------------------------------
	char rawName[256];
	char SaveName[256];
	//struct stat filestat;
	Uint32 sFileSize;

	//sprintf(rawName , "%s" , pFileName );
	gxUtil::GetFileNameWithoutExt( pFileName , rawName );

	//---------------------------------------------
	//�X�N���v�g�t�@�C���̓ǂݍ���
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
	//�W�����v�e�[�u���̓ǂݍ���
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
	//���b�Z�[�W�e�[�u���̓ǂݍ���
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
	//�����e�[�u���̓ǂݍ���
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
	//�t�H���g�e�N�X�`���̓ǂݍ���
	//---------------------------------------------
	sprintf(SaveName , "%s.Bmp" , rawName );
	LoadTexture(enTexPageFont,SaveName,0xfe00fe00);
	UploadTexture();

	Reset();

}


void CMsgManager::LoadMemory(char* pFileName)
{
	//--------------------------------------------
	//�t�@�C���ǂݍ���
	//--------------------------------------------
	gxChar rawName[256];
	gxChar SaveName[256];
	Uint32 sFileSize;
	Uint8 *p;
	Sint32 sz;

	//sprintf(rawName , "%s" , pFileName );
	gxUtil::GetFileNameWithoutExt( pFileName , rawName );

	//---------------------------------------------
	//�X�N���v�g�t�@�C���̓ǂݍ���
	//---------------------------------------------
	sprintf(SaveName , "%s.scr" , rawName );

	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );

	sz = sizeof(StMsgFmt);
	m_sScrNum  = sFileSize/sz;
	m_pStMsgFmt = (StMsgFmt*)malloc(sFileSize);
	memcpy(m_pStMsgFmt,p,sFileSize);

	//---------------------------------------------
	//�W�����v�e�[�u���̓ǂݍ���
	//---------------------------------------------
	sprintf(SaveName , "%s.jmp" , rawName );

	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );
	sz = sizeof(StMsgJumpTbl);
	m_sJmpNum  = sFileSize/sz;//sizeof(StMsgJumpTbl);
	m_pStJumpTbl = (StMsgJumpTbl*)malloc(sFileSize);
	memcpy(m_pStJumpTbl,p,sFileSize);

	//---------------------------------------------
	//���b�Z�[�W�e�[�u���̓ǂݍ���
	//---------------------------------------------
	sprintf(SaveName , "%s.Str" , rawName );

	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );
	m_pString = (char*)malloc( sFileSize );
	memcpy(m_pString,p,sFileSize);

	//---------------------------------------------
	//�����e�[�u���̓ǂݍ���
	//---------------------------------------------
	sprintf(SaveName , "%s.Knj" , rawName );
	p = CGXImage::GetInstance()->GetFile( SaveName , &sFileSize );

	m_sKanjiNum  = sFileSize/4;

	m_pKanjiTbl = (Sint16*)malloc( sFileSize );
	memcpy(m_pKanjiTbl,p,sFileSize);

	//---------------------------------------------
	//�t�H���g�e�N�X�`���̓ǂݍ���
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
	//�X�N���v�g�̌Ăяo��
	//---------------------------------------------
	m_sCurrent = -1;

	//Num�Ԗڂ̃X�N���v�g�փW�����v����
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
	//�X�N���v�g�̎��s
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
			//��ʒ�~���w��
			m_bFreeze = gxTrue;
			break;

		case enCodeFaceChange:
			//��ԍ����L�^����
			m_sFaceNum = p->sData;
			break;

		case enCodeMessageDisp:
			//���b�Z�[�W���X�g�A����
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
			//�{�^���҂����s��
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
	//���ԑ҂��H
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
	//�{�^���҂��H
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
	//�t���[�Y���H
	//---------------------------------------------
	return m_bFreeze;

}


char* CMsgManager::GetMessage()
{
	//---------------------------------------------
	//���b�Z�[�W�\��
	//---------------------------------------------

	if( m_sMsgNum == 0 ) return NULL;

	return RetStr;
}

Sint32 CMsgManager::GetLen()
{
	//---------------------------------------------
	//���b�Z�[�W�̒����𐔂���
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
	//�����\��(�����ԍ�����)
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
	//�����\��(�����񂩂�)
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
	case    ' ': sprintf(buf,"�@");break;
	case    '!': sprintf(buf,"�I");break;
	case    '"': sprintf(buf,"�h");break;
	case    '#': sprintf(buf,"��");break;
	case    '$': sprintf(buf,"��");break;
	case    '%': sprintf(buf,"��");break;
	case    '&': sprintf(buf,"��");break;
	case    '\'':sprintf(buf,"�f");break;
	case    '(': sprintf(buf,"�i");break;
	case    ')': sprintf(buf,"�j");break;
	case    '*': sprintf(buf,"��");break;
	case    '+': sprintf(buf,"�{");break;
	case    ',': sprintf(buf,"�C");break;
	case    '-': sprintf(buf,"�|");break;
	case    '.': sprintf(buf,"�D");break;
	case    '/': sprintf(buf,"�^");break;
	case    '0': sprintf(buf,"�O");break;
	case    '1': sprintf(buf,"�P");break;
	case    '2': sprintf(buf,"�Q");break;
	case    '3': sprintf(buf,"�R");break;
	case    '4': sprintf(buf,"�S");break;
	case    '5': sprintf(buf,"�T");break;
	case    '6': sprintf(buf,"�U");break;
	case    '7': sprintf(buf,"�V");break;
	case    '8': sprintf(buf,"�W");break;
	case    '9': sprintf(buf,"�X");break;
	case    ':': sprintf(buf,"�F");break;
	case    ';': sprintf(buf,"�G");break;
	case    '<': sprintf(buf,"��");break;
	case    '=': sprintf(buf,"��");break;
	case    '>': sprintf(buf,"��");break;
	case    '?': sprintf(buf,"�H");break;
	case    '@': sprintf(buf,"��");break;
	case    'A': sprintf(buf,"�`");break;
	case    'B': sprintf(buf,"�a");break;
	case    'C': sprintf(buf,"�b");break;
	case    'D': sprintf(buf,"�c");break;
	case    'E': sprintf(buf,"�d");break;
	case    'F': sprintf(buf,"�e");break;
	case    'G': sprintf(buf,"�f");break;
	case    'H': sprintf(buf,"�g");break;
	case    'I': sprintf(buf,"�h");break;
	case    'J': sprintf(buf,"�i");break;
	case    'K': sprintf(buf,"�j");break;
	case    'L': sprintf(buf,"�k");break;
	case    'M': sprintf(buf,"�l");break;
	case    'N': sprintf(buf,"�m");break;
	case    'O': sprintf(buf,"�n");break;
	case    'P': sprintf(buf,"�o");break;
	case    'Q': sprintf(buf,"�p");break;
	case    'R': sprintf(buf,"�q");break;
	case    'S': sprintf(buf,"�r");break;
	case    'T': sprintf(buf,"�s");break;
	case    'U': sprintf(buf,"�t");break;
	case    'V': sprintf(buf,"�u");break;
	case    'W': sprintf(buf,"�v");break;
	case    'X': sprintf(buf,"�w");break;
	case    'Y': sprintf(buf,"�x");break;
	case    'Z': sprintf(buf,"�y");break;
	case    '[': sprintf(buf,"�m");break;
	case    '\\':sprintf(buf,"��");break;
	case    ']': sprintf(buf,"�n");break;
	case    '^': sprintf(buf,"�O");break;
	case    '_': sprintf(buf,"�Q");break;
	case    '`': sprintf(buf,"�e");break;
	case    'a': sprintf(buf,"��");break;
	case    'b': sprintf(buf,"��");break;
	case    'c': sprintf(buf,"��");break;
	case    'd': sprintf(buf,"��");break;
	case    'e': sprintf(buf,"��");break;
	case    'f': sprintf(buf,"��");break;
	case    'g': sprintf(buf,"��");break;
	case    'h': sprintf(buf,"��");break;
	case    'i': sprintf(buf,"��");break;
	case    'j': sprintf(buf,"��");break;
	case    'k': sprintf(buf,"��");break;
	case    'l': sprintf(buf,"��");break;
	case    'm': sprintf(buf,"��");break;
	case    'n': sprintf(buf,"��");break;
	case    'o': sprintf(buf,"��");break;
	case    'p': sprintf(buf,"��");break;
	case    'q': sprintf(buf,"��");break;
	case    'r': sprintf(buf,"��");break;
	case    's': sprintf(buf,"��");break;
	case    't': sprintf(buf,"��");break;
	case    'u': sprintf(buf,"��");break;
	case    'v': sprintf(buf,"��");break;
	case    'w': sprintf(buf,"��");break;
	case    'x': sprintf(buf,"��");break;
	case    'y': sprintf(buf,"��");break;
	case    'z': sprintf(buf,"��");break;
	case    '{': sprintf(buf,"�o");break;
	case    '|': sprintf(buf,"�b");break;
	case    '}': sprintf(buf,"�p");break;
	default:
		return;
	}

}


void CMsgManager::Printf(Sint32 x, Sint32 y , Sint32 sPrio , Sint32 sCol , char* pMsg)
{
	//---------------------------------------------
	//�����\��
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


