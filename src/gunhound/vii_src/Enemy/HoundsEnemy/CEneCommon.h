#ifndef _CENECOMMON_H_
#define _CENECOMMON_H_

//-------------------------------------------------
//メルキア軍軍用サイン
//-------------------------------------------------

class CEneCmnSignal
{
public:
	enum {
		enSignalTypeSOS = 0,
		enSignalTypeNORMAL,
	};

	CEneCmnSignal();
	~CEneCmnSignal();

	void Draw();
	void SetSignal(Sint32 x, Sint32 y , Sint32 sType=enSignalTypeSOS );

	void On()
	{
		m_bSignal = gxTrue;
	}

	void Off()
	{
		m_bSignal = gxFalse;
	}


private:
	enum
	{
		enSignalInterval = 8,
	};

	OBJ_POS_T m_Pos;
	Sint32 m_sTimer;
	Sint32 m_sAlpha;
	Sint32 m_sScale;
	gxBool m_bSignal;
	Sint32 m_sType;

};



typedef struct StPartsInfo {
	StPartsInfo()
	{
		sParent=0;
		sSprite=0;
		sDispSprite=0;
		x=y=z=rx=ry=rz=a=sx=sy=0;
		ox=oy=oz=0;
		dx=dy=dz=dr=0;
		bDisp = gxTrue;
		sAtribute = ATR_DFLT;
		bNoDamage = gxFalse;
	}

	//元データ

	Sint32 sParent;
	Sint32 sSprite;
	Sint32 sDispSprite;
	Sint32 x,y,z,rx,ry,rz,a,sx,sy;
	Sint32 ox,oy,oz;

	//表示時専用

	Sint32 dx,dy,dz,dr;
	gxBool bDisp;
	gxBool bNoDamage;
	Sint32 sAtribute;

}StPartsInfo;


//-------------------------------------------------
//アニメツールからのアニメデータ取得用
//-------------------------------------------------
typedef struct StPatrsOffset{
	StPatrsOffset()
	{
		dist = 0;
		rot  = 0;
	}

	Sint32 dist;
	Sint32 rot;
}StPatrsOffset;

class CEnemyAnime
{
public:
	enum {
		enPartsMax = 128,
	};
	CEnemyAnime();
	~CEnemyAnime();

	void SetParts( Sint32 id , Sint32 sMax , Sint32 sParent , Sint32 *pPatrs );
	void PosCalc();

	StPartsInfo* GetParts(Sint32 n)
	{
		return m_StPartsInfo[n];
	}

	void GetPos( ghPos *pPos , Sint32 num , Sint32 sDirection );
	void Draw( gxBool m_bDamage , Sint32 x , Sint32 y , gxBool bReverse ,gxSprite *pSpr);

	gxBool GetPartsDist(Sint32 sParts , StPatrsOffset* pStPatrsOffset );

	void SetPrio(Sint32 n)
	{
		m_sPrio = n;
	}

	StPartsInfo* GetParent(Sint32 n)
	{
		return GetParts( GetParts( n )->sParent );
	}
private:

	StPartsInfo *m_StPartsInfo[enPartsMax];
	Sint32 m_sPatrtsMax;
	Sint32 m_sPrio;
};



//-------------------------------------------------
// キャタピラ（大）処理
//-------------------------------------------------

class CCatapiller
{
public:

	CCatapiller()
	{
		m_sSprMin = 1;
		m_sSprMax = 1;
		m_sDist = 1;
		m_sAdd = 0;
		m_sRotation = 0;
		m_sSpeed = 10;
	}

	~CCatapiller()
	{
		
	}

	void SetSpeed(Sint32 sSpeed);
	void SetSpriteIndex( Sint32 sMin , Sint32 sMax );
	void MoveLeft();
	void MoveRight();
	void MoveNone();
	void Action( CEnemyAnime* pAnime );
	Sint32 GetRotation();

private:

	Sint32 m_sSprMin;
	Sint32 m_sSprMax;
	Sint32 m_sDist;
	Sint32 m_sRotation;
	Sint32 m_sAdd;
	Sint32 m_sSpeed;

};

//-------------------------------------------------
//電気アニメ
//-------------------------------------------------

class CEneCmnSpark
{
public:

	CEneCmnSpark(Sint32 x1, Sint32 y1 ,Sint32 x2,Sint32 y2);
	~CEneCmnSpark();

	void Set(Sint32 x1, Sint32 y1 ,Sint32 x2,Sint32 y2);
	void Action();
	void Draw();

	void SetRotation(Float32 fRot)
	{
		m_bRotSet = gxTrue;
		m_fRotSet = fRot;
	}
	void SetLengthControl()
	{
		m_bScaleControl = gxTrue;
	}
private:

	OBJ_POS_T m_Pos;
	OBJ_POS_T m_Tgt;
	Sint32 m_sDist;
	Sint32 m_sCos;

	Sint32 m_sLoop;
	Float32 m_fRot;
	Float32 m_fTgtRot;
	Float32 m_fRotAdd;
	Float32 m_fScale;

	Sint32 m_sTimer;
	Sint32 m_sAlpha;

	gxBool m_bRotSet;
	gxBool m_bScaleControl;
	Float32 m_fRotSet;

};

//-------------------------------------------------
//ハウンド入退場制御
//-------------------------------------------------

class CEntranceControl : public CEnemyBase// , public CHoundEnemyBase
{
public:
	enum {
		enExit,
		enEntrance,
	};
	CEntranceControl(Sint32 sMode , Sint32 sDir);
	~CEntranceControl();

	void SeqMain();
	void Draw();
private:

	void InofScreen();
	void OutofScreen();

	ghPos m_Add;
	Sint32 m_sMode;
	Sint32 m_sSequence;
	Sint32 m_sSeqWait;

};



#endif
