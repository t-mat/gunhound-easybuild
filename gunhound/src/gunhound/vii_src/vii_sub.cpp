//--------------------------------------------------------
//
// サブルーチン集
//
//--------------------------------------------------------
#include <gunvalken.h>
#include "Effect/EffectBase.h"
#include <gxLib/Util/gxImage/CGXImage.h>

#ifdef NO_SOUND
//#define _VII_SOUND_OFF_
#endif

viiMus* viiMus::s_pInstance = NULL;
Sint32 viiMus::m_sLoopSoundCnt[enSoundMax];
Float32 viiMus::m_fFadeSoundAdd[enSoundMax];
Float32 viiMus::m_fFadeSoundAvg[enSoundMax];

//--------------------------------------------
//-----------------------------------------------------
void viiDraw::Sprdmg(gxBool damage,gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr,unsigned long col,float scl,Sint32 rot)
{
	//--------------------------------------------
	//ダメージエフェクトを考慮したスプライト表示
	//--------------------------------------------

	viiDraw::Sprite(p,x,y,prio,atr,col,scl,rot);
	if(!damage) return;

	for(Sint32 i=0;i<3;i++){
		viiDraw::Sprite(p,x,y,prio,atr|ATR_ALPHA_PLUS,0xffffffff,scl,rot);
	}
}


void viiDraw::Spr_dmg_NoScrl(gxBool damage,gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr,unsigned long col,float scl,Sint32 rot)
{
	//--------------------------------------------
	//ダメージエフェクトを考慮したスプライト表示
	//--------------------------------------------

	viiDraw::Spr_NoScrl(p,x,y,prio,atr,col,scl,rot);
	if(!damage) return;

	for(Sint32 i=0;i<3;i++){
		viiDraw::Spr_NoScrl(p,x,y,prio,atr|ATR_ALPHA_PLUS,0xffffffff,scl,rot);
	}
}


void viiDraw::Sprite(gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr,unsigned long col,float scl,Sint32 rot)
{
	//--------------------------------------------
	//スクロールを考慮したスプライトを描画する
	//--------------------------------------------
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	x = x/100 - sx;
	y = y/100 - sy;

	viiDraw::Spr_NoScrl(	p,x*100,y*100,prio,atr,col,scl,rot	);
}

void viiDraw::Spr_NoScrl(gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr,unsigned long col,float scl,Sint32 rot)
{
	//--------------------------------------------
	//スクロールを考慮しないスプライトを描画する
	//--------------------------------------------
	if( pGame && pGame->QuakeFlag ) y += (pGame->GetGameTimer()%2)*400;

	PutSpriteDX(
		x/100,y/100,prio,
		p->page,p->u,p->v,p->w,p->h,
		//オプション
		p->cx,p->cy,
		atr,
		col,
		(float)rot,scl,scl
		);

}



void viiDraw::BoxScrl_s(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2,Sint32 z,gxBool flag,Sint32 col,Sint32 atr)
{
	//--------------------------------------------
	//ボックス描画のラッパー
	//--------------------------------------------
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	x1 = x1 - sx;
	y1 = y1 - sy;

	x2 = x2 - sx;
	y2 = y2 - sy;

	GGX_DrawBox(x1,y1,x2,y2,z,flag,col,atr);
}


void viiDraw::Box(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2,Sint32 z,gxBool flag,Sint32 col,Sint32 atr)
{
	//--------------------------------------------
	//ボックス描画のラッパー
	//--------------------------------------------
	GGX_DrawBox(x1,y1,x2,y2,z,flag,col,atr);
}

void viiDraw::Pixel(Sint32 x1,Sint32 y1,Sint32 z,Sint32 col,Sint32 atr)
{
	//--------------------------------------------
	//ボックス描画のラッパー
	//--------------------------------------------
	GGX_DrawPixel(x1,y1,z,col,atr);

}


void viiDraw::Grad(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2,Sint32 z,Sint32 atr,Sint32 col1,Sint32 col2,Sint32 col3,Sint32 col4)
{
	//--------------------------------------------
	//特殊グラデBOXF
	//--------------------------------------------
	GRA_POS_T pos[4];
	pos[0].x=x1*1.f;
	pos[0].y=y1*1.f;
	pos[0].z=z*1.f;
	pos[0].col=col1;

	pos[1].x=x2*1.f;
	pos[1].y=y1*1.f;
	pos[1].z=z*1.f;
	pos[1].col=col2;

	pos[2].x=x2*1.f;
	pos[2].y=y2*1.f;
	pos[2].z=z*1.f;
	pos[2].col=col3;

	pos[3].x=x1*1.f;
	pos[3].y=y2*1.f;
	pos[3].z=z*1.f;
	pos[3].col=col4;
	PutGraphics( GRA_BOXF,0,0,z,pos,ATR_DFLT);
}

void viiDraw::Line(int x1,int y1,int x2,int y2,int z,int atr,int col)
{

	GGX_DrawLine( x1 , y1 , x2 , y2 , z , col , atr );

}


void viiDraw::Line_s(int x1,int y1,int x2,int y2,int z,int atr,int col)
{
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	x1 = x1 - sx;
	y1 = y1 - sy;

	x2 = x2 - sx;
	y2 = y2 - sy;

	GGX_DrawLine( x1 , y1 , x2 , y2 , z , col , atr );

}

