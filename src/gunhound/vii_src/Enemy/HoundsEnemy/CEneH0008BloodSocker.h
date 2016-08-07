//--------------------------------------------------------------------------------
//
// Stage00:�{���]�C�i�u���b�h�T�b�J�[�j
//
//--------------------------------------------------------------------------------
class CAtkH0001Thunder;
class CEneH0008BloodSocker : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0008BloodSocker( Sint32 x, Sint32 y );
	~CEneH0008BloodSocker();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	//�{���]�C�̃J�X�^�}�C�Y
	void SetCustomIndex(Sint32 n);

private:
	enum
	{
		enWeaponMax = 8,
	};

	//������
	void SetInitialize();
	void SetCustomize( Sint32 sBody ,Sint32 sArm , Sint32 sLeg);
	void SetArms();
	void WeaponReset();
	//�U��
	void Atack( Sint32 sAtk );

	//���t���[���̏���
	void ActionMain();

	//�r�p�x�␳
	void AdjustArmAngle( Sint32 ax , Sint32 ay ,Sint32 sOffset=0);

	//����`�F���W�\���H
	gxBool IsWeaponChangeChance();


	//-----------------------------
	//�U��
	//-----------------------------
	void AtackShortBurrel();		//�N���b�V���[�𗐎�
	void AtackLongBurrel();			//���e��_���������Ă���
	void AtackSolidShooter();		//�_�������o�Y�[�J
	void AtackShoulderMissilePod();	//�ǔ����~�T�C��
	void AtackLogGun();				//���ߑł��ɑ����[�U�[
	void AtackLegPod();				//�Q�A�����q�~�j�~�T�C��
	void AtackBodyCrasher();		//�Q�A�����q�~�j�~�T�C��

	//-----------------------------
	//���W�b�N����֘A
	//-----------------------------
	void LogicAI();
	void PadControl(Sint32 n);

	//-----------------------------
	//�p�[�c�ԍ��Ǘ��p
	//-----------------------------
	Sint32 m_sBody;
	Sint32 m_sLegs;
	Sint32 m_sArms;

	//-----------------------------
	//�U������֘A
	//-----------------------------

	Sint32 m_sWeapon[enWeaponMax];	//����ԍ��ۑ��p
	Sint32 m_sWeaponNum;			//�莝���̕���̐�
	Sint32 m_sMainWeapon;			//���ݑI�𒆂̕���

	Sint32 m_sBackWait;				//�m�b�N�o�b�N����
	Sint32 m_sReboundWait;			//�o�Y�[�J�r�̃��o�E���h����
	Sint32 m_sTargetSeq;			//�^�[�Q�b�g�����␳����
	Sint32 m_sArmRotation;			//�^�[�Q�b�g�����␳�p�A�[���p�x
	Sint32 m_sCrashTimer;			//�j�󂳂��܂ł̃^�C�����O

	gxBool m_bGuard;				//�K�[�h��������
	gxBool m_bArmCrash;				//�A�[���N���b�V��

	//CEffAtkLaser *m_pLaser;			//���[�U�[����p
	CAtkH0001Thunder *m_pLaser;
	gxBool m_bLaser;
	//-----------------------------
	//���W�b�N����֘A
	//-----------------------------
	Sint32 m_sPushControl;
	Sint32 m_sTrigControl;

	Sint32 m_sAtackLag;				//�ˌ��Ԃ̃^�C�����O
	Sint32 m_sAtackCnt;				//�U����

	//-----------------------------
	//���̑�
	//-----------------------------
	OBJ_POS_T m_Src;				//�����ʒu
	OBJ_POS_T m_Grd;				//�����ʒu
	CHitCollision m_HitKurai;
	CHitCollision m_HitGuard;		//�K�[�h�p
	CEasyLeynos m_Leynos;

	gxBool m_bSpecialVersion;
	gxBool m_bEscaped;
};

