enum {
 enTexPageBase = enTexCEneH0003Vorzoy,
};
gxSprite SprCEneH0003Vorzoy[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,32,0,32,32,16,16},//胴体
    {enTexPageBase+0,0,0,24,16,8,8},//アーム
    {enTexPageBase+0,72,0,16,32,8,12},//右足
    {enTexPageBase+0,104,0,16,32,8,12},//左足
    {enTexPageBase+0,0,32,32,16,16,8},//マシンガン
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0003Vorzoy[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {-5,-32,0,0,0,0,100,100,100,1},//胴体
    {0,0,2,0,0,0,100,100,100,2},//アーム
    {8,12,1,0,0,0,100,100,100,3},//右足
    {3,12,0,0,0,0,100,100,100,4},//左足
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
Sint32 m_sParentCEneH0003Vorzoy[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enARM,
    
};