void viiDraw::printf(Sint32 x,Sint32 y , Sint32 sCol , char *p,...)
{
	//--------------------------------------
	//システム文字を表示する
	//--------------------------------------

	char msg[0xff];
	va_list app;

	va_start(app, p);
	vsprintf(msg,p,app);
	va_end(app);

	Sint32 xx,yy;

	for(Uint32 i=0;i<strlen(msg);i++)
	{
		yy = (msg[i])/32;
		xx = (msg[i])%32;

		PutSpriteDX(x+i*13,y,PRIO_GAMETOP-1,enTexPageSystemUI3,xx*8,yy*8,8,8,0,0,ATR_ALPHA_MINUS,0x40202020,0,1.5f,1.5f);
		PutSpriteDX(x+i*13,y,PRIO_GAMETOP,enTexPageSystemUI3,xx*8,yy*8,8,8,0,0,ATR_DFLT,sCol,0,1.25f,1.25f);
	}
}


void viiDraw::sml_printf(Sint32 x,Sint32 y , Sint32 sCol , char *p,...)
{
	//--------------------------------------
	//システム文字を表示する
	//--------------------------------------

	char msg[0xff];
	va_list app;

	va_start(app, p);
	vsprintf(msg,p,app);
	va_end(app);

	Sint32 xx,yy;

	for(Uint32 i=0;i<strlen(msg);i++)
	{
		yy = (msg[i])/32;
		xx = (msg[i])%32;

		PutSpriteDX(x+i*8,y,PRIO_GAMETOP,enTexPageSystemUI3,xx*8,yy*8,8,8,0,0,ATR_DFLT,sCol);
	}
}


void viiDraw::big_printf(Sint32 x,Sint32 y , Sint32 sCol , char *p,...)
{
	//--------------------------------------
	//システム文字を表示する
	//--------------------------------------

	char msg[0xff];
	va_list app;

	va_start(app, p);
	vsprintf(msg,p,app);
	va_end(app);

	Sint32 xx,yy,off=0;
	for(Uint32 i=0;i<strlen(msg);i++)
	{

		ghPos sPos;

//		n = msg[i]-'A';

		GetBigFont( msg[i], &sPos );

		xx = sPos.x;
		yy = sPos.y;

		if( msg[i] == '@' )
		{
			PutSpriteDX(off+x+i*8,y,109,enTexPageString,xx*32,yy*40,288,64,0,0,ATR_DFLT,sCol);
			off += 256;
			continue;
		}
		else if( msg[i] == ' ' )
		{
			continue;
		}

		PutSpriteDX(off+x+i*28,y,PRIO_GAMETOP,enTexPageString,xx*32,yy*40,32,40,0,0,ATR_DFLT,sCol);
	}
}

void viiDraw::big_printf2(Sint32 x,Sint32 y , Sint32 sPrio , Sint32 sCol , char *p,...)
{
	//--------------------------------------
	//システム文字を表示する
	//--------------------------------------

	char msg[0xff];
	va_list app;

	va_start(app, p);
	vsprintf(msg,p,app);
	va_end(app);

	Sint32 xx,yy,off=0;
	for(Uint32 i=0;i<strlen(msg);i++)
	{
		ghPos sPos;

		GetBigFont( msg[i], &sPos );

		xx = sPos.x;
		yy = sPos.y;

		if( msg[i] == ' ' )
		{
			continue;
		}

		PutSpriteDX(off+x+i*28,y,sPrio,enTexPageString,xx*32,yy*40,32,40,0,0,ATR_DFLT,sCol,0,1.f );//0.75f);
	}
}

//--------------------------------------------
void viiDraw::GetBigFont( Sint32 n ,ghPos *p )
{
	p->x = 0;
	p->y = 0;

	switch( n){
	case    0:	p->x = 0;p->y = 0;	break;
	case    '@':p->x = 7;p->y = 1;	break;
	case    'A':p->x = 0;p->y = 0;	break;
	case    'B':p->x = 1;p->y = 0;	break;
	case    'C':p->x = 2;p->y = 0;	break;
	case    'D':p->x = 3;p->y = 0;	break;
	case    'E':p->x = 4;p->y = 0;	break;
	case    'F':p->x = 5;p->y = 0;	break;
	case    'G':p->x = 6;p->y = 0;	break;
	case    'H':p->x = 7;p->y = 0;	break;
	case    'I':p->x = 8;p->y = 0;	break;
	case    'J':p->x = 9;p->y = 0;	break;
	case    'K':p->x =10;p->y = 0;	break;
	case    'L':p->x =11;p->y = 0;	break;
	case    'M':p->x =12;p->y = 0;	break;
	case    'N':p->x =13;p->y = 0;	break;
	case    'O':p->x =14;p->y = 0;	break;
	case    'P':p->x =15;p->y = 0;	break;

	case    'Q':p->x = 0;p->y = 1;	break;
	case    'R':p->x = 1;p->y = 1;	break;
	case    'S':p->x = 2;p->y = 1;	break;
	case    'T':p->x = 3;p->y = 1;	break;
	case    'U':p->x = 4;p->y = 1;	break;
	case    'V':p->x = 5;p->y = 1;	break;
	case    'W':p->x = 6;p->y = 1;	break;

	case    'X':p->x = 0;p->y = 2;	break;
	case    'Y':p->x = 1;p->y = 2;	break;
	case    'Z':p->x = 2;p->y = 2;	break;
	}
}

Sint32 viiMath::Dist(Sint32 x , Sint32 y)
{
	double dx = x;
	double dy = y;

	dx = dx/100;	//10000->100
	dy = dy/100;	//10000->100

	double d = sqrt( POW(dx) + POW(dy) );

	return s_cast<Sint32>(d * 100);
}


Sint32 viiMath::Dist(Sint32 x1 , Sint32 y1 ,Sint32 x2, Sint32 y2)
{
	return Dist(x1-x2 , y1-y2);
}



Sint32 viiMath::Cos100(Sint32 n)
{
	return (Sint32)(cos(DEG2RAD(n))*100);

}

Sint32 viiMath::Sin100(Sint32 n)
{
	return (Sint32)(sin(DEG2RAD(n))*100);

}

