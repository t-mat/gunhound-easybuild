// --------------------------------------------------------
/*
	DirectX 9.0c による描画処理
*/
// --------------------------------------------------------


#include <gxLib.h>
#include <gxLib/gx.h>
#include <gxLib/hard/machine.h>
#include <gxLib/gxOrderManager.h>
#include <gxLib/gxTexManager.h>
#include <gxLib/gxRender.h>
#include "../win32cmn/win32cmn.h"
#include "dxDraw.h"

#include <tchar.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

SINGLETON_DECLARE_INSTANCE( CDXDraw )

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//|D3DFVF_TEX1|D3DFVF_NORMAL

LPDIRECT3DVERTEXBUFFER9  m_pVB;
LPDIRECT3DINDEXBUFFER9   m_pIB;

Sint32 g_sScreenModeIndex = 2;
//Sint32 g_sWideScreen      = 0;
Sint32 g_sAntialiesMode   = D3DTEXF_NONE;

LPDIRECT3DSURFACE9 g_lpSurface = NULL;

/*
struct CUSTOMVERTEX {
    D3DXVECTOR3 position; // The position
	float       rhw;
	DWORD		col;
	float 		u,v;
};
D3DXVECTOR3 gTest[4];
*/

IDirect3DTexture9* g_pTexture;
UINT g_refreshRate = 0;

#define _USE_ANTIALIES

CDXDraw::CDXDraw()
{
	m_pDirect3D       = NULL;
	m_pDirect3DDevice = NULL;
	m_pDirect3DFont   = NULL;
	ZeroMemory( &m_pDirect3DPrePara , sizeof( D3DPRESENT_PARAMETERS ) );

	m_pVB = NULL;
	m_pIB = NULL;

	for(Uint32 ii=0;ii<CTexManager::enMasterPageNum; ii++)
	{
		m_D3dTexture[ii] = NULL;
	}

	m_pSurf = NULL;
	m_pDest = NULL;

	m_sScreenMode = 0;
	g_pTexture = NULL;

	m_RectDisp.top    = 0;
	m_RectDisp.left   = 0;
	m_RectDisp.right  = WINDOW_W;
	m_RectDisp.bottom = WINDOW_H;

}


CDXDraw::~CDXDraw()
{
	for(Uint32 ii=0;ii<CTexManager::enMasterPageNum; ii++)
	{
		SAFE_RELEASE( m_D3dTexture[ii] );
	}
	SAFE_RELEASE( g_pTexture );
	SAFE_RELEASE( m_pVB );
	SAFE_RELEASE( m_pIB );
	SAFE_RELEASE( m_pDirect3DFont );
	SAFE_RELEASE( m_pDirect3DDevice );
	SAFE_RELEASE( m_pDirect3D );

	SAFE_RELEASE( g_lpSurface );

}


