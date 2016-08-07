enum {
 enTexPageBase = enTexCEneH0503Tapache,
};
gxSprite SprCEneH0503Tapache[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,184,136,96,96},//胴体
    {enTexPageBase+0,0,136,224,8,112,8},//プロペラ１
    {enTexPageBase+0,0,144,208,8,112,8},//プロペラ２
    {enTexPageBase+0,0,152,192,8,112,8},//プロペラ３
    {enTexPageBase+0,184,0,24,16,12,8},//ガトリング１
    {enTexPageBase+0,208,0,24,16,12,8},//ガトリング２
    {enTexPageBase+0,184,16,24,16,12,8},//ガトリング３
    {enTexPageBase+0,208,16,24,16,12,8},//ガトリング４
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0503Tapache[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {3,-38,1,0,0,0,100,100,100,1},//胴体
    {-5,-70,0,0,0,0,100,100,100,2},//プロペラ１１
    {-5,-70,0,0,0,0,0,100,100,3},//プロペラ２２
    {-5,-70,0,0,0,0,0,100,100,4},//プロペラ３２
    {-75,0,0,0,0,0,100,100,100,5},//ガトリング１１
    {-75,0,0,0,0,0,0,100,100,6},//ガトリング２２
    {-75,0,0,0,0,0,0,100,100,7},//ガトリング３２
    {-75,0,0,0,0,0,0,100,100,8},//ガトリング４２
    {68,0,0,0,0,0,100,100,100,5},//ガトリング５１
    {68,0,0,0,0,0,0,100,100,6},//ガトリング６２
    {68,0,0,0,0,0,0,100,100,7},//ガトリング７２
    {68,0,0,0,0,0,0,100,100,8},//ガトリング８２
    {0,0,0,0,0,0,0,0,0,9},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enPUROPERA1,
    enPUROPERA2,
    enPUROPERA3,
    enGATRING1,
    enGATRING2,
    enGATRING3,
    enGATRING4,
    enGATRING5,
    enGATRING6,
    enGATRING7,
    enGATRING8,
    enMax,
    
};
Sint32 m_sParentCEneH0503Tapache[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    
};
