enum {
 enTexPageBase = enTexCEneEx0002Laser,
};
gxSprite SprCEneEx0002Laser[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,48,32,16,12},//対空砲根
    {enTexPageBase+0,48,0,48,32,12,24},//対空砲先
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneEx0002Laser[][16]={
    {0,0,0,0,0,0,100,100,100,0},//シーン
    {0,0,0,0,0,0,100,100,100,1},//対空砲根
    {4,0,-1,0,0,0,100,100,100,2},//対空砲先
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