gxBool CDXDraw::Init()
{
	if ( NULL == ( m_pDirect3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		gxLib::DebugLog( "[error] Direct3Dの初期化ができませんでした" );
		return gxFalse;
	}

	//	画面解像度取得
	bool isSuccess = false;

	HMONITOR hMonitor;
	MONITORINFO info = { sizeof( MONITORINFO ) };
	POINT pt   = { 1, 1 };
	hMonitor   = MonitorFromPoint( pt, MONITOR_DEFAULTTONEAREST );
	GetMonitorInfo( hMonitor, &info );
	RECT rect  = info.rcMonitor;
	int width  = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	//	画面解像度と同じのがあるはず
	int num = m_pDirect3D->GetAdapterModeCount( 0, D3DFMT_X8R8G8B8 );

	for ( int i = 0; i < num; ++i )
	{
		D3DDISPLAYMODE mode;
		m_pDirect3D->EnumAdapterModes( 0, D3DFMT_X8R8G8B8, i, &mode );

		if ( mode.Width == width && mode.Height == height )
		{
			g_refreshRate = mode.RefreshRate;

			if ( mode.RefreshRate == 60 )
			{
				isSuccess = true;
				break;
			}
		}
	}

	//	画面の設定が想定外に謎な時
	if ( !isSuccess )
	{
		width  = WINDOW_W;
		height = WINDOW_H;
		//return gxFalse;
	}

	m_pDirect3DPrePara.BackBufferWidth  = 320*((Sint32)(WINDOW_W/320)+1);
	m_pDirect3DPrePara.BackBufferHeight = 240*((Sint32)(WINDOW_H/240)+1);
	/* TODO: ここはディスプレイの解像度 */
	m_pDirect3DPrePara.BackBufferWidth  = width;
	m_pDirect3DPrePara.BackBufferHeight = height;
	m_pDirect3DPrePara.BackBufferFormat = D3DFMT_X8R8G8B8;//D3DFMT_UNKNOWN;
	m_pDirect3DPrePara.BackBufferCount  = 1;
	m_pDirect3DPrePara.SwapEffect       = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY ;//D3DSWAPEFFECT_COPY; //D3DSWAPEFFECT_FLIP 
	m_pDirect3DPrePara.hDeviceWindow    = g_pWindows->m_hWindow;
	m_pDirect3DPrePara.Windowed         = TRUE;
	m_pDirect3DPrePara.EnableAutoDepthStencil = TRUE;
	m_pDirect3DPrePara.AutoDepthStencilFormat = D3DFMT_D16;
	m_pDirect3DPrePara.Flags = 0;
	m_pDirect3DPrePara.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_pDirect3DPrePara.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//D3DPRESENT_INTERVAL_IMMEDIATE;


	m_pDirect3DPrePara.MultiSampleType  = D3DMULTISAMPLE_NONE;
	m_pDirect3DPrePara.MultiSampleQuality = 0;

	checkAntialias( D3DMULTISAMPLE_16_SAMPLES  );

	g_pWindows->m_bFullScreen = !m_pDirect3DPrePara.Windowed;

	resetDevice();

	return gxTrue;
}

void CDXDraw::SetFullScreenMode( gxBool bOn )
{
	m_pDirect3DPrePara.Windowed = !bOn;
}

//アンチエイリアシングが使用できるかチェックし、使用できるタイプをD3DPRESENT_PARAMETERS構造体に格納する。
HRESULT CDXDraw::checkAntialias( D3DMULTISAMPLE_TYPE AntialiasMode )
{
   HRESULT hr = E_FAIL;
   DWORD QualityBackBuffer = 0;
   DWORD QualityZBuffer = 0;
   DWORD m = (DWORD)AntialiasMode;

   while( m )
   {
      //レンダリングターゲットでアンチエイリアシングがサポートされているかをチェック
      hr = m_pDirect3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT,
                                                     D3DDEVTYPE_HAL,
                                                     m_pDirect3DPrePara.BackBufferFormat,
                                                     m_pDirect3DPrePara.Windowed,
                                                     (D3DMULTISAMPLE_TYPE)m,
                                                     &QualityBackBuffer );
      if( SUCCEEDED(hr) )
      { 
         //深度ステンシル サーフェイスでアンチエイリアシングがサポートされているかをチェック
         hr = m_pDirect3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT,
                                                        D3DDEVTYPE_HAL,
                                                        m_pDirect3DPrePara.AutoDepthStencilFormat,
                                                        m_pDirect3DPrePara.Windowed,
                                                        (D3DMULTISAMPLE_TYPE)m,
                                                        &QualityZBuffer );
         if( SUCCEEDED(hr) )
         {
            //アンチエイリアシングが使用できるのでD3DPRESENT_PARAMETERSにタイプをセットする。

            m_pDirect3DPrePara.MultiSampleType = (D3DMULTISAMPLE_TYPE)m;
            
            //QualityBackBufferとQualityZBufferで小さい方の値を有効にする。どんなパターンで値が返るのかわからんので、こうしたほうが安全かと。
            if( QualityBackBuffer < QualityZBuffer )
               m_pDirect3DPrePara.MultiSampleQuality = QualityBackBuffer - 1;
            else
               m_pDirect3DPrePara.MultiSampleQuality = QualityZBuffer - 1;

            break;
         }
      }
      
      //現在のタイプでアンチエイリアシングが使用できないので、１段階低いタイプで再チェックする。
      m--;
   }

   return hr;
}

