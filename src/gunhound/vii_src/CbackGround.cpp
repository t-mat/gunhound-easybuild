//@
//@
//@
//@
//@

#include <gunvalken.h>
#include <gxLib/Util/gxImage/CGXImage.h>

const char kblock1[][8]={
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
};

const char kblock12[][8]={
	{0,0,0,0,0,0,0,9},
	{0,0,0,0,0,0,9,9},
	{0,0,0,0,0,9,9,9},
	{0,0,0,0,9,9,9,9},
	{0,0,0,9,9,9,9,9},
	{0,0,9,9,9,9,9,9},
	{0,9,9,9,9,9,9,9},
	{9,9,9,9,9,9,9,9},
};

const char kblock13[][8]={
	{3,0,0,0,0,0,0,0},
	{3,3,0,0,0,0,0,0},
	{3,3,3,0,0,0,0,0},
	{3,3,3,3,0,0,0,0},
	{3,3,3,3,3,0,0,0},
	{3,3,3,3,3,3,0,0},
	{3,3,3,3,3,3,3,0},
	{3,3,3,3,3,3,3,3},
};

const char kblock14[][8]={
	{5,5,5,5,5,5,5,5},
	{5,5,5,5,5,5,5,0},
	{5,5,5,5,5,5,0,0},
	{5,5,5,5,5,0,0,0},
	{5,5,5,5,0,0,0,0},
	{5,5,5,0,0,0,0,0},
	{5,5,0,0,0,0,0,0},
	{5,0,0,0,0,0,0,0},
};


const char kblock15[][8]={
	{7,7,7,7,7,7,7,7},
	{0,7,7,7,7,7,7,7},
	{0,0,7,7,7,7,7,1},
	{0,0,0,7,7,7,7,7},
	{0,0,0,0,7,7,7,7},
	{0,0,0,0,0,7,7,7},
	{0,0,0,0,0,0,7,7},
	{0,0,0,0,0,0,0,7},
};

const char kblock2[][8]={
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
};

CBackGround::CBackGround()
{
	Sint32 i;

	ResetFilter();

	m_bDebugTipDraw = gxFalse;

	if(DEBUG_ATARIMAP)
	{
#ifdef _VII_DEBUG_
	m_bDebugTipDraw = gxTrue;
#else
	m_bDebugTipDraw = gxFalse;
#endif
	}
	m_bDummy        = gxFalse;

	sColor[0] = 0x00000000;
	sColor[1] = 0x00000000;
	sHalfAvr = 0;
	SetSkyColor(0,0xff010101,0xff010101);

	scx_ = sx_ = 0;
	scy_ = sy_ = 0;
	offset_x = offset_y = 0;
	repeat_x_goal_ = repeat_x_start_ = 0;
	memset(&stVitmap,0x00,sizeof(StVitmap));

	//レイヤー情報の初期化
	for(i=0;i<enBgTypeMax;i++)
	{
		m_sChangePrio[i] = 0;
		memset(&stMapData[i],0x00,sizeof(StMapData));
		stMapData[i].disp = gxTrue;
	}

	stMapData[0].disp = gxFalse;	//スター面だけは消しておく

	//repeat_x_goal_ = 320;
	no_judge_flag = gxFalse;

	for(i=0;i<STAR_MAX;i++)
	{
		stStar[i].x = viiSub::Rand()%SCR_W;
		stStar[i].y = viiSub::Rand()%SCR_H;
		stStar[i].layer = 1+viiSub::Rand()%4;
	}

	m_sScreenX = SCR_W*30;
	m_sScreenY = SCR_H*6;

}


CBackGround::~CBackGround()
{
	//テクスチャを開放


}


void CBackGround::action()
{
	
}

