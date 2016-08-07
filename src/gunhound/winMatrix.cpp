//-------------------------------------------------
//行列計算
//-------------------------------------------------

#include <gunhound/gga2.h>

void mtxSetUnit(void);
void mtxSetUnit2(MATRIX *m);
void mtxTrans(VECTOR *v);
void mtxRotZ(Float32 r);
void mtxRotX(Float32 r);
void mtxRotY(Float32 r);
void mtxScale(Float32 x, Float32 y, Float32 z);
void mtxAffin2(VECTOR *d, MATRIX *m, VECTOR *s);
void mtxAffin(VECTOR *d, VECTOR *s);
void mtxAffinRot(VECTOR *d, VECTOR *s);

void vctAdd(VECTOR *d, VECTOR *v1, VECTOR *v2);
void vctSub(VECTOR *d, VECTOR *v1, VECTOR *v2);
void vctMul(VECTOR *d, VECTOR *v1, VECTOR *v2);
void vctDiv(VECTOR *d, VECTOR *v1, VECTOR *v2);
void mtxMul(MATRIX *m2,MATRIX *m1);


static MATRIX	unitmtx={
	//単位行列
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f,
};

MATRIX cm;	//カレントマトリックス


void mtxSetUnit(void)
{
	cm = unitmtx;
}

void mtxSetUnit2(MATRIX *m)
{
	*m = unitmtx;
}


void mtxTrans(VECTOR *v)
{
	MATRIX	m;

	m = unitmtx;

	m._41 = v->x;
	m._42 = v->y;
	m._43 = v->z;

	mtxMul(&cm,&m);

}

void mtxRotZ(Float32 r)
{
	MATRIX	m;
	Float32	s,c;

	s = (Float32)sin(r);
	c = (Float32)cos(r);

	m = unitmtx;

	m._11 = c;
	m._12 = -s;

	m._21 = s;
	m._22 = c;

	mtxMul(&cm,&m);
}

void mtxRotX(Float32 r)
{
	MATRIX	m;
	Float32	s,c;

	s = (Float32)sin(r);
	c = (Float32)cos(r);

	m = unitmtx;

	m._22 = c;
	m._23 = -s;

	m._32 = s;
	m._33 = c;

	mtxMul(&cm,&m);

}

void mtxRotY(Float32 r)
{
	MATRIX	m;
	Float32	s,c;

	s = (Float32)sin(r);
	c = (Float32)cos(r);

	m = unitmtx;

	m._11 = c;
	m._13 = s;

	m._31 = -s;
	m._33 = c;

	mtxMul(&cm,&m);

}
void mtxScale(Float32 x, Float32 y, Float32 z)
{
	MATRIX	m;

	m = unitmtx;

	m._11 = x;
	m._22 = y;
	m._33 = z;

	mtxMul(&cm,&m);
}


void mtxAffin2(VECTOR *d, MATRIX *m, VECTOR *s)
{
	VECTOR	v;
	v.x = s->x*m->_11 + s->y*m->_21 + s->z*m->_31 + m->_41 ;
	v.y = s->x*m->_12 + s->y*m->_22 + s->z*m->_32 + m->_42 ;
	v.z = s->x*m->_13 + s->y*m->_23 + s->z*m->_33 + m->_43 ;
	*d =v;
}

void mtxAffin(VECTOR *d, VECTOR *s)
{
	VECTOR	v;
	v.x = s->x*cm._11 + s->y*cm._21 + s->z*cm._31 + cm._41 ;
	v.y = s->x*cm._12 + s->y*cm._22 + s->z*cm._32 + cm._42 ;
	v.z = s->x*cm._13 + s->y*cm._23 + s->z*cm._33 + cm._43 ;


//	v.x = v.x/(10-v.z);
//	v.y = v.y/(10-v.z);
	*d =v;
}

void mtxAffinRot(VECTOR *d, VECTOR *s)
{
	VECTOR	v;
	v.x = s->x*cm._11 + s->y*cm._21 + s->z*cm._31 ;
	v.y = s->x*cm._12 + s->y*cm._22 + s->z*cm._32 ;
	v.z = s->x*cm._13 + s->y*cm._23 + s->z*cm._33 ;
	*d =v;
}

#define	CAMKAKU		(65.0f)
#define	Z_RATE		(10.0f)
void TransPers(VECTOR *v)
{
	Float32 siya = (Float32)tan(CAMKAKU/2/180*3.1416)*2.f;

	v->x = (Float32)( WINDOW_H*v->x/(v->z*siya)+WINDOW_W/2);
	v->y = (Float32)(-WINDOW_H*v->y/(v->z*siya)+WINDOW_H/2);

//	v->rhw = 1/v->z;

	Float32 ZrateR = (1/Z_RATE);
	v->z = v->z*ZrateR;
}