gxBool CDXDraw::resetDevice()
{
	// デバイス生成
	if( m_pDirect3D == NULL ) return gxFalse;
//	if( m_pDirect3DDevice ) delete m_pDirect3DDevice;
//	SAFE_RELEASE( m_pDirect3DDevice );

	SAFE_RELEASE( m_pVB );
	SAFE_RELEASE( m_pIB );
	SAFE_RELEASE( m_pDirect3DFont );
	SAFE_RELEASE( g_pTexture );
	SAFE_RELEASE( g_lpSurface );

	if ( m_pDirect3DPrePara.Windowed ) {
		m_pDirect3DPrePara.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		m_RectDisp.top    = 0;
		m_RectDisp.left   = 0;
		m_RectDisp.right  = WINDOW_W;
		m_RectDisp.bottom = WINDOW_H;

	} else {
		m_pDirect3DPrePara.FullScreen_RefreshRateInHz = g_refreshRate;//D3DPRESENT_RATE_DEFAULT;
	}

	if ( m_pDirect3DDevice == NULL ) {

		HRESULT hr;
		////HARDWARET&L
		hr = m_pDirect3D->CreateDevice( D3DADAPTER_DEFAULT,
								D3DDEVTYPE_HAL,
								g_pWindows->m_hWindow,
								D3DCREATE_HARDWARE_VERTEXPROCESSING,
								&m_pDirect3DPrePara,
								&m_pDirect3DDevice);
		if(FAILED( hr ) )
		{
			switch( hr ){
			case D3DERR_INVALIDCALL:
				gxLib::DebugLog( "メソッドの呼び出しが無効である。たとえば、メソッドのパラメータに無効な値が設定されている場合などである。");
				break;
			case D3DERR_NOTAVAILABLE:
				gxLib::DebugLog( "このデバイスは、問い合わせたテクニックをサポートしていない。");
				break;
			case D3DERR_OUTOFVIDEOMEMORY:
				gxLib::DebugLog( "Direct3D が処理を行うのに十分なディスプレイ メモリがない。");
				break;
			}
			//SOFTWARET&L
			if(FAILED(m_pDirect3D->CreateDevice( D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									g_pWindows->m_hWindow,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&m_pDirect3DPrePara,
									&m_pDirect3DDevice)))
			{
				gxLib::DebugLog( "[error] Direct3DのCreateDeviceに失敗" );
				return gxFalse;
			}
		}
	} else {
		m_pDirect3DDevice->Reset( &m_pDirect3DPrePara );
	}

	m_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_NONE );
	m_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE  , FALSE );
    m_pDirect3DDevice->SetRenderState( D3DRS_LIGHTING , gxFalse );

	D3DXMATRIX		m_mtxProj;	//投影行列
	m_pDirect3DDevice->SetTransform( D3DTS_PROJECTION, &m_mtxProj );
	D3DXMatrixPerspectiveFovLH( &m_mtxProj, D3DX_PI/4, ((float)WINDOW_W)/WINDOW_H, 0.1f,500.f);
	m_pDirect3DDevice->SetTransform(
					(D3DTRANSFORMSTATETYPE)D3DTS_TEXTURE0,
					&D3DXMATRIX(0.5,0,0,0, 0,-0.5,0,0, 0,0,1,0, 0.5,0.5,0,1)
				);


	m_pDirect3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pDirect3DDevice->SetVertexShader( NULL );

	// ---------------------------------------
	HRESULT hr;

	//頂点バッファ作成
	hr = m_pDirect3DDevice->CreateVertexBuffer( sizeof(StCustomVertex)*enVertexMax, 0,/*D3DUSAGE_WRITEONLY, */
				D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL );

	if(FAILED(hr))
	{
		gxLib::DebugLog( "[error] 頂点バッファ作成失敗" );
		return gxFalse;
	}

	//インデックスバッファ作成
#if INDEXBUFFER_BIT == 32
	hr = m_pDirect3DDevice->CreateIndexBuffer( sizeof(Uint32)*enIndexMax, 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIB, NULL);
#else
	hr = m_pDirect3DDevice->CreateIndexBuffer( sizeof(Uint16)*enIndexMax, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);
#endif

	if(FAILED(hr))
	{
		gxLib::DebugLog( "[error] インデックスバッファ作成失敗" );
		return gxFalse;
	}

	// ---------------------------------------
	getVideoDeviceInfo();

	//フォント

	D3DXFONT_DESC logFont;

	logFont.Height			= DEBUG_FONT_SIZE;
	logFont.Width		  	= 0;
	logFont.Weight			= (false) * FW_NORMAL;
	logFont.MipLevels		= 0;
	logFont.Italic			= gxFalse;
	logFont.CharSet			= SHIFTJIS_CHARSET;
	logFont.OutputPrecision = OUT_DEFAULT_PRECIS;
	logFont.Quality			= CLIP_DEFAULT_PRECIS;
	logFont.PitchAndFamily	= FIXED_PITCH | FF_SCRIPT;
	_tcscpy_s( logFont.FaceName, _T( DEBUG_FONT_NAME ) );

	D3DXCreateFontIndirect( m_pDirect3DDevice, &logFont, &m_pDirect3DFont);

	CTexManager::GetInstance()->UploadTexture( gxTrue );

	hr = D3DXCreateTexture( m_pDirect3DDevice, WINDOW_W, WINDOW_H, 0, D3DUSAGE_RENDERTARGET, D3DFMT_X8B8G8R8, D3DPOOL_DEFAULT, &g_pTexture );
	if ( FAILED( hr ) ) {
		gxLib::DebugLog( "[error]	テクスチャ作成失敗" );
		return gxFalse;
	}

	return gxTrue;
}

