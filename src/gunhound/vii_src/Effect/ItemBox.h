#ifndef _ITEMBOX_H_
#define _ITEMBOX_H_

enum {
	enItemPowerChip,
	enItemWeaponChip,
	enItemEnergyChip,
};

/*
enum EnItemStatus {
	enStatBox,
	enStatChip,
};
*/

class CItemBox : public CAction
{

public:
	CItemBox(Sint32 x,Sint32 y, Sint32 itemType);
	~CItemBox();

//	void Action();
	void Draw();

//	void SeqInit();
	void SeqMain();
	void SeqCrash();
//	void SeqEnd();

private:

	Sint32 m_sStatItem;
	CHitCollision m_HitKurai;

	OBJ_POS_T m_Pos;
};


class CItemChip : public CAction
{

public:
	CItemChip(Sint32 x,Sint32 y, Sint32 itemType);
	~CItemChip();

//	void Action();
	void SeqMain();

	void Draw();

private:

	gxBool isGet();

	Sint32 m_sSprite;
	Sint32 m_sStatItem;
	OBJ_POS_T m_Pos;
	gxBool m_bChakuti;
};


#endif

