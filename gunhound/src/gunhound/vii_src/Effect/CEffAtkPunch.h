class CEffAtkPunch : public CEffectBase
{
public:
	CEffAtkPunch( Sint32 x,Sint32 y , Sint32 rot , Sint32 sDist , Sint32 sLevel );
	~CEffAtkPunch();

	void SeqMain();
	void SeqCrash();

	void Draw();

private:

	Sint32 m_sAlpha;
	Sint32 m_sCol_Red;
	Sint32 m_sCol_Grn;
	Sint32 m_sCol_Blu;
	gxBool m_bHit;
	Sint32 m_sScale;

//	OBJ_POS_T m_sScroll;
	OBJ_POS_T m_Disp;
	OBJ_POS_T m_Add;

	CHitCollision m_HitAtari;

	void SetColor();
	gxBool IsHitKabe();


};
