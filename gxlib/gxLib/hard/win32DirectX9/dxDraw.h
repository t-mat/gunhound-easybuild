// --------------------------------------------------------
/*

*/
// --------------------------------------------------------

class CDXDraw
{
public:

	CDXDraw();
	~CDXDraw();

	gxBool Init();
	void Action();
	void Draw();
	void FlipScreen();

	void Resume()
	{
		resetDevice();
		CTexManager::GetInstance()->UploadTexture( gxTrue );
	}

	void ReadTexture(Sint32 sBank);

	void SetFullScreenMode( gxBool bOn );

	RECT* GetWindowRect()
	{
		return &m_RectDisp;
	}

	SINGLETON_DECLARE( CDXDraw );

	
private:

	HRESULT checkAntialias( D3DMULTISAMPLE_TYPE AntialiasMode );
	gxBool resetDevice();
	void render();

	void getVideoDeviceInfo();

	void screenModeFull();
	void screenModeOriginal();
	void screenModeZoom( );
	void drawTie();

	void capture( gxChar *pFileName );

	LPDIRECT3D9        		m_pDirect3D;
	LPDIRECT3DDEVICE9  		m_pDirect3DDevice;
	D3DPRESENT_PARAMETERS	m_pDirect3DPrePara;
	LPDIRECT3DTEXTURE9		m_D3dTexture[CTexManager::enMasterPageNum];
	LPD3DXFONT				m_pDirect3DFont;

	Sint32 m_sScreenMode ;

	IDirect3DSurface9* m_pSurf;
	IDirect3DSurface9* m_pDest;

	RECT m_RectDisp;
};


