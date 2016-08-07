// --------------------------------------------------------
/*
	OpenGL による描画処理
*/
// --------------------------------------------------------


#include <gxLib.h>
#include <gxLib/gx.h>
#include <gxLib/hard/machine.h>
#include <gxLib/gxOrderManager.h>
#include <gxLib/gxTexManager.h>
#include <gxLib/gxRender.h>
#include <gxLib/gxPadManager.h>
#include "../win32cmn/win32cmn.h"
#include "glDraw.h"

//#pragma comment(lib,"glut32.lib")

SINGLETON_DECLARE_INSTANCE( CGLDraw )

CGLDraw::CGLDraw()
{
}


CGLDraw::~CGLDraw()
{
	m_Font.Finalize();
	wglMakeCurrent( g_pWindows->m_WinDC , 0 );
	wglDeleteContext( m_hRC );
}

gxBool CGLDraw::Init()
{
	int pixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // support window 
		PFD_SUPPORT_OPENGL |   // support OpenGL 
		PFD_DOUBLEBUFFER,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		24,                    // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		32,                    // 32-bit z-buffer 
		1,                     // no stencil buffer 
		1,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
	}; 

    pixelFormat = ChoosePixelFormat( g_pWindows->m_WinDC,&pfd );
    SetPixelFormat( g_pWindows->m_WinDC,pixelFormat,&pfd );

	m_hRC = wglCreateContext( g_pWindows->m_WinDC );
	wglMakeCurrent( g_pWindows->m_WinDC , m_hRC );

	glGenTextures( CTexManager::enMasterPageNum , m_sTexIndex );

	m_Font.Initialize( 16, NULL );

	resetDevice();

	//	行列
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	return gxTrue;
}

gxBool CGLDraw::resetDevice()
{
	Uint32 w = WINDOW_W;
	Uint32 h = WINDOW_H;

	w = g_pWindows->m_uScreenWidth;
	h = g_pWindows->m_uScreenHeight;

	glViewport(0,0, w,h );

	gluPerspective(45, (double)WINDOW_W / (double)WINDOW_H, 0.1, 100.0);
	Float32 z = sin( DEG2RAD(67.5) )*1.f;
	gluLookAt(0.0 , 0 , 5 , 0.0,0.0,0.0, 0.0, 1.0, 0.0);

	return gxTrue;

}

void CGLDraw::Action()
{

}

