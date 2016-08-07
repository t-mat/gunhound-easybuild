//-------------------------------------------------
//
//アポジモーター
//
//-------------------------------------------------
class CEffApogyMotor : public CEffectBase
{
public:
	CEffApogyMotor(Sint32 x,Sint32 y, Sint32 prio = PRIO_EFFECT , Sint32 rot = 0 , Sint32 speed = 0);
	~CEffApogyMotor();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	OBJ_POS_T m_Add;
	Sint32 m_sAlpha;
	Float32 m_fScale;
	Sint32 m_sRotation;
};

