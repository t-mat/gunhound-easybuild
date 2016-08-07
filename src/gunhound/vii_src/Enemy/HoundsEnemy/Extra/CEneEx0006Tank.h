//--------------------------------------------------------------------------------
//
// Stage00:�T���v���G�l�~�[
//
//--------------------------------------------------------------------------------
enum {
	enEngineItemNone,
	enEngineItemShaft,
	enEngineItemGear_S,
	enEngineItemGear_L,
	enEngineItemFuta,
	enEngineItemCyrinder,
	enEngineItemCore,
};

class CEneEx0006Tank : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneEx0006Tank( Sint32 x, Sint32 y );
	~CEneEx0006Tank();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	gxBool IsDamage()
	{
		return m_bDamage;
	}
private:

	typedef struct StItem {
		Sint32 sType;
		Sint32 sSpr;
		ghPos  stPos;
		ghPos  stAdd;

		Float32 fBaseRot;	//���Ƃ��Ƃ̊p�x

		Float32 fRot;		//�V���t�g�p�x
		Float32 fRAdd;		//�V���t�g�p�x�|�i�s���x
		Float32 fRange;		//�V���t�g��[�ړ��͈�

		Float32 fPosRange;	//�V���t�g���ړ�����
		Float32 fPosRot;	//�V���t�g���ړ��p�x

		Float32 fDspRot;	//�ŏI�I�ȕ\���p�x

		Float32 fScl;
		StItem()
		{
			sType = enEngineItemNone;
			sSpr  = 0;
			stPos.x = 0;
			stPos.y = 0;
			stPos.z = 0;

			stAdd.x = 0;
			stAdd.y = 0;
			stAdd.z = 0;

			fPosRange = ZERO;
			fPosRot   = ZERO;

			fBaseRot = fDspRot = fRot  = ZERO;
			fRAdd = 1.f;

			fScl  = 1.f;
			fRange = 360.f;
		}
	} StItem;

	enum {
		enPartsMax = 24,
	};
	void Atack();
	void LogicAI();
	void Gimmick();

	void LogicShaft(Sint32 n);
	void LogicGear(Sint32 n);
	void LogicCyrinder(Sint32 n);
	void LogicFuta(Sint32 n);
	void LogicCore(Sint32 n);

	gxBool SeqCrashCore();

	void DrawShaft(Sint32 n);
	void DrawGear(Sint32 n);
	void DrawCyrinder(Sint32 n);
	void DrawFuta(Sint32 n);
	void DrawCore(Sint32 n);


	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;
	ghPos m_Add;

	StItem m_stItem[enPartsMax];

	Sint32 m_sSeq;
	Sint32 m_sCrashTimer;
	Sint32 m_sAtackWait;
	Float32 m_fRotation;
	Float32 m_fRotationAdd;

	gxBool m_bDamage;
	gxBool m_bCoreDamage;
	gxBool m_bDisp;
};

