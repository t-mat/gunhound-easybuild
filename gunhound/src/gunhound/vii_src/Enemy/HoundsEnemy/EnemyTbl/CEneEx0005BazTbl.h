enum {
 enTexPageBase = enTexCEneEx0005Baz,
};
gxSprite SprCEneEx0005Baz[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,96,32,36,20},//砲台
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneEx0005Baz[][16]={
    {0,0,0,0,0,0,100,100,100,0},//シーン
    {0,0,0,0,0,0,100,100,100,1},//砲台
    {0,0,0,0,0,0,0,0,0,2},//0
};
enum {
    enPARENT,
    enHOUDAI,
    enMax,
    
};
Sint32 m_sParentCEneEx0005Baz[]={
    -1,
    enPARENT,
    
};
