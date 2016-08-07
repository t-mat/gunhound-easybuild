enum {
 enTexPageBase = enTexCEneEx0004Missile,
};
gxSprite SprCEneEx0004Missile[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,80,80,28,44},//ポッド
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneEx0004Missile[][16]={
    {0,0,0,0,0,0,100,100,100,0},//シーン
    {0,0,0,0,0,0,100,100,100,1},//ポッド
    {0,0,0,0,0,0,0,0,0,2},//0
};
enum {
    enPARENT,
    enPOD,
    enMax,
    
};
Sint32 m_sParentCEneEx0004Missile[]={
    -1,
    enPARENT,
    
};
