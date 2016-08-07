class CMechBone;
class CCustomHound
{

public:

	CCustomHound();
	~CCustomHound();

	void Action();
	void Draw();
	void SetBodyColor( CMechBone *pMech=NULL ,Sint32 col1=-1 , Sint32 col2=-1);

	void SetDestroy();
	gxBool IsDead();
	void Clear();

private:

	Sint32 m_sAlpha;
	Sint32 m_sScrollW;
	Sint32 m_sSequence;
	Sint32 m_sSelect;
	Sint32 m_sSelectYoko;
	Sint32 m_sColoringMax[2];
	StHoundConfig m_stConfig;
	Sint32 m_sWeaponSelect[4];
	Sint32 m_sTempWeaponSelect[4];
	Float32 m_fWindowSlideX;

	gxBool m_bEnd;
	gxBool m_bDestroy;

	CMechBone *m_pMech;

	char *m_pAnnounce;

	void Init();

	void MenuMain();
	void MenuWeaponSet();
	void DrawMain();
	void DrawWeaponSet();
	char *GetWeaponName(Sint32 sID );
	Sint32 GetWeaponNameIndex( Sint32 sID );

	Sint32 GetWeaponIndex(Sint32 *pTbl , Sint32 sID );

};