void CGLDraw::Draw()
{

	wglMakeCurrent( g_pWindows->m_WinDC , m_hRC );

	Float32 a = CRender::GetInstance()->getBgColor(0);
	Float32 r = CRender::GetInstance()->getBgColor(1);
	Float32 g = CRender::GetInstance()->getBgColor(2);
	Float32 b = CRender::GetInstance()->getBgColor(3);

	if( a )
	{
		glClearColor( r,g,b,1.f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	//　3D　→　2D
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D(-1.0,1.0,-1.0,1.0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// ----------------------------

	glEnable( GL_NORMALIZE );
	glEnable( GL_ALPHA_TEST );
	glEnable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	render();

	//glEnable(GL_ALPHA_TEST);
	//glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);

	glFlush();

	// 描画終了
//	hr = m_pDirect3DDevice->EndScene();


}


void CGLDraw::FlipScreen()
{
	// 画面更新

	SwapBuffers( g_pWindows->m_WinDC );
}


void CGLDraw::render()
{
	CCommandList* pCommand = NULL;
	Sint32 sCommandMax      = CRender::GetInstance()->GetCommandNum();
	StCustomVertex *pVertex = CRender::GetInstance()->GetVertex(0);

#if INDEXBUFFER_BIT == 32
	Uint32         *pIndex  = CRender::GetInstance()->GetIndexBuffer( 0 );
#else
	Uint16         *pIndex  = CRender::GetInstance()->GetIndexBuffer( 0 );
#endif

	Sint32 debug = 0;


#ifdef GX_DEBUG
	switch( glGetError() ){
	case GL_NO_ERROR:
		break;
	case GL_INVALID_ENUM:
		ASSERT( "aaa" );	//	エラー監視
		break;
	case GL_INVALID_VALUE:
		ASSERT( glGetError() == GL_NO_ERROR );	//	エラー監視
		break;
	case GL_INVALID_OPERATION:
		ASSERT( glGetError() == GL_NO_ERROR );	//	エラー監視
		break;
	case GL_STACK_OVERFLOW:
		ASSERT( glGetError() == GL_NO_ERROR );	//	エラー監視
		break;
	case GL_STACK_UNDERFLOW:
		ASSERT( glGetError() == GL_NO_ERROR );	//	エラー監視
		break;
	case GL_OUT_OF_MEMORY:
		ASSERT( glGetError() == GL_NO_ERROR );	//	エラー監視
		break;
	}
#endif

	if( debug )
	{
		Uint32 vtx_max = CRender::GetInstance()->GetVertexNum();
		Uint32 idx_max = CRender::GetInstance()->GetIndexNum();
		Uint32 ii;

		gxLib::DebugLog( "command数[%d]" , sCommandMax );

		gxLib::DebugLog( "vertex List" );

		for(ii=0;ii<vtx_max;ii++)
		{
			gxLib::DebugLog( "vtx[%d] = { %3.2f , %3.2f , %3.2f}" , ii , pVertex[ii].x , pVertex[ii].y, pVertex[ii].z );
		}

		gxLib::DebugLog( "index List" );

		for(ii=0;ii<idx_max;ii++)
		{
			gxLib::DebugLog( "idx[%d] = %d" , ii , pIndex[ii] );
		}
	}

	//コマンドバッファにしたがって描画を行う

	for(Sint32 n=0; n<sCommandMax; n++)
	{
		pCommand = CRender::GetInstance()->GetCommandList(n);
		Sint32 v_start = pCommand->arg[0];
		Sint32 v_num   = pCommand->arg[1];
		Sint32 i_start = pCommand->arg[2];
		Sint32 i_num   = pCommand->arg[3];

		if( debug )
		{
			gxLib::DebugLog("Primitive( [%d],%d,%d,%d,%d)", pCommand->eCommand , v_start,v_num,i_start,i_num );
		}

		switch( pCommand->eCommand ){
		case eCmdNoneTexture:
			//テクスチャをはずす
			glDisable(GL_TEXTURE_2D);
			break;

		case eCmdChgTexture:
			//テクスチャをつける
			glEnable(GL_TEXTURE_2D);
			glBindTexture( GL_TEXTURE_2D, m_sTexIndex[ pCommand->arg[0] ]);
			break;

		case eCmdChgAttributeAlphaNml:
			//ブレンディング(標準)
			glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
			break;

		case eCmdChgAttributeAlphaAdd:
			//ブレンディング(加算)
			glBlendFunc( GL_SRC_ALPHA , GL_ONE);
			break;

		case eCmdChgAttributeAlphaSub:
			//ブレンディング(減算)
			//glBlendFunc( GL_SRC_ALPHA , GL_ONE);
			//glBlendFunc( GL_ZERO , GL_ONE_MINUS_SRC_COLOR);
			glBlendFunc( GL_ZERO , GL_ONE_MINUS_SRC_ALPHA);
			break;

		case eCmdChgAttributeAlphaCrs:
			//ブレンディング(乗算)
			glBlendFunc( GL_ZERO , GL_SRC_COLOR);
			break;

		case eCmdChgAttributeAlphaRvs:
			//ブレンディング(反転)
			//glBlendFunc(GL_ZERO , GL_ONE_MINUS_SRC_COLOR);
			glBlendFunc( GL_ONE_MINUS_DST_COLOR , GL_ZERO );
			break;

		case eCmdChgAttributeAlphaXor:
			//XOR
			glBlendFunc( GL_ONE_MINUS_DST_COLOR , GL_ONE_MINUS_SRC_COLOR );
			break;

		case eCmdChgAttributeAlphaScr:
			//スクリーン乗算
			glBlendFunc( GL_ONE_MINUS_DST_COLOR , GL_ONE );
			break;

		case eCmdRenderPoint:
			//点の描画
			//m_pDirect3DDevice->DrawPrimitiveUP     ( D3DPT_POINTLIST, v_num, &pVertex[ v_start ], sizeof( StCustomVertex ) );
			// サポートされていないらしい
			//m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_POINTLIST, 0,v_start,v_num,i_start,v_num );//v_num );
			//m_pDirect3DDevice->DrawPrimitive( D3DPT_POINTLIST , v_start , v_num );
			{
				Float32 alp,red,grn,blu;
				for(Sint32 ii=0;ii<i_num;ii++)
				{
					glBegin ( GL_POINTS );
					alp = (1.f*((pVertex[ pIndex[i_start+ii] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );
					glVertex2f( pVertex[ pIndex[i_start+ii] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii] ].y/(WINDOW_H/2)+1.0f );
					glEnd();
				}
			}

			break;

		case eCmdRenderLineStrip:
			//連続線の描画
//			m_pDirect3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, pCommand->arg[1]-1, &pVertex[ pCommand->arg[0] ], sizeof( StCustomVertex ) );
//			m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP, i_start,v_start,v_num,i_start , v_num );
//			m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP, 0,v_start,v_num,i_start , 4 );
			{
				Float32 alp,red,grn,blu;
//				glEnable( GL_LINE_SMOOTH );
				glLineWidth( pCommand->opt );
				glBegin ( GL_LINE_STRIP );
				for(Sint32 ii=0;ii<i_num;ii++)
				{
					alp = (1.f*((pVertex[ pIndex[i_start+ii] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );

					glVertex2f( pVertex[ pIndex[i_start+ii] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii] ].y/(WINDOW_H/2)+1.0f );
				}
				glEnd();
			}
			break;

		case eCmdRenderLineNormal:
			//線の描画
//			m_pDirect3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, pCommand->arg[1], &pVertex[ pCommand->arg[0] ], sizeof( StCustomVertex ) );
//			m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_LINELIST,0,v_start,v_num,i_start,v_num/2 );
			{
				Float32 alp,red,grn,blu;
//				glEnable( GL_LINE_SMOOTH );
				for(Sint32 ii=0;ii<i_num;ii+=2)
				{
					glLineWidth( pCommand->opt );
					//glGetFloatv(GL_LINE_WIDTH , &pCommand->opt );
					
					glBegin ( GL_LINE_LOOP );
					alp = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );
					glVertex2f( pVertex[ pIndex[i_start+ii+0] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii+0] ].y/(WINDOW_H/2)+1.0f );

					alp = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );
					glVertex2f( pVertex[ pIndex[i_start+ii+1] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii+1] ].y/(WINDOW_H/2)+1.0f );

					glEnd();
				}
			}
			break;

		case eCmdRenderTriangle:
			//三角形の描画
//			m_pDirect3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 1/*pCommand->arg[1]*/, &pVertex[ pCommand->arg[0] ], sizeof( StCustomVertex ) );
//			m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,v_start,v_num,i_start,i_num/3 );
			{
				Float32 alp,red,grn,blu;
				//glEnable( GL_LINE_SMOOTH );
				for(Sint32 ii=0;ii<i_num;ii+=3)
				{
					glBegin ( GL_TRIANGLES );
					alp = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );
					glTexCoord2f(pVertex[pIndex[i_start + ii + 0]].u, 1.f - pVertex[pIndex[i_start + ii + 0]].v);
					glVertex2f( pVertex[ pIndex[i_start+ii+0] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii+0] ].y/(WINDOW_H/2)+1.0f );

					alp = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );
					glTexCoord2f(pVertex[pIndex[i_start + ii + 1]].u, 1.f - pVertex[pIndex[i_start + ii + 1]].v);
					glVertex2f( pVertex[ pIndex[i_start+ii+1] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii+1] ].y/(WINDOW_H/2)+1.0f );

					alp = (1.f*((pVertex[ pIndex[i_start+ii+2] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii+2] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii+2] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii+2] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );
					glTexCoord2f(pVertex[pIndex[i_start + ii + 2]].u, 1.f - pVertex[pIndex[i_start + ii + 2]].v);
					glVertex2f( pVertex[ pIndex[i_start+ii+2] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii+2] ].y/(WINDOW_H/2)+1.0f );

					glEnd();
				}
			}
			break;

		case eCmdRenderSquare:
			//スプライトの描画
//			m_pDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,v_start,v_num,i_start,i_num/3 );//v_num);
			{
				Float32 alp,red,grn,blu;

				glDisable( GL_LINE_SMOOTH );

				for(Sint32 ii=0;ii<i_num;ii+=3)
				{	//Indexed Array対応が必要
					glBegin ( GL_TRIANGLES );

					alp = (1.f*((pVertex[ pIndex[i_start+ii+2] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii+2] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii+2] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii+2] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );
					glTexCoord2f( pVertex[ pIndex[i_start+ii+2] ].u , 1.f-pVertex[ pIndex[i_start+ii+2] ].v );
					glVertex2f( pVertex[ pIndex[i_start+ii+2] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii+2] ].y/(WINDOW_H/2)+1.0f );
					if (debug)
					{
						gxLib::DebugLog("1::(%f,%f) [%f,%f]",
							pVertex[pIndex[i_start + ii + 2]].x,     -pVertex[pIndex[i_start + ii + 2]].y,
							pVertex[pIndex[i_start + ii + 2]].u, 1.f -pVertex[pIndex[i_start + ii + 2]].v );
					}
					alp = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii+1] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );
					glTexCoord2f( pVertex[ pIndex[i_start+ii+1] ].u , 1.f-pVertex[ pIndex[i_start+ii+1] ].v );
					glVertex2f( pVertex[ pIndex[i_start+ii+1] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii+1] ].y/(WINDOW_H/2)+1.0f );

					if (debug)
					{
						gxLib::DebugLog("2::(%f,%f) [%f,%f]",
							pVertex[pIndex[i_start + ii + 1]].x, pVertex[pIndex[i_start + ii + 1]].y,
							pVertex[pIndex[i_start + ii + 1]].u, pVertex[pIndex[i_start + ii + 1]].v);
					}

					alp = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>24)&0x000000ff)/0xff);
					red = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>16)&0x000000ff)/0xff);
					grn = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>8 )&0x000000ff)/0xff);
					blu = (1.f*((pVertex[ pIndex[i_start+ii+0] ].col>>0 )&0x000000ff)/0xff);
					glColor4f( red, grn, blu, alp );
					glTexCoord2f( pVertex[ pIndex[i_start+ii+0] ].u , 1.f-pVertex[ pIndex[i_start+ii+0] ].v );
					glVertex2f( pVertex[ pIndex[i_start+ii+0] ].x/(WINDOW_W/2)-1.0f ,  -pVertex[ pIndex[i_start+ii+0] ].y/(WINDOW_H/2)+1.0f );

					if (debug)
					{
						gxLib::DebugLog("3::(%f,%f) [%f,%f]",
							pVertex[pIndex[i_start + ii +0]].x, pVertex[pIndex[i_start + ii + 0]].y,
							pVertex[pIndex[i_start + ii +0]].u, pVertex[pIndex[i_start + ii + 0]].v);
					}
					glEnd();
				}
			}
			break;

		case eCmdRenderFont:
			//フォントの描画
			{
				m_Font.Draw(
					(const char*) pCommand->pString,
					pCommand->x, pCommand->y,
					(Uint8) pCommand->arg[0],
					(Uint8) pCommand->arg[1],
					(Uint8) pCommand->arg[2],
					(Uint8) pCommand->arg[3]
				);
/*
				RECT rect;
				wchar_t wPosS[0xff];
				mbstowcs( wPosS , (char*)pCommand->pString , 0xff );
				SetRect(&rect, pCommand->x, pCommand->y, WINDOW_W,WINDOW_H);
				m_pDirect3DFont->DrawTextW( NULL, wPosS, -1, &rect, DT_LEFT, D3DCOLOR_ARGB( (pCommand->arg[0]), (pCommand->arg[1]), (pCommand->arg[2]),(pCommand->arg[3])) );
*/
			}
			break;
		}
	}
}

