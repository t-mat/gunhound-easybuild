//----------------------------------------
//CSV操作クラス
//CCsv.cpp
//----------------------------------------

#include <gxLib.h>
#include "CCsv.h"

//------------------------------------------------------------------------
CCsv::CCsv()
{
	//コンストラクタ
	rangeX = -1;
	rangeY = -1;
	temporaryBuf = new gxChar[TEMP_SIZE_MAX];

	//初期化
	for(Sint32 y=0;y<CSV_MAX_Y;y++)
	{
		for(Sint32 x=0;x<CSV_MAX_X;x++)
		{
			pCell[y][x] = NULL;
		}
	}
	m_bIgnoreComment = gxFalse;
	m_bCommentActive = gxFalse;
}


CCsv::~CCsv()
{
	//全てのセルを解放する

	for(Sint32 y=0;y<CSV_MAX_Y;y++)
	{
		for(Sint32 x=0;x<CSV_MAX_X;x++)
		{
			DelCell(x,y);
		}
	}

	delete[] temporaryBuf;
}


gxBool CCsv::DelCell( Sint32 x,Sint32 y )
{
	//セルバッファを削除する

	if(pCell[y][x] != NULL)	delete[] pCell[y][x];

	pCell[y][x] = NULL;

	return true;
}

gxBool CCsv::SetCell(Sint32 x,Sint32 y,gxChar *msg,...)
{
	//新しくセルにデータを書き込む

	va_list app;
	va_start(app, msg);
	vsprintf(temporaryBuf, msg, app);
	va_end(app);

	if(checkRange(x,y)==false) return gxFalse;

	DelCell(x,y);
	gxChar *p = setNewCell(x,y,strlen(temporaryBuf));
	sprintf(p,"%s",temporaryBuf);

	return true;
}

gxChar* CCsv::GetCell(Sint32 x,Sint32 y)
{
	//セルのデータを取得する

	if(pCell[y][x] == NULL)
	{	//空白を返す
		temporaryBuf[0] = CSV_DATA_NONE;
	}else{
		//データ内容をコピーして返す
		sprintf(temporaryBuf,"%s",pCell[y][x]);
		IsIgnore(temporaryBuf);
	}
	return temporaryBuf;
}

void CCsv::GetCell( Sint32 x,Sint32 y,gxChar *buf,Uint32 sz )
{
	//セルのデータを取得する

	if(pCell[y][x] == NULL)
	{	//空白を返す
		buf[0] = CSV_DATA_NONE;//temporaryBuf[0] = CSV_DATA_NONE;
	}else{
		//データ内容をコピーして返す
		sprintf(temporaryBuf,"%s",pCell[y][x]);
		IsIgnore(temporaryBuf);
		gxUtil::MemCpy(buf,temporaryBuf,sz);
	}

	return;
}

gxBool CCsv::IsIgnore(gxChar *p)
{
	//コメントを無視するか？

	if(m_bIgnoreComment == gxFalse) return gxFalse;

	for(Uint32 i=0;i<strlen(p);i++) {
		if(strlen(p)==1) break;
		if(p[i]=='/' && p[i+1]=='/')
		{
			p[i] = CSV_DATA_NONE;		//コメントなので無視する
			return true;
		}
	}

	return gxFalse;
}

Sint32 CCsv::COUNTA( Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 )
{
	//指定範囲のデータ数を数える
	Sint32 cnt = 0;
	gxChar *p;

	for( Sint32 y=y1;y<=y2;y++ )
	{
		if( y2 >= GetHeight() ) continue;

		for( Sint32 x=x1;x<=x2;x++ )
		{
			if( x >= GetWidth() ) continue;

			p = GetCell( x , y );

			if(p[0] == CSV_DATA_NONE) continue;

			cnt ++;
		}
	}

	return cnt;
}


gxBool CCsv::LoadFile(gxChar *filename,gxBool cmtActive)
{
	//ＣＳＶファイルをロードする

	m_bCommentActive = cmtActive;

	Uint32 sz;
	Uint8* p = gxLib::LoadFile( filename , &sz );

	if( p == NULL ) return gxFalse;

	Uint8 *pCsv = new Uint8[sz + 1024];

	gxUtil::MemCpy( pCsv , p , sz );

	delete[] p;

	analysingCsv( (gxChar*)pCsv , sz );

	delete[] pCsv;

	return gxTrue;
}


gxBool CCsv::ReadFile( Uint8 *pMem , Uint32 sz , gxBool cmtActive)
{
	//------------------------------------------------
	//ＣＳＶファイルをロードする
	//------------------------------------------------

	Uint8 *p = new Uint8[ sz + 1024 ];

	gxUtil::MemCpy( p , pMem , sz );

	analysingCsv( (gxChar*)p , sz );

	delete[] p;

	return gxTrue;

}


