//-----------------------------------------------
//倒木シーン
//-----------------------------------------------
class CEffScn04Tree : public CEffectBase
{
public:
	enum {
		enTreeMax = 6,
	};
	CEffScn04Tree(Sint32 x,Sint32 y ,Sint32 z );
	~CEffScn04Tree();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

	void SetBombStart();

private:

	Sint32 m_sEnsyutuTimer;
	CEffExPerticle *m_pTree[enTreeMax];	//パーティクルエフェクト

	void TreeDown(Sint32 id,Sint32 x,Sint32 y);
	void TreeDrop( Sint32 x,Sint32 y );
	void TreeLeaf( Sint32 x,Sint32 y );
};


//-----------------------------------------------
//倒木シーン
//-----------------------------------------------
class CEffTreeEda : public CEffectBase
{
public:
	CEffTreeEda( Sint32 x,Sint32 y );
	~CEffTreeEda();

//	void SeqInit();
	void SeqMain();
	void SeqCrash();
//	void SeqEnd();

	void Draw();

private:

	OBJ_POS_T m_Add;

	Sint32 m_sScale;
	Sint32 m_sColor;
	Sint32 m_sRotation;

};


//-----------------------------------------------
//倒木シーン
//-----------------------------------------------
class CEffTreeLeaf : public CEffectBase
{
public:
	CEffTreeLeaf( Sint32 x,Sint32 y );
	~CEffTreeLeaf();

//	void SeqInit();
	void SeqMain();
	void SeqCrash();
//	void SeqEnd();

	void Draw();

private:

	Sint32 m_sScale;
	Sint32 m_sColor;
	Sint32 m_sRotation;
	Sint32 m_sPattern;

};