Float32 viiMath::Cos( Sint32 deg )
{
	return gxLib::Cos( deg );
}

Float32 viiMath::Sin( Sint32 deg )
{
	return gxLib::Sin( deg );

}


Sint32 viiMath::Atan2_100(Sint32 y,Sint32 x)
{
	return (Sint32)(gxLib::Atan(x,y)*100);
}


Float32 viiMath::Atan2d(Sint32 y,Sint32 x)
{
	return gxLib::Atan(x,y);
}



Float32 viiMath::Sqrt( Float32 val )
{
	return gxLib::Sqrt( val );
}

Sint32 viiMath::GetSyougen(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2)
{
	//象現を得る（my象現なので注意）
	//　　⑥
	//　⑤　⑦
	//④　＋　⑧(0)
	//　③　①
	//　　②

	Sint32 kaku = viiMath::Atan2_100(y2-y1,x2-x1)/100;

	while(kaku<0)
	{
		kaku += 360;
	}

	if((kaku>(360-22.5) && kaku<=360) || (kaku>=0 && kaku<=22.5)) return 0;

	if( (kaku>(22.5+45*0)) && (kaku<(22.5+45*1)) ) return 1;
	if( (kaku>(22.5+45*1)) && (kaku<(22.5+45*2)) ) return 2;
	if( (kaku>(22.5+45*2)) && (kaku<(22.5+45*3)) ) return 3;
	if( (kaku>(22.5+45*3)) && (kaku<(22.5+45*4)) ) return 4;
	if( (kaku>(22.5+45*4)) && (kaku<(22.5+45*5)) ) return 5;
	if( (kaku>(22.5+45*5)) && (kaku<(22.5+45*6)) ) return 6;
	if( (kaku>(22.5+45*6)) && (kaku<(22.5+45*7)) ) return 7;

	return 0;
}

Sint32 viiMath::GetTargetAngle(Sint32 angle,Sint32 target_angle)
{
	//自分の向いている方向に対して
	//ターゲット角度を－１８０～０～１８０の角度で返す
	Sint32 sabun;

	NORMALIZE(angle);
	NORMALIZE(target_angle);

	sabun = target_angle-angle;

	if(sabun<-180){
		sabun = 360+sabun;
	}else if(sabun>180){
		sabun = (180-(sabun-180))*-1;
	}

	return sabun;
}


Sint32 viiMath::LineLogic(Sint32 x0,Sint32 y0,Sint32 x1,Sint32 y1,Sint32 n,Sint32 &xx,Sint32 &yy)
{
	//--------------------------------------------
	//２点間をむすぶラインのロジック
	//xx,yyにn工程目の座標をいれて返す
	//戻り値は全工程数
	//--------------------------------------------

	Sint32 E,x,y,cnt=0;
	Sint32 dx,dy,sx,sy,i;

	sx = ( x1 > x0 ) ? 1 : -1;
	dx = ( x1 > x0 ) ? x1 - x0 : x0 - x1;
	sy = ( y1 > y0 ) ? 1 : -1;
	dy = ( y1 > y0 ) ? y1 - y0 : y0 - y1;

	x = x0;
	y = y0;

	// 傾きが1以下の場合

	if( dx >= dy )
	{
		cnt = dx;
		E = -dx;

		for( i = 0; i <= dx; i++ )
		{
			xx = x;
			yy = y;

			if(i==n) return cnt;

			//      Position( x, y, atr,col );
			x += sx;
			E += 2 * dy;

			if( E >= 0 )
			{
				y += sy;
				E -= 2 * dx;
			}
		}
	}
	else
	{
		// 傾きが1より大きい場合 
		cnt = dy;
		E = -dy;

		for( i = 0; i <= dy; i++ )
		{
			xx = x;
			yy = y;
			if(i==n) return cnt;
			//Position( x, y, atr,col );
			y += sy;
			E += 2 * dx;

			if( E >= 0 )
			{
				x += sx;
				E -= 2 * dy;
			}
		}
	}

	return cnt;
}


//-----------------------------------------------
//音楽関連
//-----------------------------------------------

void viiMus::Action()
{
	//ループ再生が必要なものは

	for(Sint32 ii=0;ii<enSoundMax;ii++)
	{
		if( m_sLoopSoundCnt[ii] > 0 )
		{
			m_sLoopSoundCnt[ii] --;
		}

		//フェード処理

		if( m_fFadeSoundAdd[ii] != 0.f )
		{
			m_fFadeSoundAvg[ii] -= m_fFadeSoundAdd[ii];

			if( m_fFadeSoundAvg[ii] < 0.f )
			{
				m_fFadeSoundAvg[ii] = ZERO;
				m_fFadeSoundAdd[ii] = ZERO;
				StopBGM(ii);
			}
			else
			{
				ghLib::SetSoundVolume( ii, m_fFadeSoundAvg[ii]  );
			}
		}
	}

}


void viiMus::PlayLoopSound(Sint32 n,Sint32 sFrm)
{

	if( GetInstance()->m_sLoopSoundCnt[n] == 0 )
	{
		ghLib::PlayWave(n);
		GetInstance()->m_sLoopSoundCnt[n] = sFrm;
	}

}


gxBool viiMus::ReadBGM(long no, Uint8 *pData , Uint32 uSize )
{
	//サウンドのロードのラッピング（デバッグ時のＢＧＭＯＦＦ用）
#ifdef _VII_SOUND_OFF_
	return gxFalse;
#endif

	m_fFadeSoundAdd[no] = ZERO;
	m_fFadeSoundAvg[no] = ZERO;

	ghLib::StopWave( no );
	ghLib::ReadWaveData( no , pData , uSize );

	return gxTrue;

}

