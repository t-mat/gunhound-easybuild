//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
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

		Float32 fBaseRot;	//もともとの角度

		Float32 fRot;		//シャフト角度
		Float32 fRAdd;		//シャフト角度－進行速度
		Float32 fRange;		//シャフト先端移動範囲

		Float32 fPosRange;	//シャフト軸移動距離
		Float32 fPosRot;	//シャフト軸移動角度

		Float32 fDspRot;	//最終的な表示角度

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