void CBackGround::Draw()
{
	if( m_bDebugTipDraw )
	{
		debug_mode();
	}

	//背景色描画

	if( (sColor[0]&0xff000000) != 0x00000000 )
	{
		viiDraw::Grad(0,0  ,WINDOW_W,sHalfAvr	  , PRIO_BG0+1  ,ATR_DFLT,sColor[0],sColor[0],sColor[1],sColor[1] );
		viiDraw::Grad(0,sHalfAvr,WINDOW_W,WINDOW_H , PRIO_BG0+1  ,ATR_DFLT,sColor[1],sColor[1],sColor[1],sColor[1] );
	}

	disp(0);	//★
	disp(1);	//最奥
	disp(2);	//アサルト奥
	disp(3);	//アサルト手前
}


void CBackGround::scroll_control( Sint32 x,Sint32 y )
{
	//スクロール位置を滑らかにコントロールする

	x = x/100;
	y = y/100;

	Sint32 adx = (x-scx_)/10;
	Sint32 ady = (y-scy_)/10;

	if(adx<=-10) adx = -10;
	if(ady<=-10) ady = -10;
	if(adx>=10) adx = 10;
	if(ady>=10) ady = 10;

	scx_ += adx;
	scy_ += ady;

}


gxBool CBackGround::LoadVmpFile(char *filename)
{
	//-------------------------------------------------------------------
	//ＶＭＰファイルを読み込む
	//-------------------------------------------------------------------

	Uint32 uSize;
	Uint8 *pData = gxLib::LoadFile( filename, &uSize );

	if( pData == NULL )
	{
		//-------------------------------------------------------------------
		viiDbg::log("指定されたVMPファイルが存在しません。");
		//-------------------------------------------------------------------
		return gxFalse;
	}

	LoadVmpData( pData );

	delete[] ( pData );

	return gxTrue;
}


gxBool CBackGround::LoadVmpData(Uint8 *p)
{
	//-----------------------------------------------
	//メモリからVMPを読み込む
	//-----------------------------------------------

	memcpy(stVitmap.format,p,16);
	memcpy(stVitmap.uHantei,&p[16],VMP_DATA_SIZE);

	m_sScreenX = stVitmap.sWidth;
	m_sScreenY = stVitmap.sHeight;

	return gxTrue;
}


void CBackGround::LoadMapFile(Sint32 n,gxChar *filename)
{
	//-------------------------------------------------------------------
	//ＭＡＰファイルを読み込む
	//-------------------------------------------------------------------
	Sint32 i;
	gxChar name[255];
	gxChar name2[255];

	Uint32 uSize;
	Uint8 *pData = gxLib::LoadFile( filename , &uSize );

	if( pData == NULL )
	{
		//-------------------------------------------------------------------
		viiDbg::log("指定されたMAPファイルが存在しません。");
		//-------------------------------------------------------------------
		//return;
	}

	memcpy( &stMapData[n].tips , pData , sizeof(MapFileData_t) );

	gxUtil::GetPath(filename,name);

	Sint32 page = enTexPageBackGround;

	/*
	重要：：読み込ませる順番によって背景データが壊れる
	１、３、２の順で読む、とか１，２，３のあと、２だけ差し替えたりすると問題が発生するので注意！！！
	*/

	for(i=0;i<n;i++)
	{
		page += stMapData[i].tips.num;
	}
	stMapData[n].page = page;


	for(i=0;i<stMapData[n].tips.num;i++)
	{
		sprintf(name2,"%s\\%s_%003d.bmp",name,stMapData[n].tips.filename,i+1);
		LoadTexture(page+i,name2,0x0000ff00);
	}

}

void CBackGround::LoadMapData(Sint32 n, gxChar *filename)
{
	//-------------------------------------------------------------------
	//ＭＡＰファイルを読み込む（メモリから）
	//-------------------------------------------------------------------
	Sint32 i;
	char name[255];
	char name2[255];

	Uint32 uSize;
	Uint8 *pFile = CGXImage::GetInstance()->GetFile( filename , &uSize );

	memcpy( (char*)&stMapData[n].tips , pFile , sizeof(MapFileData_t) );

	gxUtil::GetPath(filename, name);

	Sint32 page = enTexPageBackGround;

	/*
	重要：：読み込ませる順番によって背景データが壊れる
	１、３、２の順で読む、とか１，２，３のあと、２だけ差し替えたりすると問題が発生するので注意！！！
	*/

	for(i=0;i<n;i++)
	{
		page += stMapData[i].tips.num;
	}
	stMapData[n].page = page;


	for(i=0;i<stMapData[n].tips.num;i++)
	{
		sprintf(name2,"%s\\%s_%003d.bmp",name,stMapData[n].tips.filename,i+1);

		CGXImage::GetInstance()->SetTexture( page+i , name2 );
	}

}

