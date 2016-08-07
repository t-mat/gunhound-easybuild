enum enSoldierCommandType {
	enSoldierCommandFuttobi,
};

enum enSoldierWalkType {
	enWalkTypeNormal,
	enWalkTypeEscape,
	enWalkTypeDrawn,
};

enum enMoveType {

	enMoveTypeWait=0,		//�w���҂�
	enMoveTypeMove,			//�ڕW�n�_�ֈړ�(����)
	enMoveTypeFlyUp,		//�ӂ��Ƃ�(����)
	enMoveTypeFlyDown,		//�ӂ��Ƃ�(���Ԃ�)
	enMoveTypeDownUp,		//�_�E��(����)
	enMoveTypeDownDown,		//�_�E��(���Ԃ�)
	enMoveTypeBackRolling,	//��]
	enMoveTypeFrontRolling,	//�O�]
	enMoveTypeGetUpU,		//�N���オ��i�����j
	enMoveTypeGetUpD,		//�N���オ��i���Ԃ��j
	enMoveTypeEscape,		//������
	enMoveTypeDrawn,		//�M���
	enMoveTypeFrontFalling,	//����

	enMoveTypeFrontJump,	//�W�����v�i�������j
	enMoveTypeFrontLand,	//���n�i�������j
	enMoveTypeClimb,		//���

	enMoveTypeRideOffHeli,	//�w������~���

	enMoveTypeTrgtGun,		//�e���\����
	enMoveTypeShottGun,		//�e������
	enMoveTypeRideOn,		//�g���b�N�ւ̂�
	enMoveTypeRideOff,		//�g���b�N����~���
	enMoveTypeRopeUp,		//���[�v���̂ڂ�
	enMoveTypeLadderDown,	//���[�v���~���
	enMoveTypeLadderUp,		//�͂���������
	enMoveTypeSearch,		//�T��
	enMoveTypeThrowBomb,	//������
	enMoveTypeTrgtBuz,		//�o�Y�[�J���\����
	enMoveTypeShotBuz,		//�o�Y�[�J������
	enMoveTypeShotMG,		//�@�e�C���Ō���
	enMoveTypeSupportStand,	//�e��T�|�[�g(����)
	enMoveTypeSupportSit,	//�e��T�|�[�g(�����)
	enMoveTypePush,			//����
	enMoveTypePull,			//����

	enMoveTypeBombSet,		//���e�Z�b�g
	enMoveTypeBombSwitch,	//���e�X�C�b�`
	enMoveTypeLeading,		//���������[�h

	enMoveTypeDirectAssault,//�ˌ����w��
	enMoveTypeDirectEscape,	//�ދp���w��

	enMoveTypeEscapeMachine,//����ĒE�o

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

	virtual void MoveCommon();			//�ړ����䋤��

	enMoveType eMoveType;

	//------------------------------------------------------
	void Command(Sint32 n);



	//------------------------------------------------------

	virtual void MoveTypeWait();		//�w���҂�
	virtual void MoveTypeMove();		//�ڕW�n�_�ֈړ�(����)
	virtual void MoveTypeFlyUp();		//�ӂ��Ƃ�(����)
	virtual void MoveTypeFlyDown();		//�ӂ��Ƃ�(���Ԃ�)
	virtual void MoveTypeGetUpD();		//�N���オ��(���Ԃ�)
	virtual void MoveTypeGetUpU();		//�N���オ��(����)
	virtual void MoveTypeDownUp();		//�_�E��(����)
	virtual void MoveTypeDownDown();	//�_�E��(���Ԃ�)
	virtual gxBool MoveTypeClimb();		//�i�������

	virtual void MoveTypeRideOffHeli();	//�w������~���
	virtual void MoveTypeRideOn();		//�g���b�N�ւ̂�
	virtual void MoveTypeRideOff();		//�g���b�N����~���
	virtual void MoveTypeRopeUp();		//���[�v���̂ڂ�
	virtual void MoveTypeLadderDown();	//���[�v���~���
	virtual void MoveTypeLadderUp();	//�͂���������
	virtual void MoveTypeSearch();		//�T��
	virtual void MoveTypeThrowBomb();	//������
	virtual void MoveTypeTrgtGun();		//�e���\����
	virtual void MoveTypeShottGun();	//�e������
	virtual void MoveTypeTrgtBuz();		//�o�Y�[�J���\����
	virtual void MoveTypeShotBuz();		//�o�Y�[�J������
	virtual void MoveTypeShotMG();		//�@�e�C���Ō���
	virtual void MoveTypeEscape();		//������
	virtual void MoveTypeDrawn();		//�M���
	virtual void MoveTypeSupportStand();//�e��T�|�[�g(����)
	virtual void MoveTypeSupportSit();	//�e��T�|�[�g(�����)
	virtual void MoveTypePush();		//����
	virtual void MoveTypePull();		//����

	virtual void MoveTypeBombSet();		//���e�Z�b�g
	virtual void MoveTypeBombSwitch();	//���e�X�C�b�`
	virtual void MoveTypeLeading();		//���������[�h

	virtual void MoveTypeDirectAssault();	//�ˌ����w��
	virtual void MoveTypeDirectEscape();	//�ދp���w��

	virtual void MoveTypeBackRolling();		//��]
	virtual void MoveTypeFrontRolling();	//�O�]
	virtual void MoveTypeFalling();			//����
	virtual void MoveTypeEscapeMachine();	//�E�o

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
	Sint32 m_sTargetPosition;	//�ړ���|�W�V����
	Sint16 m_sJumpDamageTimer;	//�ӂ��Ƃу_���[�W�^�C�}�[
	Sint16 m_sDownTimer;		//�_�E���^�C�}�[

	Sint8 m_sWalkType;			//�������A���ʁA������A���ڂ��

	Sint32 m_sPositionX,m_sPositionY;
	gxSprite *m_pSprite;

	ghPos m_stOffset;			//�\���I�t�Z�b�g

	void DebugControl();

	bool IsLanding()
	{
		//���n���Ă��邩�H
		if( m_sGravit == 0) return gxTrue;
		return gxFalse;
	}

	int GetSpritePattern(int _cnt,int _frm, int _ptnNum ,bool _bNoLoop = gxTrue )
	{
		//�A�j���̃p�^�[�����Z�o
		int max;

		if( !_bNoLoop )
		{
			//���[�v���Ȃ��ꍇ�͍Ō�̃t���[���łƂ߂Ă���
			if( _cnt >= _frm*_ptnNum ) _cnt = _frm*_ptnNum-1;
		}

		max = (_cnt+(_frm*_ptnNum))%(_frm*_ptnNum);

		return max/_frm;
	}

	bool MoveSpeedDecrement()
	{
		//�ړ����x����

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
		m_sJumpDamageTimer = 0;		//�_���[�W�^�C�}�[��������
		m_sDownTimer = 0;			//�_�E�����Ԃ�������
	}

	bool IsTargetPosition()
	{
		if(abs(m_pObj->pos.x - m_sTargetPosition)<800) return gxTrue;
		return gxFalse;
	}

	bool IsStopped()
	{
		//�i��Q���ȂǂŁj��~�����H
		if(m_pObj->mov.x == 0 && m_pObj->mov.y == 0) return gxTrue;

		return gxFalse;
	}

	//�����I�ɕ����^�C�v���i���A�n��Łj�؂�ւ�
	Sint32 AutoDetectWalkType();

	bool IsNodamage();

	void SetPrioDefault()
	{
		m_sPrio = PRIO_ENEMY+10;
	}

	Float32 m_fScale;	//�̊i
	Sint32 m_sMaxSpeed;
	gxBool m_bMoveEnd;
	gxBool m_bWaitScReenIn;
};


