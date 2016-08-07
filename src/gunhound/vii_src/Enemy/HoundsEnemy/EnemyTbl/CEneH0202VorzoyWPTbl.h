enum {
 enTexPageBase = enTexCEneH0202VorzoyWP,
};
gxSprite SprCEneH0202VorzoyWP[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,32,0,32,32,8,8},//胴体
    {enTexPageBase+0,0,0,24,24,8,8},//アーム
    {enTexPageBase+0,72,0,24,32,8,4},//右足
    {enTexPageBase+0,104,0,24,32,8,4},//左足
    {enTexPageBase+0,208,0,48,16,16,8},//マシンガン
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0202VorzoyWP[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {-10,-48,0,0,0,0,100,100,100,1},//胴体
    {3,5,2,0,0,0,100,100,100,2},//アーム
    {10,20,1,0,0,0,100,100,100,3},//右足
    {0,20,0,0,0,0,100,100,100,4},//左足
    {5,6,-1,0,0,0,100,100,100,5},//マシンガン
    {0,0,0,0,0,0,0,0,0,6},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enARM,
    enMIGIASI,
    enHIDARIASI,
    enMASINGUN,
    enMax,
    
};
Sint32 m_sParentCEneH0202VorzoyWP[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enARM,
    
};
