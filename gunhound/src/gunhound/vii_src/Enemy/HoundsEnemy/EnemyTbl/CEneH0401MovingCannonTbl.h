enum {
 enTexPageBase = enTexCEneH0401MovingCannon,
};
gxSprite SprCEneH0401MovingCannon[]={
    {enTexPageBase+0,0,0,1,1,0,0},//�_�~�[
    {enTexPageBase+0,0,0,32,16,0,7},//�C��
    {enTexPageBase+0,40,0,64,40,24,32},//����
    {enTexPageBase+0,0,40,72,16,40,8},//�L���^�s���[�P
    {enTexPageBase+0,0,64,72,16,40,8},//�L���^�s���[�Q
    {enTexPageBase+0,0,88,72,16,40,8},//�L���^�s���[�R
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0401MovingCannon[][16]={
    {160,200,0,0,0,0,100,100,100,0},//�V�[��
    {-10,-20,0,0,0,0,100,100,100,2},//���̂P
    {40,-9,-1,0,0,0,100,100,100,1},//�C��P�P
    {0,-9,1,0,0,0,100,100,100,3},//�L���^�s���[�P�P
    {0,-9,1,0,0,0,0,100,100,4},//�L���^�s���[�Q�Q
    {0,-9,1,0,0,0,0,100,100,5},//�L���^�s���[�R�Q
    {0,0,0,0,0,0,0,0,0,6},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enHOUDAI,
    enKYATAPIRA1,
    enKYATAPIRA2,
    enKYATAPIRA3,
    enMax,
    
};
Sint32 m_sParentCEneH0401MovingCannon[]={
    -1,
    enPARENT,
    enDOUTAI,
    enPARENT,
    enPARENT,
    enPARENT,
    
};