void mtxcpy(MATRIX *d)
{
	*d = cm;
}

void vctUnit(VECTOR *d, VECTOR *s)
{
	Float32	l;

	*d = *s;
	l = ((Float32)1)/(Float32)sqrt(s->x*s->x + s->y*s->y + s->z*s->z);
	d->x *= l;
	d->y *= l;
	d->z *= l;
}

// 内積
Float32 vctInp(VECTOR *v1, VECTOR *v2)
{
	return (Float32)(v1->x*v2->x + v1->y*v2->y + v1->z*v2->z);
}

// 外積
void vctOutp(VECTOR *d, VECTOR *v1, VECTOR *v2)
{
	d->x = v1->y * v2->z - v1->z * v2->y;
	d->y = v1->z * v2->x - v1->x * v2->z;
	d->z = v1->x * v2->y - v1->y * v2->x;
}

//------------------------------------------------
//行列計算
//------------------------------------------------
void vctPolyNorm(VECTOR *vd,VECTOR *v1,VECTOR *v2,VECTOR *v3)
{
	VECTOR	t1,t2;

	vctSub(&t1,v2,v1);
	vctSub(&t2,v3,v1);
	vctOutp(vd,&t1,&t2);
	vctUnit(vd,vd);
}

void vctAdd(VECTOR *d, VECTOR *v1, VECTOR *v2)
{
	d->x = v1->x + v2->x;
	d->y = v1->y + v2->y;
	d->z = v1->z + v2->z;
}

void vctSub(VECTOR *d, VECTOR *v1, VECTOR *v2)
{
	d->x = v1->x - v2->x;
	d->y = v1->y - v2->y;
	d->z = v1->z - v2->z;
}

void vctMul(VECTOR *d, VECTOR *v1, VECTOR *v2)
{
	d->x = v1->x * v2->x;
	d->y = v1->y * v2->y;
	d->z = v1->z * v2->z;
}

void vctDiv(VECTOR *d, VECTOR *v1, VECTOR *v2)
{
	d->x = v1->x / v2->x;
	d->y = v1->y / v2->y;
	d->z = v1->z / v2->z;
}

void mtxMul(MATRIX *m2,MATRIX *m1)
{
	MATRIX	m;

	m._11 = (m1->_11*m2->_11) + (m1->_12*m2->_21) + (m1->_13*m2->_31) + (m1->_14*m2->_41) ;
	m._12 = (m1->_11*m2->_12) + (m1->_12*m2->_22) + (m1->_13*m2->_32) + (m1->_14*m2->_42) ;
	m._13 = (m1->_11*m2->_13) + (m1->_12*m2->_23) + (m1->_13*m2->_33) + (m1->_14*m2->_43) ;
	m._14 = (m1->_11*m2->_14) + (m1->_12*m2->_24) + (m1->_13*m2->_34) + (m1->_14*m2->_44) ;

	m._21 = (m1->_21*m2->_11) + (m1->_22*m2->_21) + (m1->_23*m2->_31) + (m1->_24*m2->_41) ;
	m._22 = (m1->_21*m2->_12) + (m1->_22*m2->_22) + (m1->_23*m2->_32) + (m1->_24*m2->_42) ;
	m._23 = (m1->_21*m2->_13) + (m1->_22*m2->_23) + (m1->_23*m2->_33) + (m1->_24*m2->_43) ;
	m._24 = (m1->_21*m2->_14) + (m1->_22*m2->_24) + (m1->_23*m2->_34) + (m1->_24*m2->_44) ;

	m._31 = (m1->_31*m2->_11) + (m1->_32*m2->_21) + (m1->_33*m2->_31) + (m1->_34*m2->_41) ;
	m._32 = (m1->_31*m2->_12) + (m1->_32*m2->_22) + (m1->_33*m2->_32) + (m1->_34*m2->_42) ;
	m._33 = (m1->_31*m2->_13) + (m1->_32*m2->_23) + (m1->_33*m2->_33) + (m1->_34*m2->_43) ;
	m._34 = (m1->_31*m2->_14) + (m1->_32*m2->_24) + (m1->_33*m2->_34) + (m1->_34*m2->_44) ;

	m._41 = (m1->_41*m2->_11) + (m1->_42*m2->_21) + (m1->_43*m2->_31) + (m1->_44*m2->_41) ;
	m._42 = (m1->_41*m2->_12) + (m1->_42*m2->_22) + (m1->_43*m2->_32) + (m1->_44*m2->_42) ;
	m._43 = (m1->_41*m2->_13) + (m1->_42*m2->_23) + (m1->_43*m2->_33) + (m1->_44*m2->_43) ;
	m._44 = (m1->_41*m2->_14) + (m1->_42*m2->_24) + (m1->_43*m2->_34) + (m1->_44*m2->_44) ;

	*m2 = m;

}