Sint32 viiSub::LoadSound( Sint32 sPg , CGXImage *p,char* pName )
{
	Uint8 * pFile = NULL;
	Uint32 uSize;

    pFile = p->GetFile( pName , &uSize);
    ghLib::ReadWaveData( sPg,pFile , uSize);

	return 1;
}

void viiMus::PlayBGM( Sint32 no , Sint32 bgm_no)
{
	//サウンド再生のラッピング（デバッグ時のＢＧＭＯＦＦ用）
	if( !g_StGameInfo.m_bMusic ) return;

#ifdef _VII_SOUND_OFF_
	return;
#endif

	m_fFadeSoundAdd[no] = ZERO;
	ghLib::PlayWave( no , gxTrue , 0 , 0 );

}


void viiMus::StopBGM( Sint32 no , Sint32 sFrm )
{
	//サウンド再生のラッピング（デバッグ時のＢＧＭＯＦＦ用）

#ifdef _VII_SOUND_OFF_
	return;
#endif

	if( sFrm == 0 )
	{
		ghLib::StopWave( no , sFrm );
	}
	else
	{
		m_fFadeSoundAdd[no] = 1.f / sFrm;
		m_fFadeSoundAvg[no] = 1.f ;
	}

}


gxBool viiMus::IsPlayBGM( Sint32 no )
{
	//--------------------------------------
	//サウンド再生中か？？
	//--------------------------------------

	return ghLib::IsSoundPlay( no );

}


void viiMus::PlaySound(Sint32 n , gxBool bSilence)
{
#ifdef _VII_SOUND_OFF_
	return;
#endif

	if( !g_StGameInfo.m_bSound ) return;

	if( bSilence ) return;

	if( n == enSoundOutofRange )
	{
		ghLib::PlayWave(n,gxFalse,70,1);
	}
	else
	{
		ghLib::PlayWave(n,gxFalse,90,1);
	}
}


CEffect* viiEff::Set(Sint32 type,Sint32 x,Sint32 y,Sint32 *opt , gxSprite *pSpr )
{
	return pGame->pEffManager->set_effect(type,x,y,opt,pSpr);
}

void viiEff::SetBombSimple(Sint32 x,Sint32 y,Sint32 num,Sint32 dist)
{
	//--------------------------------------------
	//爆発エフェクトのラッパー（シンプルな爆発）
	//--------------------------------------------

	new CEffBomb(x,y,num,dist/2);

}

void viiEff::SetBombEffect(Sint32 x,Sint32 y,Sint32 num,Sint32 dist)
{
	//--------------------------------------------
	//爆発エフェクトのラッパー（ビッグボム付き）
	//--------------------------------------------

	EffectMakeBomb( x, y, 4 , 2 );
}


void viiEff::SetBombDeluxe(Sint32 x,Sint32 y,Sint32 num,Sint32 dist)
{
	//--------------------------------------------
	//爆発エフェクトのラッパー（破片付き）
	//--------------------------------------------

	EffectMakeBomb( x, y, 8 , 2/*dist*/ );
	EffectMakeBomb( x, y, 8 , 8/*dist*/ );

}


void viiEff::SetBrokenParts(Sint32 x,Sint32 y,Sint32 n,Sint32 spd)
{
	//------------------------------
	//標準的なパーツばら撒き関数
	//------------------------------

	for(Sint32 i=0;i<n;i++) {


		if(i%2==0)
		{
			viiEff::Set(EFF_BROKENPARTS,x+(-400+viiSub::Rand()%800),y+(-400+viiSub::Rand()%800),argset(18*(-5+viiSub::Rand()%10),spd));
		}
		else
		{
			viiEff::Set(EFF_BROKENPARTS,x+(-400+viiSub::Rand()%800),y+(-400+viiSub::Rand()%800),argset(18*(5+viiSub::Rand()%10),spd));
		}

	}

	viiEff::Set(EFF_BROKENPARTS,x+(-200+viiSub::Rand()%400),y+(-200+viiSub::Rand()%400),argset(-120+viiSub::Rand()%40,spd));
}


void viiDbg::printf(Sint32 x,Sint32 y,char *msg,...)
{
	//---------------------
	//デバッグ文字の表示
	//---------------------

#ifdef _VII_DEBUG_
	char buf[0xff];
	va_list app;

	va_start(app, msg);
	vsprintf(buf, msg, app);
	va_end(app);

	msg_printfwithCol(x,y,enMaxPriority-1,0xffffffff,&buf[0]);
#endif

}



void viiDbg::printc(Sint32 x,Sint32 y,Uint32 col,char *msg,...)
{
	//---------------------
	//デバッグ文字の表示
	//---------------------

#ifdef _VII_DEBUG_
	char buf[0xff];
	va_list app;

	va_start(app, msg);
	vsprintf(buf, msg, app);
	va_end(app);

	PutStringDX(x,y,enMaxPriority,buf,col);
#endif

}

void viiDbg::log(char *msg,...)
{

#ifdef _VII_DEBUG_
	char buf[0xff];
	va_list app;

	va_start(app, msg);
	vsprintf(buf, msg, app);
	va_end(app);

	dbg_printf(buf);
#endif

}

//------------------------------------------------------

Sint32 viiSub::vSyncTimer()
{
	//-----------------------------------------------
	//Pauseに関係なく進行するタイマー
	//-----------------------------------------------

	return gtimer_ex;
}

Sint32 viiSub::GameCounter()
{
	//-----------------------------------------------
	//ゲーム中だけ（ポーズの影響を受ける）タイマー
	//-----------------------------------------------

	if( pGame )
	{
		return pGame->GetGameTimer();
	}

	return vSyncTimer();

}

