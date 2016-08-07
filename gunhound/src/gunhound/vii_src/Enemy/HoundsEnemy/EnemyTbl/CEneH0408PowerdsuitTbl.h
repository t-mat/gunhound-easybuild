enum {
 enTexPageBase = enTexCEneH0408Powerdsuit,
};
gxSprite SprCEneH0408Powerdsuit[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,32,64,32,48,16,32},//胴体
    {enTexPageBase+0,0,64,24,16,8,8},//アーム
    {enTexPageBase+0,72,64,16,32,8,12},//右足
    {enTexPageBase+0,104,64,16,32,8,12},//左足
    {enTexPageBase+0,208,120,32,16,16,8},//マシンガン
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0408Powerdsuit[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {-5,-36,0,0,0,0,100,100,100,1},//胴体
    {10,0,2,0,0,0,100,100,100,2},//アーム
    {8,15,1,0,0,0,100,100,100,3},//右足
    {-3,15,0,0,0,0,100,100,100,4},//左足
    {5,3,-1,0,0,0,100,100,100,5},//マシンガン
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
Sint32 m_sParentCEneH0408Powerdsuit[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enARM,
    
};