void CDXDraw::Action()
{
	m_sScreenMode = g_sScreenModeIndex;

	if( gxLib::KeyBoard(VK_F5 )&enStatTrig ) g_sScreenModeIndex = 0;	//オリジナル
	if( gxLib::KeyBoard(VK_F6 )&enStatTrig ) g_sScreenModeIndex = 1;	//ズーム
	if( gxLib::KeyBoard(VK_F7 )&enStatTrig ) g_sScreenModeIndex = 2;	//フル
	if( gxLib::KeyBoard(VK_F8 )&enStatTrig ) g_sAntialiesMode   = D3DTEXF_NONE;		//アンチなし
	if( gxLib::KeyBoard(VK_F9 )&enStatTrig ) g_sAntialiesMode   = D3DTEXF_LINEAR;	//アンチエイリアス


}

void CDXDraw::Draw()
{
	HRESULT hr;
	if ( g_pTexture == NULL )
	{
		hr = m_pDirect3DDevice->TestCooperativeLevel();
		if ( hr == D3DERR_DEVICENOTRESET )
		{
			CGameGirl::GetInstance()->SetResume();
		}
		return;
	}

	Uint32 a = (Uint32)(0xff*CRender::GetInstance()->getBgColor(0));
	Uint32 r = (Uint32)(0xff*CRender::GetInstance()->getBgColor(1));
	Uint32 g = (Uint32)(0xff*CRender::GetInstance()->getBgColor(2));
	Uint32 b = (Uint32)(0xff*CRender::GetInstance()->getBgColor(3));

	if( a )
	{
		hr = m_pDirect3DDevice->Clear( 0 , NULL , D3DCLEAR_TARGET  , D3DCOLOR_XRGB( r,g,b ), 1.0f , 0 );
	}
	else
	{
		//１度は綺麗にしておく
		static Sint32 cnt = 0;
		if ( cnt < 5 )
		{
			cnt++;
			m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		}
	}

	hr = m_pDirect3DDevice->BeginScene();

	//----------------------------------------------------------
	//帯部分を描く
	//----------------------------------------------------------
	drawTie();


	//----------------------------------------------------------
	//ゲーム部位をレンダリング
	//----------------------------------------------------------
	render();


	//----------------------------------------------------------
	//サーフェイスを設定
	//----------------------------------------------------------
	m_pDirect3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pDest );
	g_pTexture->GetSurfaceLevel( 0, &m_pSurf );
	m_pDirect3DDevice->SetRenderTarget( 0, m_pSurf );

	if( g_pWindows->m_bFullScreen )
	{
		if ( m_pDest )
		{
			switch(m_sScreenMode){

			case 0:	//ドットバイドット（枠なし）「オリジナル」
				screenModeOriginal();
				break;

			case 1:	//ドットバイドット（最大引き伸ばし）「ズーム」
				screenModeZoom();
				break;

			default:
			case 2:	//引き伸ばし（枠なし）「フル」
				screenModeFull();
				break;
			}
		}
	}
	else
	{
		//Windowモード

		RECT src,dst;
		src.left = 0;
		src.top  = 0;
		src.bottom  = WINDOW_H;
		src.right   = WINDOW_W;

		dst.left = 0;
		dst.top  = 0;
		dst.bottom  = WINDOW_H-1;
		dst.right   = WINDOW_W-1;


		if( g_sAntialiesMode == D3DTEXF_NONE )
		{
			m_pDirect3DDevice->StretchRect( m_pSurf, NULL/*&src*/, m_pDest, NULL,D3DTEXF_NONE );
		}
		else
		{
			m_pDirect3DDevice->StretchRect( m_pSurf, NULL/*&src*/, m_pDest, NULL/*&dst*/,D3DTEXF_LINEAR );
		}

	}

	if( m_pDest ) m_pDest->Release();
	if( m_pSurf ) m_pSurf->Release();

	//----------------------------------------------------------
	// 描画終了
	//----------------------------------------------------------

	hr = m_pDirect3DDevice->EndScene();

}


