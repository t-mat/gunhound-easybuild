enum {
 enTexPageBase = enTexCEneEx0002Laser,
};
gxSprite SprCEneEx0002Laser[]={
    {enTexPageBase+0,0,0,1,1,0,0},//�_�~�[
    {enTexPageBase+0,0,0,48,32,16,12},//�΋�C��
    {enTexPageBase+0,48,0,48,32,12,24},//�΋�C��
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneEx0002Laser[][16]={
    {0,0,0,0,0,0,100,100,100,0},//�V�[��
    {0,0,0,0,0,0,100,100,100,1},//�΋�C��
    {4,0,-1,0,0,0,100,100,100,2},//�΋�C��
    {0,0,0,0,0,0,0,0,0,3},//0
};
enum {
    enPARENT,
    enTAIKUHOUNE,
    enTAIKUHOUSAKI,
    enMax,
    
};
Sint32 m_sParentCEneEx0002Laser[]={
    -1,
    enPARENT,
    enTAIKUHOUNE,
    
};
