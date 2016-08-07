enum enSoldierCommandType {
	enSoldierCommandFuttobi,
};

enum enSoldierWalkType {
	enWalkTypeNormal,
	enWalkTypeEscape,
	enWalkTypeDrawn,
};

enum enMoveType {

	enMoveTypeWait=0,		//指示待ち
	enMoveTypeMove,			//目標地点へ移動(中腰)
	enMoveTypeFlyUp,		//ふっとび(仰向け)
	enMoveTypeFlyDown,		//ふっとび(うつぶせ)
	enMoveTypeDownUp,		//ダウン(仰向け)
	enMoveTypeDownDown,		//ダウン(うつぶせ)
	enMoveTypeBackRolling,	//後転
	enMoveTypeFrontRolling,	//前転
	enMoveTypeGetUpU,		//起き上がり（仰向け）
	enMoveTypeGetUpD,		//起き上がり（うつぶせ）
	enMoveTypeEscape,		//逃げる
	enMoveTypeDrawn,		//溺れる
	enMoveTypeFrontFalling,	//落下

	enMoveTypeFrontJump,	//ジャンプ（未実装）
	enMoveTypeFrontLand,	//着地（未実装）
	enMoveTypeClimb,		//上る

	enMoveTypeRideOffHeli,	//ヘリから降りる

	enMoveTypeTrgtGun,		//銃を構える
	enMoveTypeShottGun,		//銃を撃つ
	enMoveTypeRideOn,		//トラックへのる
	enMoveTypeRideOff,		//トラックから降りる
	enMoveTypeRopeUp,		//ロープをのぼる
	enMoveTypeLadderDown,	//ロープを降りる
	enMoveTypeLadderUp,		//はしごをつたう
	enMoveTypeSearch,		//探す
	enMoveTypeThrowBomb,	//投げる
	enMoveTypeTrgtBuz,		//バズーカを構える
	enMoveTypeShotBuz,		//バズーカを撃つ
	enMoveTypeShotMG,		//機銃砲座で撃つ
	enMoveTypeSupportStand,	//弾薬サポート(たち)
	enMoveTypeSupportSit,	//弾薬サポート(すわり)
	enMoveTypePush,			//押す
	enMoveTypePull,			//引く

	enMoveTypeBombSet,		//爆弾セット
	enMoveTypeBombSwitch,	//爆弾スイッチ
	enMoveTypeLeading,		//部下をリード

	enMoveTypeDirectAssault,//突撃を指示
	enMoveTypeDirectEscape,	//退却を指示

	enMoveTypeEscapeMachine,//上って脱出

};

enum {
	enForceVersis=0,
	enForceEnemy,
};

class CSoldierBase : public CEnemyBase
{

public:
	enum {
		enMemberMax = 8,
	};

	CSoldierBase();
	~CSoldierBase();

//	void Action();
	void Draw();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Main();
	void EveryAction();
	void SetJump();

	void SetLeader( CSoldierBase *p );
	void SetMember( CSoldierBase *p );
	void SetForce( Sint32 sForce );
	void SetWaitScreenIn()
	{
		m_bWaitScReenIn = gxTrue;
	}

	virtual void MoveCommon();			//移動制御共通

	enMoveType eMoveType;

	//------------------------------------------------------
	void Command(Sint32 n);



	//------------------------------------------------------

	virtual void MoveTypeWait();		//指示待ち
	virtual void MoveTypeMove();		//目標地点へ移動(中腰)
	virtual void MoveTypeFlyUp();		//ふっとび(仰向け)
	virtual void MoveTypeFlyDown();		//ふっとび(うつぶせ)
	virtual void MoveTypeGetUpD();		//起き上がり(うつぶせ)
	virtual void MoveTypeGetUpU();		//起き上がり(仰向け)
	virtual void MoveTypeDownUp();		//ダウン(仰向け)
	virtual void MoveTypeDownDown();	//ダウン(うつぶせ)
	virtual gxBool MoveTypeClimb();		//段差を上る

	virtual void MoveTypeRideOffHeli();	//ヘリから降りる
	virtual void MoveTypeRideOn();		//トラックへのる
	virtual void MoveTypeRideOff();		//トラックから降りる
	virtual void MoveTypeRopeUp();		//ロープをのぼる
	virtual void MoveTypeLadderDown();	//ロープを降りる
	virtual void MoveTypeLadderUp();	//はしごをつたう
	virtual void MoveTypeSearch();		//探す
	virtual void MoveTypeThrowBomb();	//投げる
	virtual void MoveTypeTrgtGun();		//銃を構える
	virtual void MoveTypeShottGun();	//銃を撃つ
	virtual void MoveTypeTrgtBuz();		//バズーカを構える
	virtual void MoveTypeShotBuz();		//バズーカを撃つ
	virtual void MoveTypeShotMG();		//機銃砲座で撃つ
	virtual void MoveTypeEscape();		//逃げる
	virtual void MoveTypeDrawn();		//溺れる
	virtual void MoveTypeSupportStand();//弾薬サポート(たち)
	virtual void MoveTypeSupportSit();	//弾薬サポート(すわり)
	virtual void MoveTypePush();		//押す
	virtual void MoveTypePull();		//引く

