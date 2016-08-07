//----------------------------------------------------------------
//
// openCVを使用したムービーデコーダー
//
//----------------------------------------------------------------
#include <gxLib.h>
#include <gxLib/util/CFileTarga.h>
#include <gxLib/gxMovieManager.h>

#ifdef GX_BUILD_OPTIONx64
//64bit版しか動画に対応してないよ

	#include <opencv2/opencv.hpp>
	#include <opencv2/highgui/highgui.hpp>

	#ifdef GX_DEBUG
		#pragma comment(lib,"openCV/opencv_world310d.lib")
	#else
		#pragma comment(lib,"openCV/opencv_world310.lib")
	#endif


using namespace cv;
using namespace std;

class CCVFile
{
public:

	CCVFile()
	{
		m_pVideo = new cv::VideoCapture();
		m_pFrame = new Mat();
		m_uOldFrm = 0;
	}

	~CCVFile()
	{
		if( m_pVideo )
		{
			delete m_pVideo;
			delete m_pFrame;

		}
		m_pVideo = NULL;
	}

	void Renew()
	{
		delete m_pVideo;
		m_pVideo = new cv::VideoCapture();
		m_pFrame = new Mat();
	}

	Uint32 m_uOldFrm;

	cv::VideoCapture *m_pVideo;
	Mat *m_pFrame;
};


class CCvManager
{

public:

	CCvManager()
	{
		m_sCounter = 0;
	}

	~CCvManager()
	{

	}

	CCVFile *GetMovie( Sint32 index )
	{
		return &m_Video[index];
	}

	void Action();

	Sint32 GetCounter()
	{
		return m_sCounter;
	}

	SINGLETON_DECLARE(CCvManager);

private:
	Sint32 m_sCounter;

	CCVFile m_Video[CMovieManager::enMovieFileMax];
};

SINGLETON_DECLARE_INSTANCE( CCvManager );

gxBool loadMovie( Sint32 n , gxChar *pFileName )
{
	CCvManager::GetInstance()->GetMovie(n)->m_pVideo->open( pFileName );
	if( CCvManager::GetInstance()->GetMovie(n)->m_pVideo->isOpened() ) return gxFalse;
	return gxTrue;
}

void CCvManager::Action()
{
	m_sCounter ++;
}

void actionMovie( Sint32 n )
{
	if( !CCvManager::GetInstance()->GetMovie( n )->m_pVideo->grab() )
	{
		//grabするごとにフレームが更新される
		return;
	}

	Mat *pMat = CCvManager::GetInstance()->GetMovie(n)->m_pFrame;

	CCvManager::GetInstance()->GetMovie( n )->m_pVideo->retrieve( *pMat , 0);

	CMovieManager::GetInstance()->GetMovie(n)->pData = pMat->data;
}


void Movie()
{
	CCvManager::GetInstance()->Action();

	for( Sint32 ii=0; ii<CMovieManager::enMovieFileMax; ii++ )
	{
		CMovieFile *pMovie = CMovieManager::GetInstance()->GetMovie( ii );

		pMovie->bUpdate = gxFalse;

		if( pMovie->m_Stat&CMovieManager::enMovieReqLoad )
		{
			pMovie->m_Stat = BIT_OFF( pMovie->m_Stat , CMovieManager::enMovieExist );

			CCvManager::GetInstance()->GetMovie(ii)->Renew();

			CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->open( pMovie->m_FileName );

			if( CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->isOpened() )
			{
				pMovie->m_Stat = BIT_ON( pMovie->m_Stat , CMovieManager::enMovieExist );

				//1フレームだけごまかす
				pMovie->width = CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->get(CAP_PROP_FRAME_WIDTH);
				pMovie->height = CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->get(CAP_PROP_FRAME_HEIGHT);

				pMovie->fps = CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->get(CAP_PROP_FPS);
				pMovie->max = CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->get(CAP_PROP_FRAME_COUNT);

			}
		}

		//double test;
		//test = CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->get(CAP_PROP_POS_MSEC);
		//test = CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->get(CAP_PROP_POS_AVI_RATIO);
		if( pMovie->m_Stat&CMovieManager::enMovieReqStop )
		{
			pMovie->m_Stat = BIT_OFF( pMovie->m_Stat , CMovieManager::enMovieReqPlay );
			pMovie->m_Stat = BIT_OFF( pMovie->m_Stat , CMovieManager::enMovieReqStop );

			//gxLib::StopAudio( pMovie->GetMusicIndex() );
		}

		Uint32 nowFrm = 0;

		if( pMovie->m_Stat&CMovieManager::enMovieReqPlay )
		{
			Uint32 nowTime = gxLib::GetTime();
			nowTime = (nowTime - pMovie->m_BeganTime);

			if (pMovie->m_Stat&CMovieManager::enMovieReqPosition)
			{
				pMovie->m_Stat = BIT_OFF(pMovie->m_Stat, CMovieManager::enMovieReqPosition);
				CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->set(CAP_PROP_POS_FRAMES, pMovie->JumpFrm);
				nowFrm = pMovie->JumpFrm;
			}

			if( pMovie->m_Stat&CMovieManager::enMovieReqAutoFrm )
			{
				//自動フレーム調整（処理落ちしても時間で音楽に合わせる）
				Uint32 mSec = 1000 / 60 * 60 / (pMovie->fps-1.0f);

				nowFrm = nowTime / mSec;

				//CCvManager::GetInstance()->GetMovie( ii )->m_pVideo->set( CAP_PROP_POS_FRAMES , nowFrm );
			}
			else
			{
				//特定フレームを表示する
				//CCvManager::GetInstance()->GetMovie( ii )->m_pVideo->set( CAP_PROP_POS_FRAMES , pMovie->now + 1 );
				//nowFrm = pMovie->now;// + 1;
				{
					nowFrm = pMovie->JumpFrm;
				}
			}

			if (nowFrm >= pMovie->max) nowFrm = pMovie->max;

			CCvManager::GetInstance()->GetMovie( ii )->m_pVideo->set( CAP_PROP_POS_FRAMES , nowFrm );

			if( /*pMovie->now*/nowFrm >= pMovie->max )
			{
				//ループ処理

				if( pMovie->m_Stat&CMovieManager::enMovieReqLoop )
				{
					CCvManager::GetInstance()->GetMovie( ii )->m_pVideo->set( CAP_PROP_POS_FRAMES , 0 );
					if( pMovie->IsMusic() )
					{
						gxLib::StopAudio( pMovie->GetMusicIndex() );
						gxLib::PlayAudio( pMovie->GetMusicIndex() , gxFalse );
					}
					pMovie->m_BeganTime = gxLib::GetTime();
				}
			}

			//if( CCvManager::GetInstance()->GetCounter() % ( 60 / pMovie->fps ) == 0 )
			if( pMovie->now != nowFrm )
			{
				actionMovie( ii );
				pMovie->bUpdate = gxTrue;
			}

			pMovie->now = CCvManager::GetInstance()->GetMovie(ii)->m_pVideo->get(CAP_PROP_POS_FRAMES);

		}

	}
}

#else

	void Movie(){}

#endif