void viiSub::SetRumble(Sint32 time,Sint32 mag)
{

	ghLib::SetRumble(0,time,mag,mag);

}


Sint32 viiSub::Rand()
{
	Sint32 n = gxLib::Rand();

	return (n<0)? -n : n;
}

Sint32 viiSub::GetBoundBlock_Player(Sint32 x,Sint32 y)
{
	//-------------------------------------
	//バウンディングできる壁を判定する
	//-------------------------------------

	Sint32 type = pGame->pBg->get_kabetype(x/100,y/100);

	switch(type){
	case 1:
	case 12:
	case 13:
	case 14:
	case 15:
		//壁だった
		return type;
	}
	return 0;
}


Sint32 viiSub::GetBoundBlock_Enemy(Sint32 x,Sint32 y)
{
	//-------------------------------------
	//バウンディングできる壁を判定する
	//-------------------------------------
	Sint32 type = pGame->pBg->get_kabetype(x/100,y/100);

	switch(type){
	case 1:
//	case 3:
	case 12:
	case 13:
	case 14:
	case 15:
		//壁だった
		return type;
	}
	return 0;
}

gxBool viiSub::IsGroundBlock(Sint32 x , Sint32 y)
{
	if( pGame->pBg->get_kabetype(x/100,y/100) == enKabeTypeBlock )
	{
		return gxTrue;
	}

	return gxFalse;
}

void viiSub::OpenSecretWeapon()
{
	//隠し武器をオープンする
	//game_dat.secret_weapon = gxTrue;
}


Sint32 viiSub::GetRandColor(Sint32 alpha)
{
	//ランダムな色を返す

	Sint32 r = 128+viiSub::Rand()%128;
	Sint32 g = 128+viiSub::Rand()%128;
	Sint32 b = 128+viiSub::Rand()%128;

	return ARGB(alpha,r,g,b);
}


void viiSub::AddScore(Sint32 n)
{
	//スコア追加

	n = n*CCockpit::GetInstance()->GetScoreBairitsu();

	g_StGameInfo.AddScore( n );

	if( CCockpit::GetInstance() )
	{
		CCockpit::GetInstance()->SetScore( g_StGameInfo.m_uStageScore );
	}

}

void viiSub::SetGravitMove(Sint32 &x,Sint32 &y)
{
	//ゲーム内で強制的に付加される移動増分値を加える
	//必ず１００倍座標でもらうこと

	x += pGame->get_scroll_power_x();
	y += pGame->get_scroll_power_y();
}


void viiSub::GetScreenPosition(Sint32 &x,Sint32 &y)
{
	//スクロール座標を加味したポジションを返す

	Sint32 sx,sy;

	viiSub::GetScroll_s( sx,sy );

	x -= sx;
	y -= sy;
}

gxBool viiSub::IsScrollOut(Sint32 x,Sint32 y,Sint32 ofx,Sint32 ofy)
{
	//１点の座標について画面から出ているかどうかを判定する
	//ofx,ofyは、画面外許容範囲
	//座標は、100倍座標でもらう

	Sint32 sx,sy;

	x = x/100;
	y = y/100;
	ofx=ofx/100;
	ofy=ofy/100;

	viiSub::GetScroll_s( sx,sy );

	if(y >= sy-ofy && y <= sy+WINDOW_H+ofy){
		if(x >= sx-ofx && x <= sx+WINDOW_W+ofx){
			return gxFalse;
		}
	}

	return gxTrue;
}


void viiSub::GetIdealScroll_l(Sint32 &sx,Sint32 &sy)
{
	//-----------------------------------------
	//デフォルトのスクロール位置をセットする
	//-----------------------------------------
	Sint32 sDist = (50*WINDOW_H/100);	//60
	Sint32 sDist2 = (10*WINDOW_H/100);	//60
	Sint32 sBodyHeight = 2400;

#ifdef GUNHOUND
	sx = GetTargetPlayer()->x+pGame->GetHound()->GetDirection()*sDist*100-(WINDOW_W/2)*100;
	Sint32 yy  = viiMath::Sin100( pGame->GetHound()->GetArmRotation() )*sDist2-sBodyHeight;
	sy = GetTargetPlayer()->y + yy -(WINDOW_H/2)*100;

#else
	sx = GetTargetPlayer()->x+pGame->valken()->get_dir()*sDist*100-(WINDOW_W/2)*100;
	Sint32 yy =  viiMath::Sin100( pGame->valken()->get_armrot() )*sDist-sBodyHeight;
	sy = GetTargetPlayer()->y + yy -(WINDOW_H/2)*100;
#endif

	//特定ポジションより下は表示させない

	if( !pGame->GetDetectUnderLine() ) return;

	Sint32 ay = GetTargetPlayer()->y/800 - WINDOW_H/8;
	for(Sint32 ii=ay;ii<pGame->pBg->getHeight()/8;ii++)
	{
		Sint32 x,y;
		x = GetTargetPlayer()->x/100;
		y = ii*8;

		if( pGame->pBg->get_kabetype(x , y) == 16 )
		{
			if(sy >= (y-WINDOW_H)*100 ) sy = (y-WINDOW_H)*100;
			return;
		}
	}

}


void viiSub::AdjScroll_l(Sint32 sx , Sint32 sy)
{
	pGame->pBg->scroll_control( sx,sy );
}

void viiSub::GetScroll_s( Sint32 &x,Sint32 &y )
{
	//スクロール位置を取得する
	if(pGame==NULL){
		x = 0;
		y = 0;
		return;
	}
	pGame->pBg->getscroll(x,y);
}


void viiSub::SetScroll_l( Sint32 x,Sint32 y )
{
	pGame->pBg->setScroll100( x , y );
}



