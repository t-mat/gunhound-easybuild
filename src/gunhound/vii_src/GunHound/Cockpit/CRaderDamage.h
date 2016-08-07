//--------------------------------------
//���ʂ��Ƃ̃_���[�W�ʊǗ�
//--------------------------------------
class CDamageDisp
{
public:
	CDamageDisp()
	{
		sId   = 0;
		sTime = 0;
		bDead = gxFalse;
	}
	~CDamageDisp()
	{

	}

	void SetID(Sint32 Id )
	{
		sId = Id;
	}

	void SetDamage()
	{
		sTime = 120;
	}

	void SetCrash()
	{
		bDead = gxTrue;
	}

	void Action()
	{
		if( sTime > 0) sTime --;
	}

	Sint32 sId;
	Sint32 sTime;
	gxBool bDead;

private:


};

//--------------------------------------
//���ʂ��Ƃ̊Ǘ�
//--------------------------------------
class CRaderDamage
{
public:

	CRaderDamage();
	~CRaderDamage();

	void Action();
	void Draw( Sint32 bx , Sint32 by );
	void DrawSekigaisen( Sint32 bx , Sint32 by );

	void SetDamage( Sint32 sIndex );
	void SetCrash( Sint32 sIndex );

	void SetSwitch(gxBool bSekigaisen)
	{
		//�ԊO���X�C�b�`�I��
		m_bSwSekigaisen = bSekigaisen;
	}

	void SetDisp(gxBool bDisp)
	{
		//�ԊO���X�C�b�`�I��
		m_bDispSekigaisen = bDisp;
	}

	void SetHitPoint(Sint32 sHp )
	{
		m_sHitPoint = sHp;
	}

private:
	enum {
		enMax = 4,
	};

	void DrawHitPoint(Sint32 bx , Sint32 by);
	CDamageDisp m_CDamageDisp[enMax];

	gxBool m_bDispSekigaisen;
	Sint32 m_sPosX;
	gxBool m_bSwSekigaisen;
	Sint32 m_sTimer;
	Sint32 m_sHitPoint;
};

