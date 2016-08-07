//--------------------------------------------------------------------------------
//
// Stage00:ボルゾイ（ブラッドサッカー）
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

	//ボルゾイのカスタマイズ
	void SetCustomIndex(Sint32 n);

private:
	enum
	{
		enWeaponMax = 8,
	};

	//初期化
	void SetInitialize();
	void SetCustomize( Sint32 sBody ,Sint32 sArm , Sint32 sLeg);
	void SetArms();
	void WeaponReset();
	//攻撃
	void Atack( Sint32 sAtk );

	//毎フレームの処理
	void ActionMain();

	//腕角度補正
	void AdjustArmAngle( Sint32 ax , Sint32 ay ,Sint32 sOffset=0);

	//武器チェンジ可能か？
	gxBool IsWeaponChangeChance();


	//-----------------------------
	//攻撃
	//-----------------------------
	void AtackShortBurrel();		//クラッシャーを乱射
	void AtackLongBurrel();			//実弾を狙い撃ちしてくる
	void AtackSolidShooter();		//狙い撃ちバズーカ
	void AtackShoulderMissilePod();	//追尾式ミサイル
	void AtackLogGun();				//溜め打ち極太レーザー
	void AtackLegPod();				//２連式巡航ミニミサイル
	void AtackBodyCrasher();		//２連式巡航ミニミサイル

	//-----------------------------
	//ロジック制御関連
	//-----------------------------
	void LogicAI();
	void PadControl(Sint32 n);

	//-----------------------------
	//パーツ番号管理用
	//-----------------------------
	Sint32 m_sBody;
	Sint32 m_sLegs;
	Sint32 m_sArms;

	//-----------------------------
	//攻撃制御関連
	//-----------------------------

	Sint32 m_sWeapon[enWeaponMax];	//武器番号保存用
	Sint32 m_sWeaponNum;			//手持ちの武器の数
	Sint32 m_sMainWeapon;			//現在選択中の武器

	Sint32 m_sBackWait;				//ノックバック時間
	Sint32 m_sReboundWait;			//バズーカ腕のリバウンド処理
	Sint32 m_sTargetSeq;			//ターゲット方向補正制御
	Sint32 m_sArmRotation;			//ターゲット方向補正用アーム角度
	Sint32 m_sCrashTimer;			//破壊されるまでのタイムラグ

	gxBool m_bGuard;				//ガード属性判定
	gxBool m_bArmCrash;				//アームクラッシュ

	//CEffAtkLaser *m_pLaser;			//レーザー制御用
	CAtkH0001Thunder *m_pLaser;
	gxBool m_bLaser;
	//-----------------------------
	//ロジック制御関連
	//-----------------------------
	Sint32 m_sPushControl;
	Sint32 m_sTrigControl;

	Sint32 m_sAtackLag;				//射撃間のタイムラグ
	Sint32 m_sAtackCnt;				//攻撃回数

	//-----------------------------
	//その他
	//-----------------------------
	OBJ_POS_T m_Src;				//初期位置
	OBJ_POS_T m_Grd;				//初期位置
	CHitCollision m_HitKurai;
	CHitCollision m_HitGuard;		//ガード用
	CEasyLeynos m_Leynos;

	gxBool m_bSpecialVersion;
	gxBool m_bEscaped;
};

