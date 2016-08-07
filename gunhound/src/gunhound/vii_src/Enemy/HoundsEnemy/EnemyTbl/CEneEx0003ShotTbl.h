enum {
 enTexPageBase = enTexCEneEx0003Shot,
};
gxSprite SprCEneEx0003Shot[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,64,32,20,16},//ツイン砲
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneEx0003Shot[][16]={
    {0,0,0,0,0,0,100,100,100,0},//シーン
    {0,0,0,0,0,0,100,100,100,1},//ツイン砲１
    {0,0,0,0,0,0,0,0,0,2},//0
};
enum {
    enPARENT,
    enTWINHOU,
    enMax,
    
};
Sint32 m_sParentCEneEx0003Shot[]={
    -1,
    enPARENT,
    
};
