class CEffAtkHomingMissile : public CEffectBase
{
public:
	CEffAtkHomingMissile( Sint32 x,Sint32 y , Sint32 sRot , Sint32 sLevel );
	~CEffAtkHomingMissile();

	void SeqMain();
	void SeqCrash();

	void Draw();

private:

	Sint32 m_sRotation;			//初期方向
	Sint32 m_sRotationMaster;	//初期方向記録用
	Sint32 m_sRotationAdd;		//方向補正用
	Sint32 m_sHomingLevel;		//ホーミングレベル
	Sint32 m_sHomingWait;		//ホーミング開始時間
	Sint32 m_sHomingAdjustSpd;	//軌道修正（ずらし）レベル
	Sint32 m_sSpeed;			//スピード

	OBJ_POS_T m_Add;

	CHitCollision m_HitAtari;

	void missileLv1();
	void missileLv2();
	void missileLv3();

	gxBool getTargetEnemyPosition(Sint32 x,Sint32 y,Sint32 &px,Sint32 &py);

};