gxBool CCsv::analysingCsv( gxChar *p,Uint32 sz)
{
	//ロードしたＣＳＶファイルを解析する

	Sint32 xmax=0,xcnt=0,ymax=0;
	gxChar buf[TEMP_SIZE_MAX];
	Sint32 start=0;
	gxBool bCmt=false;

	CleanTab();

	Uint8 word;

	for( Sint32 i=0;i<=sz;i++)
	{
		word = p[i];

		if (i == sz - 1 && (start - i < 0))
		{
			//最後に改行がなかった場合
			p[i+1] = '\n';
		}
		switch(word){
		//区切りを発見
		case ',':
			if(bCmt) continue;
		case '\n':
		case '\r':
			if( i - start > 0)
			{
				//データを発見
				gxUtil::MemCpy(buf, &p[start], i - start);
				buf[i - start] = 0x00;
				SetCell(xcnt, ymax, "%s", buf);
			}else{
				//データなし
				SetCell(xcnt, ymax, "");
			}
			start = i+1;
			xcnt++;
			if(xcnt >= xmax) xmax = xcnt;

			switch(p[i+1]){
			case '\r':
				i++;
			case '\n':
				i++;
				xcnt=0;
				ymax++;
				bCmt = gxFalse;
				break;
			default:
				if(p[i] == '\n')
				{
					xcnt=0;
					ymax++;
					bCmt = gxFalse;
				}
				break;
			}
			start = i+1;
			break;
		case '/':
			if(p[i+1]=='/')
			{	//以降を無効とする
				if(m_bCommentActive) bCmt = true;
			}
			break;
		}
	}

	rangeX = xmax;
	rangeY = ymax;

	return true;
}

gxBool CCsv::SaveFile( gxChar* pFileName )
{
	//ＣＳＶファイルとして保存する

	gxChar *pStrBuf     = NULL;
	gxChar *pSaveBuf    = NULL;
	gxChar *pOldSave    = NULL;

	Uint32 uOldSize = 0;

	pStrBuf = new gxChar[ TEMP_SIZE_MAX*CSV_MAX_X ];

	pStrBuf[0] = 0x00;

	Uint32 uLength = 0;

	for(Sint32 y=0;y<GetHeight();y++)
	{
		pStrBuf[0] = 0x00;

		for(Sint32 x=0;x<GetWidth();x++)
		{
			if (x == 0)
			{
				sprintf(pStrBuf, "%s", GetCell(x, y));
			}
			else
			{
				sprintf(pStrBuf, "%s,%s", pStrBuf, GetCell(x, y));
			}
		}

		sprintf( pStrBuf , "%s,\r\n" , pStrBuf );

		if( pStrBuf[0] == 0x00 )
		{
			continue;
		}

		pOldSave = pSaveBuf;
		uOldSize = uLength;

		uLength += strlen( pStrBuf );

		pSaveBuf = new gxChar[ uLength ];

		gxUtil::MemCpy( pSaveBuf , pOldSave , uOldSize );
		gxUtil::MemCpy( &pSaveBuf[ uOldSize ] , pStrBuf , uLength - uOldSize );

		delete pOldSave;
		pOldSave = NULL;
	}

	delete pStrBuf;

	gxLib::SaveFile( pFileName , (Uint8*)pSaveBuf , uLength );

	delete pOldSave;

	return gxTrue;
}


gxBool CCsv::SearchWord(gxChar *msg,Sint32 &x,Sint32 &y)
{
	//特定の文字列を探す
	gxChar buf[255];

	for(Sint32 iy=0;iy<rangeY;iy++)
	{
		for(Sint32 ix=0;ix<rangeX;ix++)
		{
			sprintf( buf,"%s",GetCell(ix,iy) );
			long l1 = strlen( buf );
			long l2 = strlen(msg);
			long len = (l1>l2)? l1 : l2;
			if(strncmp(buf,msg,len)==0)
			{
				x = ix;
				y = iy;
				return true;
			}
		}
	}

	return gxFalse;
}

gxBool CCsv::CleanTab()
{
	//空白とかを破棄する

	gxChar *p = new gxChar[ TEMP_SIZE_MAX ];

	Sint32 len,cnt,add=0;

	for(Sint32 y=0;y<GetHeight();y++)
	{
		for(Sint32 x=0;x<GetWidth();x++)
		{

			//破棄します
			sprintf(p,"%s",GetCell(x,y));
			len = strlen(p);
			cnt = 0;
			add = 0;

			while(len>0)
			{
				switch(p[cnt]){
				case '\t':	//TAB発見
//					sprintf(&p[cnt],"%s",&GetCell(x,y)[cnt+1]);
					break;
				default:
					p[add] = GetCell(x,y)[cnt];
					add++;
					break;
				}
				cnt ++;
				len --;
			}
			p[add] = 0x00;
			SetCell(x,y,p);

		}
	}

	delete[] p;

	return gxTrue;

}

//------------------------ ここまで ↑↑↑ -------------------------------

void CommaSeparatedValue()
{
	//テスト用

	CCsv *pCsv = new CCsv();

	pCsv->LoadFile("test.txt");
	pCsv->SetCell(0,0,"test.txt");
	pCsv->SetCell(3,5,"testMessage2");
	pCsv->SaveFile("test2.txt");

	delete pCsv;

}


