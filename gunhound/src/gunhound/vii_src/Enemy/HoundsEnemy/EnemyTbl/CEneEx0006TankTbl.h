enum {
 enTexPageBase = enTexCEneEx0006Tank,
};
gxSprite SprCEneEx0006Tank[]={
    {enTexPageBase+0,0,0,1,1,0,0},//�_�~�[
    {enTexPageBase+0,0,0,64,32,24,16},//�C��
    {enTexPageBase+0,64,0,48,32,8,18},//�C��
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneEx0006Tank[][16]={
    {0,0,0,0,0,0,100,100,100,0},//�V�[��
    {0,0,0,0,0,0,100,100,100,1},//�C��
    {-5,-13,-1,0,0,0,100,100,100,2},//�C��
    {0,0,0,0,0,0,0,0,0,3},//0
};
enum {
    enPARENT,
    enHOUDAI,
    enHOUTOU,
    enMax,
    
};
Sint32 m_sParentCEneEx0006Tank[]={
    -1,
    enPARENT,
    enHOUDAI,
    
};