void viiSub::SetBgLayer( Sint32 n,Sint32 x,Sint32 y )
{
	pGame->pBg->SetPos( n , x , y );
}


void viiSub::SetBgDisp(Sint32 n,gxBool flag)
{
	pGame->pBg->set_disp(n,flag);
}

void viiSub::LimitMoveVH()
{
	//GV座標をスクロール範囲外にさせない

	if( pGame->GetHound()->GetTobidashi() )
	{
		return;
	}

	Sint32 sx,sy;

	viiSub::GetScroll_s( sx,sy );

	if(GetTargetPlayer()->y < (sy+40)*100)  SetTargetPlayerPosY( (sy+40)*100 );
	if(GetTargetPlayer()->y > (sy+WINDOW_H)*100) SetTargetPlayerPosY( (sy+WINDOW_H)*100 );
	if(GetTargetPlayer()->x > (sx+WINDOW_W)*100) SetTargetPlayerPosX( (sx+WINDOW_W)*100 );
	if(GetTargetPlayer()->x < (sx)*100) SetTargetPlayerPosX( (sx)*100 );
}

void viiSub::LimitMoveOnlyV()
{
	//GV座標をスクロール範囲外にさせない（Ｙ座標を考慮しない）

#ifdef GUNHOUND

#else
	if(pGame->valken()->getControl() == enControlMode_KICKSTART) return;	//キックスタート時は無視する
#endif

	if( pGame->GetHound()->GetTobidashi() )
	{
		return;
	}

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );
//	if(GetTargetPlayer()->y < (sy+40)*100)  SetTargetPlayerPosY( (sy+40)*100 );
//	if(GetTargetPlayer()->y > (sy+240)*100) SetTargetPlayerPosY( (sy+240)*100 );
	if(GetTargetPlayer()->x > (sx+WINDOW_W)*100) SetTargetPlayerPosX( (sx+WINDOW_W)*100 );
	if(GetTargetPlayer()->x < (sx)*100) SetTargetPlayerPosX( (sx)*100 );
}


void viiSub::MenuPrintf(Sint32 x,Sint32 y,Sint32 col,char *msg,...)
{
	//---------------------
	//メニュー用文字表示
	//---------------------

	char buf[0xff];
	va_list app;

	va_start(app, msg);
	vsprintf(buf, msg, app);
	va_end(app);

	msg_printfwithCol(x,y,250,col,&buf[0]);
}


void viiSub::MenuSprite(gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr,unsigned long col,float scl,Sint32 rot)
{
	//--------------------------------------------
	//スクロールを考慮しないスプライトを描画する
	//--------------------------------------------

	PutSpriteDX(
		x,y,prio,
		p->page,p->u,p->v,p->w,p->h,
		//オプション
		p->cx,p->cy,
		atr,
		col,
		(float)rot,scl,scl
		);

}


void viiSub::MenuSprite( int x,	int y, int prio,
                 int page, int u, int v, int w, int h, int cx, int cy,
                 int atr, unsigned long col,float rot, float scl_x,float scl_y )
{
	//--------------------------------------------
	//メニュー用のスプライトを描画する
	//--------------------------------------------

	PutSpriteDX(
		x,y,prio,
		page,u,v,w,h,
		//オプション
		cx,cy,
		atr, col, (float)rot,scl_x,scl_y	);

}


Sint32 viiSub::GetPadInputAngle( Sint32 sPad )
{
	//-------------------------------------------------
	//入力方向を返す。(0:真右)
	//-------------------------------------------------

	int j,dir=-1;

	j=sPad&(JOY_L|JOY_R|JOY_U|JOY_D);

	switch(j){
	case JOY_R:
		dir = 0;
		break;
	case JOY_D:
		dir = 90;
		break;
	case JOY_L:
		dir = 180;
		break;
	case JOY_U:
		dir = 270;
		break;

	case JOY_R|JOY_D:
		dir = 45;
		break;
	case JOY_D|JOY_L:
		dir = 135;
		break;
	case JOY_L|JOY_U:
		dir = 225;
		break;
	case JOY_U|JOY_R:
		dir = 315;
		break;
	default:
		return -1;
	}

	return dir;
}

Sint32 viiSub::SetKakudo(Sint32 sDirection ,Sint32 sKakudo)
{
	//--------------------------------------------
	//向きに応じた角度を返す
	//--------------------------------------------

	return (sDirection == DIR_RIGHT)? sKakudo : (-sKakudo+180);
}


Sint32 viiSub::GetMorphingColor( Sint32 per , Sint32 col1 , Sint32 col2 )
{
	//--------------------------------------------
	//色のモーフィング
	//--------------------------------------------

	Sint32 a1,r1,g1,b1;
	Sint32 a2,r2,g2,b2;
	Sint32 a3,r3,g3,b3;

	a1 = (col1&0xff000000)>>24;
	r1 = (col1&0x00ff0000)>>16;
	g1 = (col1&0x0000ff00)>>8;
	b1 = (col1&0x000000ff)>>0;

	a2 = (col2&0xff000000)>>24;
	r2 = (col2&0x00ff0000)>>16;
	g2 = (col2&0x0000ff00)>>8;
	b2 = (col2&0x000000ff)>>0;

	if( per >= 100) per = 100;
	if( per <= 0  ) per = 0;

	a3 = a1+per*(a2 -a1)/100;
	r3 = r1+per*(r2 -r1)/100;
	g3 = g1+per*(g2 -g1)/100;
	b3 = b1+per*(b2 -b1)/100;

	a3 = (a3<0)?    0    : a3;
	a3 = (a3>0xff)? 0xff : a3;

	r3 = (r3<0)?    0    : r3;
	r3 = (r3>0xff)? 0xff : r3;

	g3 = (g3<0)?    0    : g3;
	g3 = (g3>0xff)? 0xff : g3;

	b3 = (b3<0)?    0    : b3;
	b3 = (b3>0xff)? 0xff : b3;

	return ARGB(a3,r3,g3,b3);

}