void CBackGround::star_screen()
{
	//スター面描画

	Sint32 px,py;
	for(Sint32 i=0;i<STAR_MAX;i++) {
		Sint32 sx = scx_/stStar[i].layer;
		Sint32 sy = scy_/stStar[i].layer;
		sx = sx%SCR_W;
		sy = sy%SCR_W;
		px = stStar[i].x-sx;
		py = stStar[i].y-sy;
		px = (SCR_W+px)%SCR_W;
		py = (SCR_H+py)%SCR_H;
		viiDraw::Pixel(px,py,10,ARGB_DFLT,ATR_DFLT);
	}
}


void CBackGround::disp(Sint32 n)
{
	//背景スプライトを描画する

	Sint32 prio = PRIO_DEBUG;
	short *q;

	q = (short*)&stMapData[n].tips.ptn[0];

	if(!stMapData[n].disp) return;

	if(n==0) star_screen();

	Sint32 px,py,no;
	Sint32 sx,sy,w=0,h=0;

	sx = stMapData[n].x;
	sy = stMapData[n].y;
	w = stMapData[n].tips.sWidth;
	h = stMapData[n].tips.sHeight;

	Sint32 ax = sx/MAP_BLOCK_SIZE;
	Sint32 ay = sy/MAP_BLOCK_SIZE;
	Sint32 bx = sx%MAP_BLOCK_SIZE;
	Sint32 by = sy%MAP_BLOCK_SIZE;

	switch(n){
	case 0:	prio = PRIO_BG0;	break;
	case 1:	prio = PRIO_BG1;	break;
	case 2:	prio = PRIO_BG2;	break;
	case 3:	prio = PRIO_BG3;	break;
	}

	if(m_sChangePrio[n])
	{
		prio = m_sChangePrio[n];
	}

	//地震の増分値
	Sint32 quakey = pGame->Quake();
	if(n==1) quakey = 0;


	if( stMapData[n].Raster.bEnable )
	{
		//ラスターの増分値
		Sint32 sCnt = (viiSub::GameCounter()*stMapData[n].Raster.sSpeed)%360;

		//ラスター処理つきの場合
		for(Sint32 y=-2;y<(WINDOW_H/MAP_BLOCK_SIZE)+2;y++)
		{
			Sint32 rr = ( (ay+y)*stMapData[n].Raster.sDetail)%360+sCnt;
			Sint32 rx = ( viiMath::Cos100(rr)*stMapData[n].Raster.sWidth )/100;

			for(Sint32 x=-2;x<(WINDOW_W/MAP_BLOCK_SIZE)+2;x++)
			{
				px = ax+x;
				py = ay+y;

				if(px<0 || py<0 || px>=w/MAP_BLOCK_SIZE || py>=h/MAP_BLOCK_SIZE) continue;

				no = q[getMapTipPos( n , px , py )];

				if( no )
				{
					PutSprMapParts(( x*MAP_BLOCK_SIZE-bx+rx ) , (quakey+y*MAP_BLOCK_SIZE-by) , prio,no,stMapData[n].page , m_bgFilterColor[n] , m_bgAtribute[n] );
				}
			}
		}
	}
	else
	{
		//ラスター処理なしの場合
		for(Sint32 y=-2;y<(WINDOW_H/MAP_BLOCK_SIZE)+2;y++)
		{
			for(Sint32 x=-2;x<(WINDOW_W/MAP_BLOCK_SIZE)+2;x++)
			{
				px = ax+x;
				py = ay+y;

				if(px<0 || py<0 || px>=w/MAP_BLOCK_SIZE || py>=h/MAP_BLOCK_SIZE) continue;

				no = q[getMapTipPos( n , px , py )];

				if( no )
				{
					PutSprMapParts(( x*MAP_BLOCK_SIZE-bx ) , (quakey+y*MAP_BLOCK_SIZE-by) , prio,no,stMapData[n].page , m_bgFilterColor[n] , m_bgAtribute[n] );
				}
			}
		}
	}

}