void CDXDraw::FlipScreen()
{
	// 画面更新
	RECT src,dst;
	HRESULT hr=S_OK;
	
	if( g_pWindows->m_bFullScreen )
	{
		//フルスクリーン

		hr = m_pDirect3DDevice->Present( NULL, NULL, NULL , NULL );
	}
	else
	{
		//ウインドウモード

		src.top    = 0;
		src.bottom = WINDOW_H;
		src.left   = 0;
		src.right  = WINDOW_W;

		dst.top    = 0;
		dst.bottom = WINDOW_H;
		dst.left   = 0;
		dst.right  = WINDOW_W;

//		hr = m_pDirect3DDevice->Present( &src , &dst , NULL , NULL );
		hr = m_pDirect3DDevice->Present( NULL , NULL , NULL , NULL );
	}

	// デバイスロスト対策

	if(hr == D3DERR_DEVICELOST)
	{
		gxLib::DebugLog( "[warning] Direct3Dデバイスをロストしました" );

		hr = m_pDirect3DDevice->TestCooperativeLevel();

		if(hr != D3D_OK)
		{
			if(hr == D3DERR_DEVICENOTRESET)
			{
				gxLib::DebugLog( "[warning] デバイス復旧に入ります" );
				//m_pDirect3DDevice = NULL;
				CGameGirl::GetInstance()->SetResume();
			}
		}
	}
}


void CDXDraw::render()
{
	m_pDirect3DDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_NONE );
	m_pDirect3DDevice->SetRenderState( D3DRS_ZENABLE  , FALSE );
    m_pDirect3DDevice->SetRenderState( D3DRS_LIGHTING , gxFalse );

	m_pDirect3DDevice->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );

	m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_pDirect3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pDirect3DDevice->SetRenderState( D3DRS_LASTPIXEL, FALSE );

	CCommandList* pCommand = NULL;
	Sint32 sCommandMax      = CRender::GetInstance()->GetCommandNum();
	StCustomVertex *pVertex = CRender::GetInstance()->GetVertex(0);

#if INDEXBUFFER_BIT == 32
	Uint32         *pIndex  = CRender::GetInstance()->GetIndexBuffer( 0 );
#else
	Uint16         *pIndex  = CRender::GetInstance()->GetIndexBuffer( 0 );
#endif

	Uint32 vtx_max = CRender::GetInstance()->GetVertexNum();
	Uint32 idx_max = CRender::GetInstance()->GetIndexNum();

	// ---------------------------------------------------------------

	StCustomVertex*	pV = NULL;

	if( FAILED( m_pVB->Lock( 0, 0, (void**)&pV, 0 ) ) ) return;

	//pV
	//for(Sint32 ii=0;ii<sCommandMax; ii++)
	{
		memcpy( pV , pVertex , sizeof(StCustomVertex)*vtx_max );
	}

	if(FAILED( m_pVB->Unlock() ))return;

	// ---------------------------------------------------------------

	Uint32* wIndex;

	if( FAILED( m_pIB->Lock(0, 0, (VOID**)&wIndex, 0 ) ) ) return;

#if INDEXBUFFER_BIT == 32
	memcpy( wIndex , pIndex , sizeof(Uint32)*idx_max );
#else
	memcpy( wIndex , pIndex , sizeof(Uint16)*idx_max );
