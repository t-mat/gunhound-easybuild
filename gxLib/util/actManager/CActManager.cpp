//--------------------------------------------------
//
// CActManager.cpp
// タスク管理を行います
//
//--------------------------------------------------

#include <gxLib.h>
#include "CActManager.h"

SINGLETON_DECLARE_INSTANCE( CActManager );

//CActManager *g_pActManager;

CAction::CAction()
{
	m_sActionSeq = enActionSeqInit;
	CActManager::GetInstance()->Regist(this);
	m_sTimer = 0;
	m_sActionPrio = enActionPrio50;
	m_bDestroy = false;
}

CAction::~CAction()
{
	CActManager::GetInstance()->UnRegist(_ID);
}


void CAction::Action()
{
	switch ( GetActionSeq( ) ) {
	case enActionSeqInit:
		SeqInit();
		break;
	case enActionSeqMain:
		SeqMain();
		break;
	case enActionSeqCrash:
		SeqCrash();
		break;
	case enActionSeqEnd:
		SeqEnd();
		break;
	}

}


void CAction::SeqInit()
{
	SetActionSeq( enActionSeqMain );
}


void CAction::SeqMain()
{
	SetActionSeq( enActionSeqCrash );
}


void CAction::SeqCrash()
{
	SetActionSeq( enActionSeqEnd );
}

void CAction::SeqEnd()
{

}

void CActManager::Clear(int sActPriority)
{
	//特定優先順位”以下”は全て削除します

	for(int i=0;i<ACTION_MAX;i++)
	{
		if( pAction[i] )
		{
			if( pAction[i]->GetPrio() <= sActPriority || sActPriority == enActionPrioAll)
			{
				pAction[i]->Destroy();
			}
		}
	}
}

void CActManager::Regist(CAction* pAct)
{
	//-----------------------------------
	//アクションマネージャに登録を行う
	//-----------------------------------
	Sint32 i;

	for(i=0;i<ACTION_MAX;i++)
	{
		if(pAction[i] == NULL)
		{
			pAction[i] = pAct;
			pAction[i]->SetID( i );
			return;
		}
	}

#ifdef GX_DEBUG
	gxLib::DebugLog( "Action登録に充分な空きがない" );
#endif

}