	virtual void MoveTypeBombSet();		//爆弾セット
	virtual void MoveTypeBombSwitch();	//爆弾スイッチ
	virtual void MoveTypeLeading();		//部下をリード

	virtual void MoveTypeDirectAssault();	//突撃を指示
	virtual void MoveTypeDirectEscape();	//退却を指示

	virtual void MoveTypeBackRolling();		//後転
	virtual void MoveTypeFrontRolling();	//前転
	virtual void MoveTypeFalling();			//落下
	virtual void MoveTypeEscapeMachine();	//脱出

	void SetTargetPosition(int _x)
	{
		m_sTargetPosition = _x;
	}
	void SetSpeed(Sint32 sMaxSpeed )
	{
		m_sMaxSpeed = sMaxSpeed;
	}
	void ChangeMoveType(enMoveType _eMoveType);

	CSoldierBase* GetMember(Sint32 n)
	{
		if( n >= enMemberMax || n >= m_sMemberNum ) return NULL;

		return m_pMember[n];
	}

	void CommandMember( enMoveType _eMoveType=enMoveTypeMove );

	int GetPositionX()
	{
		return m_sPositionX;
	}

	Sint32 GetSeq()
	{
		return m_sActSeq;
	};

	void SetSeq(Sint32 n)
	{
		m_sActSeq = n;
	};

	void SetFuttobi( Sint32 sSpd );

	void SetEnd()
	{
		m_bEnd = gxTrue;
	}
	void SetCivilian(Sint32 sCivilType)
	{
		m_sCivilType = sCivilType;
	}

//private:
protected:

	CObj *m_pObj;
	CSoldierBase *m_pLeader;
	CSoldierBase *m_pMember[enMemberMax];
	Sint32 m_sMemberNum;
	Sint32 m_sForce;
	int m_sMoveX;
	int m_sMoveY;
	int m_sGravit;
	int m_sDir;
	int m_sSpeed;
	int m_sPrio;
	int m_sWalkCnt;
	int m_sSprite;
	Sint32 m_sActSeq;
	gxBool m_bGravit;
//	gxBool m_bMuteki;
	Sint32 m_sCivilType;
	CHitCollision m_HitKurai;

	gxBool m_bEnd;
	Uint16 m_sWaitTimer;
	Sint32 m_sTargetPosition;	//移動先ポジション
	Sint16 m_sJumpDamageTimer;	//ふっとびダメージタイマー
	Sint16 m_sDownTimer;		//ダウンタイマー

	Sint8 m_sWalkType;			//歩き方、普通、逃げる、おぼれる

	Sint32 m_sPositionX,m_sPositionY;
	gxSprite *m_pSprite;

	ghPos m_stOffset;			//表示オフセット

	void DebugControl();

	bool IsLanding()
	{
		//着地しているか？
		if( m_sGravit == 0) return gxTrue;
		return gxFalse;
	}

	int GetSpritePattern(int _cnt,int _frm, int _ptnNum ,bool _bNoLoop = gxTrue )
	{
		//アニメのパターンを算出
		int max;

		if( !_bNoLoop )
		{
			//ループしない場合は最後のフレームでとめておく
			if( _cnt >= _frm*_ptnNum ) _cnt = _frm*_ptnNum-1;
		}

		max = (_cnt+(_frm*_ptnNum))%(_frm*_ptnNum);

		return max/_frm;
	}

	bool MoveSpeedDecrement()
	{
		//移動速度減衰

		m_pObj->mov.x = m_pObj->mov.x*95/100;

		if( abs(m_pObj->mov.x) < 10)
		{
			m_pObj->mov.x = 0;
			m_sMoveX = 0;
			return gxTrue;
		}
		return gxFalse;
	} 

	void ResetJumpDamageTimer()
	{
		m_sJumpDamageTimer = 0;		//ダメージタイマーを初期化
		m_sDownTimer = 0;			//ダウン時間を初期化
	}

	bool IsTargetPosition()
	{
		if(abs(m_pObj->pos.x - m_sTargetPosition)<800) return gxTrue;
		return gxFalse;
	}

	bool IsStopped()
	{
		//（障害物などで）停止中か？
		if(m_pObj->mov.x == 0 && m_pObj->mov.y == 0) return gxTrue;

		return gxFalse;
	}

	//自動的に歩きタイプを（水、地上で）切り替え
	Sint32 AutoDetectWalkType();

	bool IsNodamage();

	void SetPrioDefault()
	{
		m_sPrio = PRIO_ENEMY+10;
	}

	Float32 m_fScale;	//体格
	Sint32 m_sMaxSpeed;
	gxBool m_bMoveEnd;
	gxBool m_bWaitScReenIn;
};


