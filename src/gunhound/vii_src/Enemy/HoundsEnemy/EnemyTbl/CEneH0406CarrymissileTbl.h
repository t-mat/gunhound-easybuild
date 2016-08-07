enum {
 enTexPageBase = enTexCEneH0405Nancy,
};
gxSprite SprCEneH0406Carrymissile[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,96,64,64,32},//胴体
    {enTexPageBase+0,96,0,128,32,16,16},//開口部１
    {enTexPageBase+0,96,32,128,32,16,16},//開口部２
    {enTexPageBase+0,224,0,32,64,8,32},//先端
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0406Carrymissile[][16]={
    {0,0,0,0,0,0,100,100,100,0},//シーン
    {0,0,0,0,0,0,100,100,100,1},//胴体１
    {16,-10,0,0,0,0,100,100,100,2},//開口部１１
    {16,10,0,0,0,0,100,100,100,3},//開口部２１
    {120,0,-1,0,0,0,100,100,100,4},//先端位置
    {0,0,0,0,0,0,0,0,0,5},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enKAIKOUBU1,
    enKAIKOUBU2,
    enSENTAN,
    enMax,
    
};
Sint32 m_sParentCEneH0406Carrymissile[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    
};
