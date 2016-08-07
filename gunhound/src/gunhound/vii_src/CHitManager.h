//#define HITTEST_MAX (1024)
#define ID_VERSIS_ATK (enCollisionID_A_Atack)
#define ID_VERSIS_DEF (enCollisionID_A_Defence)
#define ID_ENEMY_ATK  (enCollisionID_B_Atack)
#define ID_ENEMY_DEF  (enCollisionID_B_Defence)

typedef struct StHitObjSml{
	Sint32 x,y,id;
}StHitObjSml;

class CHitCollision;

class CHitManager : public CCollisionManager
{

public:


	CHitManager()
	{
		m_sOldHitMax = 0;
	}

	~CHitManager()
	{

	}

	void init()
	{

	}

	void Action();

	Sint32 GetOldMax()
	{
		return m_sOldHitMax;
	}

	Sint32 GetOldID( Sint32 n )
	{
		//@HIT@
		return m_stHit[n].id;//ID_ENEMY_DEF;
	}

/*
	Sint32 GetID( Sint32 n )
	{
		//@HIT@
		return 0;
	}
*/

	Sint32 GetOldX( Sint32 n )
	{
		//@HIT@
		n = ( n >= 0 && n <= m_sOldHitMax)? n : -1;
		return m_stHit[n].x;//*100;
	}

	Sint32 GetOldY( Sint32 n )
	{
		//@HIT@
		n = ( n >= 0 && n <= m_sOldHitMax)? n : -1;
		return m_stHit[n].y;//*100;
	}


private:
	void ClearHitCount()
	{
		//���d�v
		m_sMaxCollision = 0;
	}

	gxBool HitCheck( Sint32 i,Sint32 j );
	void DrawDebug(Sint32 n);

	//@HIT@ old_hit[i].id
	Sint32 m_sOldHitMax;
	StHitObjSml m_stHit[MaxCollisionNum];
//	CHitCollision *m_pHitCollision[MaxCollisionNum];
};


class CHitCollision : public CCollision
{
public:
	CHitCollision()
	{
		m_bJudge = gxFalse;

		m_sForceID = 0;			//�����̍U���A�G�̍U���A�����̂��炢�A�G�̂��炢
		x1=y1=x2=y2=0;			//������i���炢�j����p��`
		ax1=ay1=ax2=ay2=0;			//������i���炢�j����p��`
		dx = dy = 0;
		m_sAtackType = 0;		//���������ꏊ�̍����ۑ��px,y,�U���^�C�v
		m_sHP = 1;				//�̗�
		m_sAP = 1;				//�U����
		m_bImpact = gxFalse;	//���������u�Ԃɂ������t���O
		m_sDamageCount = 0;		//�_���[�W�p�J�E���^
		m_bFisrtDead = gxFalse;
		m_bFlag = gxFalse;		//�Q�x������h�~�p�t���O
		m_bMuteki = gxFalse;
		m_bHomingable = gxTrue;
	}

	~CHitCollision()
	{

	}

	void init()
	{

	}

	void DisableAiming()
	{
		m_bHomingable = gxFalse;
	}

	gxBool is_damage()
	{
		//�_���[�W�\�������H

		if( m_sDamageCount > 0 )
		{
			m_sDamageCount --;
			return gxTrue;
		}
		return gxFalse;
	}

	gxBool is_dead()
	{
		//�̗͐؂�Ă��邩�H
		if( m_sHP <= 0 ) return gxTrue;
		return gxFalse;
	}

	void SetAtackType( Sint32 type )
	{
		m_sAtackType = type;
	}

	Sint32 IsAtackType( Sint32 type )
	{
		return ( m_sAtackType == type)? gxTrue : gxFalse;
	}

	gxBool IsImpacted()
	{
		return m_bImpact;
	}

	void SetOffImpact()
	{
		m_bImpact = gxFalse;
	}

	void set_hp(Sint32 n)
	{
		//�̗͐ݒ�
		m_sMax = n*100;
		m_sHP = n*100;
	}
	Sint32 get_hp()
	{
		//�̗͐ݒ�
		return m_sHP/100;
	}

	void set_ap(Sint32 n)
	{
		m_sAP = n*100;
	}

	Sint32 GetTotalDamage()
	{
		Sint32 n = (m_sMax-m_sHP)/100;
		if( n <= 0 ) n= 0;
		if( n >= m_sMax/100 ) n = m_sMax/100;
		return n;
	}

	void SetDirectHitPosition(Sint32 n)
	{
		m_sHP = n;
	}

	Sint32 GetDirectHitPosition()
	{
		return m_sHP;
	}

	void SetDirectAromorPosition(Sint32 n)
	{
		m_sAP = n;
	}

	void Activate()
	{
		//�����蔻���L���ɂ���
		m_bJudge = gxTrue;
	}

	Sint32 GetDamageDir()
	{
		//�_���[�W������Ԃ�
		return 1;
	}

	gxBool isFirstdead()
	{
		//�����񂾏u�Ԃ��H
		return m_bFisrtDead;
	}

	void SetMutekiOneTime()
	{
		m_bMuteki = gxTrue;
	}


	void SetHantei(Sint32 id , OBJ_POS_T *pPos)
	{
		m_sForceID = id;

		x1 = pPos->x/100 + ax1;
		y1 = pPos->y/100 + ay1;
		x2 = pPos->x/100 + ax2;
		y2 = pPos->y/100 + ay2;

		Sint32 tmpx,tmpy;
		tmpx = x2;
		tmpy = y2;

		if( x2<x1 )
		{
			x2 = x1;
			x1 = tmpx;
		}

		if( y2<y1 )
		{
			y2 = y1;
			y1 = tmpy;
		}

		Activate();
	}

	void Set(Sint32 id , OBJ_POS_T *pPos)
	{
		m_sForceID = id;

		if( m_sForceID&(ID_VERSIS_ATK|ID_ENEMY_ATK) )
		{
			x1 = pPos->x/100+pPos->ax1;
			y1 = pPos->y/100+pPos->ay1;
			x2 = pPos->x/100+pPos->ax2;
			y2 = pPos->y/100+pPos->ay2;
		}
		else
		{
			x1 = pPos->x/100+pPos->kx1;
			y1 = pPos->y/100+pPos->ky1;
			x2 = pPos->x/100+pPos->kx2;
			y2 = pPos->y/100+pPos->ky2;
		}
	}

	Sint32  m_sForceID;		//�����̍U���A�G�̍U���A�����̂��炢�A�G�̂��炢
	Sint32  x1,y1,x2,y2;		//������i���炢�j����p��`
	Sint32  ax1,ay1,ax2,ay2;	//������i���炢�j����p�͈�
	Sint32  dx,dy;			//���������ꏊ�̍����ۑ��px,y
	Sint32  m_sAtackType;	//�U���^�C�v
	Sint32  m_sMax;			//�̗�
	Sint32  m_sHP;			//�̗�
	Sint32  m_sAP;			//�U����
	Sint32  m_sDamageCount;//�_���[�W�p�J�E���^
	gxBool m_bImpact;		//���������u�Ԃɂ������t���O����x�ł�������Η���
	gxBool m_bFisrtDead;	//�̗͂��Ȃ��Ȃ����t���[���̂ݗ���
	gxBool m_bFlag;		//�Q�x������h�~�p�t���O
	gxBool m_bJudge;		//�����L���ɂ���
	gxBool m_bMuteki;
	gxBool m_bHomingable;	//�z�[�~���O�ł���
private:

};

