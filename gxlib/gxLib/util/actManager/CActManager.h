//--------------------------------------------------
//
// CActManager.h
// タスク管理を行います
//
//--------------------------------------------------

#ifndef _CACTMANAGER_H_
#define _CACTMANAGER_H_
#define ACTION_MAX (1024*12)	//(2048+512)

enum {
	enActionPrioAll,
	enActionPrio00,
	enActionPrio10,
	enActionPrio20,
	enActionPrio30,
	enActionPrio40,
	enActionPrio50,
	enActionPrio60,
	enActionPrio70,
	enActionPrio80,
	enActionPrio90,
	enActionPrioDefault = enActionPrio50,
};

enum {
	enActionSeqInit,
	enActionSeqMain,
	enActionSeqCrash,
	enActionSeqEnd,
};

class CActManager;

class CAction {

public:
	CAction();
	virtual ~CAction();
	virtual void Action();
	virtual void SeqInit();
	virtual void SeqMain();
	virtual void SeqCrash();
	virtual void SeqEnd();
	virtual void Draw()   = 0;

	void SetID(int n)	{	_ID = n;	}

	void SetPosition(int type,int x,int y,int *arg){
		//-----------------------------------------
		//Actionオブジェクトの種類、座標を設定する
		//-----------------------------------------
		_x = x;
		_y = y;

		if(arg == NULL) return;

		for(int i=0;i<10;i++)
		{
			_arg[i] = arg[i];
		}
	}

	void SetPrio( int sActPriority )
	{
		m_sActionPrio = sActPriority;
	}

	bool IsInit()
	{
		return (m_sTimer==0)? gxTrue : gxFalse;
	}
	void TimerAdd()
	{
		m_sTimer ++;
	}

	int GetActionSeq()
	{
		return m_sActionSeq;
	}

	void SetActionSeq(int n)
	{
		m_sActionSeq = n;
	}

	bool IsSeqEnd()
	{
		if( enActionSeqEnd == m_sActionSeq) return gxTrue;
		return gxFalse;
	}

	int GetPrio()
	{
		return m_sActionPrio;
	}

	void Destroy()
	{
		m_bDestroy  = true;
	}
	bool IsDestroy()
	{
		return m_bDestroy;
	}

	int _x,_y;

private:

	int _ID;

	int _type;
	int _arg[10];
	int m_sActionSeq;
	int m_sActionPrio;
	bool m_bDestroy;
protected:

	int m_sTimer;
};


class CActManager 
{
private:

	Sint32 m_sActionNum;
	CAction *pAction[ACTION_MAX];
	gxBool m_bPause;

public:

	CActManager() {
		m_bPause = gxFalse;
		m_sActionNum = 0;
		for(int i=0;i<ACTION_MAX;i++)
		{
			pAction[i] = NULL;
		}
	}

	~CActManager() {
		for(int i=0;i<ACTION_MAX;i++) {
			if(pAction[i]) delete pAction[i];
			pAction[i] = NULL;
		}
	}

	void Regist(CAction* pAct);

	void UnRegist(int id) {
		if(pAction[id]) {
			pAction[id] = NULL;
		}
	}

	void SetPause(gxBool bOn = gxTrue)
	{
		m_bPause = bOn;
	}

	gxBool IsPause()
	{
		return m_bPause;
	}

	void Clear(int sActPriority = enActionPrioAll );

	void Action() {
		int i;
		m_sActionNum = 0;
		for(i=0;i<ACTION_MAX;i++) {
			if(pAction[i]) {
//				if(pAction[i]->IsInit()){
//					pAction[i]->Init();
//				}
				if( !pAction[i]->IsDestroy() ) pAction[i]->Action();

				if( pAction[i]->IsSeqEnd() || pAction[i]->IsDestroy() )
				{
					delete pAction[i];
					pAction[i] = NULL;
					continue;
				}

				if(pAction[i]) pAction[i]->TimerAdd();
				m_sActionNum ++;
			}
		}
	}

	void Draw() {
		int i;
		for(i=0;i<ACTION_MAX;i++) {
			if(pAction[i])
			{
				if(pAction[i]->IsInit())
				{
					continue;
				}
				pAction[i]->Draw();
			}
		}
	}

	Sint32 GetNum()
	{
		//アクションはいくつ登録されているのか？
		return m_sActionNum;
	}

	SINGLETON_DECLARE( CActManager )

};

//extern CActManager *g_pActManager;
#endif