#endif

	if( FAILED( m_pIB->Unlock() ) ) return;

	// ---------------------------------------------------------------

	//頂点バッファのセット
	if( FAILED( m_pDirect3DDevice->SetStreamSource( 0, m_pVB, 0, sizeof(StCustomVertex) ) ) ) return;

	//インデックスバッファのセット
	if( FAILED( m_pDirect3DDevice->SetIndices(m_pIB) ) ) return;

	// ---------------------------------------------------------------

	Sint32 debug = 0;

	if( debug )
	{
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

/*
	m_pDirect3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);		//D3DTEXF_GAUSSIANQUAD	//D3DTEXF_LINEAR
	m_pDirect3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);		//D3DTEXF_GAUSSIANQUAD	//D3DTEXF_LINEAR
	m_pDirect3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	m_pDirect3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);	//D3DTADDRESS_CLAMP
	m_pDirect3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);	//D3DTADDRESS_CLAMP
	m_pDirect3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0xffff0000);
*/
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
		    m_pDirect3DDevice->SetTexture( 0, NULL );
			break;

		case eCmdChgTexture:
			//テクスチャをつける
			m_pDirect3DDevice->SetTexture( 0, m_D3dTexture[ pCommand->arg[0] ] );
			m_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLOROP   , D3DTOP_MODULATE);
			m_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP   , D3DTOP_MODULATE);
			m_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1 , D3DTA_TEXTURE);
			m_pDirect3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE);
			m_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1 , D3DTA_TEXTURE);
			m_pDirect3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2 , D3DTA_DIFFUSE);
			break;

		case eCmdChgAttributeAlphaNml:
			//ブレンディング(標準)
			m_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			m_pDirect3DDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE );
			m_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE);
			break;

		case eCmdChgAttributeAlphaAdd:
			//ブレンディング(加算)
			m_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			m_pDirect3DDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE );
			m_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE);
			break;

		case eCmdChgAttributeAlphaSub:
			//ブレンディング(減算)
			m_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
			m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			//m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			m_pDirect3DDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE );
			m_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE);
			break;

		case eCmdChgAttributeAlphaCrs:
			//ブレンディング(乗算)
			m_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
			m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
			m_pDirect3DDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE );
			m_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE);
			break;

		case eCmdChgAttributeAlphaRvs:
			//ブレンディング(反転)
			m_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
			m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO);
			m_pDirect3DDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE );
			m_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE);
			break;

		case eCmdChgAttributeAlphaXor:
			//ブレンディング(XOR)
			m_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
			m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
			m_pDirect3DDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE );
			m_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE);
			break;

		case eCmdChgAttributeAlphaScr:
			//ブレンディング(スクリーン乗算)
			m_pDirect3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
			m_pDirect3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			m_pDirect3DDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE );
			m_pDirect3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE);
			break;

		case eCmdRenderPoint:
			//点の描画
			//m_pDirect3DDevice->DrawPrimitiveUP     ( D3DPT_POINTLIST, v_num, &pVertex[ v_start ], sizeof( StCustomVertex ) );
			// サポートされていないらしい
			//m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_POINTLIST, 0,v_start,v_num,i_start,v_num );//v_num );
			m_pDirect3DDevice->DrawPrimitive( D3DPT_POINTLIST , v_start , v_num );
			break;

		case eCmdRenderLineStrip:
			//連続線の描画
//			m_pDirect3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, pCommand->arg[1]-1, &pVertex[ pCommand->arg[0] ], sizeof( StCustomVertex ) );
//			m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP, i_start,v_start,v_num,i_start , v_num );
			//m_pDirect3DDevice->SetWidth( pCommand->opt );	;DirectXでは線の太さを設定できない
			m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP, 0,v_start,v_num,i_start , 4 );
			break;

		case eCmdRenderLineNormal:
			//線の描画
//			m_pDirect3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, pCommand->arg[1], &pVertex[ pCommand->arg[0] ], sizeof( StCustomVertex ) );
			//m_pDirect3DDevice->SetWidth( pCommand->opt );	;DirectXでは線の太さを設定できない	
			//HRESULT SetAntialias(BOOL bAntiAlias);
			m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_LINELIST,0,v_start,v_num,i_start,v_num/2 );
			break;

		case eCmdRenderTriangle:
		case eCmdRenderSquare:
			//三角形の描画
//			m_pDirect3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 1/*pCommand->arg[1]*/, &pVertex[ pCommand->arg[0] ], sizeof( StCustomVertex ) );
			m_pDirect3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,v_start,v_num,i_start,i_num/3 );
			break;

//			//スプライトの描画
//			m_pDirect3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,v_start,v_num,i_start,i_num/3 );//v_num);
//			break;

		case eCmdRenderFont:
			//フォントの描画
			{
#if 0
				RECT rect;
				wchar_t wStr[0xff];
				Uint32 len;
				len = MultiByteToWideChar( CP_ACP , 0 ,(char*)pCommand->pString , strlen((char*)pCommand->pString)/*0xff*/ , wStr , 0xff );
				SetRect(&rect, pCommand->x, pCommand->y, WINDOW_W,WINDOW_H);
				m_pDirect3DFont->DrawText( NULL, wStr, len, &rect, DT_LEFT, D3DCOLOR_ARGB( (pCommand->arg[0]), (pCommand->arg[1]), (pCommand->arg[2]),(pCommand->arg[3])) );
//				m_pDirect3DFont->DrawText( NULL, (LPCWSTR)pCommand->pString, -1, &rect, DT_LEFT, D3DCOLOR_ARGB( (pCommand->arg[0]), (pCommand->arg[1]), (pCommand->arg[2]),(pCommand->arg[3])) );
#else
				RECT rect;
				Uint32 len = strlen( (char*) pCommand->pString );
				SetRect(&rect, pCommand->x, pCommand->y, WINDOW_W,WINDOW_H);
				m_pDirect3DFont->DrawTextA( NULL, (char*) pCommand->pString, len, &rect, DT_LEFT, D3DCOLOR_ARGB( (pCommand->arg[0]), (pCommand->arg[1]), (pCommand->arg[2]),(pCommand->arg[3])) );
#endif
			}
			break;
		}

	}


}