Sint32 viiSub::LoadTexture(Sint32 sPg , CGXImage *p,char* pName , Sint32 sCol)
{


	Uint32 uFileSize;
	Uint8 * pFile = NULL;

	pFile = p->GetFile( pName , &uFileSize );

	ReadTexture( sPg,pFile,uFileSize,sCol);


	return 1;
}




void* GetTargetObjPosAddr()
{
#ifdef GUNHOUND
	return (void*)&pGame->GetHound()->m_Pos;

#else
	return (void*)&pGame->valken()->pos;
#endif

}


const ghPos* GetTargetPlayer()
{
	//ゲーム中のターゲットプレイヤーを返す

	static ghPos s_stPos;

#ifdef GUNHOUND
	s_stPos.x = pGame->GetHound()->m_Pos.x;
	s_stPos.y = pGame->GetHound()->m_Pos.y;

#else
	s_stPos.x = pGame->valken()->pos.x;
	s_stPos.y = pGame->valken()->pos.y;
#endif

	return &s_stPos;

}


void SetTargetPlayerPosX( Sint32 x )
{
	//ゲーム中のターゲットプレイヤーを返す
	pGame->GetHound()->m_Pos.x = x;
	return;

}


void SetTargetPlayerPosY( Sint32 y )
{
	//ゲーム中のターゲットプレイヤーを返す

	pGame->GetHound()->m_Pos.y = y;
	return;
}


Sint32* argset(Sint32 a1,Sint32 a2,Sint32 a3,Sint32 a4,Sint32 a5,Sint32 a6,Sint32 a7,Sint32 a8,Sint32 a9,Sint32 a10)
{
	//引数セット用

	_opt_[0] = a1;
	_opt_[1] = a2;
	_opt_[2] = a3;
	_opt_[3] = a4;
	_opt_[4] = a5;
	_opt_[5] = a6;
	_opt_[6] = a7;
	_opt_[7] = a8;
	_opt_[8] = a9;
	_opt_[9] = a10;
	return _opt_;
}

void PutSystemWord(Sint32 x,Sint32 y,char *p,...)
{
	//システム文字を表示する

	char msg[0xff];
	va_list app;

	va_start(app, p);
	vsprintf(msg,p,app);
	va_end(app);

	Sint32 xx,yy;

	for(Uint32 i=0;i<strlen(msg);i++)
	{
		yy = (msg[i]-0x20)/16;
		xx = (msg[i]-0x20)%16;

		PutSpriteDX(x+i*6,y,PRIO_SYSTEM,TPG_SYSTEM,xx*8,yy*8,8,8);
	}
}


void get_item(Sint32 type)
{
	//アイテムを取得
	switch( type ){
	case 0:	//POW
		break;
	case 1:	//WEAPON
		break;
	case 2:	//HP
		pGame->GetHound()->AddHitPoint(50);
		break;
	}

}

Sint32 get_valken_rot(Sint32 x,Sint32 y)
{
	//GVと自分の角度を得る
	Sint32 r = viiMath::Atan2_100( (GetTargetPlayer()->y-y),(GetTargetPlayer()->x-x) )/100;
	while(r<=0){
		r += 360;
	}

	return r;
}

Sint32 get_valken_dist(Sint32 x,Sint32 y)
{
	//GVと自分の距離を得る
	Sint32 yy = (GetTargetPlayer()->y - y);
	Sint32 xx = (GetTargetPlayer()->x - x);
	return viiMath::Dist( xx , yy );
}

#if 0
  RRGGBB
0 ------ 透明 
1 000000 黒 
2 000080 暗い青 
3 0000FF 青 
4 800000 暗い赤 
5 FF0000 赤 
6 800080 暗い紫 
7 FF00FF 紫 
8 808080 灰色 
9 00FF00 緑 
A 008000 暗い緑 
B 00FFFF 水色 
C 008080 暗い水色 
D FFFF00 黄色 
E 808000 暗い黄色 
F FFFFFF 白 
#endif

