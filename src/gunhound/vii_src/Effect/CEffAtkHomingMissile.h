class CEffAtkHomingMissile : public CEffectBase
{
public:
	CEffAtkHomingMissile( Sint32 x,Sint32 y , Sint32 sRot , Sint32 sLevel );
	~CEffAtkHomingMissile();

	void SeqMain();
	void SeqCrash();

	void Draw();

private:

	Sint32 m_sRotation;			//��������
	Sint32 m_sRotationMaster;	//���������L�^�p
	Sint32 m_sRotationAdd;		//�����␳�p
	Sint32 m_sHomingLevel;		//�z�[�~���O���x��
	Sint32 m_sHomingWait;		//�z�[�~���O�J�n����
	Sint32 m_sHomingAdjustSpd;	//�O���C���i���炵�j���x��
	Sint32 m_sSpeed;			//�X�s�[�h

	OBJ_POS_T m_Add;

	CHitCollision m_HitAtari;

	void missileLv1();
	void missileLv2();
	void missileLv3();

	gxBool getTargetEnemyPosition(Sint32 x,Sint32 y,Sint32 &px,Sint32 &py);

};
