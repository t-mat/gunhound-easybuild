#ifndef _CPENDURAM_H_
#define _CPENDURAM_H_

//-----------------------------------------------------
//振り子（っぽい）シミュレーション
//-----------------------------------------------------
class CPenduram
{
	enum
	{
		enPenduramWeight = 5000,
	};

public:

	CPenduram();
	~CPenduram();

	Float32 GetRotation();
	void SetWeight(Sint32 per);
	void Set(Sint32 n);
	void Add(Sint32 n);
	void Action();
	void Draw();
	void SetInfinity()
	{
		m_bInfinity = gxTrue;
	}
private:
	Float32 m_fRotation;
	Float32 m_fRotationAdd;
	Float32 m_fScale;
	Float32 m_fMasatsu;
	Float32 m_fWeight;

	gxBool m_bInfinity;
};

#endif	//_CPENDURAM_H_
