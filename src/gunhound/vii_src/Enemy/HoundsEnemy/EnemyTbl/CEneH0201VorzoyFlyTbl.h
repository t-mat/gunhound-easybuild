enum {
 enTexPageBase = enTexCEneH0201VorzoyFly,
};
gxSprite SprCEneH0201VorzoyFly[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,32,0,32,32,16,16},//胴体
    {enTexPageBase+0,0,0,24,16,8,8},//アーム
    {enTexPageBase+0,72,0,16,32,8,12},//右足
    {enTexPageBase+0,104,0,16,32,8,12},//左足
    {enTexPageBase+0,208,0,32,16,16,8},//マシンガン
    {enTexPageBase+0,0,40,120,56,52,56},//ハウンドキャリア
    {enTexPageBase+0,0,96,112,4,54,4},//プロペラ１
    {enTexPageBase+0,0,104,104,8,54,4},//プロペラ２
    {enTexPageBase+0,0,112,96,8,54,4},//プロペラ３
    {enTexPageBase+0,0,120,8,24,4,4},//フック
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0201VorzoyFly[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {-5,-32,1,0,0,0,100,100,100,1},//胴体
    {0,0,2,0,0,0,100,100,100,2},//アーム
    {8,12,1,0,0,0,100,100,100,3},//右足
    {3,12,0,0,0,0,100,100,100,4},//左足
    {5,3,-1,0,0,0,100,100,100,5},//マシンガン
    {0,-40,0,0,0,0,100,100,100,6},//ハウンドキャリア
    {1,-57,0,0,0,0,100,100,100,7},//プロペラ１１
    {1,-57,0,0,0,0,0,100,100,8},//プロペラ２２
    {1,-57,0,0,0,0,0,100,100,9},//プロペラ３２
    {0,-22,0,0,0,0,100,100,100,10},//フック１１
    {-3,-22,0,0,0,0,100,90,90,10},//フック２１
    {0,0,0,0,0,0,0,0,0,11},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enARM,
    enMIGIASI,
    enHIDARIASI,
    enMASINGUN,
    enHOUNDCARIA,
    enPUROPERA1,
    enPUROPERA2,
    enPUROPERA3,
    enHUKKU1,
    enHUKKU2,
    enMax,
    
};
Sint32 m_sParentCEneH0201VorzoyFly[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enARM,
    enPARENT,
    enHOUNDCARIA,
    enHOUNDCARIA,
    enHOUNDCARIA,
    enHOUNDCARIA,
    enHOUNDCARIA,
    
};