gxBool CBackGround::IsBgObj( Sint32 x, Sint32 y)
{
	//-----------------------------------------------------
	//指定座標に背景があるか？
	//-----------------------------------------------------
	Sint32 ax,ay,sx,sy,no;
	short *q;

	for(Sint32 n=1;n<=3;n++)
	{
		q = (short*)&stMapData[n].tips.ptn[0];

		sx = stMapData[n].x;
		sy = stMapData[n].y;

		ax = (sx+x)/MAP_BLOCK_SIZE;
		ay = (sy+y)/MAP_BLOCK_SIZE;

		if(ax<0 || ay<0 || ax>=stMapData[n].tips.sWidth/MAP_BLOCK_SIZE || ay>=stMapData[n].tips.sHeight/MAP_BLOCK_SIZE) continue;

		no = q[ getMapTipPos( n , ax , ay ) ];

		if( no )
		{
			return gxTrue;
		}
	}


	return gxFalse;
}


void CBackGround::PutSprMapParts(Sint32 x,Sint32 y,Sint32 prio,Sint32 no,Sint32 bg_no , Sint32 sFilter , Sint32 sAtr)
{
	//マップチップ描画

	Sint32 page = (no/(MAP256_BLOCK_MAX));
	Sint32 u,v;

	u = (no%(256/MAP_BLOCK_SIZE))*MAP_BLOCK_SIZE;
	v = ((no-(MAP256_BLOCK_MAX*page))/(256/MAP_BLOCK_SIZE))*MAP_BLOCK_SIZE;

	gxSprite spr;

	spr.page = bg_no+page;
	spr.u = u;
	spr.v = v;
	spr.w = MAP_BLOCK_SIZE;
	spr.h = MAP_BLOCK_SIZE;
	spr.cx = 0;
	spr.cy = 0;

	if( sAtr == ATR_ALPHA_PLUS )
	{
		viiDraw::Spr_NoScrl(&spr,x*100,y*100,prio,ATR_DFLT,0xffffffff );
		viiDraw::Spr_NoScrl(&spr,x*100,y*100,prio,ATR_ALPHA_PLUS,0xffffffff );
	}

	viiDraw::Spr_NoScrl(&spr,x*100,y*100,prio,sAtr,sFilter );
}


INLINE void CBackGround::pos_cnv(Sint32 &x,Sint32 &y)
{
	//強制スクロール用などに場所データを変換する

	Sint32 dist = (repeat_x_goal_-repeat_x_start_);
	if(repeat_x_goal_ != repeat_x_start_ && dist!=0) {
		//繰り返し処理
		if(x > repeat_x_goal_){
			x = repeat_x_start_ + (x-repeat_x_start_)%dist;
		}
	}
	x = x;
	y = y;
}

Sint32 CBackGround::get_kabetype(Sint32 x,Sint32 y)
{
	//１ドット単位でのあたり判定を行う

	Sint32 no = get_kabetype8(x/8,y/8);

	if(no_judge_flag) return 0;

	if(no<12 || no>15) return no;

	//より正確なアタリを検証する
	char *p=NULL;

	switch(no){
	case 2:
		p = (char*)&kblock2[0][0];
		break;
	case 12:
		p = (char*)&kblock12[0][0];
		break;
	case 13:
		p = (char*)&kblock13[0][0];
		break;
	case 14:
		p = (char*)&kblock14[0][0];
		break;
	case 15:
		p = (char*)&kblock15[0][0];
		break;
	}

	x = x%8;
	y = y%8;
	if(p[y*8+x]==0) return 0;

	return no;
}