gxBool viiSub::MakeEnemy(Sint32 sID , Sint32 x , Sint32 y )
{
	//-------------------------------------------------------
	//
	//敵キャラクタを精製する
	//
	//-------------------------------------------------------

	switch( sID ){
	case enCEneH0413Fannel:
		new CEneH0413Fannel(x,y,0);
		return gxTrue;

	case enCEneH0002Commanch:
		new CEneH0002Commanch(x,y);
		//CEneH0002Commanch *p;
		//p = new CEneH0002Commanch( x , y );
		//p->SetTarget( (OBJ_POS_T*)GetTargetPlayer() );
		return gxTrue;

	case enCEneH0003Vorzoy:
		new CEneH0003Vorzoy(x,y);
		return gxTrue;

	case enCEneH0004Tachikoma:
		new CEneH0004Tachikoma(x,y);
		return gxTrue;

	case enCEneH0005TransHeli:
		new CEneH0005TransHeli(x,y);
		return gxTrue;

	case enCEneH0006UGTortika:
		new CEneH0006UGTortika(x,y);
		return gxTrue;

	case enCEneH0006UGTortika2:
		new CEneH0006UGTortika(x,y,gxTrue);
		return gxTrue;

	case enCEneH0007YusouHeli:
		new CEneH0007YusouHeli(x,y);
		return gxTrue;

	case enCEneH0101HeavyTank:
		new CEneH0101HeavyTank(x,y);
		return gxTrue;

	case enCEneH0102NightSpider:
		new CEneH0102NightSpider(x,y);
		return gxTrue;

	case enCEneH0103ClubGunner:
		new CEneH0103ClubGunner(x,y);
		return gxTrue;

	case enCEneH0104LightTank:
		new CEneH0104LightTank(x,y);
		return gxTrue;

	case enCEneH0105TankGuard:
		new CEneH0105TankGuard(x,y);
		return gxTrue;

	case enCEneH0106Yagura:
		new CEneH0106Yagura(x,y);
		return gxTrue;

	case enCEneH0107Walker:
		new CEneH0107Walker(x,y);
		return gxTrue;

	case enCEneH0108Hovered:
		new CEneH0108Hovered(x,y);
		return gxTrue;

	case enCEneH0201VorzoyFly:
		//船の前に落とすタイプ
		new CEneH0201VorzoyFly(x,y);
		return gxTrue;

	case enCEneH0201VorzoyFlyAfter:
		//船の後ろに落とすタイプ
		new CEneH0201VorzoyFly(x,y);
		return gxTrue;

	case enCEneH0202VorzoyWP:
		new CEneH0202VorzoyWP(x,y);
		return gxTrue;

	case enCEneH0203Serpent:
		new CEneH0203Serpent(x,y);
		return gxTrue;

	case enCEneH0204Grazzi:
		new CEneH0204Grazzi(x,y);
		return gxTrue;

	case enCEneH0206Jourikusensya:
		new CEneH0206Bee(x,y,0);
		return gxTrue;

	case enCEneH0207TransShips:
		new CEneH0207TransShips(x,y);
		return gxTrue;

	case enCEneH0301Gunpelly:
		new CEneH0301Gunpelly(x,y);
		return gxTrue;

	case enCEneH0008GunTank:
		new CEneH0008GunTank(x,y);
		return gxTrue;

	case enCEneH0303Valkylie:
		new CEneH0303Valkylie(x,y);
		return gxTrue;

	case enCEneH0304Bee:
		new CEneH0304Bee(x,y);
		return gxTrue;

	case enCEneH0305Missile:
		new CEneH0305Missile(x,y);
		return gxTrue;

	case enCEneH0401MovingCannon:
		new CEneH0401MovingCannon(x,y);
		return gxTrue;

	case enCEneH0008BloodSocker:
		new CEneH0008BloodSocker(x,y);
		return gxTrue;

	case enCEneH0403Tarantura:
		new CEneH0403Tarantura(x,y);
		return gxTrue;

	case enCEneH0404GravityStar:
		new CEneH0404GravityStar(x,y);
		return gxTrue;

	case enCEneH0406Carrymissile:
		new CEneH0406Carrymissile(x,y);
		return gxTrue;

	case enCEneH0407Nautilus:
		new CEneH0407Nautilus(x,y);
		return gxTrue;

	case enCEneH0408Powerdsuit:
		new CEneH0408Powerdsuit(x,y);
		return gxTrue;

	case enCEneH0409SlideCannon:
		new CEneH0409SlideCannon(x,y);
		return gxTrue;

	case enCEneH0410Wall:
		new CEneH0410Wall(x,y);
		return gxTrue;

	case enCEneH0411Stunbarrier:
		new CEneH0411Stunbarrier(x,y);
		return gxTrue;

	case enCEneH0501PlasmaMachine:
		new CEneH0501PlasmaMachine(x,y);
		return gxTrue;

	case enCEneH0422StunBarrierTate:
		new CEneH0412StunbarrierTate(x,y);
		return gxTrue;

	case enCEneH0503HoundFly:
		new CEneH0503HoundFly(x,y);
		return gxTrue;

	case 200:
	case 201:
	case 202:
	case 203:
	case 204:
	case 205:
	case 206:
	case 207:
	case 208:
	case 209:
	case 210:
	case 211:
	case 212:
	case 213:
	case 214:
	case 215:
	case 216:
	case 217:
	case 218:
	case 219:
	case 220:
		{
			CEneH0008BloodSocker *p;
			p = new CEneH0008BloodSocker(x,y);
			p->SetCustomIndex( sID-200 );
		}
		break;

	case enCEneEx0001Cannon:
		new CEneEx0001Cannon( x , y );
		return gxTrue;

	case enCEneEx0002Laser:
		new CEneEx0002Laser( x , y );
		return gxTrue;

	case enCEneEx0003ShotRight:
		new CEneEx0003Shot( x , y , 0);
		return gxTrue;

	case enCEneEx0003ShotLeft:
		new CEneEx0003Shot( x , y , 1 );
		return gxTrue;

	case enCEneEx0004Missile:
		new CEneEx0004Missile( x , y );
		return gxTrue;

	case enCEneEx0005Baz:
		new CEneEx0005Wii( x , y );
		return gxTrue;

	case enCEneEx0006Tank:
		new CEneEx0006Tank( x , y );
		return gxTrue;

	case enCEneEx0007Nozzle:
		new CEneEx0007Nozzle( x , y );
		return gxTrue;

	case enCEneEx0008BombRight:
		new CEneEx0008Bomb( x , y , 0 );
		return gxTrue;

	case enCEneEx0008BombLeft:
		new CEneEx0008Bomb( x , y ,1);
		return gxTrue;

	case enCEneEx0009Body:
		new CEneEx0009Body(x,y);
		return gxTrue;

	case enCEneEx0010Suihei:
		new CEneEx0010Suihei(x,y);
		return gxTrue;

	case enCEneEx0010SuiheiRight:
		{
			CEneEx0010Suihei *pCEneEx0010Suihei;
			pCEneEx0010Suihei = new CEneEx0010Suihei(x,y);
			pCEneEx0010Suihei->SetRight();
		}
		return gxTrue;

	default:
		return gxFalse;
	}

	return gxFalse;
}