void CDXDraw::ReadTexture( Sint32 n )
{
	Uint32 col = 0x0000FF00;

	HRESULT hr;
	D3DXIMAGE_INFO	info;

	SAFE_RELEASE( m_D3dTexture[n] );

	hr = D3DXCreateTextureFromFileInMemoryEx(
		m_pDirect3DDevice,
		(LPCVOID)CTexManager::GetInstance()->GetFileImage(n),
		CTexManager::GetInstance()->GetFileSize(n),
		D3DX_DEFAULT, D3DX_DEFAULT,		// 画像のサイズ
		D3DX_DEFAULT,					// ミップマップレベル
		0,								// テクスチャーの使用方法
		D3DFMT_UNKNOWN ,
		D3DPOOL_MANAGED,
		D3DX_FILTER_POINT,
		D3DX_FILTER_POINT,
//		D3DX_FILTER_LINEAR,				//D3DX_FILTER_POINT,//D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,//
//		D3DX_FILTER_LINEAR,				//D3DX_FILTER_POINT,//D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,//
		D3DCOLOR_ARGB(0xff, ((col>>16)&0xff), ((col>>8)&0xff), ((col>>0)&0xff)),	// 透明にする色
		&info,							// テクスチャーの情報をえるポインタ
		NULL,							// パレットを得る
		&m_D3dTexture[n] );					// テクスチャーのポインタ

}



void CDXDraw::getVideoDeviceInfo( )
{
	//Deviceの性能をログに出力する

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

	gxLib::DebugLog( "[info] アンチエイリアス %d ｘ%d" , m_pDirect3DPrePara.MultiSampleType ,m_pDirect3DPrePara.MultiSampleQuality+1 );
}


void CDXDraw::screenModeOriginal()
{
	//-------------------------------------------------
	//原寸サイズ～整数倍サイズへの拡大
	//-------------------------------------------------
	RECT src;
	Sint32 width;
	Sint32 height;

	src.left = 0;
	src.top  = 0;
	src.right   = WINDOW_W;
	src.bottom  = WINDOW_H;

	width  = src.right  - src.left;
	height = src.bottom - src.top;

	Sint32 sAdd = 1;

	for(Sint32 ii=2;ii<5;ii++)
	{
		if( m_pDirect3DPrePara.BackBufferWidth < width * ii )
		{
			break;
		}

		if( m_pDirect3DPrePara.BackBufferHeight < height * ii )
		{
			break;
		}
		sAdd ++;
	}

	//------------------------------
	//中央に補正する
	//------------------------------

	RECT dst;
	dst.left    = (m_pDirect3DPrePara.BackBufferWidth  - width*sAdd)/2;
	dst.top     = (m_pDirect3DPrePara.BackBufferHeight - height*sAdd)/2;
	dst.bottom  = dst.top  + height*sAdd;
	dst.right   = dst.left + width*sAdd;

	if( g_sAntialiesMode == D3DTEXF_NONE )
	{
		m_pDirect3DDevice->StretchRect( m_pSurf, &src, m_pDest, &dst,D3DTEXF_NONE );
	}
	else
	{
		m_pDirect3DDevice->StretchRect( m_pSurf, &src, m_pDest, &dst,D3DTEXF_LINEAR );
	}

	m_RectDisp = dst;
}