void CBackGround::SetSkyColor(Sint32 HalfAvr, Sint32 Col1,Sint32 Col2)
{
	//背景色を設定

	sColor[0] = Col1;
	sColor[1] = Col2;

	if( sHalfAvr < 0   ) sHalfAvr = 0;
	if( sHalfAvr > 100 ) sHalfAvr = 100;

	sHalfAvr = HalfAvr*WINDOW_H/100;
}


Sint32 CBackGround::get_kabetype8(Sint32 x,Sint32 y)
{
	//8x8単位のブロックでのあたり判定を行う
	x -= offset_x/8;
	y -= offset_y/8;

	pos_cnv(x,y);

	return stVitmap.uHantei[ getVmpPos(x,y) ];

}


void CBackGround::debug_mode()
{
	//デバッグ用ＢＧ描画
	Sint32 sx=0,sy=0;

	stMapData[0].x = scx_;
	stMapData[0].y = scy_;

	sx_ = scx_;
	sy_ = scy_;

	viiDbg::printf(10,32,"SX=%d,SY=%d",sx_,sy_);

	getscroll( sx,sy );

	sx = sx/8;
	sy = sy/8;

	for(Sint32 y=0;y<SCR_H/8+1;y++)
	{
		for(Sint32 x=0;x<SCR_W/8+1;x++)
		{
			debugMapDraw(x,y,get_kabetype8(sx+x,sy+y) );
		}
	}

}

void CBackGround::SetRaster( Sint32 sLayer , StRaster* p_stRaster )
{
	//-----------------------------------------
	//ラスターの設定
	//-----------------------------------------

	if( p_stRaster == NULL )
	{
		stMapData[sLayer].Raster.bEnable  =gxFalse;
		return;
	}
	stMapData[sLayer].Raster = *p_stRaster;
}


void CBackGround::debugMapDraw(Sint32 x,Sint32 y,Sint32 no)
{
	//デバッグ用壁描画
	Sint32 sx1,sy1;
	getscroll( sx1,sy1 );
	Sint32 col = 0x40;
	Sint32 xx=x*8-sx1%8;
	Sint32 yy=y*8-sy1%8;

	switch( no ){
	case enKabeTypeBlock:
//	case enKabeTypeBlock2:
	case enKabeTypeUnderGround:
		viiDraw::Box(xx,yy,xx+7,yy+7,PRIO_BG3+1,gxTrue,ARGB(col,0xff,0xff,0xff));
		break;
	case enKabeTypehalf:
		viiDraw::Box(xx,yy+2,xx+7,yy+4,PRIO_BG3+1,gxTrue,ARGB(col,0xff,0xff,0xff));
		break;
	case 4:
		GGX_DrawLine(xx,yy,xx+7,yy+7,PRIO_BG3+1,ARGB(col,0x00,0xff,0x00));
		break;
//	case 5:
//		GGX_DrawLine(xx+7,yy,xx,yy+7,PRIO_BG3+1,ARGB(col,0xff,0x00,0x00));
//		break;
	case enKabeTypeTriangleUL:
		GGX_DrawTriangle(xx,yy+7,xx+7,yy,xx+7,yy+7,PRIO_BG3+1,ARGB(col,0xff,0xff,0xff));
		break;

	case enKabeTypeTriangleUR:
		GGX_DrawTriangle(xx,yy+7,xx,yy,xx+7,yy+7,PRIO_BG3+1,ARGB(col,0xff,0xff,0xff));
		break;

	case enKabeTypeTriangleDR:
		GGX_DrawTriangle(xx,yy+7,xx,yy,xx+7,yy,PRIO_BG3+1,ARGB(col,0xff,0xff,0xff));
		break;

	case enKabeTypeTriangleDL:
		GGX_DrawTriangle(xx+7,yy,xx+7,yy+7,xx,yy,PRIO_BG3+1,ARGB(col,0xff,0xff,0xff));
		break;
	}
}

