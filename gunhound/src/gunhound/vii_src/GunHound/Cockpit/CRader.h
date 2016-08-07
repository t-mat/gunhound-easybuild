enum enTypeMarker
{
	enTypeMarkerBuilding,

	enTypeMarkerPlayer,
	enTypeMarkerAssault,
	enTypeMarkerGroundObject,
	enTypeMarkerFlyingObject,
	enTypeMarkerGroundPartner,
	enTypeMarkerFlyingPartner,
	enTypeMarkerMiddleBoss,
	enTypeMarkerDefenceTarget,
	enTypeMarkerOfenceTarget,

/*
	enTypeMarkerAsPlayer,
	enTypeMarkerAsCommander,
	enTypeMarkerAsOther,
	enTypeMarkerAsOther2,
	enTypeMarkerGroundEnemy,
	enTypeMarkerGroundEnemy2,
	enTypeMarkerGroundEnemy3,
	enTypeMarkerGroundEnemy4,
	enTypeMarkerFlyingEnemy,
	enTypeMarkerFlyingEnemy2,
	enTypeMarkerFlyingEnemy3,
	enTypeMarkerFlyingEnemy4,
*/

};

enum {
	enMarkerMax = 128,
};

typedef struct StMarker
{
	Sint32 x,y;
	enTypeMarker enType;
}StMarker;

class CRaderControl
{
public:

	CRaderControl();
	~CRaderControl();
	void Action();
	void Draw( Sint32 bx , Sint32 by );
	void SetMarker( enTypeMarker eMark , Sint32 x , Sint32 y );

	static CRaderControl* GetInstance()
	{
		return g_sCRaderControl;
	}

	void ResetMarkerCnt()
	{
		m_sMarkerCnt = 0;
	}

	void SetOperationRange(Sint32 x1, Sint32 x2)
	{
		m_sRangeX1 = x1;
		m_sRangeX2 = x2;
	}

	void DrawTime();

private:

	static CRaderControl *g_sCRaderControl;
	Sint32 m_sMarkerCnt;
	StMarker stMark[enMarkerMax];

	Sint32 m_sRangeX1;
	Sint32 m_sRangeX2;


};

