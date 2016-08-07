enum {
 enTexPageBase = enTexCEneEx0001Cannon,
};
gxSprite SprCEneEx0001Cannon[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,256,96,192,48},//砲台
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneEx0001Cannon[][16]={
    {0,0,0,0,0,0,100,100,100,0},//シーン
    {0,0,0,0,0,0,100,100,100,1},//砲台
    {0,0,0,0,0,0,0,0,0,2},//0
};
enum {
    enPARENT,
    enHOUDAI,
    enMax,
    
};
Sint32 m_sParentCEneEx0001Cannon[]={
    -1,
    enPARENT,
    
};