void CGLDraw::ReadTexture( Sint32 n )
{
	Uint32 col = 0x0000FF00;
	Uint32 w, h;
	
	w = CTexManager::enMasterWidth;
	h = CTexManager::enMasterHeight;

	glBindTexture(GL_TEXTURE_2D, m_sTexIndex[n]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//----------------------------------------------------------------------
	//これをはさまないとうまくテクスチャが貼れないタイミングがある

//バイリニアフィルタリング法
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//ニアレストネイバー法
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//----------------------------------------------------------------------

	GLenum err = glGetError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, CTexManager::GetInstance()->GetAtlasTexture(n)->GetTexelImage() );

}



void CGLDraw::getVideoDeviceInfo( )
{
	//Deviceの性能をログに出力する

/*
	D3DCAPS9 d3dCaps;

	if( FAILED( m_pDirect3DDevice->GetDeviceCaps( &d3dCaps ) ) )
	{
		gxLib::DebugLog( "[warning] ビデオカードの情報が取得できません" );
		return;
	}

	if( d3dCaps.DeviceType == D3DDEVTYPE_HAL )
	{
		gxLib::DebugLog( "[info] ハードウェアでラスタライズ" );
	}
	else if( d3dCaps.DeviceType == D3DDEVTYPE_REF )
	{
		gxLib::DebugLog( "[info] ソフトウェアでラスタライズ" );
	}

	gxLib::DebugLog( "[info] テクスチャの最大サイズ %d x %d" , d3dCaps.MaxTextureWidth , d3dCaps.MaxTextureHeight );
	gxLib::DebugLog( "[info] Primitiveの最大数 %d" , d3dCaps.MaxPrimitiveCount );
	gxLib::DebugLog( "[info] Indexの最大数 %d" , d3dCaps.MaxVertexIndex );
*/

}