void CDXDraw::screenModeZoom( )
{
	//-------------------------------------------------
	//アスペクト比を保ったまま最大サイズに合わせる
	//-------------------------------------------------
	RECT src;
	Sint32 width;
	Sint32 height;

	src.left = 0;
	src.top  = 0;
	src.right   = WINDOW_W;
	src.bottom  = WINDOW_H;

	width  = src.right  - src.left;
	height = src.bottom - src.top;

	Float32 fAdd1,fAdd2,fAdd;
	fAdd1 = 1.f * m_pDirect3DPrePara.BackBufferWidth  / width;
	fAdd2 = 1.f * m_pDirect3DPrePara.BackBufferHeight / height;

	if( fAdd1 > fAdd2 )
	{
		fAdd = fAdd2;
	}
	else
	{
		fAdd = fAdd1;
	}

	//------------------------------
	//中央に補正する
	//------------------------------

	RECT dst;
	dst.left    = (m_pDirect3DPrePara.BackBufferWidth  - width*fAdd)/2;
	dst.top     = (m_pDirect3DPrePara.BackBufferHeight - height*fAdd)/2;
	dst.bottom  = dst.top  + height*fAdd;
	dst.right   = dst.left + width*fAdd;

	if( g_sAntialiesMode == D3DTEXF_NONE )
	{
		m_pDirect3DDevice->StretchRect( m_pSurf, &src, m_pDest, &dst,D3DTEXF_NONE );
	}
	else
	{
		m_pDirect3DDevice->StretchRect( m_pSurf, &src, m_pDest, &dst,D3DTEXF_LINEAR );
	}

	m_RectDisp = dst;
}


void CDXDraw::screenModeFull()
{
	//-------------------------------------------------
	//フル画面引き伸ばし
	//-------------------------------------------------

	RECT src , dst;

	src.left    = 0;
	src.top     = 0;
	src.right   = WINDOW_W;
	src.bottom  = WINDOW_H;

	dst.top     = 0;
	dst.left    = 0;
	dst.right   = m_pDirect3DPrePara.BackBufferWidth;
	dst.bottom  = m_pDirect3DPrePara.BackBufferHeight;

	if( g_sAntialiesMode == D3DTEXF_NONE )
	{
		m_pDirect3DDevice->StretchRect( m_pSurf, &src, m_pDest, &dst,D3DTEXF_NONE );
	}
	else
	{
		m_pDirect3DDevice->StretchRect( m_pSurf, &src, m_pDest, &dst,D3DTEXF_LINEAR );
	}

	m_RectDisp = dst;

}


void CDXDraw::drawTie()
{
	//-------------------------------------------------
	//帯を表示させる
	//-------------------------------------------------

	HRESULT hr;

	RECT backSrc,backDst,desktop;
	backSrc.left  = 0;
	backSrc.top   = 0;
	backSrc.right = 275;
	backSrc.bottom= 275;

	backDst.left  = 0;
	backDst.top   = 0;
	backDst.right = m_pDirect3DPrePara.BackBufferWidth;
	backDst.bottom= m_pDirect3DPrePara.BackBufferHeight;

	desktop.left  = 0;
	desktop.top   = 0;
	desktop.right = 320;
	desktop.bottom= 320;

	if( g_lpSurface == NULL )
	{
		SAFE_RELEASE( g_lpSurface );
		hr = m_pDirect3DDevice->CreateOffscreenPlainSurface( desktop.right,desktop.bottom, D3DFMT_X8R8G8B8,   D3DPOOL_DEFAULT, &g_lpSurface, NULL );
//毒		hr = D3DXLoadSurfaceFromFile( g_lpSurface, NULL, NULL, "dat/neko.jpg",  NULL, D3DX_DEFAULT, 0, NULL );
		return;
	}

	hr = m_pDirect3DDevice->StretchRect( g_lpSurface, &backSrc, m_pDest, &backDst, D3DTEXF_NONE  );//D3DTEXF_LINEAR );

}


void CDXDraw::capture( gxChar *pFileName )
{
	const char*fileName = pFileName;
	int deskTopX = GetSystemMetrics(SM_CXSCREEN);
	int deskTopY = GetSystemMetrics(SM_CYSCREEN);

	IDirect3DSurface9* pSurf = m_pSurf;
	pSurf = m_pDest;

	m_pDirect3DDevice->CreateOffscreenPlainSurface( deskTopX,deskTopY, D3DFMT_A8R8G8B8,D3DPOOL_SYSTEMMEM, &pSurf, NULL );
	m_pDirect3DDevice->GetFrontBufferData( 0 , pSurf );

	RECT rect,*pRect;
	pRect = GetWindowRect();

	if( !g_pWindows->m_bFullScreen )
	{
		pRect->bottom -= GetSystemMetrics( SM_CYDLGFRAME );
		pRect->top    += GetSystemMetrics( SM_CYDLGFRAME ) + GetSystemMetrics( SM_CYCAPTION );
		pRect->left   += GetSystemMetrics( SM_CXDLGFRAME );
		pRect->right  -= GetSystemMetrics( SM_CXDLGFRAME );
	}

//毒	D3DXSaveSurfaceToFile( fileName,D3DXIFF_BMP,pSurf,NULL,&rect );
//	RELEASE(pSurface );
}

